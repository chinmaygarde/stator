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
  if (!target) {
    return false;
  }
  if (depth && !depth->attachment.IsValid()) {
    VALIDATION_LOG << "Depth attachment was invalid.";
    return false;
  }
  if (depth) {
    target->render_target.SetDepthAttachment(depth->attachment);
  } else {
    target->render_target.SetDepthAttachment(std::nullopt);
  }
  return true;
}

bool RenderTargetSetStencilAttachment(FFIRenderTarget* target,
                                      FFIStencilAttachment* stencil) {
  if (!target) {
    return false;
  }
  if (stencil && !stencil->attachment.IsValid()) {
    VALIDATION_LOG << "Depth attachment was invalid.";
    return false;
  }
  if (stencil) {
    target->render_target.SetStencilAttachment(stencil->attachment);
  } else {
    target->render_target.SetStencilAttachment(std::nullopt);
  }

  return true;
}

FFIColor* ColorAlloc() {
  return std::make_unique<FFIColor>().release();
}

void ColorFree(FFIColor* color) {
  delete color;
}

FFIColorAttachment* ColorAttachmentNew() {
  return objffi::Make<FFIColorAttachment>().Leak();
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

FFIDepthAttachment* DepthAttachmentNew() {
  return objffi::Make<FFIDepthAttachment>().Leak();
}

bool DepthAttachmentSetTexture(FFIDepthAttachment* depth, FFITexture* texture) {
  if (!depth) {
    return false;
  }
  depth->attachment.texture = texture ? texture->Get() : nullptr;
  return true;
}

bool DepthAttachmentSetResolveTexture(FFIDepthAttachment* depth,
                                      FFITexture* texture) {
  if (!depth) {
    return false;
  }
  depth->attachment.resolve_texture = texture ? texture->Get() : nullptr;
  return true;
}

bool DepthAttachmentSetLoadAction(FFIDepthAttachment* depth,
                                  LoadAction load_action) {
  if (!depth) {
    return false;
  }
  depth->attachment.load_action = ToLoadAction(load_action);
  return true;
}

bool DepthAttachmentSetStoreAction(FFIDepthAttachment* depth,
                                   StoreAction store_action) {
  if (!depth) {
    return false;
  }
  depth->attachment.store_action = ToStoreAction(store_action);
  return true;
}

bool DepthAttachmentSetClearDepth(FFIDepthAttachment* depth_attachment,
                                  double clear_depth) {
  if (!depth_attachment) {
    return false;
  }
  depth_attachment->attachment.clear_depth = clear_depth;
  return true;
}

FFIStencilAttachment* StencilAttachmentNew() {
  return objffi::Make<FFIStencilAttachment>().Leak();
}

bool StencilAttachmentSetTexture(FFIStencilAttachment* stencil,
                                 FFITexture* texture) {
  if (!stencil) {
    return false;
  }
  stencil->attachment.texture = texture ? texture->Get() : nullptr;
  return true;
}

bool StencilAttachmentSetResolveTexture(FFIStencilAttachment* stencil,
                                        FFITexture* texture) {
  if (!stencil) {
    return false;
  }
  stencil->attachment.resolve_texture = texture ? texture->Get() : nullptr;
  return true;
}

bool StencilAttachmentSetLoadAction(FFIStencilAttachment* stencil,
                                    LoadAction load_action) {
  if (!stencil) {
    return false;
  }
  stencil->attachment.load_action = ToLoadAction(load_action);
  return true;
}

bool StencilAttachmentSetStoreAction(FFIStencilAttachment* stencil,
                                     StoreAction store_action) {
  if (!stencil) {
    return false;
  }
  stencil->attachment.store_action = ToStoreAction(store_action);
  return true;
}

bool StencilAttachmentSetClearStencil(FFIStencilAttachment* stencil_attachment,
                                      int32_t clear_stencil) {
  if (!stencil_attachment) {
    return false;
  }
  stencil_attachment->attachment.clear_stencil = clear_stencil;
  return true;
}

FFICommandBuffer* ContextCommandBufferNew(FFIContext* context) {
  if (!context) {
    return nullptr;
  }
  return objffi::Make<FFICommandBuffer>(
             context->Get()->GetImpellerContext()->CreateCommandBuffer())
      .Leak();
}

FFIRenderPass* CommandBufferCreateRenderPassNew(
    FFICommandBuffer* command_buffer,
    FFIRenderTarget* render_target) {
  if (!command_buffer || !render_target) {
    return nullptr;
  }
  auto render_pass = command_buffer->command_buffer->CreateRenderPass(
      render_target->render_target);
  if (!render_pass) {
    VALIDATION_LOG << "Could not create render pass.";
    return nullptr;
  }
  return objffi::Make<FFIRenderPass>(std::move(render_pass)).Leak();
}

bool RenderPassAddCommand(FFIRenderPass* render_pass, FFICommand* command) {
  if (!render_pass || !command) {
    return false;
  }
  return render_pass->render_pass->AddCommand(std::move(command->command));
}

bool RenderPassEncodeCommands(FFIRenderPass* render_pass) {
  if (!render_pass) {
    return false;
  }
  return render_pass->render_pass->EncodeCommands();
}

FFICommand* CommandNew() {
  return objffi::Make<FFICommand>().Leak();
}

bool CommandBufferSubmit(FFICommandBuffer* command_buffer) {
  if (!command_buffer) {
    return false;
  }
  return command_buffer->command_buffer->SubmitCommands();
}

bool CommandSetPipeline(FFICommand* command, FFIPipeline* pipeline) {
  IMPELLER_UNIMPLEMENTED;
}

bool CommandSetVertexBindings(FFICommand* command,
                              FFICommandBindings* vertex_bindings) {
  IMPELLER_UNIMPLEMENTED;
}

bool CommandSetFragmentBindings(FFICommand* command,
                                FFICommandBindings* fragment_bindings) {
  IMPELLER_UNIMPLEMENTED;
}

bool CommandSetIndexBuffer(FFICommand* command,
                           FFIBufferView* index_buffer_view) {
  IMPELLER_UNIMPLEMENTED;
}

bool CommandSetVertexCount(FFICommand* command, uint64_t vertex_count) {
  IMPELLER_UNIMPLEMENTED;
}

bool CommandSetIndexType(FFICommand* command, IndexType index_type) {
  IMPELLER_UNIMPLEMENTED;
}

bool CommandSetStencilReference(FFICommand* command,
                                uint32_t stencil_reference) {
  IMPELLER_UNIMPLEMENTED;
}

bool CommandSetBaseVertex(FFICommand* command, uint64_t base_vertex) {
  IMPELLER_UNIMPLEMENTED;
}

bool CommandSetViewport(FFICommand* command, FFIViewport* viewport) {
  IMPELLER_UNIMPLEMENTED;
}

bool CommandSetScissor(FFICommand* command, FFIIRect* scissor_rect) {
  IMPELLER_UNIMPLEMENTED;
}

bool CommandSetInstanceCount(FFICommand* command, uint64_t instance_count) {
  IMPELLER_UNIMPLEMENTED;
}

FFIViewport* FFIViewportAlloc() {
  return std::make_unique<FFIViewport>().release();
}

void FFIViewportFree(FFIViewport* viewport) {
  delete viewport;
}

FFIIRect* FFIIRectAlloc() {
  return std::make_unique<FFIIRect>().release();
}

void FFIIRectFree(FFIIRect* rect) {
  delete rect;
}

FFIPipelineLibrary* ContextPipelineLibraryCopy(FFIContext* context) {
  IMPELLER_UNIMPLEMENTED;
}

FFIPipeline* PipelineLibraryGetPipelineCopy(
    FFIPipelineLibrary* pipeline_library,
    FFIPipelineDescriptor* pipeline_descriptor) {
  IMPELLER_UNIMPLEMENTED;
}

FFIPipelineDescriptor* PipelineDescriptorNew() {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDescriptSetSampleCount(FFIPipelineDescriptor* pipeline_descriptor,
                                    SampleCount count) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDescriptorAddStage(FFIPipelineDescriptor* pipeline_descriptor,
                                FFIShaderFunction* shader_function) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDescriptorSetVertexDescriptor(
    FFIPipelineDescriptor* pipeline_descriptor,
    FFIVertexDescriptor* vertex_descriptor) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDescriptorSetColorAttachmentDescriptor(
    FFIPipelineDescriptor* pipeline_descriptor,
    FFIPipelineColorAttachmentDescriptor* pipeline_color_attachment_descriptor,
    uint32_t index) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDescriptorSetDepthAttachmentDescriptor(
    FFIPipelineDescriptor* pipeline_descriptor,
    FFIPipelineDepthAttachmentDescriptor*
        pipeline_depth_attachment_descriptor) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDescriptorSetStencilAttachmentDescriptor(
    FFIPipelineDescriptor* pipeline_descriptor,
    FFIPipelineStencilAttachmentDescriptor*
        pipeline_stencil_attachment_descriptor,
    Facing facing) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDescriptorSetCullMode(FFIPipelineDescriptor* pipeline_descriptor,
                                   Facing facing) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDescriptorSetWindingOrder(
    FFIPipelineDescriptor* pipeline_descriptor,
    WindingOrder winding) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDescriptorSetPrimitiveType(
    FFIPipelineDescriptor* pipeline_descriptor,
    PrimitiveType primitive_type) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDescriptorSetPolygonMode(
    FFIPipelineDescriptor* pipeline_descriptor,
    PolygonMode polygon_mode) {
  IMPELLER_UNIMPLEMENTED;
}

FFIPipelineColorAttachmentDescriptor* PipelineColorAttachmentDescriptorNew() {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineColorAttachmentDescriptorSetPixelFormat(
    FFIPipelineColorAttachmentDescriptor* desc,
    PixelFormat format) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineColorAttachmentDescriptorSetBlendingEnabled(
    FFIPipelineColorAttachmentDescriptor* desc,
    bool enabled) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineColorAttachmentDescriptorSetSourceColorBlendFactor(
    FFIPipelineColorAttachmentDescriptor* desc,
    BlendFactor factor) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineColorAttachmentDescriptorSetColorBlendOperation(
    FFIPipelineColorAttachmentDescriptor* desc,
    BlendOperation operation) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineColorAttachmentDescriptorSetDestinationColorBlendFactor(
    FFIPipelineColorAttachmentDescriptor* desc,
    BlendFactor factor) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineColorAttachmentDescriptorSetSourceAlphaBlendFactor(
    FFIPipelineColorAttachmentDescriptor* desc,
    BlendFactor factor) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineColorAttachmentDescriptorSetAlphaBlendOperation(
    FFIPipelineColorAttachmentDescriptor* desc,
    BlendOperation operation) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineColorAttachmentDescriptorSetDestinationAlphaBlendFactor(
    FFIPipelineColorAttachmentDescriptor* desc,
    BlendFactor factor) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineColorAttachmentDescriptorSetColorWriteMask(
    FFIPipelineColorAttachmentDescriptor* desc,
    uint64_t write_mask) {
  IMPELLER_UNIMPLEMENTED;
}

FFIPipelineDepthAttachmentDescriptor* PipelineDepthAttachmentDescriptorNew() {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDepthAttachmentDescriptorSetPixelFormat(
    FFIPipelineDepthAttachmentDescriptor* desc,
    PixelFormat format) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDepthAttachmentDescriptorSetDepthCompareFunction(
    FFIPipelineDepthAttachmentDescriptor* desc,
    CompareFunction depth_compare) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineDepthAttachmentDescriptorSetDepthWriteEnabled(
    FFIPipelineDepthAttachmentDescriptor* desc,
    bool depth_write_enabled) {
  IMPELLER_UNIMPLEMENTED;
}

FFIPipelineStencilAttachmentDescriptor*
PipelineStencilAttachmentDescriptorNew() {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineStencilAttachmentDescriptorSetPixelFormat(
    FFIPipelineStencilAttachmentDescriptor* desc,
    PixelFormat format) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineStencilAttachmentDescriptorSetStencilCompare(
    FFIPipelineStencilAttachmentDescriptor* desc,
    CompareFunction func) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineStencilAttachmentDescriptorSetStencilFailure(
    FFIPipelineStencilAttachmentDescriptor* desc,
    StencilOperation operation) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineStencilAttachmentDescriptorSetDepthFailure(
    FFIPipelineStencilAttachmentDescriptor* desc,
    StencilOperation operation) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineStencilAttachmentDescriptorSetDepthStencilPass(
    FFIPipelineStencilAttachmentDescriptor* desc,
    StencilOperation operation) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineStencilAttachmentDescriptorSetReadMask(
    FFIPipelineStencilAttachmentDescriptor* desc,
    uint32_t read_mask) {
  IMPELLER_UNIMPLEMENTED;
}

bool PipelineStencilAttachmentDescriptorSetWriteMask(
    FFIPipelineStencilAttachmentDescriptor* desc,
    uint32_t write_mask) {
  IMPELLER_UNIMPLEMENTED;
}

}  // namespace impeller::stator::renderer
