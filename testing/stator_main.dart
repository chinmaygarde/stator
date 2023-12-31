// THIS FILE IS GENERATED BY THE EPOXY FFI BINDIGS GENERATOR VERSION 0.1.0.

// ignore_for_file: always_specify_types

import 'dart:async';
import 'dart:ffi';
import 'dart:math';

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
    final texture = renderer.FFISwapchainNextDrawableNew(swapchain);
    final wrapped = Texture(texture);
    objffi.Release(texture.cast());
    return wrapped;
  }

  bool presentDrawable(Texture texture) {
    final result = renderer.FFISwapchainPresentDrawable(swapchain, texture.texture);
    texture.dispose();
    return result;
  }

  FFISwapchainPointer swapchain;
}

final class Context extends OBJFFIObject {
  Context(this.context) : super(context.cast());

  Swapchain copySwapchain() {
    return Swapchain(renderer.FFIContextSwapchainCopy(context));
  }

  CommandBuffer createCommandBuffer() {
    return CommandBuffer(renderer.FFIContextCommandBufferNew(context));
  }

  FFIContextPointer context;
}

final class CommandBuffer extends OBJFFIObject {
  CommandBuffer(this.commandBuffer) : super(commandBuffer.cast());

  RenderPass createRenderPass(RenderTarget renderTarget) {
    return RenderPass(renderer.FFICommandBufferCreateRenderPassNew(commandBuffer, renderTarget.target));
  }

  bool submit() {
    return renderer.FFICommandBufferSubmit(commandBuffer);
  }

  FFICommandBufferPointer commandBuffer;
}

final class RenderPass extends OBJFFIObject {
  RenderPass(this.renderPass) : super(renderPass.cast());

  bool addCommand(Command command) {
    return renderer.FFIRenderPassAddCommand(renderPass, command.command);
  }

  bool encodeCommands() {
    return renderer.FFIRenderPassEncodeCommands(renderPass);
  }

  FFIRenderPassPointer renderPass;
}

final class Command extends OBJFFIObject {
  Command(this.command) : super(command.cast());

  FFICommandPointer command;
}

final class RenderTarget extends OBJFFIObject {
  RenderTarget() : this.fromPtr(renderer.RenderTargetNew());

  RenderTarget.fromPtr(this.target) : super(target.cast());

  Map<int, ColorAttachment> get colorAttachments => _colorAttachments;

  bool setColorAttachment(ColorAttachment attachment, int index) {
    if (index < 0) {
      return false;
    }
    _colorAttachments[index] = attachment;
    return renderer.FFIRenderTargetSetColorAttachment(target, attachment.attachment, index);
  }

  DepthAttachment? get depthAttachment => _depthAttachment;

  set depthAttachment(DepthAttachment? attachment) {
    if (attachment == _depthAttachment) {
      return;
    }
    _depthAttachment = attachment;
    renderer.FFIRenderTargetSetDepthAttachment(target, _depthAttachment == null ? nullptr : _depthAttachment!.attachment);
  }

  StencilAttachment? get stencilAttachment => _stencilAttachment;

  set stencilAttachment(StencilAttachment? attachment) {
    if (_stencilAttachment == attachment) {
      return;
    }
    _stencilAttachment = attachment;
    renderer.FFIRenderTargetSetStencilAttachment(target, _stencilAttachment == null ? nullptr : _stencilAttachment!.attachment);
  }

  FFIRenderTargetPointer target;
  final Map<int, ColorAttachment> _colorAttachments = <int, ColorAttachment>{};
  DepthAttachment? _depthAttachment;
  StencilAttachment? _stencilAttachment;
}

final class Color {
  Color(this.red, this.green, this.blue, this.alpha);

  Color.transparentBlack();

  double red = 0.0;
  double green = 0.0;
  double blue = 0.0;
  double alpha = 0.0;
}

final class ColorAttachment extends OBJFFIObject {
  ColorAttachment() : this.fromPtr(renderer.ColorAttachmentNew());

  ColorAttachment.fromPtr(this.attachment) : super(attachment.cast());

  LoadAction get loadAction => _loadAction;

  set loadAction(LoadAction action) {
    if (action == _loadAction) {
      return;
    }
    renderer.FFIColorAttachmentSetLoadAction(attachment, action.index);
    _loadAction = action;
  }

  StoreAction get storeAction => _storeAction;

  set storeAction(StoreAction action) {
    if (action == _storeAction) {
      return;
    }
    renderer.FFIColorAttachmentSetStoreAction(attachment, action.index);
    _storeAction = action;
  }

  Texture? get texture => _texture;

  set texture(Texture? texture) {
    if (_texture == texture) {
      return;
    }

    _texture = texture;
    renderer.FFIColorAttachmentSetTexture(attachment, _texture != null ? _texture!.texture : nullptr);
  }

  Texture? get resolveTexture => _resolveTexture;

  set resolveTexture(Texture? texture) {
    if (_resolveTexture == texture) {
      return;
    }

    _resolveTexture = texture;
    renderer.FFIColorAttachmentSetResolveTexture(attachment, _resolveTexture != null ? _resolveTexture!.texture : nullptr);
  }

  Color get clearColor => _clearColor;

