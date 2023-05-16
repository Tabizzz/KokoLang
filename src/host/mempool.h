#pragma once

#include <cstddef>

void* mempoolAlloc(size_t size);

void mempoolDealloc(void* ptr, size_t size);

void freeAllocs();