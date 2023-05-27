#include "../klvm_internal.h"
#include "DataTypes/KLState.h"

thread_local KLThreadState threadState;
KLState *klState;

KLThreadState *klGetThreadState() {
	return &threadState;
}

KLThreadState::KLThreadState() {
	threadId = this_thread::get_id();
}