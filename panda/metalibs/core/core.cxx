// Filename: core.cxx
// Created by:  rdb (28Dec10)
//
////////////////////////////////////////////////////////////////////

#include "core.h"

#include "config_downloader.h"
#include "config_express.h"
#include "config_linmath.h"
#include "config_nativenet.h"
#include "config_net.h"
#include "config_pipeline.h"
#include "config_pnmimage.h"
#include "config_pstats.h"
#include "config_util.h"

// By including checkPandaVersion.h, we guarantee that runtime
// attempts to load libpanda.so/.dll will fail if they inadvertently
// link with the wrong version of libdtool.so/.dll.

#include "checkPandaVersion.h"

////////////////////////////////////////////////////////////////////
//     Function: init_core
//  Description: Initializes the library.  This must be called at
//               this library can be used.  Normally it will be
//               least once before any of the functions or classes in
//               called by the static initializers and need not be
//               called explicitly, but special cases exist.
////////////////////////////////////////////////////////////////////
void
init_core() {
  init_libdownloader();
  init_libexpress();
  init_liblinmath();
  init_libnativenet();
  init_libnet();
  init_libpipeline();
  init_libpnmimage();
  init_libpstatclient();
  init_libputil();
}
