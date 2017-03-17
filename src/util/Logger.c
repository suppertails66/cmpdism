#include "util/Logger.h"

FILE* logger = NULL;

void initLogger() {
  #ifndef CPD_DISABLE_LOGGING
/*    logger = fopen("log.txt", "w"); */
    logger = stdout;
  #endif
}

void error(const char* str) {
  fputs(str, stderr);
}

void fatal() {
  error("\n");
  exit(1);
}

void cpdlog(const char* str) {
  #ifndef CPD_DISABLE_LOGGING
    fputs(str, logger);
    fputs("\n", logger);
  #endif
}
