#include "klvm.h"
#include "mempool.h"
#include "pool.hpp"
#include "boost/pool/singleton_pool.hpp"

#define SIZE(x) (sizeof(kl_bool) + (sizeof(void*) * x))
#define POOL(x) using pool##x = boost::singleton_pool<pool_tag, SIZE(x), memallocator, boost::details::pool::default_mutex, 4087 / SIZE(x)>;
#define ALLOC(x) base += sptr; if(size <= base) return pool##x::malloc();
#define DEALLOC(x) base += sptr; if(size <= base) { pool##x::free(ptr); return; }
#define PURGE(x) pool##x::purge_memory();
#define rep(x) x(1) x(2) x(3) x(4) x(5) x(6) x(7) x(8) x(9) x(10) x(11) x(12) x(13) x(14) x(15)

struct pool_tag {
};

#define HAVE_MMAP
#  ifdef MS_WINDOWS
#    include <windows.h>
#  elif defined(HAVE_MMAP)

#    include <sys/mman.h>

#    ifdef MAP_ANONYMOUS
#      define ARENAS_USE_MMAP
#    endif
#  endif

struct custom_allocator {
	typedef std::size_t size_type; //!< An unsigned integral type that can represent the size of the largest object to be allocated.
	typedef std::ptrdiff_t difference_type; //!< A signed integral type that can represent the difference of any two pointers.

	static char *
	malloc BOOST_PREVENT_MACRO_SUBSTITUTION(const size_type size) { //! Attempts to allocate n bytes from the system. Returns 0 if out-of-memory
#ifdef MS_WINDOWS
		return VirtualAlloc(NULL, size,
						MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#elif defined(ARENAS_USE_MMAP)
		void *ptr;
		ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE,
				   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (ptr == MAP_FAILED)
			return nullptr;
		assert(ptr != nullptr);
		return static_cast<char *>(ptr);
#else
		return malloc(size);
#endif
	}

	static void free BOOST_PREVENT_MACRO_SUBSTITUTION(void *const ptr, size_t size) { //! Attempts to de-allocate block.
#ifdef MS_WINDOWS
		VirtualFree(ptr, 0, MEM_RELEASE);
#elif defined(ARENAS_USE_MMAP)
		munmap(ptr, size);
#else
		free(ptr);
#endif
	}
};

using memallocator = custom_allocator;

POOL(0)
rep(POOL)

void *mempoolAlloc(size_t size) {
	auto sptr = sizeof(void *);
	auto base = sizeof(kl_bool);
	if (size <= base) {
		return pool0::malloc();
	}
	rep(ALLOC)

	return malloc(size);
}

void mempoolDealloc(void *ptr, size_t size) {
	auto sptr = sizeof(void *);
	auto base = sizeof(kl_bool);
	if (size <= base) {
		pool0::free(ptr);
		return;
	}
	rep(DEALLOC)

	free(ptr);
}

void freeAllocs() {
	PURGE(0)
	rep(PURGE)
}
