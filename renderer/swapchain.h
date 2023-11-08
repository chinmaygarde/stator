// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <chrono>
#include <deque>
#include <memory>

#include "impeller/base/thread.h"
#include "impeller/core/texture.h"
#include "impeller/core/texture_descriptor.h"

namespace impeller::stator {

class Context;

class Swapchain {
 public:
  explicit Swapchain(const std::shared_ptr<Context>& context,
                     size_t max_drawable_count = 3u,
                     std::chrono::milliseconds recylable_expiry =
                         std::chrono::milliseconds{32});

  ~Swapchain();

  bool IsValid() const;

  TextureDescriptor CopyTextureDescriptor() const;

  void SetTextureDescriptor(TextureDescriptor desc);

  Swapchain(const Swapchain&) = delete;

  Swapchain& operator=(const Swapchain&) = delete;

  void Shutdown();

  std::shared_ptr<Texture> AcquireNextDrawable();

  void RecycleDrawable(std::shared_ptr<Texture> texture);

  std::shared_ptr<Texture> AcquireNextPresentable();

  bool PresentDrawable(std::shared_ptr<Texture> texture);

 private:
  struct Recyclable {
    std::chrono::time_point<std::chrono::steady_clock> recycle_point;
    std::shared_ptr<Texture> texture;

    explicit Recyclable(std::shared_ptr<Texture> p_texture)
        : recycle_point(std::chrono::steady_clock::now()),
          texture(std::move(p_texture)) {}
  };

  std::weak_ptr<Context> context_;
  const size_t max_drawable_count_;
  const std::chrono::milliseconds recyclable_expiry_;
  mutable Mutex mutex_;
  std::deque<std::shared_ptr<Texture>> presentable_drawables_
      IPLR_GUARDED_BY(mutex_);
  bool shutdown_ IPLR_GUARDED_BY(mutex_) = false;
  ConditionVariable not_empty_cv_;
  ConditionVariable not_full_cv_;
  mutable Mutex recyclables_mutex_;
  std::deque<Recyclable> recyclables_ IPLR_GUARDED_BY(recyclables_mutex_);
  TextureDescriptor texture_desc_ IPLR_GUARDED_BY(mutex_)
      IPLR_GUARDED_BY(recyclables_mutex_);
  bool is_valid_ = false;

  std::shared_ptr<Texture> CreateDrawable() const;

  std::shared_ptr<Texture> AcquireNextRecyclableDrawable();

  void ClearExpiredRecyclables() IPLR_REQUIRES(recyclables_mutex_);
};

}  // namespace impeller::stator