  set clearColor(Color color) {
    _clearColor = color;
    final ffiColor = renderer.ColorAlloc();
    ffiColor.ref.red = color.red;
    ffiColor.ref.green = color.green;
    ffiColor.ref.blue = color.blue;
    ffiColor.ref.alpha = color.alpha;
    renderer.FFIColorAttachmentSetClearColor(attachment, ffiColor);
    renderer.ColorFree(ffiColor);
  }

  FFIColorAttachmentPointer attachment;
  Texture? _texture;
  Texture? _resolveTexture;
  LoadAction _loadAction  = LoadAction.DontCare;
  StoreAction _storeAction = StoreAction.DontCare;
  Color _clearColor = Color.transparentBlack();
}

final class DepthAttachment extends OBJFFIObject {
  DepthAttachment(this.attachment) : super(attachment.cast());

  LoadAction get loadAction => _loadAction;

  set loadAction(LoadAction action) {
    if (action == _loadAction) {
      return;
    }
    renderer.FFIDepthAttachmentSetLoadAction(attachment, action.index);
    _loadAction = action;
  }

  StoreAction get storeAction => _storeAction;

  set storeAction(StoreAction action) {
    if (action == _storeAction) {
      return;
    }
    renderer.FFIDepthAttachmentSetStoreAction(attachment, action.index);
    _storeAction = action;
  }

  Texture? get texture => _texture;

  set texture(Texture? texture) {
    if (_texture == texture) {
      return;
    }

    _texture = texture;
    renderer.FFIDepthAttachmentSetTexture(
        attachment, _texture != null ? _texture!.texture : nullptr);
  }

  Texture? get resolveTexture => _resolveTexture;

  set resolveTexture(Texture? texture) {
    if (_resolveTexture == texture) {
      return;
    }

    _resolveTexture = texture;
    renderer.FFIDepthAttachmentSetResolveTexture(attachment,
        _resolveTexture != null ? _resolveTexture!.texture : nullptr);
  }

  double get clearDepth => _clearDepth;

  set clearDepth(double depth) {
    if (_clearDepth == depth) {
      return;
    }

    _clearDepth = depth;
    renderer.FFIDepthAttachmentSetClearDepth(attachment, _clearDepth);
  }

  FFIDepthAttachmentPointer attachment;
  Texture? _texture;
  Texture? _resolveTexture;
  LoadAction _loadAction = LoadAction.DontCare;
  StoreAction _storeAction = StoreAction.DontCare;
  double _clearDepth = 0.0;
}

final class StencilAttachment extends OBJFFIObject {
  StencilAttachment(this.attachment) : super(attachment.cast());

  LoadAction get loadAction => _loadAction;

  set loadAction(LoadAction action) {
    if (action == _loadAction) {
      return;
    }
    renderer.FFIStencilAttachmentSetLoadAction(attachment, action.index);
    _loadAction = action;
  }

  StoreAction get storeAction => _storeAction;

  set storeAction(StoreAction action) {
    if (action == _storeAction) {
      return;
    }
    renderer.FFIStencilAttachmentSetStoreAction(attachment, action.index);
    _storeAction = action;
  }

  Texture? get texture => _texture;

  set texture(Texture? texture) {
    if (_texture == texture) {
      return;
    }

    _texture = texture;
    renderer.FFIStencilAttachmentSetTexture(
        attachment, _texture != null ? _texture!.texture : nullptr);
  }

  Texture? get resolveTexture => _resolveTexture;

  set resolveTexture(Texture? texture) {
    if (_resolveTexture == texture) {
      return;
    }

    _resolveTexture = texture;
    renderer.FFIStencilAttachmentSetResolveTexture(attachment,
        _resolveTexture != null ? _resolveTexture!.texture : nullptr);
  }

  int get clearStencil => _clearStencil;

  set clearStencil(int stencil) {
    if (stencil == _clearStencil) {
      return;
    }
    _clearStencil = stencil;
    renderer.FFIStencilAttachmentSetClearStencil(attachment, _clearStencil);
  }

  FFIStencilAttachmentPointer attachment;
  Texture? _texture;
  Texture? _resolveTexture;
  LoadAction _loadAction = LoadAction.DontCare;
  StoreAction _storeAction = StoreAction.DontCare;
  int _clearStencil = 0;
}

final Context context = Context(renderer.GetGlobalContext());
final Swapchain swapchain = context.copySwapchain();

void main () {
  Timer.periodic(const Duration(milliseconds: 16), (timer) {
    final texture = swapchain.acquireNextDrawable();

    final color0 = ColorAttachment();
    color0.loadAction = LoadAction.Clear;
    color0.storeAction = StoreAction.Store;
    color0.clearColor = Color(Random().nextDouble(), Random().nextDouble(), Random().nextDouble(), 1.0);
    color0.texture = texture;

    final renderTarget = RenderTarget();
    renderTarget.setColorAttachment(color0, 0);

    final commandBuffer = context.createCommandBuffer();

    final renderPass = commandBuffer.createRenderPass(renderTarget);

    renderPass.encodeCommands();

    commandBuffer.submit();

    swapchain.presentDrawable(texture);
  });
}
