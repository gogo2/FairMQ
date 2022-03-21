# Changelog

## `v1.4.49` - 2022-02-28
#### Bugfix
* shm: check region size when opening existing (b747a8787)
* Shm: fix number of region events (8efe7adf0)
* Shm: bring back thread-safety for `fRegions` (intra-process) (29827f042)
* Skip error message only for tcp (fa0bf96eb)


## `v1.4.48` - 2022-02-02
#### Changed/Removed
* shm: Add size to UnmanagedRegion debug output (4f9aeda8e)
* shm: remove UR queues on ResetContent (92a56c26b)
* shm: monitor: disable number of msgs in the ack queue output (2f82eb4f0)
* shm: allow `monitor::ResetContent` to cleanup after a crash (1a75141fc)


## `v1.4.47` - 2022-01-28
#### Changed/Removed
* Avoid fixed ports in the test suites (f4d39d224)
* Don't use to-be-deprecated names (bfd08bb33)
#### New
* GUI Controller (ad894c79c)
#### Bugfix
* use `[[maybe_unused]]` for values used in assertions (f15f66985)


## `v1.4.46` - 2022-01-13
#### Changed/Removed
* modify keep-alive example executable a bit, make it configurable (f6bade32b)
#### Bugfix
* shm: fix double unlock() (f79a0714b)
* shm: keep mng segment around when skipping cleanup (ddf9bc727)


## `v1.4.45` - 2022-01-10
#### Changed/Removed
* shm: reduce contention on region events (c04958e2a)


## `v1.4.44` - 2021-12-17
#### Changed/Removed
* Do not catch and rethrow exception from state handlers (0eaea3c66)
* Avoid accessing Device.fChannels directly, use getters (dbdf17c66)
* refactor: deduplicate more zmq/shmem code (953c4a75c)
* consolidate UnmanagedRegion options (d630fbb1e)
* simplify region cleanup (eef42d2de)
* extend region config (80ed45df6)
* shm: integrate mtx and cv into management segment (1839f7e8c)
* shm: always open_or_create segment (eb4620b1e)
#### New
* feat: Add `Device::GetNumSubChannels(channel)` (a3bb5fb4b)
* Add configurable default snd/rcv timeout (f24dee33c)
* shm: add APIs for implementing keep-alive process (692576a5b)
#### Bugfix
* fix: install `tools/Exceptions.h` (856780f88)
* Do not print logo, if not requested (acfb49541)


## `v1.4.43` - 2021-10-19
#### Changed/Removed
* Define copy/move ctors and assignment ops (ad824b4de)
* refactor: Deduplicate `GetConstant()` (7329cb442)
#### Deprecated
* fix: Deprecate `Message::Close` because it is really a dtor (b8503bfbd)
#### New
* feat: add options to control allocation attempts (1449166d4)
* feat: add tool for noncanonical input (5fe2f53c7)
* feat: Add new `assertm` macro (8796ce5b2)
* feat: Add new `GetNumberOfConnectedPeers()` API (fda8126a4)
* feat: Add interactive controller button to print connected peers (ebcbe2dde)
#### Bugfix
* Fix compiler warnings (293401658, 1ac30b51b, f33c597f3, 310204a89, 1ee9d2d22, cf9b45cd7, 9590b5be4, 597d88277, 153dcfab9, 36600dce2, 1a5d0eddb, e84a16da8)
* fix(pmix): Avoid deprecated value (efca8e0ad)
* fix(shm): fix regression in debug mode data (d7fb01908)
#### Buildsystem/CI
* ci: Add macos-11-arm64 (apple-clang-13) build (55a2cfcc3)


## `v1.4.42` - 2021-09-20
#### Deprecated
* fix: Deprecate static string helper (b442483dc)
#### Bugfix
* fix: `-Winfinite-recursion` (727a709af)
* Shm: Fix `SetUsedSize()` (e484bf457)


## `v1.4.41` - 2021-09-07
#### Changed/Removed
* feat(tools): Move the error code to the Tools target (9cbaf7e0f)
#### Deprecated
* refactor: Prepare deprecation of non-namespaced types and headers (8e6c50e7c)
   * `<FairMQ*.h>` headers will be deprecated in the future. Use `<fairmq/*.h>` instead!
   * `::FairMQ*` typenames will be deprecated in the future. Use `fair::mq::*` instead!
#### New
* feat(tidy): Add new `fairmq-tidy` tool (db727092c, dff2b4b7d)
* Implement shmem msg zero-copy (bce380d87)
#### Bugfix
* fix: First round of using new non-namespaced typenames (4e8f247a0)
* shm: reimplement alignment (815b2f1d7)
#### Buildsystem/CI
* build: Have color output depend on a common switch `DISABLE_COLOR` (d392f60c0)
* build: Use `fairmq-tidy` on our own codebase when `RUN_FAIRMQ_TIDY=ON` (24fbf9494)
* Extend test for empty messages (c57410b82)


## `v1.4.40` - 2021-07-30
#### Bugfix
* Protect access to options container (db0500fb2)


