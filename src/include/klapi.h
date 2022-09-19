#pragma once

#include "KLImports.h"
#include "Runtime/KLRuntime.h"

#ifndef CAPI_H
#define CAPI_H

CAPI KLPackage* klCreateProgram();

CAPI void klDestroyProgram(KLPackage* obj);

#endif // !CAPI_H

