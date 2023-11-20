// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/stator/testing/stator_test.h"

#include "flutter/runtime/isolate_configuration.h"
#include "impeller/stator/renderer/compositor.h"
#include "impeller/stator/renderer/renderer.h"

namespace impeller::stator::testing {

static fml::RefPtr<fml::TaskRunner> GetDefaultTaskRunner() {
  fml::MessageLoop::EnsureInitializedForCurrentThread();
  return fml::MessageLoop::GetCurrent().GetTaskRunner();
}

static std::shared_ptr<flutter::DartIsolate> CreateRunningRootIsolateOnUIRunner(
    const flutter::TaskRunners& task_runners,
    const flutter::Settings& settings,
    const flutter::DartVMData& vm_data) {
  FML_CHECK(task_runners.GetUITaskRunner()->RunsTasksOnCurrentThread());

  auto isolate_configuration =
      flutter::IsolateConfiguration::InferFromSettings(settings);

  flutter::UIDartState::Context context(task_runners);

  context.advisory_script_uri = "main.dart";
  context.advisory_script_entrypoint = "main";

  auto weak_isolate = flutter::DartIsolate::CreateRunningRootIsolate(
      vm_data.GetSettings(),               // settings
      vm_data.GetIsolateSnapshot(),        // isolate snapshot
      nullptr,                             // platform configuration
      flutter::DartIsolate::Flags{},       // flags
      nullptr,                             // root_isolate_create_callback
      settings.isolate_create_callback,    // isolate create callback
      settings.isolate_shutdown_callback,  // isolate shutdown callback
      "main",                              // dart entrypoint
      std::nullopt,                        // dart entrypoint library
      {},                                  // dart entrypoint arguments
      std::move(isolate_configuration),    // isolate configuration
      context                              // engine context
  );

  auto root_isolate = weak_isolate.lock();
  FML_CHECK(root_isolate);
  FML_CHECK(root_isolate->GetPhase() == flutter::DartIsolate::Phase::Running);
  return root_isolate;
}

StatorTest::StatorTest() {}

StatorTest::~StatorTest() {
  fml::AutoResetWaitableEvent latch;
  fml::TaskRunner::RunNowOrPostTask(task_runners_->GetUITaskRunner(), [&]() {
    FML_CHECK(root_isolate_->Shutdown());
    root_isolate_.reset();
    latch.Signal();
  });
  latch.Wait();
}

void StatorTest::SetUp() {
  // Setup the playground. This sets up the WSI we need for the context.
  PlaygroundTest::SetUp();

  // Setup the context. This sets up the FFI for the isolate.
  context_ = Context::Make(GetContext());
  renderer::SetGlobalContext(objffi::Make<renderer::FFIContext>(context_));

  // Setup the isolate.
  settings_ = CreateSettingsForFixture();
  settings_.log_tag = "Stator";

  FML_CHECK(!flutter::DartVMRef::IsInstanceRunning());
  vm_ = std::make_unique<flutter::DartVMRef>(
      flutter::DartVMRef::Create(settings_));
  FML_CHECK(flutter::DartVMRef::IsInstanceRunning());
  auto vm_data = vm_->GetVMData();
  FML_CHECK(vm_data);

  task_runners_ = std::make_unique<flutter::TaskRunners>(
      flutter::testing::GetCurrentTestName(),  // label
      GetDefaultTaskRunner(),                  // platform runner
      raster_thread_.GetTaskRunner(),          // raster runner
      ui_thread_.GetTaskRunner(),              // ui runner
      io_thread_.GetTaskRunner()               // io runner
  );

  fml::AutoResetWaitableEvent latch;
  fml::TaskRunner::RunNowOrPostTask(task_runners_->GetUITaskRunner(), [&]() {
    root_isolate_ =
        CreateRunningRootIsolateOnUIRunner(*task_runners_, settings_, *vm_data);
    latch.Signal();
  });
  latch.Wait();
}

void StatorTest::TearDown() {
  context_.reset();
  PlaygroundTest::TearDown();
}

const std::shared_ptr<Context>& StatorTest::GetStatorContext() const {
  return context_;
}

bool StatorTest::OpenStatorPlaygroundHere(const StatorCallback& callback) {
  if (!callback) {
    return false;
  }

  return OpenPlaygroundHere([&](RenderPass& pass) {
    return callback() && context_->GetCompositor()->Composite(pass);
  });
}

}  // namespace impeller::stator::testing
