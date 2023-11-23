// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "impeller/core/buffer_view.h"
#include "impeller/renderer/command.h"
#include "impeller/renderer/pipeline.h"
#include "impeller/renderer/pipeline_library.h"
#include "impeller/renderer/render_pass.h"
#include "impeller/renderer/render_target.h"
#include "impeller/renderer/vertex_descriptor.h"
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

class FFIPipelineDescriptor final : public objffi::Object {
 public:
  impeller::PipelineDescriptor descriptor;
};

class FFIPipelineLibrary final : public objffi::Object {
 public:
  explicit FFIPipelineLibrary(
      std::shared_ptr<impeller::PipelineLibrary> library)
      : pipeline_library(std::move(library)) {}

  std::shared_ptr<impeller::PipelineLibrary> pipeline_library;
};

class FFIPipeline final : public objffi::Object {
 public:
  explicit FFIPipeline(PipelineFuture<PipelineDescriptor> pipeline)
      : pipeline_(std::move(pipeline)) {}

  PipelineFuture<PipelineDescriptor> pipeline_;
};

class FFICommandBindings final : public objffi::Object {
 public:
  explicit FFICommandBindings(impeller::Bindings p_bindings)
      : bindings(std::move(p_bindings)) {}

  impeller::Bindings bindings;
};

class FFIBufferView final : public objffi::Object {
 public:
  explicit FFIBufferView(impeller::BufferView buffer_view)
      : buffer_view(std::move(buffer_view)) {}

  impeller::BufferView buffer_view;
};

class FFIShaderFunction final : public objffi::Object {
 public:
  explicit FFIShaderFunction(
      std::shared_ptr<impeller::ShaderFunction> shader_function)
      : shader_function(std::move(shader_function)) {}

  std::shared_ptr<impeller::ShaderFunction> shader_function;
};

class FFIVertexDescriptor final : public objffi::Object {
 public:
  std::shared_ptr<VertexDescriptor> vertex_descriptor;
};

class FFIPipelineColorAttachmentDescriptor final : public objffi::Object {
 public:
  ColorAttachmentDescriptor descriptor;
};

class FFIPipelineDepthAttachmentDescriptor final : public objffi::Object {
 public:
  impeller::PixelFormat pixel_format = impeller::PixelFormat::kUnknown;
  DepthAttachmentDescriptor descriptor;
};

class FFIPipelineStencilAttachmentDescriptor final : public objffi::Object {
 public:
  impeller::PixelFormat pixel_format = impeller::PixelFormat::kUnknown;
  StencilAttachmentDescriptor descriptor;
};

class FFIHostBuffer final : public objffi::Object {
 public:
  explicit FFIHostBuffer(std::shared_ptr<HostBuffer> p_buffer)
      : buffer(std::move(p_buffer)) {}

  std::shared_ptr<HostBuffer> buffer;
};

void SetGlobalContext(objffi::ScopedObject<FFIContext> context);

}  // namespace impeller::stator::renderer
