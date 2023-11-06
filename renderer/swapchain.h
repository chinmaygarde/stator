// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

namespace impeller::stator {

class Swapchain {
 public:
  Swapchain();

  ~Swapchain();

  Swapchain(const Swapchain&) = delete;

  Swapchain& operator=(const Swapchain&) = delete;
};

}  // namespace impeller::stator