## `v1.4.39` - 2021-07-16
#### Changed/Removed
* shm: optimize monitor heartbeats (28a887a45)
* shm: improve exception handling (37c059177)
* feat(sdk): Remove unused fairmq executable (09d257410)
* feat: Require DDS `3.5.13.7` (1df338b26)
* feat: Drop public dependency to Boost.Asio and use standalone asio (9585c20b7, 0c4921d65)
* feat(ofi): Require asiofi 0.5 (1007de8e4)
#### New
* Add `--shm-mlock-segment-on-creation` option (a6193a380)
* Add `--shm-zero-segment-on-creation` option (03ba9eb55)
* feat: Add new function `GetEnabledTransports()` (e3d3be888)
* feat: Add `<fairmq/FwdDecls.h>` (479d6e071)
#### Bugfix
* SDK: Add missing header <thread> (ce0a05225)
* Examples: Fix DDS agent log retrieval (f2d7bbeb2)
* Fix clang-tidy warnings (ccbd62213, e1b229522, acf63d3c1, 9444de586, 6aeac265e, f25cca207, c847a7ca0, 882edbbdb)
* shm: throw `TransportError` if could not lock region (a8bdb9116)
* Add empty msg check for transport compatibility checker (4dbb5535c)
* shm: avoid meta data copy on recv and fix its alignment (5c9ba5e5b)
* Fix stack-use-after-scope (4fdf9d340)
* Fix heap-use-after-free (ac3293fcc)
* shmmonitor: handle missing segmentInfos (c5e40fd18)
* fix(sdk): Silence boost warnings (e2de214a1)
* fix(examples/dds): Restore infinite loop (3702d3bfc)
* fix(plugins): Avoid double device control releases (e2452fa7e)
#### Buildsystem/CI
* extern/googletest: Bump for GCC11 support (bac5b90d8)
* extern/asio: Bump and require `v1.18.1` (d15bc17b1)
* CMake: Remove cotire dependency (ed78ccd29)
* Bundle and use FairCMakeModules (575054a11)
* CMake: Set CXX language level via `target_compile_features()` (7616b0b0a, 42606f9f1)
* Add `codemeta.json` (947c4a73a)
* Add `codemeta_update.py` (415232b56)
* CI: Add a fedora 34 (gcc 11) check (9c3478252)
* CI: Add ubuntu 20.04 (gcc 9) check (2b438452a)
* CMake: Do not unconditionally override settings without good reason (2e655823e, ab54668ae)
* build: Do not hardcode include dirs (8bf9e1d0a)
* build: Fix default for `BUILD_TESTING` (f413aa197)
* test(sdk): Rename `sdk/test_topo.xml` to something less confusing (08d72d492)
* build: Pick shared flatbuffers lib first (8859c563a)
* build: Revert removal of asio find module (170557ace)


## `v1.4.38` - 2021-05-20
#### Changed/Removed
* runDevice: remove const from getDevice parameter (a7dbeadd1)
#### New
* shm: add monitor method to retrieve free segment memory (021c1b1c4)
#### Bugfix
* shm: revert some changes from c85d6e0 that introduced a race (9bf908fb5)
#### Buildsystem/CI
* reduce noise in examples (aaf74ad93)


## `v1.4.37` - 2021-05-07
#### Bugfix
* Fix Ofi interface (e6f67b365)


## `v1.4.36` - 2021-05-07
#### Changed/Removed
* Improve Events API (6dfea32ae, 091d0824d)
* PluginManager: Do not load built-in plugins via dlopen/dlsym (057ba0377)
* use thread local cache to avoid interprocess lock on shm `GetData` (f7ba3052a)
* improve message counter cache line use (ef5b3c782)
* shm: reduce shm contention when dealing with ack queues (c85d6e079)
#### New
* Add `operator<<` for `fair::mq::Transport` (ed2dcedf0)
* Add `GetType()` to `UnmanagedRegion` (9defa7162)
* shmmonitor: add severity setting (b67b80e0a, 8b4056e40)
* add mlock/zero options to unmanaged region (857aa84fa)
#### Bugfix
* Fallback to Boost.Filesystem on GCC 8 (9724f184f)
* Fix `-Wunused-result` (a90dbf64d)
* shm region cache: fix multiple sessions issue (2ca62d06d, 87e0ca545)
* shm: check result of region acquisition (8a2641d84)
* Check transport type of msg and corresponding region (c6a6a5f21)
* shm: eliminate race/deadlock in region subscriptions (2c89b2485)
* region example: fix msg counter (4e466514d)


## `v1.4.35` - 2021-04-08
#### Bugfix
* Fallback to `<boost/filesystem>` on GCC 7 (ea9aede65)


## `v1.4.34` - 2021-04-08
#### Changed/Removed
* shmmonitor: optimize startup to avoid repeated start (72175e575)
* shmmonitor: use fairlogger (be5556561)
* shm: reduce delay between monitor daemon launch & HBs (097646533)
#### New
* Introduce `<fairmq/Device.h>` (cef6d0afc)
* Introduce `<fairmq/runDevice.h>` (978191fa6)
* shmmonitor: allow getting shmids based on session/userid (30e81d58f)
* shmmonitor: distinguish daemon from monitor mode (orthogonal) (522840793)
* shmmonitor: enable read only access (efd42075a)
* shmmonitor: add session name and creator id to the output (effba534f)
* shmmonitor: daemon output to file if `FAIRMQ_SHMMONITOR_VERBOSE` is true (9144258b8)
* Add tools: `StrStartsWith`, `StrEndsWith` (2602f5358)
* shmmonitor: add `--list-all` (5facc441b, f3bc9e05a)
#### Bugfix
* Fix aggregate initialization issue before C++20 (4b6cf8b18)
#### Buildsystem/CI
* CI: Run clang-tidy (21d6cf983, 68ceaba50)
* CI: Collect DDS logs on error (b4aeb320e)


## `v1.4.33` - 2021-03-23
#### Changed/Removed
* Provide a better syntax for `--channel-config` (ce4584b3d, cb4335e59)
#### New
* shm: make `shmId` also available as `uint64_t` (edbdc5733)
* Add `--shm-no-cleanup` option (c8ad684b1)
#### Bugfix
* shm: Make sure all region events are fired (9b48b31a7)
* shm: Make sure no event notifications are missed (0fd2fcadc)
* Fix incorrect channel index passed to `OnData` callback (fc2241ece)
* shm: Improve error message when segment cannot be opened (62438bd99)
* shm: handle shrink failure gracefully (bffe74c5c)
#### Buildsystem/CI
* CI: Adapt to new alfaci build hosts (72f319e27)


## `v1.4.32` - 2021-03-02
#### New
* Add optional file output to `FairMQSink` (bbc1dd460)
#### Bugfix
* Warn on unknown `--channel-config` args (832781094)


