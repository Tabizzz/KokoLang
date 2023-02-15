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
	 * @brief reserved field.
	 */
	uint32_t reserved; // to be used as flags for garbage collection.
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