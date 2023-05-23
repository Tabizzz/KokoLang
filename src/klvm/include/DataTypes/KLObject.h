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
/**
 * @brief Ignored reference counting or this object.
 */
#define KLOBJ_FLAG_IGNORE_REF 4
/**
 * @brief The object will be used as reference ignoring clone and copy operations.
 */
#define KLOBJ_FLAG_CONST 8

struct KLType;

struct KlObjectFlags {
	kbyte use_delete: 1;
	kbyte no_inscount: 1;
	kbyte ignore_ref: 1;
	kbyte constant: 1;
};

/**
 * @brief  The global dynamic object representation in kokolang.
 *
 * Any type can be used a pointer to this class to be a valid kokolang object.
 */
struct CPPAPI KlObject {
	/**
	 * @brief The type of the object.
	*/
	KLType *type;
	/**
	 * @brief How many objects are referencing this object.
	 *
	 * In types with the KLTYPE_FLAG_NOINSTANCE flag the refs count is not change and is always 1.
	 *
	 * @see KLTYPE_FLAG_NOINSTANCE
	 */
	uint32_t refs;
	union {
		/**
		 * @brief flags field.
		 */
		uint32_t flags; // to be used as flags for garbage collection.
		KlObjectFlags rflags;
	};
};

#ifdef __cplusplus
}
#endif

/**
 * @brief Map of kokolang objects.
 *
 * Used mainly to store metadata.
 */
typedef std::unordered_map<std::string, KlObject *> MetaMap;
/**
 * @brief Pair of key-value of a MetaMap
 */
typedef std::pair<std::string, KlObject *> MetaPair;