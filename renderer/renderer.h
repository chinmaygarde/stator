// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "impeller/stator/objffi/scoped_object.h"
#include "impeller/stator/renderer/context.h"
#include "impeller/stator/renderer/swapchain.h"
#include "renderer.epoxy.h"

namespace impeller::stator::renderer {

class FFIContext final : public objffi::Object {
 public:
  explicit FFIContext(std::shared_ptr<Context> context)
      : context_(std::move(context)) {}

  const std::shared_ptr<Context>& Get() const { return context_; }

 private:
  std::shared_ptr<Context> context_;
};

class FFISwapchain final : public objffi::Object {
 public:
  explicit FFISwapchain(std::shared_ptr<Swapchain> swapchain)
      : swapchain_(std::move(swapchain)) {}

 private:
  std::shared_ptr<Swapchain> swapchain_;
};

void SetGlobalContext(objffi::ScopedObject<FFIContext> context);

}  // namespace impeller::stator::renderer
