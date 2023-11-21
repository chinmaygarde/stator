// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "impeller/renderer/command.h"
#include "impeller/renderer/render_pass.h"
#include "impeller/renderer/render_target.h"
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

  const std::shared_ptr<Swapchain>& Get() const { return swapchain_; }

 private:
  std::shared_ptr<Swapchain> swapchain_;
};

class FFITexture final : public objffi::Object {
 public:
  explicit FFITexture(std::shared_ptr<Texture> texture)
      : texture_(std::move(texture)) {}

  const std::shared_ptr<Texture>& Get() const { return texture_; }

 private:
  std::shared_ptr<Texture> texture_;
};

class FFIRenderTarget final : public objffi::Object {
 public:
  RenderTarget render_target;
};

class FFIColorAttachment final : public objffi::Object {
 public:
  ColorAttachment attachment;
};

class FFIDepthAttachment final : public objffi::Object {
 public:
  DepthAttachment attachment;
};

class FFIStencilAttachment final : public objffi::Object {
 public:
  StencilAttachment attachment;
};

class FFICommandBuffer final : public objffi::Object {
 public:
  explicit FFICommandBuffer(std::shared_ptr<impeller::CommandBuffer> buffer)
      : command_buffer(std::move(buffer)) {}

  std::shared_ptr<impeller::CommandBuffer> command_buffer;
};

class FFIRenderPass final : public objffi::Object {
 public:
  explicit FFIRenderPass(std::shared_ptr<impeller::RenderPass> pass)
      : render_pass(std::move(pass)) {}

  std::shared_ptr<impeller::RenderPass> render_pass;
};

class FFICommand final : public objffi::Object {
 public:
  impeller::Command command;
};

void SetGlobalContext(objffi::ScopedObject<FFIContext> context);

}  // namespace impeller::stator::renderer
