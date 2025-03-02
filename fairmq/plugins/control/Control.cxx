/********************************************************************************
 * Copyright (C) 2017-2022 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH  *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "Control.h"

#include <fairmq/tools/IO.h>

#include <atomic>
#include <chrono>
#include <csignal> // catching system signals
#include <cstdlib>
#include <functional>
#include <iostream>
#include <thread>

#include <poll.h> // for the interactive mode

using namespace std;

namespace
{
    std::atomic<sig_atomic_t> gLastSignal(0);
    std::atomic<int> gSignalCount(0);

    extern "C" auto sigint_handler(int signal) -> void
    {
        ++gSignalCount;
        gLastSignal = signal;

        if (gSignalCount > 1) {
            std::abort();
        }
    }

    extern "C" auto sigterm_handler(int signal) -> void
    {
        ++gSignalCount;
        gLastSignal = signal;
    }
}

namespace fair::mq::plugins
{

Control::Control(const string& name, Plugin::Version version, const string& maintainer, const string& homepage, PluginServices* pluginServices)
    : Plugin(name, version, maintainer, homepage, pluginServices)
    , fDeviceShutdownRequested(false)
    , fDeviceHasShutdown(false)
    , fPluginShutdownRequested(false)
{
    SubscribeToDeviceStateChange([&](DeviceState newState) {
        LOG(trace) << "control plugin notified on new state: " << newState;

        if (newState == DeviceState::Error) {
            fPluginShutdownRequested = true;
            fStateQueue.Push(newState, [this]{ fDeviceShutdownRequested = true; });
        } else {
            fStateQueue.Push(newState);
        }
    });

    try {
        auto control = GetProperty<string>("control");

        if (control != "none") {
            TakeDeviceControl();
        }

        if (control == "static") {
            LOG(debug) << "Running builtin controller: static";
            fControllerThread = thread(&Control::StaticMode, this);
        } else if (control == "none") {
            LOG(debug) << "Builtin controller: disabled";
        } else if (control == "gui") {
            LOG(debug) << "Running builtin controller: gui";
            fControllerThread = thread(&Control::GUIMode, this);
        } else if (control == "dynamic" || control == "external" || control == "interactive") {
            LOG(debug) << "Running builtin controller: interactive";
            fControllerThread = thread(&Control::InteractiveMode, this);
        } else {
            LOG(error) << "Unrecognized control mode '" << control << "' requested. " << "Ignoring and falling back to static control mode.";
            fControllerThread = thread(&Control::StaticMode, this);
        }
    } catch (PluginServices::DeviceControlError& e) {
        // If we are here, it means another plugin has taken control. That's fine, just print the exception message and do nothing else.
        LOG(debug) << e.what();
    }

    if (GetProperty<int>("catch-signals") > 0) {
        LOG(debug) << "Plugin '" << name << "' is setting up signal handling for SIGINT and SIGTERM";
        fSignalHandlerThread = thread(&Control::SignalHandler, this);
        signal(SIGINT, sigint_handler);
        signal(SIGTERM, sigterm_handler);
    } else {
        LOG(warn) << "Signal handling (e.g. Ctrl-C) has been deactivated.";
    }
}

auto Control::RunStartupSequence() -> void
{
    using Transition = DeviceStateTransition;
    using State = DeviceState;
    auto shutdownRequested = [this]{ return fDeviceShutdownRequested.load(); };

    ChangeDeviceState(Transition::InitDevice);
    fStateQueue.WaitForStateOrCustom(State::InitializingDevice, shutdownRequested);
    if (fDeviceShutdownRequested) { return; /* --> shutdown sequence */ }

    ChangeDeviceState(Transition::CompleteInit);
    fStateQueue.WaitForStateOrCustom(State::Initialized,        shutdownRequested);
    if (fDeviceShutdownRequested) { return; /* --> shutdown sequence */ }

    ChangeDeviceState(Transition::Bind);
    fStateQueue.WaitForStateOrCustom(State::Binding,            shutdownRequested);
    if (fDeviceShutdownRequested) { return; /* --> shutdown sequence */ }

    fStateQueue.WaitForStateOrCustom(State::Bound,              shutdownRequested);
    if (fDeviceShutdownRequested) { return; /* --> shutdown sequence */ }

    ChangeDeviceState(Transition::Connect);
    fStateQueue.WaitForStateOrCustom(State::Connecting,         shutdownRequested);
    if (fDeviceShutdownRequested) { return; /* --> shutdown sequence */ }

    fStateQueue.WaitForStateOrCustom(State::DeviceReady,        shutdownRequested);
    if (fDeviceShutdownRequested) { return; /* --> shutdown sequence */ }

    ChangeDeviceState(Transition::InitTask);
    fStateQueue.WaitForStateOrCustom(State::InitializingTask,   shutdownRequested);
    if (fDeviceShutdownRequested) { return; /* --> shutdown sequence */ }

    fStateQueue.WaitForStateOrCustom(State::Ready,              shutdownRequested);
    if (fDeviceShutdownRequested) { return; /* --> shutdown sequence */ }

    ChangeDeviceState(Transition::Run);
    fStateQueue.WaitForStateOrCustom(State::Running,            shutdownRequested);
    if (fDeviceShutdownRequested) { return; /* --> shutdown sequence */ }
}

