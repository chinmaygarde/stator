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

bool SwapchainPresentDrawable(FFISwapchain* swapchain, FFITexture* texture) {
  if (!swapchain || !texture) {
    return false;
  }
  return swapchain->Get()->PresentDrawable(texture->Get());
}

FFIRenderTarget* RenderTargetNew() {
  return objffi::Make<FFIRenderTarget>().Leak();
}

bool RenderTargetSetColorAttachment(FFIRenderTarget* target,
                                    FFIColorAttachment* color,
                                    uint32_t index) {
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

bool RenderTargetSetDepthAttachment(FFIRenderTarget* target,
                                    FFIDepthAttachment* depth) {
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

bool RenderTargetSetStencilAttachment(FFIRenderTarget* target,
                                      FFIStencilAttachment* stencil) {
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

FFIColor* ColorAlloc() {
  return reinterpret_cast<FFIColor*>(std::calloc(1u, sizeof(FFIColor)));
}

void ColorFree(FFIColor* color) {
  return free(color);
}

bool ColorAttachmentSetTexture(FFIColorAttachment* color, FFITexture* texture) {
  if (!color) {
    return false;
  }

  color->attachment.texture = texture ? texture->Get() : nullptr;
  return true;
}

bool ColorAttachmentSetResolveTexture(FFIColorAttachment* color,
                                      FFITexture* texture) {
  if (!color) {
    return false;
  }
  color->attachment.resolve_texture = texture ? texture->Get() : nullptr;
  return true;
}

constexpr impeller::LoadAction ToLoadAction(LoadAction action) {
  switch (action) {
    case LoadAction::DontCare:
      return impeller::LoadAction::kDontCare;
    case LoadAction::Load:
      return impeller::LoadAction::kLoad;
    case LoadAction::Clear:
      return impeller::LoadAction::kClear;
  }
  return impeller::LoadAction::kClear;
}

bool ColorAttachmentSetLoadAction(FFIColorAttachment* color,
                                  LoadAction load_action) {
  if (!color) {
    return false;
  }
  color->attachment.load_action = ToLoadAction(load_action);
  return true;
}

constexpr impeller::StoreAction ToStoreAction(StoreAction action) {
  switch (action) {
    case StoreAction::DontCare:
      return impeller::StoreAction::kDontCare;
    case StoreAction::Store:
      return impeller::StoreAction::kStore;
    case StoreAction::MultisampleResolve:
      return impeller::StoreAction::kMultisampleResolve;
    case StoreAction::StoreAndMultisampleResolve:
      return impeller::StoreAction::kStoreAndMultisampleResolve;
      break;
  }
  return impeller::StoreAction::kDontCare;
}

bool ColorAttachmentSetStoreAction(FFIColorAttachment* color,
                                   StoreAction store_action) {
  if (!color) {
    return false;
  }
  color->attachment.store_action = ToStoreAction(store_action);
  return true;
}

bool ColorAttachmentSetClearColor(FFIColorAttachment* color_attachment,
                                  FFIColor* color) {
  if (!color_attachment || !color) {
    return false;
  }
  color_attachment->attachment.clear_color =
      impeller::Color{color->red, color->green, color->blue, color->alpha};
  return true;
}

}  // namespace impeller::stator::renderer
