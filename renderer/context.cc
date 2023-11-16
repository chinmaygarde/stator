// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/stator/renderer/context.h"

#include "impeller/base/validation.h"
#include "impeller/stator/renderer/compositor.h"
#include "impeller/stator/renderer/swapchain.h"

namespace impeller::stator {

std::shared_ptr<Context> Context::Make(
    std::shared_ptr<impeller::Context> impeller_context) {
  if (!impeller_context) {
    VALIDATION_LOG << "Impeller context to be wrapped was null.";
    return nullptr;
  }
  auto context = std::shared_ptr<Context>(new Context());
  context->Setup(std::move(impeller_context));
  if (!context->IsValid()) {
    VALIDATION_LOG << "Stator context was null.";
    return nullptr;
  }
  return context;
}

Context::Context() = default;

Context::~Context() {
  if (swapchain_) {
    swapchain_->Shutdown();
  }
}

bool Context::IsValid() const {
  return is_valid_;
}

const std::shared_ptr<Swapchain>& Context::GetSwapchain() const {
  return swapchain_;
}

const std::shared_ptr<Compositor>& Context::GetCompositor() const {
  return compositor_;
}

void Context::Setup(std::shared_ptr<impeller::Context> context) {
  context_ = std::move(context);
  if (!context_) {
    VALIDATION_LOG << "Impeller context was null.";
    return;
  }

  auto swapchain = std::make_shared<Swapchain>(shared_from_this());
  if (!swapchain->IsValid()) {
    VALIDATION_LOG << "Could not create Stator swapchain.";
    return;
  }

  auto compositor = std::make_shared<Compositor>(context_, swapchain);
  if (!compositor->IsValid()) {
    VALIDATION_LOG << "Could not create compositor.";
    return;
  }

  swapchain_ = std::move(swapchain);
  compositor_ = std::move(compositor);
  is_valid_ = true;
}

const std::shared_ptr<impeller::Context>& Context::GetImpellerContext() const {
  return context_;
}

}  // namespace impeller::stator
