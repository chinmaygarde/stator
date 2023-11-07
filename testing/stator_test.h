// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>

#include "flutter/fml/macros.h"
#include "flutter/runtime/dart_vm_lifecycle.h"
#include "flutter/testing/dart_fixture.h"
#include "impeller/playground/playground_test.h"
#include "impeller/stator/renderer/context.h"

namespace impeller::stator::testing {

class StatorTest : public PlaygroundTest, public flutter::testing::DartFixture {
 public:
  StatorTest();

  ~StatorTest();

  // |PlaygroundTest|
  void SetUp() override;

  // |PlaygroundTest|
  void TearDown() override;

  const std::shared_ptr<Context>& GetStatorContext() const;

 private:
  fml::Thread ui_thread_;
  fml::Thread io_thread_;
  fml::Thread raster_thread_;
  flutter::Settings settings_;
  std::unique_ptr<flutter::DartVMRef> vm_;
  std::unique_ptr<flutter::TaskRunners> task_runners_;
  std::shared_ptr<flutter::DartIsolate> root_isolate_;
  std::shared_ptr<Context> context_;

  FML_DISALLOW_COPY_AND_ASSIGN(StatorTest);
};

}  // namespace impeller::stator::testing
