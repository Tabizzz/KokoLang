#pragma once
#include "KokoLang.h"

#define STR_EQUALS(x, y) (KLCAST(kl_string, src)->size == KLCAST(kl_string, dest)->size && \
strncmp(KASSTR(x), KASSTR(y), KLCAST(kl_string, x)->size) == 0)
#define STR_NOT_EQUALS(x, y) (KLCAST(kl_string, src)->size != KLCAST(kl_string, dest)->size || \
strncmp(KASSTR(x), KASSTR(y), KLCAST(kl_string, x)->size) != 0)


KLType testT =
{
	KlObject(),
	"test",
	0,
	sizeof (kl_int)
};