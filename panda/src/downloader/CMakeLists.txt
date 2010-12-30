set(sources downloader_composite1.cxx downloader_composite2.cxx)

set(scan_headers
  config_downloader.h bioPtr.h bioStreamPtr.h bioStream.h bioStreamBuf.h
  chunkedStream.h chunkedStreamBuf.h decompressor.h documentSpec.h downloadDb.h
  download_utils.h extractor.h httpAuthorization.h httpBasicAuthorization.h
  httpChannel.h httpClient.h httpCookie.h httpDate.h httpDigestAuthorization.h
  httpEntityTag.h httpEnum.h identityStream.h identityStreamBuf.h
  multiplexStream.h multiplexStreamBuf.h patcher.h socketStream.h
  stringStreamBuf.h stringStream.h urlSpec.h virtualFileHTTP.h
  virtualFileMountHTTP.h
)

set(install_headers
  bioPtr.I bioPtr.h bioStreamPtr.I bioStreamPtr.h bioStream.I bioStream.h
  bioStreamBuf.h chunkedStream.I chunkedStream.h chunkedStreamBuf.h
  chunkedStreamBuf.I config_downloader.h decompressor.h decompressor.I
  documentSpec.h documentSpec.I download_utils.h downloadDb.h downloadDb.I
  extractor.h extractor.I httpAuthorization.I httpAuthorization.h
  httpBasicAuthorization.I httpBasicAuthorization.h httpChannel.I
  httpChannel.h httpClient.I httpClient.h httpCookie.I httpCookie.h
  httpDate.I httpDate.h httpDigestAuthorization.I httpDigestAuthorization.h
  httpEntityTag.I httpEntityTag.h httpEnum.h identityStream.I identityStream.h
  identityStreamBuf.h identityStreamBuf.I multiplexStream.I multiplexStream.h
  multiplexStreamBuf.I multiplexStreamBuf.h patcher.h patcher.I socketStream.h
  socketStream.I stringStreamBuf.I stringStreamBuf.h stringStream.I
  stringStream.h urlSpec.h urlSpec.I virtualFileHTTP.I virtualFileHTTP.h
  virtualFileMountHTTP.I virtualFileMountHTTP.h
)

add_library(p3downloader ${sources})
interrogate(p3downloader core ${scan_headers} ${sources})

install_headers(${install_headers})
