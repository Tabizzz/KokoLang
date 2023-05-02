#pragma once

#if defined _WIN32 || defined __CYGWIN__
#ifdef kilasm_EXPORTS
#define LIBEXPORT __declspec(dllexport)
#elif defined KOKOLANGLIB_STATIC
#define LIBEXPORT
#else
#define LIBEXPORT __declspec(dllimport)
#endif
#else
#ifdef __GNUC__
#define LIBEXPORT  __attribute__((__visibility__("default")))
#else
#define LIBEXPORT
#endif
#endif


#define KLIBAPI EXTERN LIBEXPORT

#define SWITCH_TYPE(x, y, z)	\
y->inscount--;			\
z->inscount++;			\
x->type = z;
