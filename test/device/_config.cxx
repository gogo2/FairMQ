/********************************************************************************
 *    Copyright (C) 2017 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include <fairmq/Device.h>
#include <fairmq/DeviceRunner.h>
#include <fairmq/ProgOptions.h>

#include <gtest/gtest.h>

#include <sstream> // std::stringstream
#include <thread>

namespace _config
{

using namespace std;
using namespace fair::mq;

void control(Device& device)
{
    device.ChangeState(Transition::InitDevice);
    device.WaitForState(State::InitializingDevice);
    device.ChangeState(Transition::CompleteInit);
    device.WaitForState(State::Initialized);
    device.ChangeState(Transition::Bind);
    device.WaitForState(State::Bound);
    device.ChangeState(Transition::Connect);
    device.WaitForState(State::DeviceReady);
    device.ChangeState(Transition::InitTask);
    device.WaitForState(State::Ready);

    device.ChangeState(Transition::Run);
    device.WaitForState(State::Ready);

    device.ChangeState(Transition::ResetTask);
    device.WaitForState(State::DeviceReady);
    device.ChangeState(Transition::ResetDevice);
    device.WaitForState(State::Idle);

    device.ChangeState(Transition::End);
}

class TestDevice : public Device
{
  public:
    TestDevice(const string& transport)
    {
        fDeviceThread = thread(&Device::RunStateMachine, this);

        SetTransport(transport);

        ChangeState(Transition::InitDevice);
        WaitForState(State::InitializingDevice);
        ChangeState(Transition::CompleteInit);
        WaitForState(State::Initialized);
        ChangeState(Transition::Bind);
        WaitForState(State::Bound);
        ChangeState(Transition::Connect);
        WaitForState(State::DeviceReady);
        ChangeState(Transition::InitTask);
        WaitForState(State::Ready);

        ChangeState(Transition::Run);
    }

    TestDevice(const TestDevice&) = delete;
    TestDevice(TestDevice&&) = delete;
    TestDevice& operator=(const TestDevice&) = delete;
    TestDevice& operator=(TestDevice&&) = delete;

    ~TestDevice() override
    {
        WaitForState(State::Running);
        ChangeState(Transition::Stop);
        WaitForState(State::Ready);
        ChangeState(Transition::ResetTask);
        WaitForState(State::DeviceReady);
        ChangeState(Transition::ResetDevice);
        WaitForState(State::Idle);

        ChangeState(Transition::End);

        if (fDeviceThread.joinable()) {
            fDeviceThread.join();
        }
    }

  private:
    thread fDeviceThread;
};

class Config : public ::testing::Test
{
  public:
    Config() = default;

    string TestDeviceSetConfig(const string& transport)
    {
        ProgOptions config;

        vector<string> emptyArgs = {"dummy", "--id", "test", "--color", "false"};

        config.ParseAll(emptyArgs, true);

        config.SetProperty("transport", transport);

        Device device;
        device.SetConfig(config);

        Channel channel;
        channel.UpdateType("pub");
        channel.UpdateMethod("connect");
        channel.UpdateAddress("tcp://localhost:5558");
        device.AddChannel("data", std::move(channel));

        thread t(control, ref(device));

        device.RunStateMachine();

        if (t.joinable()) {
            t.join();
        }

        return device.GetTransportName();
    }

    string TestDeviceSetConfigWithPlugins(const string& transport)
    {
        ProgOptions config;

        vector<string> emptyArgs = {"dummy", "--id", "test", "--color", "false"};

        config.SetProperty("transport", transport);

        Device device;
        PluginManager mgr;
        mgr.LoadPlugin("s:config");
        mgr.ForEachPluginProgOptions([&](boost::program_options::options_description options) {
            config.AddToCmdLineOptions(options);
        });
        mgr.EmplacePluginServices(config, device);
        mgr.InstantiatePlugins();

        config.ParseAll(emptyArgs, true);
        DeviceRunner::HandleGeneralOptions(config);
        device.SetConfig(config);

        Channel channel;
        channel.UpdateType("pub");
        channel.UpdateMethod("connect");
        channel.UpdateAddress("tcp://localhost:5558");
        device.AddChannel("data", std::move(channel));

        thread t(control, ref(device));

        device.RunStateMachine();

        config.PrintOptions();

        if (t.joinable()) {
            t.join();
        }

        return device.GetTransportName();
    }

    string TestDeviceControlInConstructor(const string& transport)
    {
        TestDevice device(transport);

        return device.GetTransportName();
    }

    string TestDeviceSetTransport(const string& transport)
    {
        Device device;
        device.SetTransport(transport);

        Channel channel;
        channel.UpdateType("pub");
        channel.UpdateMethod("connect");
        channel.UpdateAddress("tcp://localhost:5558");
        device.AddChannel("data", std::move(channel));

        thread t(&Device::RunStateMachine, &device);

        control(device);

        if (t.joinable()) {
            t.join();
        }

        return device.GetTransportName();
    }
};

TEST_F(Config, SetConfig)
{
    string transport = "zeromq";
    string returnedTransport = TestDeviceSetConfig(transport);

    EXPECT_EQ(transport, returnedTransport);
}

TEST_F(Config, SetConfigWithPlugins)
{
    string transport = "zeromq";
    string returnedTransport = TestDeviceSetConfigWithPlugins(transport);

    EXPECT_EQ(transport, returnedTransport);
}

TEST_F(Config, SetTransport)
{
    string transport = "zeromq";
    string returnedTransport = TestDeviceSetTransport(transport);

    EXPECT_EQ(transport, returnedTransport);
}

TEST_F(Config, ControlInConstructor)
{
    string transport = "zeromq";
    string returnedTransport = TestDeviceControlInConstructor(transport);

    EXPECT_EQ(transport, returnedTransport);
}

} // namespace
