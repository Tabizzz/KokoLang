#pragma once

#include "../klvm_internal.h"

#define STRINGIFY(X) #X
#define ADD_FUNCTION(id, type, ivk, margsc, argsc)       \
auto func_##id = KLCAST(KLFunction, klIns(klfunc_t));    \
func_##id->external = true;                              \
func_##id->name = KLSTR(STRINGIFY(id));                  \
func_##id->margs = margsc;                               \
func_##id->args = argsc;                                 \
func_##id->invokable = ivk;                              \
type->functions->insert(MetaPair(STRINGIFY(id), KLWRAP(func_##id)));

KLObject *klself_return(KLObject *obj);

void global_kltype_t();

void global_klint_t();

void global_klfloat_t();

void global_klbool_t();

void global_klstring_t();

void global_klstring_t_m();

void global_klptr_t();

void global_klarray_t();

void global_kllist_t();

void global_klmap_t();

void global_klpack_t();

void global_klvar_t();

void global_klfunc_t();

void global_klinstruction_t();


void karr_init(KLObject *obj);