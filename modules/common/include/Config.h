#ifndef MODULES_COMMON_INCLUDE_CONFIG_H_
#define MODULES_COMMON_INCLUDE_CONFIG_H_

/**
 *  Configuration
 *  Pre-Compile macro, CMake's definitions and some Header Files
 *  CC_X86_64: using standard crossing-compile toolchain
 *  CC_TTTECH: using crossing-compile toolchain by tt-tech
 *  USE_FAKELOG: using FakeLog.h, not log.h
 */

/* #undef USE_FAKELOG */
/* #undef CC_TTTECH */
#define CC_X86_64
/* #undef CC_PX2 */

// ----- For glog -----
#ifdef CC_X86_64
//#include "modules/common/include/log.h"
#endif // CC_X86_64

#ifdef USE_FAKELOG
//#include "modules/common/include/FakeLog.h"
#endif // USE_FAKELOG
// ----- End for glog -----

/**
 *  Global variables
 */

/* #undef USE_OUTSIDE_ROOT */
#ifdef USE_OUTSIDE_ROOT
#define G_ATD_MASTER_ROOT ""
#else  // USE_OUTSIDE_ROOT
#define G_ATD_MASTER_ROOT "/program/atd-master-ysg/1025/"
#endif // USE_OUTSIDE_ROOT

/*
 *  MACRO CREATE_FULL_PATH(relative_path)
 *  create the full path by combined the target root path with `relative_path`
 *  e.g., project full path: /program/atd-master/, configuration relative path: modules/common/conf/xyz.conf
 *  then using CREATE_FULL_PATH(modules/common/conf/xyz.conf)  // ** NO Quotation marks
 *  Return Value: "/program/atd-master/modules/common/conf/xyz.conf"
 */
#define CREATE_FULL_PATH(relative_path) G_ATD_MASTER_ROOT#relative_path

#endif // #ifndef MODULES_COMMON_INCLUDE_CONFIG_H_
