// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/stator/renderer/renderer.h"

#include "impeller/base/config.h"
#include "impeller/base/validation.h"
#include "impeller/core/platform.h"

namespace impeller::stator::renderer {

static objffi::ScopedObject<FFIContext> gContext;

void SetGlobalContext(objffi::ScopedObject<FFIContext> context) {
  gContext = std::move(context);
}

FFIContext* GetGlobalContext() {
  return gContext.Get();
}

FFISwapchain* FFIContextSwapchainCopy(FFIContext* context) {
  if (!context) {
    return nullptr;
  }

  return objffi::Make<FFISwapchain>(context->Get()->GetSwapchain()).Leak();
}

FFITexture* FFISwapchainNextDrawableNew(FFISwapchain* swapchain_ptr) {
  auto swapchain = objffi::Ref(swapchain_ptr);
  if (!swapchain) {
    return nullptr;
  }
  return objffi::Make<FFITexture>(swapchain->Get()->AcquireNextDrawable())
      .Leak();
}

bool FFISwapchainPresentDrawable(FFISwapchain* swapchain, FFITexture* texture) {
  if (!swapchain || !texture) {
    return false;
  }
  return swapchain->Get()->PresentDrawable(texture->Get());
}

FFIRenderTarget* RenderTargetNew() {
  return objffi::Make<FFIRenderTarget>().Leak();
}

bool FFIRenderTargetSetColorAttachment(FFIRenderTarget* target,
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

bool FFIRenderTargetSetDepthAttachment(FFIRenderTarget* target,
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

bool FFIRenderTargetSetStencilAttachment(FFIRenderTarget* target,
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

bool FFIColorAttachmentSetTexture(FFIColorAttachment* color,
                                  FFITexture* texture) {
  if (!color) {
    return false;
  }

  color->attachment.texture = texture ? texture->Get() : nullptr;
  return true;
}

bool FFIColorAttachmentSetResolveTexture(FFIColorAttachment* color,
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

bool FFIColorAttachmentSetLoadAction(FFIColorAttachment* color,
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

bool FFIColorAttachmentSetStoreAction(FFIColorAttachment* color,
                                      StoreAction store_action) {
  if (!color) {
    return false;
  }
  color->attachment.store_action = ToStoreAction(store_action);
  return true;
}

bool FFIColorAttachmentSetClearColor(FFIColorAttachment* color_attachment,
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

bool FFIDepthAttachmentSetTexture(FFIDepthAttachment* depth,
                                  FFITexture* texture) {
  if (!depth) {
    return false;
  }
  depth->attachment.texture = texture ? texture->Get() : nullptr;
  return true;
}

bool FFIDepthAttachmentSetResolveTexture(FFIDepthAttachment* depth,
                                         FFITexture* texture) {
  if (!depth) {
    return false;
  }
  depth->attachment.resolve_texture = texture ? texture->Get() : nullptr;
  return true;
}

bool FFIDepthAttachmentSetLoadAction(FFIDepthAttachment* depth,
                                     LoadAction load_action) {
  if (!depth) {
    return false;
  }
  depth->attachment.load_action = ToLoadAction(load_action);
  return true;
}

bool FFIDepthAttachmentSetStoreAction(FFIDepthAttachment* depth,
                                      StoreAction store_action) {
  if (!depth) {
    return false;
  }
  depth->attachment.store_action = ToStoreAction(store_action);
  return true;
}

bool FFIDepthAttachmentSetClearDepth(FFIDepthAttachment* depth_attachment,
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

bool FFIStencilAttachmentSetTexture(FFIStencilAttachment* stencil,
                                    FFITexture* texture) {
  if (!stencil) {
    return false;
  }
  stencil->attachment.texture = texture ? texture->Get() : nullptr;
  return true;
}

bool FFIStencilAttachmentSetResolveTexture(FFIStencilAttachment* stencil,
                                           FFITexture* texture) {
  if (!stencil) {
    return false;
  }
  stencil->attachment.resolve_texture = texture ? texture->Get() : nullptr;
  return true;
}

bool FFIStencilAttachmentSetLoadAction(FFIStencilAttachment* stencil,
                                       LoadAction load_action) {
  if (!stencil) {
    return false;
  }
  stencil->attachment.load_action = ToLoadAction(load_action);
  return true;
}

bool FFIStencilAttachmentSetStoreAction(FFIStencilAttachment* stencil,
                                        StoreAction store_action) {
  if (!stencil) {
    return false;
  }
  stencil->attachment.store_action = ToStoreAction(store_action);
  return true;
}

bool FFIStencilAttachmentSetClearStencil(
    FFIStencilAttachment* stencil_attachment,
    int32_t clear_stencil) {
  if (!stencil_attachment) {
    return false;
  }
  stencil_attachment->attachment.clear_stencil = clear_stencil;
  return true;
}

FFICommandBuffer* FFIContextCommandBufferNew(FFIContext* context) {
  if (!context) {
    return nullptr;
  }
  return objffi::Make<FFICommandBuffer>(
             context->Get()->GetImpellerContext()->CreateCommandBuffer())
      .Leak();
}

FFIRenderPass* FFICommandBufferCreateRenderPassNew(
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

bool FFIRenderPassAddCommand(FFIRenderPass* render_pass, FFICommand* command) {
  if (!render_pass || !command) {
    return false;
  }
  return render_pass->render_pass->AddCommand(std::move(command->command));
}

bool FFIRenderPassEncodeCommands(FFIRenderPass* render_pass) {
  if (!render_pass) {
    return false;
  }
  return render_pass->render_pass->EncodeCommands();
}

FFICommand* CommandNew() {
  return objffi::Make<FFICommand>().Leak();
}

bool FFICommandBufferSubmit(FFICommandBuffer* command_buffer) {
  if (!command_buffer) {
    return false;
  }
  return command_buffer->command_buffer->SubmitCommands();
}

bool FFICommandSetPipeline(FFICommand* command, FFIPipeline* pipeline) {
  if (!command) {
    return false;
  }
  command->command.pipeline = pipeline ? pipeline->pipeline_.Get() : nullptr;
  return true;
}

bool FFICommandSetVertexBindings(FFICommand* command,
                                 FFICommandBindings* vertex_bindings) {
  if (!command) {
    return false;
  }
  command->command.vertex_bindings =
      vertex_bindings ? vertex_bindings->bindings : Bindings{};
  return true;
}

bool FFICommandSetFragmentBindings(FFICommand* command,
                                   FFICommandBindings* fragment_bindings) {
  if (!command) {
    return false;
  }
  command->command.fragment_bindings =
      fragment_bindings ? fragment_bindings->bindings : Bindings{};
  return true;
}

bool FFICommandSetIndexBuffer(FFICommand* command,
                              FFIBufferView* index_buffer_view) {
  if (!command) {
    return false;
  }
  command->command.index_buffer =
      index_buffer_view ? index_buffer_view->buffer_view : BufferView{};
  return true;
}

bool FFICommandSetVertexCount(FFICommand* command, uint64_t vertex_count) {
  if (!command) {
    return false;
  }
  command->command.vertex_count = vertex_count;
  return false;
}

constexpr impeller::IndexType ToIndexType(IndexType type) {
  switch (type) {
    case IndexType::Unknown:
      return impeller::IndexType::kUnknown;
    case IndexType::Bit16:
      return impeller::IndexType::k16bit;
    case IndexType::Bit32:
      return impeller::IndexType::k32bit;
    case IndexType::None:
      return impeller::IndexType::kNone;
  }
  return impeller::IndexType::kNone;
}

bool FFICommandSetIndexType(FFICommand* command, IndexType index_type) {
  if (!command) {
    return false;
  }
  command->command.index_type = ToIndexType(index_type);
  return true;
}

bool FFICommandSetStencilReference(FFICommand* command,
                                   uint32_t stencil_reference) {
  if (!command) {
    return false;
  }
  command->command.stencil_reference = stencil_reference;
  return true;
}

bool FFICommandSetBaseVertex(FFICommand* command, uint64_t base_vertex) {
  if (!command) {
    return false;
  }
  command->command.base_vertex = base_vertex;
  return true;
}

constexpr std::optional<Viewport> ToViewport(FFIViewport* ffi_viewport) {
  if (!ffi_viewport) {
    return std::nullopt;
  }
  Viewport viewport;
  viewport.rect =
      Rect::MakeXYWH(ffi_viewport->origin_x, ffi_viewport->origin_y,
                     ffi_viewport->size_width, ffi_viewport->size_height);
  viewport.depth_range = DepthRange{static_cast<Scalar>(ffi_viewport->z_near),
                                    static_cast<Scalar>(ffi_viewport->z_far)};
  return viewport;
}

bool FFICommandSetViewport(FFICommand* command, FFIViewport* viewport) {
  if (!command) {
    return false;
  }
  command->command.viewport = ToViewport(viewport);
  return true;
}

constexpr std::optional<IRect> ToIRect(FFIIRect* ffi_rect) {
  if (!ffi_rect) {
    return std::nullopt;
  }
  return IRect::MakeXYWH(ffi_rect->origin_x, ffi_rect->origin_y,
                         ffi_rect->size_width, ffi_rect->size_height);
}

bool FFICommandSetScissor(FFICommand* command, FFIIRect* scissor_rect) {
  if (!command) {
    return false;
  }
  command->command.scissor = ToIRect(scissor_rect);
  return true;
}

bool FFICommandSetInstanceCount(FFICommand* command, uint64_t instance_count) {
  if (!command) {
    return false;
  }
  command->command.instance_count = instance_count;
  return true;
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

FFIPipelineLibrary* FFIContextPipelineLibraryCopy(FFIContext* context) {
  if (!context) {
    return nullptr;
  }
  return objffi::Make<FFIPipelineLibrary>(
             context->Get()->GetImpellerContext()->GetPipelineLibrary())
      .Leak();
}

FFIPipeline* PipelineLibraryGetPipelineCopy(FFIPipelineLibrary* library,
                                            FFIPipelineDescriptor* desc) {
  if (!library || !desc) {
    return nullptr;
  }
  return objffi::Make<FFIPipeline>(
             library->pipeline_library->GetPipeline(desc->descriptor))
      .Leak();
}

FFIPipelineDescriptor* PipelineDescriptorNew() {
  return objffi::Make<FFIPipelineDescriptor>().Leak();
}

constexpr impeller::SampleCount ToSampleCount(SampleCount count) {
  switch (count) {
    case SampleCount::One:
      return impeller::SampleCount::kCount1;
    case SampleCount::Four:
      return impeller::SampleCount::kCount4;
  }
}

bool FFIPipelineDescriptorSetSampleCount(
    FFIPipelineDescriptor* pipeline_descriptor,
    SampleCount count) {
  if (!pipeline_descriptor) {
    return false;
  }
  pipeline_descriptor->descriptor.SetSampleCount(ToSampleCount(count));
  return true;
}

bool FFIPipelineDescriptorAddStage(FFIPipelineDescriptor* pipeline_descriptor,
                                   FFIShaderFunction* shader_function) {
  if (!pipeline_descriptor || !shader_function) {
    return false;
  }
  pipeline_descriptor->descriptor.AddStageEntrypoint(
      shader_function->shader_function);
  return true;
}

bool FFIPipelineDescriptorSetVertexDescriptor(
    FFIPipelineDescriptor* pipeline_descriptor,
    FFIVertexDescriptor* vertex_descriptor) {
  if (!pipeline_descriptor) {
    return false;
  }
  pipeline_descriptor->descriptor.SetVertexDescriptor(
      vertex_descriptor ? vertex_descriptor->vertex_descriptor : nullptr);
  return true;
}

bool FFIPipelineDescriptorSetColorAttachmentDescriptor(
    FFIPipelineDescriptor* pipeline_descriptor,
    FFIPipelineColorAttachmentDescriptor* pipeline_color_attachment_descriptor,
    uint32_t index) {
  if (!pipeline_descriptor) {
    return false;
  }
  pipeline_descriptor->descriptor.SetColorAttachmentDescriptor(
      index, pipeline_color_attachment_descriptor
                 ? pipeline_color_attachment_descriptor->descriptor
                 : ColorAttachmentDescriptor{});
  return true;
}

bool FFIPipelineDescriptorSetDepthAttachmentDescriptor(
    FFIPipelineDescriptor* pipeline_descriptor,
    FFIPipelineDepthAttachmentDescriptor*
        pipeline_depth_attachment_descriptor) {
  if (!pipeline_descriptor) {
    return false;
  }
  pipeline_descriptor->descriptor.SetDepthStencilAttachmentDescriptor(
      pipeline_depth_attachment_descriptor
          ? pipeline_depth_attachment_descriptor->descriptor
          : std::optional<DepthAttachmentDescriptor>{std::nullopt});
  return true;
}

bool FFIPipelineDescriptorSetStencilAttachmentDescriptor(
    FFIPipelineDescriptor* pipeline_descriptor,
    FFIPipelineStencilAttachmentDescriptor*
        pipeline_stencil_attachment_descriptor,
    Facing facing) {
  if (!pipeline_descriptor) {
    return false;
  }
  std::optional<StencilAttachmentDescriptor> front, back;
  std::optional<StencilAttachmentDescriptor> arg =
      pipeline_stencil_attachment_descriptor
          ? pipeline_stencil_attachment_descriptor->descriptor
          : std::optional<StencilAttachmentDescriptor>{std::nullopt};
  switch (facing) {
    case Facing::None:
      break;
    case Facing::Front:
      front = arg;
      break;
    case Facing::Back:
      back = arg;
      break;
    case Facing::Both:
      back = front = arg;
      break;
  }
  pipeline_descriptor->descriptor.SetStencilAttachmentDescriptors(front, back);
  return true;
}

constexpr impeller::CullMode ToCullMode(CullMode mode) {
  switch (mode) {
    case CullMode::None:
      return impeller::CullMode::kNone;
    case CullMode::FrontFace:
      return impeller::CullMode::kFrontFace;
    case CullMode::BackFace:
      return impeller::CullMode::kBackFace;
  }
  return impeller::CullMode::kNone;
}

bool FFIPipelineDescriptorSetCullMode(
    FFIPipelineDescriptor* pipeline_descriptor,
    CullMode mode) {
  if (!pipeline_descriptor) {
    return false;
  }
  pipeline_descriptor->descriptor.SetCullMode(ToCullMode(mode));
  return true;
}

constexpr impeller::WindingOrder ToWindingOrder(WindingOrder order) {
  switch (order) {
    case WindingOrder::Clockwise:
      return impeller::WindingOrder::kClockwise;
    case WindingOrder::CounterClockwise:
      return impeller::WindingOrder::kCounterClockwise;
  }
  return impeller::WindingOrder::kClockwise;
}

bool FFIPipelineDescriptorSetWindingOrder(
    FFIPipelineDescriptor* pipeline_descriptor,
    WindingOrder winding) {
  if (!pipeline_descriptor) {
    return false;
  }
  pipeline_descriptor->descriptor.SetWindingOrder(ToWindingOrder(winding));
  return true;
}

constexpr impeller::PrimitiveType ToPrimitiveType(PrimitiveType type) {
  switch (type) {
    case PrimitiveType::Triangle:
      return impeller::PrimitiveType::kTriangle;
    case PrimitiveType::TriangleStrip:
      return impeller::PrimitiveType::kTriangleStrip;
    case PrimitiveType::Line:
      return impeller::PrimitiveType::kLine;
    case PrimitiveType::LineStrip:
      return impeller::PrimitiveType::kLineStrip;
    case PrimitiveType::Point:
      return impeller::PrimitiveType::kPoint;
  }
  return impeller::PrimitiveType::kTriangle;
}

bool FFIPipelineDescriptorSetPrimitiveType(
    FFIPipelineDescriptor* pipeline_descriptor,
    PrimitiveType primitive_type) {
  if (!pipeline_descriptor) {
    return false;
  }
  pipeline_descriptor->descriptor.SetPrimitiveType(
      ToPrimitiveType(primitive_type));
  return true;
}

constexpr impeller::PolygonMode ToPolygonMode(PolygonMode mode) {
  switch (mode) {
    case PolygonMode::Fill:
      return impeller::PolygonMode::kFill;
    case PolygonMode::Line:
      return impeller::PolygonMode::kLine;
  }
  return impeller::PolygonMode::kFill;
}

bool FFIPipelineDescriptorSetPolygonMode(
    FFIPipelineDescriptor* pipeline_descriptor,
    PolygonMode polygon_mode) {
  if (!pipeline_descriptor) {
    return false;
  }
  pipeline_descriptor->descriptor.SetPolygonMode(ToPolygonMode(polygon_mode));
  return true;
}

FFIPipelineColorAttachmentDescriptor* PipelineColorAttachmentDescriptorNew() {
  return objffi::Make<FFIPipelineColorAttachmentDescriptor>().Leak();
}

constexpr impeller::PixelFormat ToPixelFormat(PixelFormat format) {
  switch (format) {
    case PixelFormat::Unknown:
      return impeller::PixelFormat::kUnknown;
    case PixelFormat::A8UNormInt:
      return impeller::PixelFormat::kA8UNormInt;
    case PixelFormat::R8UNormInt:
      return impeller::PixelFormat::kR8UNormInt;
    case PixelFormat::R8G8UNormInt:
      return impeller::PixelFormat::kR8G8UNormInt;
    case PixelFormat::R8G8B8A8UNormInt:
      return impeller::PixelFormat::kR8G8B8A8UNormInt;
    case PixelFormat::R8G8B8A8UNormIntSRGB:
      return impeller::PixelFormat::kR8G8B8A8UNormIntSRGB;
    case PixelFormat::B8G8R8A8UNormInt:
      return impeller::PixelFormat::kB8G8R8A8UNormInt;
    case PixelFormat::B8G8R8A8UNormIntSRGB:
      return impeller::PixelFormat::kB8G8R8A8UNormIntSRGB;
    case PixelFormat::R32G32B32A32Float:
      return impeller::PixelFormat::kR32G32B32A32Float;
    case PixelFormat::R16G16B16A16Float:
      return impeller::PixelFormat::kR16G16B16A16Float;
    case PixelFormat::B10G10R10XR:
      return impeller::PixelFormat::kB10G10R10XR;
    case PixelFormat::B10G10R10XRSRGB:
      return impeller::PixelFormat::kB10G10R10XRSRGB;
    case PixelFormat::B10G10R10A10XR:
      return impeller::PixelFormat::kB10G10R10A10XR;
    case PixelFormat::S8UInt:
      return impeller::PixelFormat::kS8UInt;
    case PixelFormat::D24UnormS8Uint:
      return impeller::PixelFormat::kD24UnormS8Uint;
    case PixelFormat::D32FloatS8UInt:
      return impeller::PixelFormat::kD32FloatS8UInt;
  }
  return impeller::PixelFormat::kUnknown;
}

bool FFIPipelineColorAttachmentDescriptorSetPixelFormat(
    FFIPipelineColorAttachmentDescriptor* desc,
    PixelFormat format) {
  if (!desc) {
    return false;
  }
  desc->descriptor.format = ToPixelFormat(format);
  return true;
}

bool FFIPipelineColorAttachmentDescriptorSetBlendingEnabled(
    FFIPipelineColorAttachmentDescriptor* desc,
    bool enabled) {
  if (!desc) {
    return false;
  }
  desc->descriptor.blending_enabled = enabled;
  return true;
}

constexpr impeller::BlendFactor ToBlendFactor(BlendFactor factor) {
  switch (factor) {
    case BlendFactor::Zero:
      return impeller::BlendFactor::kZero;
    case BlendFactor::One:
      return impeller::BlendFactor::kOne;
    case BlendFactor::SourceColor:
      return impeller::BlendFactor::kSourceColor;
    case BlendFactor::OneMinusSourceColor:
      return impeller::BlendFactor::kOneMinusSourceColor;
    case BlendFactor::SourceAlpha:
      return impeller::BlendFactor::kSourceAlpha;
    case BlendFactor::OneMinusSourceAlpha:
      return impeller::BlendFactor::kOneMinusSourceAlpha;
    case BlendFactor::DestinationColor:
      return impeller::BlendFactor::kDestinationColor;
    case BlendFactor::OneMinusDestinationColor:
      return impeller::BlendFactor::kOneMinusDestinationColor;
    case BlendFactor::DestinationAlpha:
      return impeller::BlendFactor::kDestinationAlpha;
    case BlendFactor::OneMinusDestinationAlpha:
      return impeller::BlendFactor::kOneMinusDestinationAlpha;
    case BlendFactor::SourceAlphaSaturated:
      return impeller::BlendFactor::kSourceAlphaSaturated;
    case BlendFactor::BlendColor:
      return impeller::BlendFactor::kBlendColor;
    case BlendFactor::OneMinusBlendColor:
      return impeller::BlendFactor::kOneMinusBlendColor;
    case BlendFactor::BlendAlpha:
      return impeller::BlendFactor::kBlendAlpha;
    case BlendFactor::OneMinusBlendAlpha:
      return impeller::BlendFactor::kOneMinusBlendAlpha;
  }
  return impeller::BlendFactor::kZero;
}

bool FFIPipelineColorAttachmentDescriptorSetSourceColorBlendFactor(
    FFIPipelineColorAttachmentDescriptor* desc,
    BlendFactor factor) {
  if (!desc) {
    return false;
  }
  desc->descriptor.src_color_blend_factor = ToBlendFactor(factor);
  return true;
}

constexpr impeller::BlendOperation ToBlendOperation(BlendOperation op) {
  switch (op) {
    case BlendOperation::Add:
      return impeller::BlendOperation::kAdd;
    case BlendOperation::Subtract:
      return impeller::BlendOperation::kSubtract;
    case BlendOperation::ReverseSubtract:
      return impeller::BlendOperation::kReverseSubtract;
  }
  return impeller::BlendOperation::kAdd;
}

bool FFIPipelineColorAttachmentDescriptorSetColorBlendOperation(
    FFIPipelineColorAttachmentDescriptor* desc,
    BlendOperation operation) {
  if (!desc) {
    return false;
  }
  desc->descriptor.color_blend_op = ToBlendOperation(operation);
  return true;
}

bool FFIPipelineColorAttachmentDescriptorSetDestinationColorBlendFactor(
    FFIPipelineColorAttachmentDescriptor* desc,
    BlendFactor factor) {
  if (!desc) {
    return false;
  }
  desc->descriptor.dst_color_blend_factor = ToBlendFactor(factor);
  return true;
}

bool FFIPipelineColorAttachmentDescriptorSetSourceAlphaBlendFactor(
    FFIPipelineColorAttachmentDescriptor* desc,
    BlendFactor factor) {
  if (!desc) {
    return false;
  }
  desc->descriptor.src_alpha_blend_factor = ToBlendFactor(factor);
  return true;
}

bool FFIPipelineColorAttachmentDescriptorSetAlphaBlendOperation(
    FFIPipelineColorAttachmentDescriptor* desc,
    BlendOperation operation) {
  if (!desc) {
    return false;
  }
  desc->descriptor.alpha_blend_op = ToBlendOperation(operation);
  return true;
}

bool FFIPipelineColorAttachmentDescriptorSetDestinationAlphaBlendFactor(
    FFIPipelineColorAttachmentDescriptor* desc,
    BlendFactor factor) {
  if (!desc) {
    return false;
  }
  desc->descriptor.dst_alpha_blend_factor = ToBlendFactor(factor);
  return true;
}

bool FFIPipelineColorAttachmentDescriptorSetColorWriteMask(
    FFIPipelineColorAttachmentDescriptor* desc,
    uint64_t write_mask) {
  if (!desc) {
    return false;
  }
  desc->descriptor.write_mask = write_mask;
  return true;
}

FFIPipelineDepthAttachmentDescriptor* PipelineDepthAttachmentDescriptorNew() {
  return objffi::Make<FFIPipelineDepthAttachmentDescriptor>().Leak();
}

bool FFIPipelineDepthAttachmentDescriptorSetPixelFormat(
    FFIPipelineDepthAttachmentDescriptor* desc,
    PixelFormat format) {
  if (!desc) {
    return false;
  }
  desc->pixel_format = ToPixelFormat(format);
  return true;
}

impeller::CompareFunction ToCompareFunction(CompareFunction func) {
  switch (func) {
    case CompareFunction::Never:
      return impeller::CompareFunction::kNever;
    case CompareFunction::Always:
      return impeller::CompareFunction::kAlways;
    case CompareFunction::Less:
      return impeller::CompareFunction::kLess;
    case CompareFunction::Equal:
      return impeller::CompareFunction::kEqual;
    case CompareFunction::LessEqual:
      return impeller::CompareFunction::kLessEqual;
    case CompareFunction::Greater:
      return impeller::CompareFunction::kGreater;
    case CompareFunction::NotEqual:
      return impeller::CompareFunction::kNotEqual;
    case CompareFunction::GreaterEqual:
      return impeller::CompareFunction::kGreaterEqual;
  }
  return impeller::CompareFunction::kNever;
}

bool FFIPipelineDepthAttachmentDescriptorSetDepthCompareFunction(
    FFIPipelineDepthAttachmentDescriptor* desc,
    CompareFunction depth_compare) {
  if (!desc) {
    return false;
  }
  desc->descriptor.depth_compare = ToCompareFunction(depth_compare);
  return true;
}

bool FFIPipelineDepthAttachmentDescriptorSetDepthWriteEnabled(
    FFIPipelineDepthAttachmentDescriptor* desc,
    bool depth_write_enabled) {
  if (!desc) {
    return false;
  }
  desc->descriptor.depth_write_enabled = depth_write_enabled;
  return true;
}

FFIPipelineStencilAttachmentDescriptor*
PipelineStencilAttachmentDescriptorNew() {
  return objffi::Make<FFIPipelineStencilAttachmentDescriptor>().Leak();
}

bool FFIPipelineStencilAttachmentDescriptorSetPixelFormat(
    FFIPipelineStencilAttachmentDescriptor* desc,
    PixelFormat format) {
  if (!desc) {
    return false;
  }
  desc->pixel_format = ToPixelFormat(format);
  return true;
}

bool FFIPipelineStencilAttachmentDescriptorSetStencilCompare(
    FFIPipelineStencilAttachmentDescriptor* desc,
    CompareFunction func) {
  if (!desc) {
    return false;
  }
  desc->descriptor.stencil_compare = ToCompareFunction(func);
  return true;
}

constexpr impeller::StencilOperation ToStencilOperation(StencilOperation op) {
  switch (op) {
    case StencilOperation::Keep:
      return impeller::StencilOperation::kKeep;
    case StencilOperation::Zero:
      return impeller::StencilOperation::kZero;
    case StencilOperation::SetToReferenceValue:
      return impeller::StencilOperation::kSetToReferenceValue;
    case StencilOperation::IncrementClamp:
      return impeller::StencilOperation::kIncrementClamp;
    case StencilOperation::DecrementClamp:
      return impeller::StencilOperation::kDecrementClamp;
    case StencilOperation::Invert:
      return impeller::StencilOperation::kInvert;
    case StencilOperation::IncrementWrap:
      return impeller::StencilOperation::kIncrementWrap;
    case StencilOperation::DecrementWrap:
      return impeller::StencilOperation::kDecrementWrap;
  }
  return impeller::StencilOperation::kKeep;
}

bool FFIPipelineStencilAttachmentDescriptorSetStencilFailure(
    FFIPipelineStencilAttachmentDescriptor* desc,
    StencilOperation operation) {
  if (!desc) {
    return false;
  }
  desc->descriptor.stencil_failure = ToStencilOperation(operation);
  return true;
}

bool FFIPipelineStencilAttachmentDescriptorSetDepthFailure(
    FFIPipelineStencilAttachmentDescriptor* desc,
    StencilOperation operation) {
  if (!desc) {
    return false;
  }
  desc->descriptor.depth_failure = ToStencilOperation(operation);
  return true;
}

bool FFIPipelineStencilAttachmentDescriptorSetDepthStencilPass(
    FFIPipelineStencilAttachmentDescriptor* desc,
    StencilOperation operation) {
  if (!desc) {
    return false;
  }
  desc->descriptor.depth_stencil_pass = ToStencilOperation(operation);
  return true;
}

bool FFIPipelineStencilAttachmentDescriptorSetReadMask(
    FFIPipelineStencilAttachmentDescriptor* desc,
    uint32_t read_mask) {
  if (!desc) {
    return false;
  }
  desc->descriptor.read_mask = read_mask;
  return true;
}

bool FFIPipelineStencilAttachmentDescriptorSetWriteMask(
    FFIPipelineStencilAttachmentDescriptor* desc,
    uint32_t write_mask) {
  if (!desc) {
    return false;
  }
  desc->descriptor.write_mask = write_mask;
  return true;
}

FFIBufferView* FFIHostBufferEmplaceUniformDataNew(FFIHostBuffer* buffer,
                                                  uint8_t* data,
                                                  uint32_t length,
                                                  uint32_t alignment) {
  return FFIHostBufferEmplaceDataNew(
      buffer,                                                   //
      data,                                                     //
      length,                                                   //
      std::max<uint32_t>(alignment, DefaultUniformAlignment())  //
  );
}

FFIBufferView* FFIHostBufferEmplaceStorageBufferDataNew(FFIHostBuffer* buffer,
                                                        uint8_t* data,
                                                        uint32_t length,
                                                        uint32_t alignment) {
  return FFIHostBufferEmplaceDataNew(
      buffer,                                                   //
      data,                                                     //
      length,                                                   //
      std::max<uint32_t>(alignment, DefaultUniformAlignment())  //
  );
}

FFIBufferView* FFIHostBufferEmplaceDataNew(FFIHostBuffer* buffer,
                                           uint8_t* data,
                                           uint32_t length,
                                           uint32_t alignment) {
  if (!buffer || !buffer->buffer) {
    return nullptr;
  }
  auto view = buffer->buffer->Emplace(data, length, alignment);
  if (!view) {
    return nullptr;
  }
  return objffi::Make<FFIBufferView>(std::move(view)).Leak();
}

FFIHostBuffer* HostBufferNew() {
  return objffi::Make<FFIHostBuffer>(HostBuffer::Create()).Leak();
}

FFIDeviceBufferDescriptor* DeviceBufferDescriptorNew() {
  return objffi::Make<FFIDeviceBufferDescriptor>().Leak();
}

FFIAllocator* FFIContextResourceAllocatorCopy(FFIContext* context) {
  if (!context) {
    return nullptr;
  }
  return objffi::Make<FFIAllocator>(
             context->Get()->GetImpellerContext()->GetResourceAllocator())
      .Leak();
}

constexpr impeller::StorageMode ToStorageMode(StorageMode mode) {
  switch (mode) {
    case StorageMode::HostVisible:
      return impeller::StorageMode::kHostVisible;
    case StorageMode::DevicePrivate:
      return impeller::StorageMode::kDevicePrivate;
    case StorageMode::DeviceTransient:
      return impeller::StorageMode::kDeviceTransient;
  }
  return impeller::StorageMode::kHostVisible;
}

bool FFITextureDescriptorSetStorageMode(FFITextureDescriptor* desc,
                                        StorageMode mode) {
  if (!desc) {
    return false;
  }
  desc->descriptor.storage_mode = ToStorageMode(mode);
  return true;
}

constexpr impeller::TextureType ToTextureType(TextureType type) {
  switch (type) {
    case TextureType::Texture2D:
      return impeller::TextureType::kTexture2D;
    case TextureType::TextureCube:
      return impeller::TextureType::kTextureCube;
  }
  return impeller::TextureType::kTexture2D;
}

bool FFITextureDescriptorSetTextureType(FFITextureDescriptor* desc,
                                        TextureType type) {
  if (!desc) {
    return false;
  }
  desc->descriptor.type = ToTextureType(type);
  return true;
}

bool FFITextureDescriptorSetPixelFormat(FFITextureDescriptor* desc,
                                        PixelFormat format) {
  if (!desc) {
    return false;
  }
  desc->descriptor.format = ToPixelFormat(format);
  return true;
}

bool FFITextureDescriptorSetSize(FFITextureDescriptor* desc,
                                 uint64_t width,
                                 uint64_t height) {
  if (!desc) {
    return false;
  }
  desc->descriptor.size = ISize::MakeWH(width, height);
  return true;
}

bool FFITextureDescriptorSetMipCount(FFITextureDescriptor* desc,
                                     uint32_t mip_count) {
  if (!desc) {
    return false;
  }
  desc->descriptor.mip_count = mip_count;
  return true;
}

bool FFITextureDescriptorSetUsage(FFITextureDescriptor* desc,
                                  uint64_t usage_mask) {
  if (!desc) {
    return false;
  }
  desc->descriptor.usage = usage_mask;
  return true;
}

bool FFITextureDescriptorSetSampleCount(FFITextureDescriptor* desc,
                                        SampleCount sample_count) {
  if (!desc) {
    return false;
  }
  desc->descriptor.sample_count = ToSampleCount(sample_count);
  return true;
}

constexpr impeller::CompressionType ToCompressionType(CompressionType type) {
  switch (type) {
    case CompressionType::Lossless:
      return impeller::CompressionType::kLossless;
    case CompressionType::Lossy:
      return impeller::CompressionType::kLossy;
  }
  return impeller::CompressionType::kLossless;
}

bool FFITextureDescriptorSetCompressionType(FFITextureDescriptor* desc,
                                            CompressionType compression_type) {
  if (!desc) {
    return false;
  }
  desc->descriptor.compression_type = ToCompressionType(compression_type);
  return true;
}

bool FFIDeviceBufferDescriptorSetStorageMode(FFIDeviceBufferDescriptor* desc,
                                             StorageMode storage_mode) {
  if (!desc) {
    return false;
  }
  desc->descriptor.storage_mode = ToStorageMode(storage_mode);
  return true;
}

bool FFIDeviceBufferDescriptorSetSize(FFIDeviceBufferDescriptor* desc,
                                      uint64_t size) {
  if (!desc) {
    return false;
  }
  desc->descriptor.size = size;
  return true;
}

FFIDeviceBuffer* FFIAllocatorCreateBufferNew(FFIAllocator* allocator,
                                             FFIDeviceBufferDescriptor* desc) {
  if (!allocator || !desc) {
    return nullptr;
  }
  auto buffer = allocator->allocator->CreateBuffer(desc->descriptor);
  if (!buffer) {
    return nullptr;
  }
  return objffi::Make<FFIDeviceBuffer>(std::move(buffer)).Leak();
}

FFITexture* FFIAllocatorCreateTextureNew(FFIAllocator* allocator,
                                         FFITextureDescriptor* desc) {
  if (!allocator || !desc) {
    return nullptr;
  }
  auto texture = allocator->allocator->CreateTexture(desc->descriptor);
  if (!texture || !texture->IsValid()) {
    return nullptr;
  }
  return objffi::Make<FFITexture>(std::move(texture)).Leak();
}

constexpr impeller::ShaderStage ToShaderStage(ShaderStage stage) {
  switch (stage) {
    case ShaderStage::Unknown:
      return impeller::ShaderStage::kUnknown;
    case ShaderStage::Vertex:
      return impeller::ShaderStage::kVertex;
    case ShaderStage::Fragment:
      return impeller::ShaderStage::kFragment;
    case ShaderStage::TessellationControl:
      return impeller::ShaderStage::kTessellationControl;
    case ShaderStage::TessellationEvaluation:
      return impeller::ShaderStage::kTessellationEvaluation;
    case ShaderStage::Compute:
      return impeller::ShaderStage::kCompute;
  }
  return impeller::ShaderStage::kUnknown;
}

FFIShaderFunction* FFIShaderLibraryGetFunctionCopy(FFIShaderLibrary* library,
                                                   ShaderStage stage,
                                                   uint8_t* name,
                                                   uint32_t name_len) {
  if (!library || name == nullptr) {
    return nullptr;
  }
  auto function = library->shader_library->GetFunction(
      std::string_view{reinterpret_cast<const char*>(name), name_len},
      ToShaderStage(stage));
  if (!function) {
    return nullptr;
  }
  return objffi::Make<FFIShaderFunction>(std::move(function)).Leak();
}

FFISampler* FFISamplerLibraryGetSamplerCopy(FFISamplerLibrary* library,
                                            FFISamplerDescriptor* desc) {
  if (!library) {
    return nullptr;
  }
  auto sampler = library->sampler_library->GetSampler(desc->descriptor);
  if (!sampler) {
    return nullptr;
  }
  return objffi::Make<FFISampler>(std::move(sampler)).Leak();
}

FFISamplerDescriptor* SamplerDescriptorNew() {
  return objffi::Make<FFISamplerDescriptor>().Leak();
}

constexpr impeller::MinMagFilter ToMinMagFilter(MinMagFilter filter) {
  switch (filter) {
    case MinMagFilter::Nearest:
      return impeller::MinMagFilter::kNearest;
    case MinMagFilter::Linear:
      return impeller::MinMagFilter::kLinear;
  }
  return impeller::MinMagFilter::kNearest;
}

constexpr impeller::MipFilter ToMipFilter(MipFilter filter) {
  switch (filter) {
    case MipFilter::Nearest:
      return impeller::MipFilter::kNearest;
    case MipFilter::Linear:
      return impeller::MipFilter::kLinear;
  }
  return impeller::MipFilter::kNearest;
}

constexpr impeller::SamplerAddressMode ToSamplerAddressMode(
    SamplerAddressMode mode) {
  switch (mode) {
    case SamplerAddressMode::ClampToEdge:
      return impeller::SamplerAddressMode::kClampToEdge;
    case SamplerAddressMode::Repeat:
      return impeller::SamplerAddressMode::kRepeat;
    case SamplerAddressMode::Mirror:
      return impeller::SamplerAddressMode::kMirror;
    case SamplerAddressMode::Decal:
      return impeller::SamplerAddressMode::kDecal;
  }
  return impeller::SamplerAddressMode::kClampToEdge;
}

bool FFISamplerDescriptorSetMinFilter(FFISamplerDescriptor* desc,
                                      MinMagFilter filter) {
  if (!desc) {
    return false;
  }
  desc->descriptor.min_filter = ToMinMagFilter(filter);
  return true;
}

bool FFISamplerDescriptorSetMagFilter(FFISamplerDescriptor* desc,
                                      MinMagFilter filter) {
  if (!desc) {
    return false;
  }
  desc->descriptor.mag_filter = ToMinMagFilter(filter);
  return true;
}

bool FFISamplerDescriptorSetMipFilter(FFISamplerDescriptor* desc,
                                      MipFilter filter) {
  if (!desc) {
    return false;
  }
  desc->descriptor.mip_filter = ToMipFilter(filter);
  return true;
}

bool FFISamplerDescriptorSetWidthAddressMode(FFISamplerDescriptor* desc,
                                             SamplerAddressMode mode) {
  if (!desc) {
    return false;
  }
  desc->descriptor.width_address_mode = ToSamplerAddressMode(mode);
  return true;
}

bool FFISamplerDescriptorSetHeightAddressMode(FFISamplerDescriptor* desc,
                                              SamplerAddressMode mode) {
  if (!desc) {
    return false;
  }
  desc->descriptor.height_address_mode = ToSamplerAddressMode(mode);
  return true;
}

bool FFISamplerDescriptorSetDepthAddressMode(FFISamplerDescriptor* desc,
                                             SamplerAddressMode mode) {
  if (!desc) {
    return false;
  }
  desc->descriptor.depth_address_mode = ToSamplerAddressMode(mode);
  return true;
}

FFIShaderLibrary* FFIContextShaderLibraryCopy(FFIContext* context) {
  if (!context) {
    return nullptr;
  }
  return objffi::Make<FFIShaderLibrary>(
             context->Get()->GetImpellerContext()->GetShaderLibrary())
      .Leak();
}

FFISamplerLibrary* FFIContextSamplerLibraryCopy(FFIContext* context) {
  if (!context) {
    return nullptr;
  }
  return objffi::Make<FFISamplerLibrary>(
             context->Get()->GetImpellerContext()->GetSamplerLibrary())
      .Leak();
}

}  // namespace impeller::stator::renderer
