// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "flutter/fml/macros.h"

namespace impeller::stator {

class Context {
 public:
  Context();

  ~Context();

  Context(const Context&) = delete;

  Context& operator=(const Context&) = delete;
};

}  // namespace impeller::stator
