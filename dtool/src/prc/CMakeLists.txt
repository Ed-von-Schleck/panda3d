configure_file(prc_parameters.h.in prc_parameters.h)

set(sources prc_composite1.cxx prc_composite2.cxx)

set(scan_headers
  bigEndian.h config_prc.h configDeclaration.h configFlags.h configPage.h
  configPageManager.h configVariable.h configVariableBase.h
  configVariableBool.h configVariableCore.h configVariableDouble.h
  configVariableEnum.h configVariableFilename.h configVariableInt.h
  configVariableInt64.h configVariableList.h configVariableManager.h
  configVariableSearchPath.h configVariableString.h encryptStreamBuf.h
  encryptStream.h globPattern.h littleEndian.h nativeNumericData.h pnotify.h
  notifyCategory.h notifyCategoryProxy.h notifySeverity.h prcKeyRegistry.h
  reversedNumericData.h streamReader.h streamWrapper.h streamWriter.h
)

set(install_headers
  bigEndian.h config_prc.h configDeclaration.I configDeclaration.h
  configFlags.I configFlags.h configPage.I configPage.h configPageManager.I
  configPageManager.h configVariable.I configVariable.h configVariableBase.I
  configVariableBase.h configVariableBool.I configVariableBool.h
  configVariableCore.I configVariableCore.h configVariableDouble.I
  configVariableDouble.h configVariableEnum.I configVariableEnum.h
  configVariableFilename.I configVariableFilename.h configVariableInt.I
  configVariableInt.h configVariableInt64.I configVariableInt64.h
  configVariableList.I configVariableList.h configVariableManager.I
  configVariableManager.h configVariableSearchPath.I configVariableSearchPath.h
  configVariableString.I configVariableString.h encryptStreamBuf.h
  encryptStreamBuf.I encryptStream.h encryptStream.I globPattern.I
  globPattern.h littleEndian.h nativeNumericData.I nativeNumericData.h
  pnotify.I pnotify.h notifyCategory.I notifyCategory.h notifyCategoryProxy.I
  notifyCategoryProxy.h notifySeverity.h prcKeyRegistry.I prcKeyRegistry.h
  reversedNumericData.I reversedNumericData.h streamReader.I streamReader.h
  streamWrapper.I streamWrapper.h streamWriter.I streamWriter.h
)

add_library(p3prc ${sources})
interrogate(p3prc core ${scan_headers} ${sources})

install_headers(${install_headers})
