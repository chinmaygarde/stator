// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>

#include "impeller/renderer/context.h"
#include "impeller/stator/renderer/swapchain.h"

namespace impeller::stator {

class Context final : public std::enable_shared_from_this<Context> {
 public:
  static std::shared_ptr<Context> Make(
      std::shared_ptr<impeller::Context> context);

  ~Context();

  bool IsValid() const;

  Context(const Context&) = delete;

  Context& operator=(const Context&) = delete;

  const std::shared_ptr<Swapchain> GetSwapchain() const;

 private:
  std::shared_ptr<impeller::Context> context_;
  std::shared_ptr<Swapchain> swapchain_;
  bool is_valid_ = false;

  Context();

  void Setup(std::shared_ptr<impeller::Context> context);
};

}  // namespace impeller::stator
