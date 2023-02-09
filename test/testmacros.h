#pragma once

#include <cstring>
#include "KokoLang.h"
#include "segvcatch.h"

#define STR_EQUALS(x, y) (KLCAST(kl_string, x)->size == KLCAST(kl_string, y)->size && \
strncmp(KASSTR(x), KASSTR(y), KLCAST(kl_string, x)->size) == 0)
#define STR_NOT_EQUALS(x, y) (KLCAST(kl_string, x)->size != KLCAST(kl_string, y)->size || \
strncmp(KASSTR(x), KASSTR(y), KLCAST(kl_string, x)->size) != 0)

#define COUNT_ARRAY(x, y) size_t x = 0; \
auto count##x = y;                      \
while(count##x[x]) {x++;}             \
delete[] count##x;

#define EXCHANGE(x, valx, y, valy, z, valz) klDeref(x); \
x = valx;                                            	\
klDeref(y);                                             \
y = valy;                                            	\
klDeref(z);                                             \
z = valz;

static bool segv_init = false;

static inline bool is_valid(KlObject* obj)
{
	if(!segv_init) {
		segvcatch::init_segv();
		segv_init = true;
	}
	try
	{
		if(!obj->type) return false;
		if(!obj->type->klbase.type) return false;
		if(obj->type->klbase.type == &klBType_Type) return false;
	}
	catch (...)
	{
		return false;
	}
	return true;
}