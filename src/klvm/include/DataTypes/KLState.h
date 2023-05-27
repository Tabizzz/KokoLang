#pragma once

#include <thread>
#include "klimports.h"
#include "klresolver.h"

#ifdef __cplusplus
extern "C" {
#endif

struct CPPAPI KLThreadState {
	std::vector<KLFunction*> callStack;
	std::thread::id threadId;

	KLThreadState();
};

struct CPPAPI KLState {
	klresolver resolver;
	KLThreadState* mainThreadState;
};

#ifdef __cplusplus
}

#endif

CAPI KLState* klState;

CAPI KLThreadState* klGetThreadState();