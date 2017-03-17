#ifndef CPD_LOGGER_H
#define CPD_LOGGER_H


#include <stdlib.h>
#include <stdio.h>

extern FILE* logger;

void initLogger();

void error(const char* str);
void fatal();
void cpdlog(const char* str);


#endif
