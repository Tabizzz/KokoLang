#pragma once
#include "KokoLang.h"

#define STR_EQUALS(x, y) (KLCAST(kl_string, src)->size == KLCAST(kl_string, dest)->size && \
strncmp(KASSTR(x), KASSTR(y), KLCAST(kl_string, x)->size) == 0)
#define STR_NOT_EQUALS(x, y) (KLCAST(kl_string, src)->size != KLCAST(kl_string, dest)->size || \
strncmp(KASSTR(x), KASSTR(y), KLCAST(kl_string, x)->size) != 0)

#define COUNT_ARRAY(x, y) size_t x = 0; \
auto count##x = y;                      \
while(count##x[x]) {x++;}             \
delete[] count##x;


static inline bool is_valid(KlObject* obj)
{
	if(!obj->type) return false;
	if(!obj->type->klbase.type) return false;
	if(obj->type->klbase.type == &klBType_Type) return false;
	return true;
}