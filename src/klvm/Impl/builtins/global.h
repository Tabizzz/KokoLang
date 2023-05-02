#pragma once

#include "../klvm_internal.h"

KlObject* klself_return(KlObject* obj);

void global_kltype_t();

void global_klint_t();
void global_klfloat_t();
void global_klbool_t();
void global_klstring_t();
void global_klptr_t();
void global_klarray_t();
void global_kllist_t();
void global_klmap_t();

void global_klpack_t();
void global_klvar_t();
void global_klfunc_t();
void global_klinstruction_t();


void karr_init(KlObject *obj);