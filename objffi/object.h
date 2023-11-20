// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <atomic>
#include <utility>

namespace impeller::stator::objffi {

class Object {
 public:
  virtual ~Object() = default;

  Object(const Object&) = delete;

  Object& operator=(const Object&) = delete;

  void Retain() { ref_count_++; }

  void Release() {
    if (ref_count_-- == 1u) {
      delete this;
    }
  }

  // Only used in tests.
  size_t GetRefCount() const { return ref_count_; }

 protected:
  Object() = default;

 private:
  std::atomic_uint_fast32_t ref_count_ = {1u};
};

}  // namespace impeller::stator::objffi