## `v1.4.31` - 2021-02-19
#### Bugfix
* Fix regression in shmmonitor (93dff3c5a)


## `v1.4.30` - 2021-02-10
#### New
* shmmonitor: non-interactive mode checks and quits (2b3e38d9a)


## `v1.4.29` - 2021-01-25
#### Changed/Removed
* Bump C++ standard requirement to C++17 (636846fcd)
* Remove deprecated methods in `fair::mq::Device` (ea746b17d):
   * `void Pause()`
   * `bool ChangeState(int)`
   * `void WaitForEndOfState(fair::mq::Transition)`
   * `void WaitForEndOfState(const std::string&)`
   * `bool CheckCurrentState(int)`
   * `bool CheckCurrentState(const std::string&)`
* Rename `TransferResult` to `TransferCode` (29f45fa77)
* Replace `tools::make_unique` with `std::make_unique` (751c53171)
* Remove custom implementation for enum hashing (c5487a11e)
#### New
* DeviceRunner: check `FAIRMQ_SEVERITY` env var for severity (749d28a3b)
* zmq: implement alignment (6815c9c17)
#### Bugfix
* zmq: correct accounting for msg size > 2GB (afadbb53e)
* Fix `shmem::Message::SetUsedSize(0)` (c6b13cd3a)


## `v1.4.28` - 2020-12-04
#### Bugfix
* shm: do mlock/zeroing under shmem lock (f46d446d5)


## `v1.4.27` - 2020-11-25
#### Changed/Removed
* Let single message methods also return `int64_t` (bb1ce794b)
* Bump DDS version to `3.5.3` (multiple sessions support) (0806720f6)
#### Bugfix
* DDS command UI: return `EXIT_FAILURE` on errors (a14502242)
* Apply suggestions from `-Wrange-loop-analysis` (e39d17d09)
* shm: ensure local segments are complete for region subscriptions (db0937f33)
#### Buildsystem/CI
* CI: Update macOS builds (73377c510, f9219dab6, c51e88e11)
* extern/asio: Bump to `1.18.0` (9e2373b55)


## `v1.4.26` - 2020-10-21
#### Changed/Removed
* FairMQChannel: remove deprecated methods (3ab10ced7)
* Remove thread-safety from channel methods (e332e20db)
* FairMQChannel: replace `ResetChannel()` with `Invalidate()` (cf004f69b)
* shmem: fail earlier if given an unsupported socket type (d03a504cc)
* SDK: Remove instance limit from `DDSSession` (f14f50758)
#### New
* Properties: add output support for shorts (0e5978b16)
#### Bugfix
* Make BasicTopology actually movable (8dd0b25c0)
* DeviceRunner: Set log severity only if one was provided (49d8a1b4d)
#### Buildsystem/CI
* Improve PMIx Package Finding on Debian (7edf43691)


## `v1.4.25` - 2020-09-23
#### Bugfix
* Update return types in `ofi::Socket` (1b30f3ac1)


## `v1.4.24` - 2020-09-22
#### Changed/Removed
* Adjust transfer methods behaviour when interrupted (6932f88c8)
#### Bugfix
* Workaround `Cpp17MoveInsertable` issue on xcode 12 (35c7959c5)
#### Buildsystem/CI
* CI: Run macOS checks on newer environment (5e6ad4722)


## `v1.4.23` - 2020-09-08
#### Changed/Removed
* Move `fairmq-dds-command-ui` to SDK (2ac27905e)
* Improve error reporting in SDK/fairmq-dds-command-ui (fe9b87e4e)
* Shm: Refactor to localize segment access (3b2d2a0ac)
* Remove built-in devices from the main lib (12e6a874d)
#### New
* Shm: Provide segment/msg debug infos (70a583d08, b63f31d0e)
* Shm: Add `Monitor::GetDebugInfo()` (72a45f78b)
* Shm: configurable allocation strategy (73109fe6d)
* Shm: Verbosity switch for Cleanup methods (b126ede45)
* Shm: initial multiple segments support (266843cda, fdbf28936)


## `v1.4.22` - 2020-08-28
#### Changed/Removed
* Shm message counter: use relaxed/acquire memory ordering (f1d6b1866)
* Avoid unconditional call to `now()` when allocating message (62ed4e5f8)
* Bump FairLogger requirement to `1.6` (be8ab06cc)
#### New
* SDK: Add helpers for AggregatedTopologyState (b5545c157)
* BenchmarkSampler: add alignment parameter (9c27051cd)
* shmmonitor: add output with `-v` (non-interactive) (b0f73017e)
* ShmMonitor: Use FairLogger for timestamp calculation (86a1dd38a)
#### Bugfix
* Shm: Add error handling for mlock (65f951991)
* Fix tag mismatch in topology files (1f0c94f89)
* Retry on EINTR in blocking zmq calls (690e8a037)


## `v1.4.21` - 2020-07-15
#### Changed/Removed
* SDK: warn if given path translates to no selected tasks (5a7dcd9fc)
#### New
* Add API to extract `ZMQ_EVENTS` from socket backend (e8cc10434, ba3a82b1d, bf909f94d)
* Add `Undefined` and `Mixed` state for use in SDK (a6da208e7)
* SDK: Provide comparison operator for device and topo states (1140c4c6a)
* Shm: add options to zero and/or mlock the segment (beb7766fc)
#### Bugfix
* QC examples: fix incorrect topology path (d5d5c2795)
#### Buildsystem/CI
* Add test for shm transport options (3eca8e9de)


## `v1.4.20` - 2020-06-30
#### Changed/Removed
* Optimize unmanaged region ReceiveAcks (f885b4618)
* Make shmid an 8-digit hex number (6dd0a4430)
#### New
* Add linger setting for unmanaged region (3364da954, d1c51e0f1)
* Expose `fair::mq::shmem::Monitor::Cleanup()` API (539e5602a)
* BenchmarkSampler: add memset option (afe2dcaa0)
#### Bugfix
* Add missing includes (79489bb50)
* Fix message counter in region example (52c6264fa)
* Handle `out_of_range` when locating `RegionInfo` (812548977)
* Do not attemp to find random port for non-TCP protocols (f70201610)
* Shm: throw if requested message size exceeds total segment size (88bc1f7a0)
#### Buildsystem/CI
* Add `-Og` flag to Debug build (9e2a00294)


