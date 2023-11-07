// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/stator/renderer/swapchain.h"

#include "impeller/stator/renderer/context.h"

namespace impeller::stator {

Swapchain::Swapchain(const std::shared_ptr<Context>& context)
    : context_(context) {
  is_valid_ = true;
}

Swapchain::~Swapchain() = default;

bool Swapchain::IsValid() const {
  return is_valid_;
}

}  // namespace impeller::stator
