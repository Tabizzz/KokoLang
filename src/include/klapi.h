#pragma once

#include "KLImports.h"
#include "Runtime/KLRuntime.h"

#ifndef CAPI_H
#define CAPI_H

CAPI KLProgram* klCreateProgramFromFile(const char* filename);

CAPI KLProgram* klCreateProgramFromString(const char* code);

CAPI void klDestroyProgram(KLProgram* obj);

#endif // !CAPI_H

