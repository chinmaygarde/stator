// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/testing/testing.h"
#include "impeller/stator/objffi/object.h"
#include "impeller/stator/objffi/scoped_object.h"

namespace impeller::stator::objffi {

class MyObject : public Object {
 public:
  MyObject(bool& destroyed) : destroyed_(destroyed) {}

  ~MyObject() { destroyed_ = true; }

 private:
  bool& destroyed_;
};

TEST(ObjFFITest, CanRefCountObjects) {
  bool destroyed = false;
  auto object = new MyObject(destroyed);
  ASSERT_FALSE(destroyed);
  ASSERT_EQ(object->GetRefCount(), 1u);
  object->Retain();
  ASSERT_FALSE(destroyed);
  ASSERT_EQ(object->GetRefCount(), 2u);
  object->Retain();
  ASSERT_FALSE(destroyed);
  ASSERT_EQ(object->GetRefCount(), 3u);
  object->Release();
  ASSERT_FALSE(destroyed);
  ASSERT_EQ(object->GetRefCount(), 2u);
  object->Release();
  ASSERT_FALSE(destroyed);
  ASSERT_EQ(object->GetRefCount(), 1u);
  object->Release();
  ASSERT_TRUE(destroyed);
}

TEST(ObjFFITest, CanRefCountObjectsByBase) {
  bool destroyed = false;
  auto object = new MyObject(destroyed);
  ASSERT_FALSE(destroyed);
  ASSERT_EQ(object->GetRefCount(), 1u);
  reinterpret_cast<Object*>(object)->Release();
  ASSERT_TRUE(destroyed);
}

TEST(ObjFFITest, CanCreateScopedObject) {
  bool destroyed = false;
  auto my_object = new MyObject(destroyed);
  {
    ScopedObject<MyObject> object(my_object);
    ASSERT_EQ(object->GetRefCount(), 2u);
  }
  ASSERT_EQ(my_object->GetRefCount(), 1u);
  ASSERT_FALSE(destroyed);
  {
    my_object->Retain();
    ASSERT_EQ(my_object->GetRefCount(), 2u);
    ScopedObject<MyObject> object(my_object, AdoptTag::kAdopt);
    ASSERT_EQ(object->GetRefCount(), 2u);
  }
  ASSERT_EQ(my_object->GetRefCount(), 1u);
  ASSERT_FALSE(destroyed);
  my_object->Release();
  ASSERT_TRUE(destroyed);
}

TEST(ObjFFITest, TypeGuards) {
  bool destroyed = false;
  auto my_object = new MyObject(destroyed);
  ScopedObject object(my_object);
  // ScopedObject object2(new int{12});
}

}  // namespace impeller::stator::objffi
