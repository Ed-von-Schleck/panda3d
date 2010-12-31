building_dll(CORE)
set(sources pnmimage_composite1.cxx pnmimage_composite2.cxx)

set(scan_headers
  config_pnmimage.h pnmBrush.h pnmFileType.h pnmFileTypeRegistry.h
  pnmImage.h pnmImageHeader.h pnmPainter.h pnmReader.h pnmWriter.h
  pnmimage_base.h
)

set(install_headers
  config_pnmimage.h pnmBrush.h pnmBrush.I pnmFileType.h pnmFileTypeRegistry.h
  pnmImage.I pnmImage.h pnmImageHeader.I pnmImageHeader.h pnmPainter.h
  pnmPainter.I pnmReader.I pnmReader.h pnmWriter.I pnmWriter.h pnmimage_base.h
)

add_library(p3pnmimage ${sources})
use_libraries(p3pnmimage ZLIB)
interrogate(p3pnmimage core ${scan_headers} ${sources})

install_headers(${install_headers})

