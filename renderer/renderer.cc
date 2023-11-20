// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/stator/renderer/renderer.h"

#include "impeller/base/config.h"
#include "impeller/base/validation.h"

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

bool SwapchainPresentDrawable(FFISwapchain* swapchain_ptr,
                              FFITexture* texture_ptr) {
  auto swapchain = objffi::Ref(swapchain_ptr);
  auto texture = objffi::Ref(texture_ptr);
  if (!swapchain || !texture) {
    return false;
  }
  return swapchain->Get()->PresentDrawable(texture->Get());
}

FFIRenderTarget* RenderTargetNew() {
  return objffi::Make<FFIRenderTarget>().Leak();
}

bool RenderTargetSetColorAttachment(FFIRenderTarget* p_target,
                                    FFIColorAttachment* p_color,
                                    uint32_t index) {
  auto target = objffi::Ref(p_target);
  auto color = objffi::Ref(p_color);
  if (!target || !color) {
    return false;
  }
  if (!color->attachment.IsValid()) {
    VALIDATION_LOG << "Color attachment was invalid.";
    return false;
  }
  target->render_target.SetColorAttachment(color->attachment, index);
  return true;
}

bool RenderTargetSetDepthAttachment(FFIRenderTarget* p_target,
                                    FFIDepthAttachment* p_depth) {
  auto target = objffi::Ref(p_target);
  auto depth = objffi::Ref(p_depth);
  if (!target || !depth) {
    return false;
  }
  if (!depth->attachment.IsValid()) {
    VALIDATION_LOG << "Depth attachment was invalid.";
    return false;
  }
  target->render_target.SetDepthAttachment(depth->attachment);
  return true;
}

bool RenderTargetSetStencilAttachment(FFIRenderTarget* p_target,
                                      FFIStencilAttachment* p_stencil) {
  auto target = objffi::Ref(p_target);
  auto stencil = objffi::Ref(p_stencil);
  if (!target || !stencil) {
    return false;
  }
  if (!stencil->attachment.IsValid()) {
    VALIDATION_LOG << "Depth attachment was invalid.";
    return false;
  }
  target->render_target.SetStencilAttachment(stencil->attachment);
  return true;
}

}  // namespace impeller::stator::renderer
