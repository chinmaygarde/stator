// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/playground/playground_test.h"
#include "impeller/stator/testing/stator_test.h"

namespace impeller::stator::testing {

TEST_P(StatorTest, CanSetupPlayground) {
  OpenPlaygroundHere([](const RenderPass& pass) { return true; });
}

INSTANTIATE_TEST_SUITE_P(
    Stator,
    StatorTest,
    ::testing::Values(PlaygroundBackend::kMetal),
    [](const ::testing::TestParamInfo<PlaygroundTest::ParamType>& info) {
      return PlaygroundBackendToString(info.param);
    });

}  // namespace impeller::stator::testing
