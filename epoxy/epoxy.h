// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "flutter/fml/build_config.h"

#if FML_OS_WIN
#define STATOR_EXPORT __declspec(dllexport)
#else  // FML_OS_WIN
#define STATOR_EXPORT __attribute__((visibility("default")))
#endif  // FML_OS_WIN

#ifdef __cplusplus
#define STATOR_EXTERN_C_BEGIN extern "C" {
#define STATOR_EXTERN_C_END }
#else  //  __cplusplus
#define STATOR_EXTERN_C_BEGIN
#define STATOR_EXTERN_C_END
#endif  // __cplusplus

namespace impeller::stator {}  // namespace impeller::stator
