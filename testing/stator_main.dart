// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// ignore_for_file: avoid_print, unused_field, non_constant_identifier_names, always_specify_types, unused_local_variable

import 'dart:ffi';

final class NativeContext extends Opaque { }
typedef NativeContextPointer = Pointer<NativeContext>;

final class Context implements Finalizable {
  Context(this.context) {
    stator.AttachReleaseContextFinalizer(this, context);
  }

  bool isDisposed() => context != nullptr;

  void dispose() {
    stator.DetachReleaseContextFinalizer(this, context);
    context = nullptr;
  }

  Pointer<NativeContext> context;
}

final class NativeSwapchain extends Opaque { }

final class Stator {
  Stator(this.dylib);

  final DynamicLibrary dylib;

  late final Pointer<NativeContext> Function() GetGlobalContext = dylib.lookup<NativeFunction<NativeContextPointer Function()>>('GetGlobalContext').asFunction();

  late final Pointer<NativeFunction<Void Function(Pointer<NativeContext>)>> ReleaseContextFN = dylib.lookup<NativeFunction<Void Function(Pointer<NativeContext>)>>('ReleaseContext');
  late final void Function(Pointer<NativeContext>) ReleaseContext = ReleaseContextFN.asFunction();
  late final ReleaseContextFinalizer = NativeFinalizer(ReleaseContextFN.cast());
  void AttachReleaseContextFinalizer(Context context, Pointer<NativeContext> ptr) {
    ReleaseContextFinalizer.attach(context, ptr.cast(), detach: context);
  }
  void DetachReleaseContextFinalizer(Context context, Pointer<NativeContext> ptr) {
    if (ptr == nullptr) {
      return;
    }
    ReleaseContext(ptr);
    ReleaseContextFinalizer.detach(context);
  }
}

final stator = Stator(DynamicLibrary.process());

void main() {
  var context = Context(stator.GetGlobalContext());
  context.dispose();
  context.dispose();
  context.dispose();
}
