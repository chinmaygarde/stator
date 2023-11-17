// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/stator/objffi/objffi.h"

#include "impeller/stator/objffi/object.h"
#include "objffi.epoxy.h"

namespace impeller::stator::objffi::epoxy {

void Retain(void* object) {
  if (!object) {
    return;
  }
  reinterpret_cast<Object*>(object)->Retain();
}

void Release(void* object) {
  if (!object) {
    return;
  }
  reinterpret_cast<Object*>(object)->Release();
}

}  // namespace impeller::stator::objffi::epoxy