## `v1.4.19` - 2020-06-05
#### Changed/Removed
* Let default shm segm size be multiple of page size (236d5a860)
* Use SHA2 instead of boost::hash to generate shmem id (36d4f3c93)
#### New
* MemoryResource: propagate alignment (a59c902c7)
* Add ability to retrieve `ZMQ_FD` (dd47b34e0)
#### Bugfix
* Fix regression in handling unregistered options (5008fa473)
* Shm: fix incorrect ptr range check (dabc48c21)
* Zmq: let `GetData` of an empty message return `nullptr` (79ca436b7)
#### Buildsystem/CI
* Add a test for unregisted options (5a782e872)
* Add PicoSHA2 dependency (42986e664, bdf895ae9, c60dd9965)


## `v1.4.18` - 2020-05-25
#### Changed/Removed
* Adjust program options style to disallow guessing (b5bb476b0)
#### Bugfix
* Initiate termination process on `SIGTERM` as well (02692e700, ea7ae0402)


## `v1.4.17` - 2020-05-20
#### Changed/Removed
* Remove nanomsg transport (a15d59c72)
* Zmq: remove global (static) state, refactor (dbdabd23a)
* Replace `exit()`s with exceptions (b56e32eb1)
* Shm: throw on bad_alloc, option to disable (0d03c76a7)
* Do not handle double `SIGTERM` as abort (20544e1f1)
#### New
* Implement bulk callbacks for unmanaged regions (d22023bcb)
* Alignment part I - Interface and shmem send (53a4d17f8)
#### Buildsystem/CI
* Do not search external GTest by default (b32e04db6)


## `v1.4.16` - 2020-05-11
#### New
* Implement region events for zmq (999281182, a3afadb82)
* Add `bool FairMQTransportFactory::SubscribedToRegionEvents()` (c8fc5ad33)
* Add `FairMQTransportFactory::GetId()` (e9318dd23)
#### Bugfix
* shmem region subscriptions: fix race condition (59e32437a)
#### Buildsystem/CI
* Add unit tests for regions (8cfc04721)


## `v1.4.15` - 2020-05-05
#### Bugfix
* Truncate the file used for the region mapping (5a49c5b9b)
* Shmem: Send acks also for local regions (4218c185a)


## `v1.4.14` - 2020-04-28
#### Changed/Removed
* SDK (EXPERIMENTAL):
   * DDS plugin: refactor to load DDS task id only once (cd83efade)
   * DDS plugin: improve error message on unexpected update (96348b846)
   * DDS plugin: Update property instead of set to avoid errors (c7b1304a2)
   * SDK: refactor subscription to allow reuse (c3127f22e)
   * DDSCommandUI: include path argument in `ChangeState` (adc4688f9)
   * Commands: Add task id to subscription status cmds (274ba5ec0)
   * SDK: track state change (un-)subscriptions (036561ab3)
* QC ex: rename qc devices, granular state control (46014118f)
* Plugin manager: extent lifetime of DLLs (5788daa41)
#### New
* SDK (EXPERIMENTAL):
   * Add `SubscriptionHeartbeat` command (330687772)
   * SDK: send heartbeats when subscribed to state changes (5721ea951)
* Add region events subscriptions (e1a113aab)
#### Bugfix
* PMIx plugin: Fix Commands API usage (7cbd15434)


## `v1.4.13` - 2020-03-24
#### Changed/Removed
* Detect network interface of the default route without use of ip (15de80cfd)
* SDK (EXPERIMENTAL):
   * SDK: update `DDSTopology::GetTasks()` (50dacbcdd)
   * SDK::DDSSession: remove channel id to task id association (0e72a9bf5)
   * SDK: Add garbage collection for completed ops (e853d121b)
   * Use SDK in dds-command-ui (e221242f9)
   * Add session id to DDS commands in dds/qc examples (fcd102299)
   * SDK Commands: remove heartbeat commands (c1719eb28, f00519b99)
   * DDS Command UI: remove direct DDS dependency (ced67d895)
   * DDS Plugin: Simplify subchannel bookkeeping (539b088ad)
   * SDK: Refactor `StateChangeOp` and add path parameter (a20ac7af0)
* DDS example: Revert the session renaming (21835cc10)
* Do not report interruption by system call as error (a545bee3b)
#### New
* SDK (EXPERIMENTAL):
   * SDK: Allow passing path to `DDSSession::SendCommand` (f42945b3a)
   * SDK: Add `DDSTopology::GetTasksMatchingPath` (25658370f)
   * SDK: Add `Topology::AsyncSetProperties` (1c8ad03f3)
   * SDK: Add `Topology::AsyncGetProperties` (264a17842)
   * SDK: Allow passing path to `Set/GetProperties` (92af82313)
   * SDK: Support `DYLD_LIBRARY_PATH` from parent env in `sdk::DDSEnv` (3785fd9ff)
   * SDK: Add `WaitForState()` (119cbe37f)
* New QC example (14d6d717a, 8123a6eca)
* Allow undefining LOG macro with FairLogger v1.6.2+ (3d4cd0281)
* Add example and test of a n-to-m topology, incuding sub-channel use (24aabdb85)
#### Bugfix
* SDK (EXPERIMENTAL):
   * SDK: Unsubscribe in the Topology destructor (334d81a1a)
   * DDS plugin: fix exiting timeout (beff0af51)
* Throw an error if shmem receives invalid meta data (0ae53fd7d, 38eb9d22e)
#### Buildsystem/CI
* Fail if build option dependency is not met (c18030030)


