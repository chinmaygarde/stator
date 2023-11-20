// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/stator/renderer/renderer.h"

namespace impeller::stator::renderer {

static objffi::ScopedObject<FFIContext> gContext;

void SetGlobalContext(objffi::ScopedObject<FFIContext> context) {
  gContext = std::move(context);
}

FFIContext* GetGlobalContext() {
  return gContext.Get();
}

FFISwapchain* ContextSwapchainCopy(FFIContext* context) {
  if (!context) {
    return nullptr;
  }

  return objffi::Make<FFISwapchain>(context->Get()->GetSwapchain()).Leak();
}

FFITexture* SwapchainNextDrawableNew(FFISwapchain* swapchain_ptr) {
  auto swapchain = objffi::Ref(swapchain_ptr);
  if (!swapchain) {
    return nullptr;
  }
  return objffi::Make<FFITexture>(swapchain->Get()->AcquireNextDrawable())
      .Leak();
}

uint32_t SwapchainPresentDrawable(FFISwapchain* swapchain_ptr,
                                  FFITexture* texture_ptr) {
  auto swapchain = objffi::Ref(swapchain_ptr);
  auto texture = objffi::Ref(texture_ptr);
  if (!swapchain || !texture) {
    return 1u;
  }
  return swapchain->Get()->PresentDrawable(texture->Get()) ? 0u : 1u;
}

}  // namespace impeller::stator::renderer
