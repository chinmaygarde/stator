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
  ScopedObject() = default;

  explicit ScopedObject(T* object) : object_(object) {
    if (object_) {
      object_->Retain();
    }
  }

  explicit ScopedObject(T* object, AdoptTag) : object_(object) {}

  ~ScopedObject() { Reset(); }

  ScopedObject(const ScopedObject& o) {
    if (o.object_) {
      object_ = o.object_;
      object_->Retain();
    }
  }

  ScopedObject(ScopedObject&& o) { std::swap(o, o.object_); }

  ScopedObject& operator=(const ScopedObject& o) {
    Reset();
    if (o.object_) {
      object_ = o.object_;
      object_->Retain();
    }
    return *this;
  }

  ScopedObject& operator=(ScopedObject&& o) {
    Reset();
    if (o.object_) {
      object_ = o.object_;
      object_->Retain();
    }
    o.Reset();
    return *this;
  }

  T* operator->() const { return Get(); }

  T* Get() const { return object_; }

  [[nodiscard]] T* Leak() const {
    if (object_) {
      object_->Retain();
    }
    return object_;
  }

  void Reset() {
    if (object_) {
      object_->Release();
      object_ = nullptr;
    }
  }

  constexpr bool operator==(const ScopedObject& o) const {
    return object_ == o.object_;
  }

  constexpr bool operator!=(const ScopedObject& o) const {
    return object_ != o.object_;
  }

  constexpr explicit operator bool() const { return object_ != nullptr; }

 private:
  T* object_ = nullptr;
};

template <class T, class = std::enable_if_t<std::is_base_of_v<Object, T>>>
[[nodiscard]] ScopedObject<T> Ref(T* object) {
  return ScopedObject<T>(object);
}

template <class T, class = std::enable_if_t<std::is_base_of_v<Object, T>>>
[[nodiscard]] ScopedObject<T> Adopt(T* object) {
  return ScopedObject<T>(object, AdoptTag::kAdopt);
}

template <class T, class = std::enable_if_t<std::is_base_of_v<Object, T>>>
[[nodiscard]] ScopedObject<T> CastAndRef(void* object) {
  return Ref(reinterpret_cast<T*>(object));
}

template <class T, class = std::enable_if_t<std::is_base_of_v<Object, T>>>
[[nodiscard]] ScopedObject<T> CastAndAdopt(void* object) {
  return Adopt(reinterpret_cast<T*>(object));
}

template <class T,
          typename... Args,
          class = std::enable_if_t<std::is_base_of_v<Object, T>>>
[[nodiscard]] ScopedObject<T> Make(Args&&... args) {
  return Adopt(new T(std::forward<Args>(args)...));
}

}  // namespace impeller::stator::objffi
