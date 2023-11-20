// THIS FILE IS GENERATED BY THE EPOXY FFI BINDIGS GENERATOR VERSION 0.1.0.

// ignore_for_file: always_specify_types

import 'dart:ffi';

import '/Users/chinmaygarde/VersionControlled/engine/src/out/host_debug_unopt_arm64/gen/flutter/impeller/stator/renderer/renderer.epoxy.dart';
import '/Users/chinmaygarde/VersionControlled/engine/src/out/host_debug_unopt_arm64/gen/flutter/impeller/stator/objffi/objffi.epoxy.dart';

abstract class OBJFFIObject implements Finalizable {
  OBJFFIObject(this.peer) {
    objffi.Retain(peer);
    finalizer.attach(this, peer, detach: this);
  }

  bool isDisposed() {
    return peer == nullptr;
  }

  void dispose() {
    if (peer == nullptr) {
      return;
    }
    finalizer.detach(this);
    objffi.Release(peer);
    peer = nullptr;
  }

  Pointer<Void> peer;

  static var finalizer = NativeFinalizer(objffi.ReleasePTR);
}

final class Texture extends OBJFFIObject {
  Texture(this.texture) : super(texture.cast());

  FFITexturePointer texture;
}

final class Swapchain extends OBJFFIObject {
  Swapchain(this.swapchain) : super(swapchain.cast());

  Texture acquireNextDrawable() {
    final texture = renderer.SwapchainNextDrawableNew(swapchain);
    final wrapped = Texture(texture);
    objffi.Release(texture.cast());
    return wrapped;
  }

  bool presentDrawable(Texture texture) {
    final result = renderer.SwapchainPresentDrawable(swapchain, texture.texture);
    texture.dispose();
    return result;
  }

  FFISwapchainPointer swapchain;
}

final class Context extends OBJFFIObject {
  Context(this.context) : super(context.cast());

  Swapchain copySwapchain() {
    return Swapchain(renderer.ContextSwapchainCopy(context));
  }

  FFIContextPointer context;
}

final Context context = Context(renderer.GetGlobalContext());
final Swapchain swapchain = context.copySwapchain();

void main () {
  assert(!context.isDisposed());
  assert(!swapchain.isDisposed());
  final texture = swapchain.acquireNextDrawable();
  swapchain.presentDrawable(texture);
}
