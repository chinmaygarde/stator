// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>

#include "impeller/entity/contents/content_context.h"
#include "impeller/renderer/render_pass.h"

namespace impeller::stator {

class Swapchain;

class Compositor {
 public:
  explicit Compositor(std::shared_ptr<impeller::Context> impeller_context,
                      std::weak_ptr<Swapchain> swapchain);

  ~Compositor();

  bool IsValid() const;

  Compositor(const Compositor&) = delete;

  Compositor& operator=(const Compositor&) = delete;

  bool Composite(RenderPass& pass);

 private:
  std::weak_ptr<Swapchain> swapchain_;
  std::shared_ptr<Texture> current_presentable_;
  // TODO(csg): Content contexts are expensive but can be inherited. For now, a
  // new one is created per compositor.
  ContentContext content_context_;
  bool is_valid_ = false;
};

}  // namespace impeller::stator