auto ControlPluginProgramOptions() -> Plugin::ProgOptions
{
    namespace po = boost::program_options;
    auto pluginOptions = po::options_description{"Control (builtin) Plugin"};
    pluginOptions.add_options()
        ("control",       po::value<string>()->default_value("dynamic"), "Control mode, 'static' or 'dynamic' (aliases for dynamic are external and interactive), 'none', 'gui'")
        ("catch-signals", po::value<int   >()->default_value(1),             "Enable signal handling (1/0).");
    return pluginOptions;
}

auto Control::RunREPL() -> void
{
    char input = 0; // hold the user console input
    pollfd cinfd[1];
    cinfd[0].fd = fileno(stdin);
    cinfd[0].events = POLLIN;
    cinfd[0].revents = 0;

    tools::NonCanonicalInput nci;

    bool color = GetProperty<bool>("color");

    if (color) {
        PrintInteractiveHelpColor();
    } else {
        PrintInteractiveHelp();
    }

    bool keepRunning = true;

    while (keepRunning) {
        if (poll(cinfd, 1, 100)) {
            if (fDeviceShutdownRequested) {
                break;
            }

            cin >> input;

            switch (input) {
                case 'c':
                    cout << "\n --> [i] check current state\n\n" << flush;
                    LOG(state) << GetCurrentDeviceState();
                break;
                case 'i':
                    cout << "\n --> [i] init device\n\n" << flush;
                    if (ChangeDeviceState(DeviceStateTransition::InitDevice)) {
                        fStateQueue.WaitForState(DeviceState::InitializingDevice);
                        ChangeDeviceState(DeviceStateTransition::CompleteInit);
                    }
                break;
                case 'b':
                    cout << "\n --> [b] bind\n\n" << flush;
                    ChangeDeviceState(DeviceStateTransition::Bind);
                break;
                case 'x':
                    cout << "\n --> [x] connect\n\n" << flush;
                    ChangeDeviceState(DeviceStateTransition::Connect);
                break;
                case 'j':
                    cout << "\n --> [j] init task\n\n" << flush;
                    ChangeDeviceState(DeviceStateTransition::InitTask);
                break;
                case 'r':
                    cout << "\n --> [r] run\n\n" << flush;
                    ChangeDeviceState(DeviceStateTransition::Run);
                break;
                case 's':
                    cout << "\n --> [s] stop\n\n" << flush;
                    ChangeDeviceState(DeviceStateTransition::Stop);
                break;
                case 't':
                    cout << "\n --> [t] reset task\n\n" << flush;
                    ChangeDeviceState(DeviceStateTransition::ResetTask);
                break;
                case 'd':
                    cout << "\n --> [d] reset device\n\n" << flush;
                    ChangeDeviceState(DeviceStateTransition::ResetDevice);
                break;
                case 'k':
                    cout << "\n --> [k] increase log severity\n\n" << flush;
                    CycleLogConsoleSeverityUp();
                break;
                case 'l':
                    cout << "\n --> [l] decrease log severity\n\n" << flush;
                    CycleLogConsoleSeverityDown();
                break;
                case 'n':
                    cout << "\n --> [n] increase log verbosity\n\n" << flush;
                    CycleLogVerbosityUp();
                break;
                case 'm':
                    cout << "\n --> [m] decrease log verbosity\n\n" << flush;
                    CycleLogVerbosityDown();
                break;
                case 'p':
                    cout << "\n --> [p] print number of connected peers for all channels\n\n" << flush;
                    PrintNumberOfConnectedPeers();
                break;
                case 'h':
                    cout << "\n --> [h] help\n\n" << flush;
                    if (color) {
                        PrintInteractiveHelpColor();
                        PrintStateMachineColor();
                    } else {
                        PrintInteractiveHelp();
                        PrintStateMachine();
                    }
                break;
                case 'q':
                    cout << "\n --> [q] end\n\n" << flush;
                    keepRunning = false;
                break;
                default:
                    cout << "Invalid input: [" << input << "]. " << flush;
                    PrintInteractiveHelp();
                break;
            }
        }

        if (GetCurrentDeviceState() == DeviceState::Error) {
            throw DeviceErrorState("Controlled device transitioned to error state.");
        }

        if (fDeviceShutdownRequested) {
            break;
        }
    }

}

