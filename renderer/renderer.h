// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "impeller/stator/objffi/scoped_object.h"
#include "impeller/stator/renderer/context.h"
#include "renderer.epoxy.h"

namespace impeller::stator::renderer {

class FFIContext final : public objffi::Object {
 public:
  explicit FFIContext(std::shared_ptr<Context> context)
      : context_(std::move(context)) {}

 private:
  std::shared_ptr<Context> context_;
};

void SetGlobalContext(objffi::ScopedObject<FFIContext> context);

}  // namespace impeller::stator::renderer