## `v1.4.12` - 2020-01-16
#### Changed/Removed
* Shmem: track number of message objects, throw if non-zero at reset (684e711b8)
#### New
* PMIx: Add commands to plugin and command ui (c290c1689)
#### Bugfix
* shmem: properly initialize received multipart messages (b2e027478)
* Fix CIDs 350455, 350453, 350452, 323467, 350451, 350448, 350447, 321250 (718795360, 38bb14e55, a1e0814a9, b6791856f, b6d9c949a, 692ec4e99)
* Avoid deadlock in FairMQChannel operator=, handle self-assignment (9f8a3553b)
#### Buildsystem/CI
* Extend multipart tests to include single part, transfer across >1 channel (5b5fecc99)


## `v1.4.11` - 2019-12-13
#### Changed/Removed
* Support and require DDS `3.0` (50be38619)
#### Buildsystem/CI
* Fix AppleClang `10.0.1` support with Boost.Asio <= `1.68` (4d1e7b9cd)
* Silence warning: unused variable 'a' `[-Wunused-variable]` (f195eeac6)


## `v1.4.10` - 2019-12-09
#### Changed/Removed
* Remove `TransitionTo` from plugin APIs (0f4595b8c)
* DDS plugin: remove static mode (b0b271d1f)
* SDK: Support and require DDS `2.5.100` (f31be6d7a)
#### New
* DDSCommandUI: add support for more commands (5607d4766)
#### Bugfix
* Fix regression in the DDS plugin shutdown handling (073f5e5c0)


## `v1.4.9` - 2019-11-17
#### Changed/Removed
* SDK (EXPERIMENTAL):
   * SDK: optimize `TryChangeStateCompletion` (44bfbe02e)
   * SDK: Replace for loop with `count_if` (47d9e282d)
   * DDS plugin: Prefix channel properties (9544d9665)
   * SDK: optimize container access (d64169a16)
* Removed deprecated methods from `FairMQDevice` (9d30ff25c):
   * `int SendAsync(FairMQMessagePtr&, const std::string&, int)`
   * `int ReceiveAsync(FairMQMessagePtr&, const std::string&, int)`
   * `int64_t SendAsync(FairMQParts&, const std::string&, int)`
   * `int64_t ReceiveAsync(FairMQParts&, const std::string&, int)`
   * `void WaitForInitialValidation()`
#### New
* SDK (EXPERIMENTAL):
   * sdk: Add `DDSCollection` class (456b65871)
   * sdk: Add `DDSTopology::GetCollections()` and extend `DDSTask` (e1b6b804b, 2973ce035)
   * Add `sdk::GroupByCollectionId(TopologyState)` (acbf57d6f, 870d0deae)
   * Add `sdk:GroupByTaskId(TopologyState)` (4b78c472b)
   * Add flatbuffers-based command format to SDK/plugin (a53e95b5f, 1c1509af3, 7502f4b42, 2f69526c0)
   * Commands: support JSON in addition to binary (72a8e9b33)
   * Docs: Add section on the Controller SDK (0d46ffe01)
* PMIx plugin (EXPERIMENTAL):
   * PMIx: Support integer values (e1134321d)
   * PMIx plugin: Add barriers and fix lookup of multiple keys (caeee626a)
* Add safety check in shmem region location (160ee9d06)
#### Bugfix
* control: Handle InitializingDevice in the ShutdownSequence (92112c812)
* SDK (EXPERIMENTAL):
   * DDS plugin: Do not block the DDS KeyValue callback (e3890a403)
   * DDS plugin: Move Boost dependency to intended target (924320a0a)
   * DDS Plugin: reset channel containers before filling (dc72262af)
   * SDK commands: Link shared flatbuffers if no static avail (f6e3183f4)
* Shmem: Fix handling of empty message parts (85a3a254d)
* Fix shmmonitor daemonization (2c6f43685)
* Shm monitor: fix startup race and enable view-only mode (2ac8f9817)
#### Buildsystem/CI
* CMake: Fix working directory for submodule update command (fa394194e)
* CI: Record DDS logs in error cases for analysis (343605899)
* CMake: Support multiple values for `REQUIRES` in `fairmq_build_option` (ea9ad6466)
* Relax required ZeroMQ version to support the CentOS 7 system package (5fc1c47e2)
* Include empty parts in multipart example/test (dd191551c)
* CMake: Add option `ENABLE_CCACHE` (71325828f)


## `v1.4.8` - 2019-09-04
#### Changed/Removed
* FairMQ: Require Boost 1.66 (31edf948d)
* SDK (EXPERIMENTAL):
   * Topology: Use `DDSTask` type, remove unused member (8feffe70b)
   * SDK: Require DDS `2.5.46` and support new Tools API (eaa8f5cbd, 33b5a2a34, 0e5f648d2)
   * SDK: Require C++14, CMake `3.11` and bundle asio `1.13.0` (1dec05910)
#### New
* SDK (EXPERIMENTAL):
   * SDK: Implement helper to find most recent running DDS session (8d12b908b)
   * SDK: Implement asio-compliant asynchronous operation helpers (73af0ed78)
   * SDK: Introduce fairmq error category (25539e99f)
   * SDK: Implement `Topology` with asio-compliant async interface (7a0d348bd)
   * SDK: Add getter for agent path (c1a17c97b)
* Tools: Add a copyable SharedSemaphore (5d6184cd1)
#### Bugfix
* SDK (EXPERIMENTAL):
   * SDK: Fix completion signature and catch completion exceptions (0f50abf3d)
   * SDK: Fix race condition which lead to frequent segfaults on destruction (5b47df301)
   * SDK: Fix data races on the local semaphores (1c49dde66)
   * DDS plugin: Wait for `IDLE->EXITING` state-change to be acknowledged (8a2c7fb60)
