set(sources express_composite1.cxx express_composite2.cxx)

set(scan_headers
  buffer.h checksumHashGenerator.h circBuffer.h config_express.h
  compress_string.h copy_stream.h datagram.h datagramGenerator.h
  datagramIterator.h datagramSink.h dcast.h encrypt_string.h error_utils.h
  hashGeneratorBase.h hashVal.h indirectLess.h memoryInfo.h
  memoryUsage.h memoryUsagePointerCounts.h memoryUsagePointers.h multifile.h
  namable.h nodePointerToBase.h nodePointerTo.h nodeReferenceCount.h
  openSSLWrapper.h ordered_vector.h password_hash.h patchfile.h pointerTo.h
  pointerToArray.h pointerToArrayBase.h pointerToBase.h pointerToVoid.h
  profileTimer.h pStatCollectorForwardBase.h pta_uchar.h pta_float.h pta_int.h
  ramfile.h referenceCount.h stringDecoder.h subStream.h subStreamBuf.h
  textEncoder.h threadSafePointerTo.h threadSafePointerToBase.h trueClock.h
  typedReferenceCount.h typedef.h unicodeLatinMap.h vector_uchar.h
  vector_float.h virtualFileComposite.h virtualFile.h virtualFileList.h
  virtualFileMount.h virtualFileMountMultifile.h virtualFileMountSystem.h
  virtualFileSimple.h virtualFileSystem.h weakPointerCallback.h weakPointerTo.h
  weakPointerToBase.h weakPointerToVoid.h weakReferenceList.h windowsRegistry.h
  zStream.h zStreamBuf.h
)

set(install_headers
  buffer.I buffer.h ca_bundle_data_src.c checksumHashGenerator.I
  checksumHashGenerator.h circBuffer.I circBuffer.h config_express.h
  compress_string.h copy_stream.h datagram.I datagram.h datagramGenerator.I
  datagramGenerator.h datagramIterator.I datagramIterator.h datagramSink.I
  datagramSink.h dcast.T dcast.h encrypt_string.h error_utils.h
  hashGeneratorBase.I hashGeneratorBase.h hashVal.I hashVal.h indirectLess.I
  indirectLess.h memoryInfo.I memoryInfo.h memoryUsage.I memoryUsage.h
  memoryUsagePointerCounts.I memoryUsagePointerCounts.h memoryUsagePointers.I
  memoryUsagePointers.h multifile.I multifile.h namable.I namable.h
  nodePointerToBase.h nodePointerToBase.I nodePointerTo.h nodePointerTo.I
  nodeReferenceCount.h nodeReferenceCount.I openSSLWrapper.h openSSLWrapper.I
  ordered_vector.h ordered_vector.I ordered_vector.T password_hash.h
  patchfile.I patchfile.h pointerTo.I pointerTo.h pointerToArray.I
  pointerToArray.h pointerToArrayBase.I pointerToArrayBase.h pointerToBase.I
  pointerToBase.h pointerToVoid.I pointerToVoid.h profileTimer.I profileTimer.h
  pStatCollectorForwardBase.h pta_uchar.h pta_float.h pta_int.h ramfile.I
  ramfile.h referenceCount.I referenceCount.h stringDecoder.h stringDecoder.I
  subStream.I subStream.h subStreamBuf.h textEncoder.h textEncoder.I
  threadSafePointerTo.I threadSafePointerTo.h threadSafePointerToBase.I
  threadSafePointerToBase.h trueClock.I trueClock.h typedReferenceCount.I
  typedReferenceCount.h typedef.h unicodeLatinMap.h vector_uchar.h
  vector_float.h virtualFileComposite.h virtualFileComposite.I virtualFile.h
  virtualFile.I virtualFileList.I virtualFileList.h virtualFileMount.h
  virtualFileMount.I virtualFileMountMultifile.h virtualFileMountMultifile.I
  virtualFileMountSystem.h virtualFileMountSystem.I virtualFileSimple.h
  virtualFileSimple.I virtualFileSystem.h virtualFileSystem.I
  weakPointerCallback.I weakPointerCallback.h weakPointerTo.I weakPointerTo.h
  weakPointerToBase.I weakPointerToBase.h weakPointerToVoid.I
  weakPointerToVoid.h weakReferenceList.I weakReferenceList.h windowsRegistry.h
  zStream.I zStream.h zStreamBuf.h
)

add_library(p3express ${sources})
interrogate(p3express core ${scan_headers} ${sources})

install_headers(${install_headers})
