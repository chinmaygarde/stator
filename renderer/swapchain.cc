// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/stator/renderer/swapchain.h"

#include "impeller/base/strings.h"
#include "impeller/base/validation.h"
#include "impeller/core/allocator.h"
#include "impeller/stator/renderer/context.h"

namespace impeller::stator {

Swapchain::Swapchain(const std::shared_ptr<Context>& context,
                     size_t max_drawable_count,
                     std::chrono::milliseconds recyclable_expiry)
    : context_(context),
      max_drawable_count_(max_drawable_count),
      recyclable_expiry_(recyclable_expiry) {
  TextureDescriptor desc;
  desc.size = ISize(100u, 100u);
  desc.format = PixelFormat::kB8G8R8A8UNormInt;
  desc.storage_mode = StorageMode::kDevicePrivate;
  texture_desc_ = desc;
  is_valid_ = true;
}

Swapchain::~Swapchain() = default;

bool Swapchain::IsValid() const {
  return is_valid_;
}

void Swapchain::Shutdown() {
  {
    Lock lock(mutex_);
    shutdown_ = true;
  }
  not_empty_cv_.NotifyAll();
  not_full_cv_.NotifyAll();
}

std::shared_ptr<Texture> Swapchain::AcquireNextPresentable(
    std::chrono::nanoseconds timeout) {
  std::shared_ptr<Texture> result;
  bool did_pop = false;
  {
    Lock lock(mutex_);
    not_empty_cv_.WaitFor(mutex_, timeout, [&]() IPLR_REQUIRES(mutex_) {
      if (shutdown_) {
        return true;
      }
      if (!presentable_drawables_.empty()) {
        return true;
      }
      return false;
    });
    if (!presentable_drawables_.empty()) {
      result = presentable_drawables_.front();
      presentable_drawables_.pop_front();
      did_pop = true;
    }
  }
  if (did_pop) {
    not_full_cv_.NotifyOne();
  }
  return result;
}

bool Swapchain::PresentDrawable(std::shared_ptr<Texture> texture) {
  if (!texture) {
    return false;
  }
  {
    Lock lock(mutex_);
    not_full_cv_.Wait(mutex_, [&]() IPLR_REQUIRES(mutex_) {
      if (shutdown_) {
        return true;
      }
      if (presentable_drawables_.size() < max_drawable_count_) {
        return true;
      }
      return false;
    });
    presentable_drawables_.push_back(std::move(texture));
  }
  not_empty_cv_.NotifyOne();
  return true;
}

std::shared_ptr<Texture> Swapchain::AcquireNextDrawable() {
  if (auto drawable = AcquireNextRecyclableDrawable()) {
    return drawable;
  }
  if (auto drawable = CreateDrawable()) {
    return drawable;
  }
  return nullptr;
}

std::shared_ptr<Texture> Swapchain::AcquireNextRecyclableDrawable() {
  Lock lock(recyclables_mutex_);
  if (recyclables_.empty()) {
    return nullptr;
  }
  auto texture = recyclables_.back().texture;
  recyclables_.pop_back();
  ClearExpiredRecyclables();
  return texture;
}

std::shared_ptr<Texture> Swapchain::CreateDrawable() const {
  auto context = context_.lock();
  if (!context) {
    VALIDATION_LOG << "Context to create the drawable from no longer exists.";
    return nullptr;
  }
  auto texture =
      context->GetImpellerContext()->GetResourceAllocator()->CreateTexture(
          texture_desc_);
  if (texture) {
    texture->SetLabel(SPrintF("Swapchain Texture %zu", texture_count_++));
  }
  return texture;
}

void Swapchain::RecycleDrawable(std::shared_ptr<Texture> texture) {
  if (!texture) {
    return;
  }

  if (texture->GetTextureDescriptor() != texture_desc_) {
    return;
  }

  Lock lock(recyclables_mutex_);
  recyclables_.push_back(Recyclable{std::move(texture)});
  ClearExpiredRecyclables();
}

void Swapchain::ClearExpiredRecyclables() {
  const auto now = std::chrono::steady_clock::now();
  while (!recyclables_.empty()) {
    if (now - recyclables_.front().recycle_point > recyclable_expiry_) {
      recyclables_.pop_front();
    } else {
      break;
    }
  }
}

void Swapchain::SetTextureDescriptor(TextureDescriptor desc) {
  Lock l1(mutex_);
  Lock l2(recyclables_mutex_);
  if (desc == texture_desc_) {
    return;
  }
  texture_desc_ = desc;
  recyclables_.clear();
}

TextureDescriptor Swapchain::CopyTextureDescriptor() const {
  Lock l1(mutex_);
  Lock l2(recyclables_mutex_);
  return texture_desc_;
}

}  // namespace impeller::stator
