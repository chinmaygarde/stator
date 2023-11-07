// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>

namespace impeller::stator {

class Context;

class Swapchain {
 public:
  explicit Swapchain(const std::shared_ptr<Context>& context);

  ~Swapchain();

  bool IsValid() const;

  Swapchain(const Swapchain&) = delete;

  Swapchain& operator=(const Swapchain&) = delete;

 private:
  std::weak_ptr<Context> context_;
  bool is_valid_ = false;
};

}  // namespace impeller::stator