auto Control::InteractiveMode() -> void
try {
    RunStartupSequence();

    if (!fDeviceShutdownRequested) {
        RunREPL();
    }

    if (!fDeviceShutdownRequested) {
        RunShutdownSequence();
    }
} catch (PluginServices::DeviceControlError& e) {
    // If we are here, it means another plugin has taken control. That's fine, just print the exception message and do nothing else.
    LOG(debug) << e.what();
} catch (DeviceErrorState&) {
    ReleaseDeviceControl();
}

auto Control::PrintInteractiveHelpColor() -> void
{
    stringstream ss;
    ss << "Following control commands are available:\n\n"
       << " [\033[01;32mh\033[0m] help, [\033[01;32mc\033[0m] check current device state, [\033[01;32mp\033[0m] print number of connected peers for channels,\n"
       << " [\033[01;32mi\033[0m] init device, [\033[01;32mb\033[0m] bind, [\033[01;32mx\033[0m] connect, [\033[01;32mj\033[0m] init task,"
       << " [\033[01;32mr\033[0m] run, [\033[01;32ms\033[0m] stop,\n"
       << " [\033[01;32mt\033[0m] reset task, [\033[01;32md\033[0m] reset device, [\033[01;32mq\033[0m] end,\n"
       << " [\033[01;32mk\033[0m] increase log severity, [\033[01;32ml\033[0m] decrease log severity, [\033[01;32mn\033[0m] increase log verbosity, [\033[01;32mm\033[0m] decrease log verbosity\n\n";
    cout << ss.str() << flush;
}

auto Control::PrintInteractiveHelp() -> void
{
    stringstream ss;
    ss << "Following control commands are available:\n\n"
       << " [h] help, [c] check current device state, [p] print number of connected peers for channels\n"
       << " [i] init device, [b] bind, [x] connect, [j] init task,\n"
       << " [r] run, [s] stop,\n"
       << " [t] reset task, [d] reset device, [q] end,\n"
       << " [k] increase log severity, [l] decrease log severity, [n] increase log verbosity, [m] decrease log verbosity.\n\n";
    cout << ss.str() << flush;
}

