// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/stator/renderer/compositor.h"

#include <optional>

#include "impeller/entity/contents/texture_contents.h"
#include "impeller/entity/entity.h"
#include "impeller/stator/renderer/swapchain.h"
#include "impeller/typographer/backends/skia/typographer_context_skia.h"

namespace impeller::stator {

Compositor::Compositor(std::shared_ptr<impeller::Context> impeller_context,
                       std::weak_ptr<Swapchain> swapchain)
    : swapchain_(std::move(swapchain)),
      content_context_(std::move(impeller_context),
                       TypographerContextSkia::Make()) {
  is_valid_ = content_context_.IsValid();
}

Compositor::~Compositor() = default;

bool Compositor::IsValid() const {
  return is_valid_;
}

static std::optional<Entity> CreateEntityForDrawable(
    std::shared_ptr<Texture> texture) {
  if (!texture) {
    return std::nullopt;
  }
  const auto src_rect = Rect::MakeSize(texture->GetSize());
  const auto dst_rect = src_rect;

  if (src_rect.IsEmpty()) {
    return std::nullopt;
  }

  SamplerDescriptor sampler;
  // Drawing at scale, so no filtering.
  sampler.min_filter = MinMagFilter::kNearest;
  sampler.mag_filter = MinMagFilter::kNearest;

  auto contents = TextureContents::MakeRect(dst_rect);
  contents->SetTexture(std::move(texture));
  contents->SetSourceRect(src_rect);
  contents->SetSamplerDescriptor(sampler);
  contents->SetOpacity(1.0f);

  Entity entity;
  entity.SetBlendMode(BlendMode::kSourceOver);
  entity.SetContents(contents);
  return entity;
}

bool Compositor::Composite(RenderPass& pass) {
  if (!IsValid()) {
    return false;
  }

  if (!pass.IsValid()) {
    return false;
  }

  auto swapchain = swapchain_.lock();
  if (!swapchain) {
    VALIDATION_LOG
        << "Swapchain is no longer active when attempting to composite.";
    return false;
  }

  auto next_presentable = swapchain->AcquireNextPresentable();

  // If the next presentable is null, the application is lagging behind and the
  // previous frame must be re-rendered. If not, the preview frame can be
  // recycled back into the swapchain.
  if (next_presentable != nullptr) {
    swapchain->RecycleDrawable(std::move(current_presentable_));
    current_presentable_ = std::move(next_presentable);
  }

  auto entity = CreateEntityForDrawable(current_presentable_);
  if (!entity.has_value()) {
    return true;
  }

  return entity->Render(content_context_, pass);
}

}  // namespace impeller::stator