* fairmq: Fix `-Wdefaulted-function-deleted` (0d182dc18)
#### Buildsystem/CI
* Execute test for DDS example only if DDS was found (2fc93994d)
* Example.DDS: Support DDS `2.5.25+` CLI (7f0237d97)
* Print install prefix in cmake summary (be06a5629)
* Add support for fairlogger dependencies (02b20c320)
* CMake: Add find module for asio (d6d9312e5)
* CMake: Bundle GTest (88ff5d8fc)
* CMake: Do not use git version in install tree (f73a6d71e)
* CMake: Fix submodule update command (75a3a80ac)
* CMake: Issue a warning if build option requirements are not met (1055f035f, 46e0796e7)
* Tests.SDK: Add optional DDS Tools API stability tests (da28b8549)
* CMake: Add config for `--graphviz` (3f5374820)


## `v1.3.9` - 2019-08-07
#### Bugfix
* Tools: Fix regression bug to support Boost < 1.66 (03e6cea)
#### Buildsystem/CI
* Support recent releases of Boost, CMake, DDS and FairLogger:
   * CMake: Support DDS 2.4 (d91a7d2)
   * FairMQ: Remove unused dependency on Boost.Signals v1 (c3f6d5c)
   * CMake: Set policies by version (de7b576)
   * CMake: Fix summary table for recent FairLogger versions (7506243)


## `v1.4.7` - 2019-07-29
#### Changed/Removed
* PluginServices: Do not throw if device control cannot be released (008be3612)
#### New
* SDK (EXPERIMENTAL):
   * SDK: Track channel to task id association (fd282fa95)
   * SDK: Improve error handling in case state-change fails on a device (7b773cde5)
   * DDS plugin: Synchronize FillChannelContainers and DDSKeyValue updates (9cbccface)
   * Tests.SDK: Add another test (07fe02a0a)
   * DDS plugin: Automatically set session and device id if not provided (1bb77bf47)
#### Bugfix
* Example.DDS: Make example topologies pass xml validation (1fa82f5f2)
#### Buildsystem/CI
* Example.DDS: Run example as unit test (7316b0e7f)


## `v1.4.6` - 2019-07-26
#### Changed/Removed
* Remove deprecated `*Async()` methods (73ccefa4c)
* Remove unused methods from `FairMQProgOptions` (f46803a8b)
* FairMQChannel: defaults values, copy-ability (fbb003b50)
* Remove FairMQChannel from parser, implement bulk operations (bf8ec968e)
* only allow `a-zA-Z0-9-_[]#` as channel name characters (3db32b03d)
* Extract States & Transitions to own header, use in plugins (4487b81de)
* Extract state queue into own class. Use in device, plugins (f515eb110)
#### Deprecated
* Deprecate `Channel::GetChannelX()` in favor of `Channel::GetX()` (3538d9f41)
#### New
* New component SDK (EXPERIMENTAL), `-DBUILD_SDK` (3da5f4d5d, fd682c339, 1a93da5be, 90496c89f, cfcdd666b, 53be96d93, 44da054e7, b5da31498, eb9dcdd1f, a9b478875, 7aa95fcd9, 5a7bf68c8, 152c8431c, 8aad6c062, b58f6ede7, 18dc536f3, cf26dd6aa, 8789664cf, be022cfab, bc98ab1ee, a93840b24, d70a20344, 388b1be05, dc5527231, 5d535163f, ac8cd1991, 5ab328b01, 363576496, 377eaf2bb, 6208cbb50, 98aeb16dc):
   * SDK: Require DDS `2.5.20` (de4fca177)
   * DDS plugin: Implement `--control external` (f7cdf5ee2)
   * DDS plugin: Do not auto-transition after Initializing (b54df715e, 6c07920fc)
   * SDK: Add convenience header `<fairmq/SDK.h>` (3f655e330)
* Shmem region: support huge pages via path to hugetlbfs mount (0e35f1cb2)
* `fairmq/ProgOptions`:
   * Add `GetProperties()` (5646d531f)
   * Add `*Property` methods to replace `*Value` methods, simplify options helper (fe241fe9e)
   * Add `SetProperties()` (29313bbec)
   * Add `DeleteProperty()` (4ce378b6b)
   * Add `UpdateProperty/ies()` (7c9744760)
* Add built-in config plugin (cba6d1978)
* Subscribe `DeviceRunner` for config properties (4281d7b27)
* Add `PluginServices::DeleteProperty` (7bea2bc0e)
* `fairmq-dds-command-ui`:
   * Introduce wait mode (66c01f2cd, d966a0a99, 14980d748)
   * Example.DDS: Add start script (b53691c8a, d4f96db69, 5e8a44234, ff581985f)
* Implement `Device::TransitionTo()` and test it (74d301a16, 5256e7c58)
* Tools: Introduce semaphore (99ed61a58)
#### Bugfix
* Include definitions of `constexpr` members in cxx (7c13503e2)
* Declare `operator<<` for `boost::any` in boost namespace (7486249c1)
* Test.PluginServices: Fix -Wdeprecated-copy (051f064c6)
* Tools: Fix various clang-tidy warnings (4351b98d8, 69268eecf)
* ProgOptions: Fix minor things (12f05355d)
* Fix `WaitForState(string stateName)` (418d42b06)
* Clear channels & transports containers in `Reset()` (937c9e892)
* Initialize initialization timeout in `InitWrapper()` (48e04b636)
* Fix -Winconsistent-missing-override (a60edc43d)
* Fix DDS example (6e353d78b, e3d39f39f, 790de84bb, 7002dcbca, 2c4a6674d)
* Tests.Message: Fix race condition (a65f0e677)
* Fix various clang-tidy warnings (499ffcd30)
#### Buildsystem/CI
* Add additional test for running device with plugins without DeviceRunner (5271d4236)
* Add test for property accessors (7bea2bc0e)
* Add tests for property conversions and subscriptions (4cefb9fb5)


## `v1.4.5` - 2019-07-02
#### Changed/Removed
* Require DDS `2.4` (3358a2ba1)
#### New
* Docs: Add section about plugins (374eb8403)
#### Buildsystem/CI
* CMake: Export build type and cxx flags (479c16a8f)
* CMake: Add Boost version requirement of DDS (a866c6d93)
* CMake: Support Boost `1.70` config mode (09853e303)
* CMake: Support FairLogger `v1.5.0` (4d53b7c02)


