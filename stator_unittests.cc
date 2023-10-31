// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/playground/playground_test.h"

namespace impeller {
namespace stator {
namespace testing {

using StatorTest = PlaygroundTest;

TEST_P(StatorTest, CanSetupPlayground) {}

INSTANTIATE_TEST_SUITE_P(
    Stator,
    StatorTest,
    ::testing::Values(PlaygroundBackend::kMetal),
    [](const ::testing::TestParamInfo<PlaygroundTest::ParamType>& info) {
      return PlaygroundBackendToString(info.param);
    });

}  // namespace testing
}  // namespace stator
}  // namespace impeller
