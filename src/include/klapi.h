#pragma once

#include "KLImports.h"
#include "Runtime/KLRuntime.h"

#ifndef CAPI_H
#define CAPI_H

CAPI KLProgram* klCreateProgram();

CAPI void klDestroyProgram(KLProgram* obj);

#endif // !CAPI_H