## `v1.4.4` - 2019-06-13
#### New
* Add array variant of `fair::mq::tools::make_unique()` (9b326c7a7)
* SHMEM transport: Batch Region ack messages (2ed217755)
* Print channel name in error log (40f6db430)
#### Changed/Removed
* Allow `PluginServices::SetProperty` in Ready state (7b16c33cc)
#### Bugfix
* Handle invalid transition to init in the control plugin (9b4c5deb0)
#### Buildsystem/CI
* Extend Readout example (7e6eb382d)
* CMake: Set policies by version (f8f997abe)
* CMake: Support running static analysis (660420e4f)
* DDS plugin: Remove unused dependency (99c8d3319)


## `v1.4.3` - 2019-04-29
#### New
* OFI transport (EXPERIMENTAL):
   * Add experimental static size mode, `--ofi-size-hint` option (3582091b1)


## `v1.4.2` - 2019-03-21
#### Changed/Removed
* OFI transport (EXPERIMENTAL):
   * Require asiofi `v0.3.1` (fb42b1e2f, 2457094b6)
   * Try to reconnect on refused connection (a8f1a4dfd)
* Use Asio to launch processes in `fair::mq::tools::execute` (922f7e9a9)
#### New
* Allow creating region with a callback with default transport (44a9946ea)
* Extending `FairMQParts` by a constructor taking list of `FairMQMessagePtr` (696257fd4)
* Process tools: add print helper, support signals (ef4d6a331)
#### Bugfix
* Fix broken pipe errors in `tools::execute` (cdc1ba084)
* Fix mismatch of docs/API, wrong return value (6809d60fa)
* OFI transport (EXPERIMENTAL): Fix recv logic (cc8fd7302)
#### Buildsystem/CI
* Extend tests of error cases (dd02c01c3)
* Run multipart example with ofi (90fdcc26b)


## `v1.4.1` - 2019-03-06
#### Buildsystem/CI
* Fix regression in exporting required dependency components


## `v1.4.0` - 2019-03-06
#### Changed/Removed
* Remove obsolete dependency to boost signals v1 (e2e476ba1)
* Load dynamic plugins with `RTLD_GLOBAL` flag (0c54aab19)
* State machine changes (fc94342db, ec519cb31, d9edcad84):
   * Split `INITIALIZING` state into Init+Bind+Connect
   * Remove `PAUSE` state
   * Convert state/transitions to enum classes (CamelCase)
   * Transition to a state only once previous handler is complete
   * Add `CompleteInit` transition to notify `Initializing` state
* Remove obsolete `FindFlatBuffers.cmake` (a670b4bbf)
* OFI transport (EXPERIMENTAL):
   * Depend on asiofi `v0.2.0` (4d7a1c81c, fc778ab3b)
   * Drop protobuf dependencies (03912e86f)
   * Deactivate control band monitor socket (91025cbc8)
   * Require C++14 (35dd9578a, 9ae48c21f)
* Decrease severity of config dump from `info` to `debug` (9ffaa5518)
#### Deprecated
* Deprecate `WaitForEndOfState(transition)` in favor of `WaitForState(state)/WaitForNextState()` (fc94342db)
* Deprecate `CheckCurrentState()` in favor of `NewStatePending()` (fc94342db, b7125b746)
#### New
* PMIx plugin proof-of-concept, build with `-DBUILD_PMIX_PLUGIN=ON`, off by default. (1191c3cda, 2358d7b03, 61a3da869, 2047dbef5, dfc6b5c4a, 62781389d)
* Add `--max-run-time` option (8375faf83)
* OFI transport (EXPERIMENTAL):
   * Implement connection mgmt (ba4e6f72c)
   * Implement ofi Send/Receive (b394feca1, 60f1f1000)
   * Implement parallel `ofi::Socket::Send` (46e242054, 672e12f45)
   * Implement parallel `ofi::Socket::Receive` (8e7cfacd7, 672e12f45)
   * Implement control band with asiofi (b31ab1cc4)
   * Retry ofi connection (241bf0833)
   * Implement Reset signal (ee890a7a4)
