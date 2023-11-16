// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>

#include "impeller/renderer/context.h"

namespace impeller::stator {

class Swapchain;
class Compositor;

class Context final : public std::enable_shared_from_this<Context> {
 public:
  static std::shared_ptr<Context> Make(
      std::shared_ptr<impeller::Context> context);

  ~Context();

  bool IsValid() const;

  Context(const Context&) = delete;

  Context& operator=(const Context&) = delete;

  const std::shared_ptr<Swapchain>& GetSwapchain() const;

  const std::shared_ptr<Compositor>& GetCompositor() const;

  const std::shared_ptr<impeller::Context>& GetImpellerContext() const;

 private:
  std::shared_ptr<impeller::Context> context_;
  std::shared_ptr<Swapchain> swapchain_;
  std::shared_ptr<Compositor> compositor_;

  bool is_valid_ = false;

  Context();

  void Setup(std::shared_ptr<impeller::Context> context);
};

}  // namespace impeller::stator
