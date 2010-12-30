SET(sources dtoolbase_composite1.cxx dtoolbase_composite2.cxx lookup3.c indent.cxx)

set(scan_headers
  addHash.h atomicAdjust.h atomicAdjustDummyImpl.h atomicAdjustI386Impl.h
  atomicAdjustPosixImpl.h atomicAdjustWin32Impl.h cmath.h deletedBufferChain.h
  deletedChain.h indent.h memoryBase.h memoryHook.h mutexImpl.h
  mutexDummyImpl.h mutexPosixImpl.h mutexWin32Impl.h mutexSpinlockImpl.h
  nearly_zero.h neverFreeMemory.h numeric_types.h pstrtod.h register_type.h
  selectThreadImpl.h stl_compares.h typeHandle.h typeRegistry.h
  typeRegistryNode.h typedObject.h pallocator.h pdeque.h plist.h pmap.h pset.h
  pvector.h lookup3.h
)

set(install_headers
  addHash.I addHash.h atomicAdjust.h atomicAdjustDummyImpl.h
  atomicAdjustDummyImpl.I atomicAdjustI386Impl.h atomicAdjustI386Impl.I
  atomicAdjustPosixImpl.h atomicAdjustPosixImpl.I atomicAdjustWin32Impl.h
  atomicAdjustWin32Impl.I cmath.I cmath.h deletedBufferChain.h
  deletedBufferChain.I deletedChain.h deletedChain.T dtoolbase.h dtoolbase_cc.h
  dtoolsymbols.h dtool_platform.h fakestringstream.h indent.I indent.h
  memoryBase.h memoryHook.h memoryHook.I mutexImpl.h mutexDummyImpl.h
  mutexDummyImpl.I mutexPosixImpl.h mutexPosixImpl.I mutexWin32Impl.h
  mutexWin32Impl.I mutexSpinlockImpl.h mutexSpinlockImpl.I nearly_zero.h
  neverFreeMemory.h neverFreeMemory.I numeric_types.h pstrtod.h register_type.I
  register_type.h selectThreadImpl.h stl_compares.I stl_compares.h typeHandle.I
  typeHandle.h typeRegistry.I typeRegistry.h typeRegistryNode.I
  typeRegistryNode.h typedObject.I typedObject.h pallocator.T pallocator.h
  pdeque.h plist.h pmap.h pset.h pvector.h lookup3.h
)

add_library(p3dtoolbase ${sources})
interrogate(p3dtoolbase core ${scan_headers} ${sources})

install_headers(${install_headers})