* Enhance region example with Builder device (7df278818)
#### Bugfix
* Search plugins in system directories and `LD_LIBRARY_PATH` (#133)
* Fix bug in `LogSocketRates` (8375faf83)
#### Buildsystem/CI
* Test macOS `10.14` (8ee989dbc)
* Set cmake policy `CMP0074` (a262d4684)
* Fix link errors with Boost (f85663bfe)


## `v1.3.8` - 2019-01-07
[bug fixes]
* 291d00c Fix regression with ignored rateLogging channel argument
* 0cb8f61 Let getMessage deal with SSO

[enhancement]
* 4dc37ef Allow implicit conversion factory->resource


## `v1.3.7` - 2018-11-29
[new features]
* 5e4876c Allow plugins to create channels
* 310b964 Adopt FairMQMessage backed memory resource collection from AliceO2
* 3a1b769 Support feeding the child process data on stdin
* 4123ebc Add interruptable FairMQDevice::WaitFor(duration) method
* e95096e Add RateLimiter class to tools
* 4aae1ad Allow DDS command UI to target parts of the topology

[enhancements]
* 653e82c Avoid copy (where possible) when switching transports
* 25fcf13 Move Bind/Connect/Attach to FairMQChannel
* 3ca0d72 Add safety checks for process tools
* 44acd49 Implement nanomsg linger in our transport
* dfa1b68 Make factory classes final (optimization potential)
* e090967 Build shmem names out of session id + user id (avoid conflicts)
* 1fdf510 Pick correct build type in CI
* 45354f2 Use future instead of thread for device rateLogger (exception safety)
* cf9a294 Introduce and export hotfix version component
* 94297f9 Tie builtin plugin version to the project version
* 88f8975 Add codacy.com integration
* 60f27b9 Support BUILD_SHARED_LIBS flag
* 1bb558a Refactor initialization - config always available, proper reinit on Init() re-entry
* c8bd19b Add experimental FAST_BUILD option
* fc0adba Support unity build
* a53ef79 Run state handlers on the main thread (breaking change for control).
* f05118f Make ";" the separateor in multi-point channel config
* ffbe90b Update to DDS 2.2 API

[bug fixes]
* 227a302 Avoid boost::uuids::entropy_error on some systems
* 3561255 Add missing channel update handlers (snd/rcvKernelSize)
* 9f32545 Make sure we reset terminal config also on exception
* cb199e7 Fix throw after quit signal case
* 1aab354 Resolve hanging process in case of uncaught exception
* f6c1f5d Fix theoretical race in signal handler
* adfa0e2 Move shmmonitor out of lib to own executable (Fix ODR violation)
* 2c6b2e7 Fix race condition in the control plugin
* b814e40 Fix multipart transfer timeout
* ee8afd7 Fix race in plugin manager/services

[cleanup and deprecations]
* 0cfa919 Deprecate Send-/ReceiveAsync, use timeout variant instead
* ce4062f Remove GetSocket interface that exposes transport details
* 00800f1 Remove support for nanomsg <= 0.6
* cfb7271 Remove set/get timeout from general socket interface
* 9851504 Remove shmem prototype code - unused
* a53ef79 Run state handlers on the main thread (breaking change for control).
* 90009b1 Remove previously deprecated Copy method (since 1yr)

[tests]
* e403d18 Add codecov reports to PRs
* a9df367 Add test for FairMQMessage::Rebuild
* bd899a2 Add test for channel validation
* 0b199e7 Add test for interface IP detection tools
* 919193a Extend transfer timeout test coverage
* d4a4ea1 Add example/test for built-in devices
* ffab4ac Add options tests and (re-)/enable more nanomsg tests
* 1d45095 Add session id to example tests
* 78acb95 Test more cases with interactive controller
* e39316c Test exceptions thrown in user code
* c4145e9 Add test for FairMQDevice::WaitFor()
* 5397cef Use same runner.cxx/.h for all tests
* b814e40 Enable multipart transfer timeout tests
* 7d5e76d Add more tests for custom main()
* 24dff2f Enable FAST_BUILD for alfa-ci
* 3f96181 Define nn tests only when nn transport is built

[dependencies]
* c02fbed Require msgpack 3.1.0
* 76aeb2c Require nanomsg 1.1.3
* ffbe90b Require DDS 2.2

Many more small fixes and improvements.


## `v1.2.3` - 2018-05-22
* [bugfix] Remove the alias target
* [enhancement] Improve compilation speed
* [new feature] Control plugin: add ability to switch log levels interactively
* [enhancement] provide `FAIRMQ_PATH` to test suites
* [enhancement] further shorten shm names
* [test coverage] Test all examples with all possible transports
* [enhancement] Use enum transport types instead of strings in Channel/Device
* [enhancement] Fix namespaces in Transports.h and add conversion map
* [enhancement] Used cached default transport in `FairMQDevice::Transport()`


## `v1.2.1` - 2018-05-04
* [bugfix] error: no matching function for call to 'find_if' with Boost 1.66+ (#29)


## `v1.2.0` - 2018-05-04
First stable release after separating FairMQ from the FairRoot repo.

since [FairRoot 17.10](https://github.com/FairRootGroup/FairRoot/releases/tag/v-17.10):

* [bugfix] `fair::mq::tools::getDefaultRouteNetworkInterface()` does not break any more with multiple default routes on Linux
* [enhancement] `FindDDS.cmake` now generates imported targets
* [bugfix] Install DDS plugin and `fairmq-dds-command-ui`
* [new feature] Add `dump-config`, heartbeat and state subscription commands to DDS plugin
* [new feature] With `--rate` one can set now a rate limit in Hz for `FairMQDevice::ConditionalRun()`
* [new feature] Add `fair::mq::PluginServices::Un/SubscribeToPropertyChangeAsString()`
* [new feature] Add callbacks with hint argument to `FairMQUnmanagedRegion` to transfer ownership of the message buffer back to the user
* [new feature] With `--session` it is supported now to run multiple topologies in parallel on the same machine
* [test coverage] Run `examples/Region` in the standard test suite
* [new feature] Add `fairmq-uuid-gen` utility to easily generate session ids
* [change] Remove deprecated header `<tools/FairMQTools.h>`
* [bugfix] Add missing explicit reference to `libboost_signals`
* [new feature] Add `FairMQMessage::SetUsedSize()`
* [change] Remove `FairMQMessage::Get/SetMessage()`
* [change] Deprecated `FairMQMessage::Copy(const std::unique_ptr<FairMQMessage>&)`, use `FairMQMessage::Copy(const FairMQMessage&)` instead
* [enhancement] Reimplement and move `FairMQLogger` to external [repo](https://github.com/FairRootGroup/FairLogger)
* [bugfix] Convert log severities to lower case to avoid name clashes with preprocessor defines
* [change] Remove `pstreams` dependency and replace it with Boost.Process
* [bugfix] Fix DDS plugin incomplete shutdown on DDS server termination
* [new feature] DDS plugin: handle n to m on single channel name case
* [enhancement] SHM: coalesce multipart messages. Transmit meta info of an shm multipart message in a single zmq message
* [new feature] Add `fair::mq::PluginServices::CountProperty()`
* [enhancement] Update `fairmq-dds-command-ui` to support DDS 2.0
* [experimental] Add a first implementation of the new OFI transport. This is mainly useful for internal testing.
* [enhancement] Replace cmd socket with transport specific unblock mechanism
* [enhancement] Modernize CMake files for `examples`
* [enhancement] Devices search `FAIRMQ_PATH` in addition to `PATH` for binaries
* [change] Prefix all FairMQ executables with `fairmq-`, e.g.: `splitter` -> `fairmq-splitter`
