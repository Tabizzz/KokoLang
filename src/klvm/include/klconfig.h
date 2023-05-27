#pragma once

#include "klimports.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *(*klallocator)(size_t);

typedef void (*kldeallocator)(void *, size_t);

/**
 * @brief Contains configuration to set before calling klInit().
 */
struct CPPAPI KLConfig {
	/**
	 * @brief Allocation function used by the runtime, <b>default = std::malloc</b>.
	 */
	klallocator alloc;
	/**
	 * @brief Deallocation function used by the runtime, <b>default is a wrapper of
	 * std::free</b>.
	 */
	kldeallocator dealloc;
	/**
	 * @brief A path pointing to the exe install dir, this is where the runtime will try
	 * to find packages, <b>default = std::filesystem::current_path()</b>.
	 */
	const char* installDir;
	/**
	 * @brief Command line args, <b>No default value</b>
	 *
	 * This argv need to skip the first value and any other option that is for the runtime
	 * and not for the program.
	 */
	const char** argv;
	/**
	 * @brief Number of elements in argv, <b>No default value</b>.
	 */
	int argc;

};

#ifdef __cplusplus
}
#endif
/**
 * @brief The config used to initialize the runtime.
 *
 * If some field of are not set they will be filled with default values
 * provided by the runtime.
 */
CAPI KLConfig klConfig;