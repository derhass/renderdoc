/******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Baldur Karlsson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#include "metal_device.h"
#include "metal_manager.h"

WrappedMTLDevice::WrappedMTLDevice(MTL::Device *realMTLDevice, ResourceId objId)
    : WrappedMTLObject(realMTLDevice, objId, this, GetStateRef())
{
  wrappedObjC = AllocateObjCWrapper(this);
  Construct();
  GetResourceManager()->AddCurrentResource(objId, this);
}

void WrappedMTLDevice::Construct()
{
  objc = AllocateObjCWrapper(this);
  m_WrappedMTLDevice = this;
  threadSerialiserTLSSlot = Threading::AllocateTLSSlot();

  m_ResourceManager = new MetalResourceManager(m_State, this);
  RDCASSERT(m_WrappedMTLDevice == this);
}

MTL::Device *WrappedMTLDevice::MTLCreateSystemDefaultDevice(MTL::Device *realMTLDevice)
{
  ResourceId objId = ResourceIDGen::GetNewUniqueID();
  WrappedMTLDevice *wrappedMTLDevice = new WrappedMTLDevice(realMTLDevice, objId);

  return UnwrapObjC<MTL::Device *>(wrappedMTLDevice);
}