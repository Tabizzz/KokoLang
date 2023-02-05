#pragma once

#include <map>
#include "klimports.h"

#ifdef __cplusplus
extern "C" {
#endif

#define KLOBJECTHEAD KlObject klbase;

#define KLINVOKE(x) if(x) x

struct KLType;

/*
 * The global dynamic object representation in kokolang.
 * Any type can be used a pointer to this class to be a valid kokolang object.
 */
typedef struct KlObject {
	struct KLType* type; 	// the type of this object
	size_t refs;			// how many objects are referencing this
} KlObject;

#ifdef __cplusplus
}
#endif

typedef std::map<std::string, KlObject *> MetaMap;