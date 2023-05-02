#pragma once

#include "klimports.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Header for all objects.
 *
 * Any structure must have this header to be a valid Kokolang object.
 */
#define KLOBJECTHEAD KlObject klbase;

/**
 * @brief Invoke a function if is not null
 */
#define KLINVOKE(x) if(x) x

/**
 * @brief Use delete instead of free() to destroy the pointer of this object.
 */
#define KLOBJ_FLAG_USE_DELETE 1
/**
 * @brief Use to not change the inscount of the type on destroy.
 */
#define KLOBJ_FLAG_NO_INSCOUNT 2

struct KLType;

/**
 * @brief  The global dynamic object representation in kokolang.
 *
 * Any type can be used a pointer to this class to be a valid kokolang object.
 */
struct CPPAPI KlObject {
	/**
	 * @brief The type of the object.
	*/
	KLType* type;
	/**
	 * @brief How many objects are referencing this object.
	 *
	 * In types with the KLTYPE_FLAG_NOINSTANCE flag the refs count is not change and is always 1.
	 *
	 * @see KLTYPE_FLAG_NOINSTANCE
	 */
	uint32_t refs;
	/**
	 * @brief flags field.
	 */
	uint32_t flags; // to be used as flags for garbage collection.
};

#ifdef __cplusplus
}
#endif

/**
 * @brief Map of kokolang objects.
 *
 * Used mainly to store metadata.
 */
typedef std::map<std::string, KlObject *> MetaMap;
/**
 * @brief Pair of key-value of a MetaMap
 */
typedef std::pair<std::string, KlObject*> MetaPair;