void Control::PrintStateMachineColor()
{
    stringstream ss;
    ss << "               @                                \n"
       << "   ┌───────────╨─────────────┐  ┌───────────┐   \n"
       << "   │           \033[01;36mIDLE\033[0m         [\033[01;32mq\033[0m]─▶  \033[01;33mEXITING\033[0m  │   \n"
       << "   └[\033[01;32mi\033[0m]─────────────────── ▲ ┘  └───────────┘   \n"
       << " ╔══ ▼ ════════════════╗ ╔═╩══════════════════╗ \n"
       << " ║ \033[01;33mINITIALIZING DEVICE\033[0m ║ ║  \033[01;33mRESETTING DEVICE\033[0m  ║ \n"
       << " ╚══════════╦══════════╝ ╚════════ ▲ ═════════╝ \n"
       << " ┌───────── ▼ ─────────┐           │  ┌────────────────────────────┐ \n"
       << " │     \033[01;36mINITIALIZED\033[0m    [\033[01;32md\033[0m]──────────┤  │ Legend:                    │ \n"
       << " └─────────[\033[01;32mb\033[0m]─────────┘           │  │----------------------------│ \n"
       << " ╔═════════ ▼ ═════════╗           │  │ [\033[01;32mk\033[0m] keyboard shortcut for  │ \n"
       << " ║       \033[01;33mBINDING\033[0m       ║           │  │     interactive controller │ \n"
       << " ╚══════════╦══════════╝           │  │ ┌────────────────────────┐ │ \n"
       << " ┌───────── ▼ ─────────┐           │  │ │      \033[01;36mIDLING STATE\033[0m      │ │ \n"
       << " │        \033[01;36mBOUND\033[0m       [\033[01;32md\033[0m]──────────┤  │ └────────────────────────┘ │ \n"
       << " └─────────[\033[01;32mx\033[0m]─────────┘           │  │ ╔════════════════════════╗ │ \n"
       << " ╔═════════ ▼ ═════════╗           │  │ ║      \033[01;33mWORKING STATE\033[0m     ║ │ \n"
       << " ║     \033[01;33mCONNECTING\033[0m      ║           │  │ ╚════════════════════════╝ │ \n"
       << " ╚══════════╦══════════╝           │  └────────────────────────────┘ \n"
       << "   ┌─────── ▼ ────────────────────[\033[01;32md\033[0m]───────┐   \n"
       << "   │              \033[01;36mDEVICE READY\033[0m              │   \n"
       << "   └───────[\033[01;32mj\033[0m]──────────────────── ▲ ───────┘   \n"
       << " ╔═════════ ▼ ═════════╗ ╔═════════╩══════════╗ \n"
       << " ║  \033[01;33mINITIALIZING TASK\033[0m  ║ ║   \033[01;33mRESETTING TASK\033[0m   ║ \n"
       << " ╚══════════╦══════════╝ ╚════════ ▲ ═════════╝ \n"
       << "   ┌─────── ▼ ────────────────────[\033[01;32mt\033[0m]───────┐   \n"
       << "   │                 \033[01;36mREADY\033[0m                  │   \n"
       << "   └───────[\033[01;32mr\033[0m]──────────────────── ▲ ───────┘   \n"
       << "    ╔══════ ▼ ════════════════════[\033[01;32ms\033[0m]══════╗    \n"
       << "    ║               \033[01;33mRUNNING\033[0m                ║    \n"
       << "    ╚══════════════════════════════════════╝    \n"
       << "                                                \n";
    cout << ss.str() << flush;
}

void Control::PrintStateMachine()
{
    stringstream ss;
    ss << "               @                                                     \n"
       << "   ┌───────────╨─────────────┐  ┌───────────┐                        \n"
       << "   │           IDLE         [q]─▶  EXITING  │                        \n"
       << "   └[i]─────────────────── ▲ ┘  └───────────┘                        \n"
       << " ╔══ ▼ ════════════════╗ ╔═╩══════════════════╗                      \n"
       << " ║ INITIALIZING DEVICE ║ ║  RESETTING DEVICE  ║                      \n"
       << " ╚══════════╦══════════╝ ╚════════ ▲ ═════════╝                      \n"
       << " ┌───────── ▼ ─────────┐           │  ┌────────────────────────────┐ \n"
       << " │     INITIALIZED    [d]──────────┤  │ Legend:                    │ \n"
       << " └─────────[b]─────────┘           │  │----------------------------│ \n"
       << " ╔═════════ ▼ ═════════╗           │  │ [k] keyboard shortcut for  │ \n"
       << " ║       BINDING       ║           │  │     interactive controller │ \n"
       << " ╚══════════╦══════════╝           │  │ ┌────────────────────────┐ │ \n"
       << " ┌───────── ▼ ─────────┐           │  │ │      IDLING STATE      │ │ \n"
       << " │        BOUND       [d]──────────┤  │ └────────────────────────┘ │ \n"
       << " └─────────[x]─────────┘           │  │ ╔════════════════════════╗ │ \n"
       << " ╔═════════ ▼ ═════════╗           │  │ ║      WORKING STATE     ║ │ \n"
       << " ║     CONNECTING      ║           │  │ ╚════════════════════════╝ │ \n"
       << " ╚══════════╦══════════╝           │  └────────────────────────────┘ \n"
       << "   ┌─────── ▼ ────────────────────[d]───────┐                        \n"
       << "   │              DEVICE READY              │                        \n"
       << "   └───────[j]──────────────────── ▲ ───────┘                        \n"
       << " ╔═════════ ▼ ═════════╗ ╔═════════╩══════════╗                      \n"
       << " ║  INITIALIZING TASK  ║ ║   RESETTING TASK   ║                      \n"
       << " ╚══════════╦══════════╝ ╚════════ ▲ ═════════╝                      \n"
       << "   ┌─────── ▼ ────────────────────[t]───────┐                        \n"
       << "   │                 READY                  │                        \n"
       << "   └───────[r]──────────────────── ▲ ───────┘                        \n"
       << "    ╔══════ ▼ ════════════════════[s]══════╗                         \n"
       << "    ║               RUNNING                ║                         \n"
       << "    ╚══════════════════════════════════════╝                         \n"
       << "                                                                     \n";
    cout << ss.str() << flush;
}

