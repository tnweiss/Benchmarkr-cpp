//
// Created by tnwei on 3/4/2022.
//

#ifndef BENCHMARKR_INCLUDE_BENCHMARKR_DEV_DEFAULTS_H_
#define BENCHMARKR_INCLUDE_BENCHMARKR_DEV_DEFAULTS_H_


namespace benchmarkr {
/**
 * GLOBAL Defaults
 */
 static const char* DEFAULT_BM_RESULTS_DIR = "results";
 static const char* DEFAULT_BM_GLOBAL_CONTEXT_FILE = "GlobalContext.json";

#ifdef WIN32
  /**
  * WINDOWS Defaults
  */
  static const char* DEFAULT_BM_DIR = "C:\\ProgramData\\Benchmarkr";
#else
  /**
   * LINUX Defaults
   */
  static const char* DEFAULT_BM_DIR = "/etc/benchmarkr";
#endif
 }

#endif //BENCHMARKR_INCLUDE_BENCHMARKR_DEV_DEFAULTS_H_
