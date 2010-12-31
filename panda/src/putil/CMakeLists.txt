building_dll(CORE)
set(sources putil_composite1.cxx putil_composite2.cxx)

set(install_headers
  animInterface.h animInterface.I bam.h bamCache.h bamCache.I bamCacheIndex.h
  bamCacheIndex.I bamCacheRecord.h bamCacheRecord.I bamEnums.h bamReader.I
  bamReader.h bamReaderParam.I bamReaderParam.h bamWriter.I bamWriter.h
  bitArray.I bitArray.h bitMask.I bitMask.h buttonHandle.I buttonHandle.h
  buttonRegistry.I buttonRegistry.h cachedTypedWritableReferenceCount.h
  cachedTypedWritableReferenceCount.I callbackData.h callbackData.I
  callbackObject.h callbackObject.I clockObject.h clockObject.I collideMask.h
  copyOnWriteObject.h copyOnWriteObject.I copyOnWritePointer.h
  copyOnWritePointer.I compareTo.I compareTo.h config_util.h configurable.h
  cPointerCallbackObject.h cPointerCallbackObject.I datagramInputFile.I
  datagramInputFile.h datagramOutputFile.I datagramOutputFile.h doubleBitMask.I
  doubleBitMask.h drawMask.h factory.I factory.h factoryBase.I factoryBase.h
  factoryParam.I factoryParam.h factoryParams.I factoryParams.h
  firstOfPairCompare.I firstOfPairCompare.h firstOfPairLess.I firstOfPairLess.h
  globalPointerRegistry.I globalPointerRegistry.h indirectCompareNames.I
  indirectCompareNames.h indirectCompareSort.I indirectCompareSort.h
  indirectCompareTo.I indirectCompareTo.h ioPtaDatagramFloat.h
  ioPtaDatagramInt.h ioPtaDatagramShort.h iterator_types.h keyboardButton.h
  lineStream.I lineStream.h lineStreamBuf.I lineStreamBuf.h linkedListNode.I
  linkedListNode.h load_prc_file.h loaderOptions.I loaderOptions.h
  modifierButtons.I modifierButtons.h mouseButton.h mouseData.I mouseData.h
  nameUniquifier.I nameUniquifier.h nodeCachedReferenceCount.h
  nodeCachedReferenceCount.I portalMask.h pbitops.I pbitops.h pta_double.h
  pta_ushort.h pythonCallbackObject.h pythonCallbackObject.I simpleHashMap.I
  simpleHashMap.h sparseArray.I sparseArray.h string_utils.I string_utils.h
  timedCycle.I timedCycle.h typedWritable.I typedWritable.h
  typedWritableReferenceCount.I typedWritableReferenceCount.h updateSeq.I
  updateSeq.h uniqueIdAllocator.h vector_double.h vector_typedWritable.h
  vector_ushort.h vector_writable.h writableConfigurable.h writableParam.I
  writableParam.h
)

set(scan_headers
  animInterface.h bam.h bamCache.h bamCacheIndex.h bamCacheRecord.h bamEnums.h
  bamReader.h bamReaderParam.h bamWriter.h bitArray.h bitMask.h buttonHandle.h
  buttonRegistry.h cachedTypedWritableReferenceCount.h callbackData.h
  callbackObject.h clockObject.h collideMask.h copyOnWriteObject.h
  copyOnWritePointer.h compareTo.h config_util.h configurable.h
  cPointerCallbackObject.h datagramInputFile.h datagramOutputFile.h
  doubleBitMask.h drawMask.h factory.h factoryBase.h factoryParam.h
  factoryParams.h firstOfPairCompare.h firstOfPairLess.h
  globalPointerRegistry.h indirectCompareNames.h indirectCompareSort.h
  indirectCompareTo.h ioPtaDatagramFloat.h ioPtaDatagramInt.h
  ioPtaDatagramShort.h keyboardButton.h lineStream.h lineStreamBuf.h
  linkedListNode.h load_prc_file.h loaderOptions.h modifierButtons.h
  mouseButton.h mouseData.h nameUniquifier.h nodeCachedReferenceCount.h
  pbitops.h portalMask.h pta_double.h pta_ushort.h pythonCallbackObject.h
  simpleHashMap.h sparseArray.h string_utils.h timedCycle.h typedWritable.h
  typedWritableReferenceCount.h updateSeq.h uniqueIdAllocator.h vector_double.h
  vector_typedWritable.h vector_ushort.h vector_writable.h
  writableConfigurable.h writableParam.h
)

add_library(p3putil ${sources})
use_libraries(p3putil OPENSSL ZLIB)
interrogate(p3putil core ${scan_headers} ${sources})

install_headers(${install_headers})