auto Control::PrintNumberOfConnectedPeers() -> void
{
    unordered_map<string, int> channelInfo(GetChannelInfo());

    for (const auto& c : channelInfo) {
        for (int i = 0; i < c.second; ++i) {
            LOG(info) << c.first << "[" << i << "]: " << GetNumberOfConnectedPeers(c.first, i) << " peers connected";
        }
    }
}

auto Control::StaticMode() -> void
try {
    RunStartupSequence();

    // Wait for next state, which is DeviceState::Ready,
    // or for device shutdown request (Ctrl-C)
    fStateQueue.WaitForNextOrCustom([this]{ return fDeviceShutdownRequested.load(); });

    if (!fDeviceShutdownRequested) {
        RunShutdownSequence();
    }
} catch (PluginServices::DeviceControlError& e) {
    // If we are here, it means another plugin has taken control. That's fine, just print the exception message and do nothing else.
    LOG(debug) << e.what();
} catch (DeviceErrorState&) {
    ReleaseDeviceControl();
}

auto Control::GUIMode() -> void
try {
    RunStartupSequence();

    // Wait for device shutdown request (Ctrl-C)
    fStateQueue.WaitForCustom([this]{ return fDeviceShutdownRequested.load(); });

    if (!fDeviceShutdownRequested) {
        RunShutdownSequence();
    }
} catch (PluginServices::DeviceControlError& e) {
    // If we are here, it means another plugin has taken control. That's fine, just print the
    // exception message and do nothing else.
    LOG(debug) << e.what();
} catch (DeviceErrorState&) {
    ReleaseDeviceControl();
}

auto Control::SignalHandler() -> void
{
    while (gSignalCount == 0 && !fPluginShutdownRequested) {
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    if (!fPluginShutdownRequested) {
        LOG(info) << "Received device shutdown request (signal " << gLastSignal << ").";
        LOG(info) << "Waiting for graceful device shutdown. Hit Ctrl-C again to abort immediately.";

        // Signal and wait for controller thread, if we are controller
        fStateQueue.Notify([this] { fDeviceShutdownRequested = true; });
        {
            unique_lock<mutex> lock(fControllerMutex);
            if (fControllerThread.joinable()) { fControllerThread.join(); }
        }

        if (!fDeviceHasShutdown) {
            // Take over control and attempt graceful shutdown
            StealDeviceControl();
            try {
                RunShutdownSequence();
            } catch (PluginServices::DeviceControlError& e) {
                LOG(info) << "Graceful device shutdown failed: " << e.what() << " If hanging, hit Ctrl-C again to abort immediately.";
            } catch (...) {
                ReleaseDeviceControl();
                LOG(info) << "Graceful device shutdown failed. If hanging, hit Ctrl-C again to abort immediately.";
            }
        }
    }
}

auto Control::RunShutdownSequence() -> void
{
    auto nextState = GetCurrentDeviceState();
    if (nextState != DeviceState::Error) {
        fStateQueue.Clear();
    }
    while (nextState != DeviceState::Exiting && nextState != DeviceState::Error) {
        switch (nextState) {
            case DeviceState::Idle:
                ChangeDeviceState(DeviceStateTransition::End);
                break;
            case DeviceState::InitializingDevice:
                ChangeDeviceState(DeviceStateTransition::CompleteInit);
                break;
            case DeviceState::Initialized:
            case DeviceState::Bound:
            case DeviceState::DeviceReady:
                ChangeDeviceState(DeviceStateTransition::ResetDevice);
                break;
            case DeviceState::Ready:
                ChangeDeviceState(DeviceStateTransition::ResetTask);
                break;
            case DeviceState::Running:
                ChangeDeviceState(DeviceStateTransition::Stop);
                break;
            default:
                // LOG(debug) << "Controller ignoring event: " << nextState;
                break;
        }

        nextState = fStateQueue.WaitForNext();
    }

    fDeviceHasShutdown = true;
    ReleaseDeviceControl();
}

Control::~Control()
{
    // Notify threads to exit
    fPluginShutdownRequested = true;

    {
        unique_lock<mutex> lock(fControllerMutex);
        if (fControllerThread.joinable()) { fControllerThread.join(); }
    }
    if (fSignalHandlerThread.joinable()) { fSignalHandlerThread.join(); }

    UnsubscribeFromDeviceStateChange();
}

} // namespace fair::mq::plugins
