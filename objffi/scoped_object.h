// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <type_traits>

#include "impeller/stator/objffi/object.h"

namespace impeller::stator::objffi {

enum class AdoptTag {
  kAdopt,
};

template <class T, class = std::enable_if_t<std::is_base_of_v<Object, T>>>
class ScopedObject {
 public:
  explicit ScopedObject(T* object) : object_(object) { object_->Retain(); }

  explicit ScopedObject(T* object, AdoptTag) : object_(object) {}

  ~ScopedObject() { object_->Release(); }

  ScopedObject(const ScopedObject&) = delete;

  ScopedObject& operator=(const ScopedObject&) = delete;

  T* operator->() const { return object_; }

 private:
  T* object_;
};

}  // namespace impeller::stator::objffi
