#include <iostream>
#include <cstring>
#include "../klvm_internal.h"
#include "KLFunctionImpl.h"

#define GETREG(y) \
if(y && y->type == klreg_t) { \
y = call.st.at(KASINT(y));}
#define REQGETREG(x, y) \
GETREG(x)               \
if(!x || x->type != y) return;
#define REGORRET(x) if(!x) return; auto reg = KASINT(x);

typedef vector<KLObject *>::reference vecref;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"

static inline bool getBool(KLObject *obj, KLCall &call) {
	GETREG(obj)
	if (obj) {
		if (obj->type == klbool_t) {
			return KASBOOL(obj);
		}
		if (obj->type == klstring_t) {
			return KASSTRSIZE(obj);
		}
		if (obj->type == klint_t) {
			return KASINT(obj);
		}
		if (obj->type == klfloat_t) {
			return KASFLOAT(obj);
		}
		if (obj->type->toBool) {
			return KASBOOL(obj->type->toBool(obj));
		}

		return true;
	}
	return false;
}

static inline void call_core(KLCall &call, KLObject *argv[], size_t argc, KLObject *function) {
	auto reg = argv[1] ? KASINT(argv[1]) : -1;
	auto obj = function;
	GETREG(obj)
	auto size = argc - 2;
	vector<KLObject *> args(size);
	for (int i = 2; i < argc; ++i) {
		auto arg = argv[i];
		GETREG(arg)
		args[i - 2] = arg;
	}
	auto ret = klInvoke(obj, args.data(), size);
	if (reg >= 0) {
		vecref save = call.st.at(reg);
		klTransfer(&ret, &save);
	} else {
		// dismiss return value
		klDeref(ret);
	}
}

static void opcode_noc(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {}

static void opcode_ldfnd(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[2])
	auto obj = argv[1];
	GETREG(obj)
	if (!obj) return;
	auto func_name = KSTRING(argv[0]);
	vecref ref = call.st.at(reg);

	KLFunction *func;
	if (klGetFunc(obj, func_name, &func)) {
		klCopy(KLWRAP(func), &ref);
	} else {
		throw runtime_error("Unable to resolve func " + func_name + " on type " + string(obj->type->name));
	}
}

static void opcode_ins(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	if (argv[0]->type == klstring_t) {
		// maybe try to check if function exists now?
		throw runtime_error("Unable to resolve type " + KSTRING(argv[0]));
	}
	vecref ref = call.st.at(reg);
	auto obj = klIns(KLCAST(KLType, argv[0]));
	klTransfer(&obj, &ref);
}

static void opcode_deref(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto obj = argv[0];
	GETREG(obj)
	klDeref(obj);
}

static void opcode_ref(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto obj = argv[0];
	GETREG(obj)
	klRef(obj);
}

static void opcode_ldfld(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[2])
	auto obj = argv[1];
	GETREG(obj)
	if (!obj) return;
	auto field_name = KSTRING(argv[0]);
	vecref ref = call.st.at(reg);

	KLVariable *field;
	if (klGetField(obj, field_name, &field)) {
		auto var = klGetVariable(field, obj);
		klCopy(var, &ref);
	} else {
		throw runtime_error("Unable to resolve field " + field_name + " on type " + string(obj->type->name));
	}
}

static void opcode_stfld(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto obj = argv[1];
	GETREG(obj)
	if (!obj) return;
	auto field_name = KSTRING(argv[0]);
	auto val = argv[2];
	GETREG(val)

	KLVariable *field;
	if (klGetField(obj, field_name, &field)) {
		klSetVariable(field, obj, val);
	} else {
		throw runtime_error("Unable to resolve field " + field_name + " on type " + string(obj->type->name));
	}
}

static void opcode_sizeof(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	if (argv[0]->type == klstring_t) {
		// maybe try to check if function exists now?
		throw runtime_error("Unable to resolve type " + KSTRING(argv[0]));
	}
	vecref ref = call.st.at(reg);
	auto t = argv[0];
	GETREG(t)
	if (t) {
		kint size;
		if (t->type == kltype_t) {
			size = KLCAST(KLType, t)->size;
		} else {
			size = static_cast<kint>(t->type->size);
		}
		size -= sizeof(KLObject);
		temp_int.value = size;
		klCopy(KLWRAP(&temp_int), &ref);
	} else {
		klMove(nullptr, &ref);
	}
}

static void opcode_new(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	if (argv[0]->type == klstring_t) {
		// maybe try to check if function exists now?
		throw runtime_error("Unable to resolve type " + KSTRING(argv[0]));
	}
	auto type = KLCAST(KLType, argv[0]);
	auto size = argc - 2;
	vector<KLObject *> args(size);
	for (int i = 2; i < argc; ++i) {
		auto arg = argv[i];
		GETREG(arg)
		args[i - 2] = arg;
	}
	auto ret = klNew(type, args.data(), size);
	vecref save = call.st.at(reg);
	klTransfer(&ret, &save);
}

static void opcode_is(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	if (argv[0]->type == klstring_t) {
		// maybe try to check if function exists now?
		throw runtime_error("Unable to resolve type " + KSTRING(argv[0]));
	}
	auto obj = argv[1];
	GETREG(obj)
	CALL_SET_FLAG(call, CALL_FLAG_CHECK, obj && KLWRAP(obj->type) == argv[0]);
}

static void opcode_typeof(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	vecref ref = call.st.at(reg);

	auto obj = argv[0];
	GETREG(obj)

	klMove(obj ? KLWRAP(obj->type) : nullptr, &ref);
}

static void opcode_type(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	if (argv[0]->type == klstring_t) {
		// maybe try to check if function exists now?
		throw runtime_error("Unable to resolve type " + KSTRING(argv[0]));
	}
	vecref ref = call.st.at(reg);

	klMove(argv[0], &ref);
}

static void opcode_ste(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto obj = argv[0];
	GETREG(obj)
	if (!obj) return;
	auto index = argv[1];
	GETREG(index)
	auto value = argv[2];
	GETREG(value)

	if (obj->type == klarray_t) {
		auto i = 0;
		if (index && index->type == klint_t) {
			i = KASINT(index);
		}
		auto length = KASARRSIZE(obj);
		auto arr = KASARR(obj);
		if (i >= 0 && i < length) {
			klCopy(value, &arr[i]);
		} else {
			throw runtime_error("Index out of bounds");
		}
	} else if (obj->type->setter) {
		obj->type->setter(obj, index, value);
	}
}

static void opcode_lde(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[0])
	vecref ref = call.st.at(reg);

	auto obj = argv[1];
	GETREG(obj)
	auto index = argv[2];
	GETREG(index)

	if (!obj) return;
	// load elements is inline for arrays
	if (obj->type == klarray_t) {
		auto i = 0;
		if (index && index->type == klint_t) {
			i = KASINT(index);
		}
		auto length = KASARRSIZE(obj);
		auto arr = KASARR(obj);
		if (i >= 0 && i < length) {
			klCopy(arr[i], &ref);
		} else {
			throw runtime_error("Index out of bounds");
		}
	} else if (obj->type->getter) {
		auto value = obj->type->getter(obj, index);
		klTransfer(&value, &ref);
	}
}

static void opcode_ard(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[0])
	vecref ref = call.st.at(reg);

	auto size = argc - 1;
	auto crt = klBuiltinArr(size);
	if (size > 0) {
		auto arr = KASARR(crt);
		for (int i = 0; i < size; ++i) {
			auto val = argv[1 + i];
			GETREG(val)
			klCopy(val, &arr[i]);
		}
	}
	klTransfer(&crt, &ref);
}

static void opcode_arl(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	vecref ref = call.st.at(reg);

	auto obj = argv[0];
	GETREG(obj)

	if (!obj) return;
	temp_int.value = 0;
	if (obj->type == klstring_t) {
		temp_int.value = KASSTRSIZE(obj);
	} else if (obj->type->size >= sizeof(kl_sptr)) {
		temp_int.value = KASARRSIZE(obj);
	}
	klCopy(KLWRAP(&temp_int), &ref);
}

static void opcode_arr(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[0])
	vecref ref = call.st.at(reg);
	auto size = argv[1];
	REQGETREG(size, klint_t)

	auto arr = klBuiltinArr(KASINT(size));
	klTransfer(&arr, &ref);
}

static void opcode_fill(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto dest = argv[0];
	REQGETREG(dest, klptr_t)

	auto src = argv[1];
	REQGETREG(src, klint_t)

	auto size = argv[2];
	REQGETREG(size, klint_t)

	memset(KASPTR(dest), static_cast<int>(KASINT(src)), KASINT(size));
}

static void opcode_copy(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto src = argv[0];
	REQGETREG(src, klptr_t)

	auto dest = argv[1];
	REQGETREG(dest, klptr_t)

	auto size = argv[2];
	REQGETREG(size, klint_t)

	memcpy(KASPTR(src), KASPTR(dest), KASINT(size));
}

static void opcode_free(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[0])
	vecref ptr = call.st.at(reg);
	if (ptr->type == klptr_t) {
		free(KLCAST(kl_ptr, ptr)->value);
		CALL_SET_FLAG(call, CALL_FLAG_CHECK, true);
	}
	CALL_SET_FLAG(call, CALL_FLAG_CHECK, false);
}

static void opcode_aloc(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	vecref regis = call.st.at(reg);
	auto val = KASINT(argv[0]);

	// this allocation is arbitrary and not an object, so we use malloc instead of klConfig.alloc
	auto ptr = KLPTR(malloc(val));
	// transfer the pointer to register
	klTransfer(&ptr, &regis);
}

static void opcode_argc(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[0])
	temp_int.value = call.argc;
	vecref regis = call.st.at(reg);
	klCopy(KLWRAP(&temp_int), &regis);
}

static void opcode_ivk(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	call_core(call, argv, argc, argv[0]);
}

static void opcode_cast(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	if (argv[0]->type == klstring_t) {
		// maybe try to check if type exists now?
		throw runtime_error("Unable to resolve type " + KSTRING(argv[0]));
	}
	auto type = KLCAST(KLType, argv[0]);
	auto val = argv[1];
	GETREG(val)
	REGORRET(argv[2])
	vecref regis = call.st.at(reg);

	if (type->cast) {
		auto str = type->cast(val);
		klTransfer(&str, &regis);
	} else {
		klMove(nullptr, &regis);
	}
}

static void opcode_tobj(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	if (argv[0]->type == klstring_t) {
		// maybe try to check if type exists now?
		throw runtime_error("Unable to resolve type " + KSTRING(argv[0]));
	}
	auto val = argv[1];
	GETREG(val)
	REGORRET(argv[2])
	vecref regis = call.st.at(reg);

	if (val && val->type->toType) {
		auto str = val->type->toType(val, argv[0]);
		klTransfer(&str, &regis);
	} else {
		klMove(nullptr, &regis);
	}
}

static void opcode_tbit(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto val = argv[0];
	GETREG(val)
	REGORRET(argv[1])
	vecref regis = call.st.at(reg);
	if (val && val->type->toBool) {
		auto str = val->type->toBool(val);
		klMove(str, &regis);
	} else {
		klMove(nullptr, &regis);
	}
}

static void opcode_tflt(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto val = argv[0];
	GETREG(val)
	REGORRET(argv[1])
	vecref regis = call.st.at(reg);
	if (val && val->type->toFloat) {
		auto str = val->type->toFloat(val);
		klTransfer(&str, &regis);
	} else {
		klMove(nullptr, &regis);
	}
}

static void opcode_tint(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto val = argv[0];
	GETREG(val)
	REGORRET(argv[1])
	vecref regis = call.st.at(reg);
	if (val && val->type->toInt) {
		auto str = val->type->toInt(val);
		klTransfer(&str, &regis);
	} else {
		klMove(nullptr, &regis);
	}
}

static void opcode_tstr(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto val = argv[0];
	GETREG(val)
	REGORRET(argv[1])
	vecref regis = call.st.at(reg);
	if (val && val->type->toString) {
		auto str = val->type->toString(val);
		klTransfer(&str, &regis);
	} else if (val) {
		klMove(nullptr, &regis);
	} else {
		auto str = KLSTR("null");
		klMove(str, &regis);
		str->refs--;
	}

}

static void opcode_mod(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[2])
	auto first = argv[0];
	GETREG(first)
	auto second = argv[1];
	GETREG(second)
	vecref regis = call.st.at(reg);

	if (first && first->type->opMod) { // first support op
		first->type->opMod(first, second, &regis);
	} else if (first && second) {
		throw runtime_error("operation mod not supported on types " + string(first->type->name) + " and " + second->type->name);
	} else if (first) {
		throw runtime_error("modulo by null not handled by type " + string(first->type->name));
	} else if (second) {
		klCopy(nullptr, &regis);
	} else {
		throw runtime_error("modulo of null by null is not allowed");
	}
}

static void opcode_div(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[2])
	auto first = argv[0];
	GETREG(first)
	auto second = argv[1];
	GETREG(second)
	vecref regis = call.st.at(reg);

	if (first && first->type->opDiv) { // first support op
		first->type->opDiv(first, second, &regis);
	} else if (first && second) {
		throw runtime_error("operation div not supported on types " + string(first->type->name) + " and " + second->type->name);
	} else if (first) {
		throw runtime_error("division by null not handled by type " + string(first->type->name));
	} else if (second) {
		klCopy(nullptr, &regis);
	} else {
		throw runtime_error("division of null by null is not allowed");
	}
}

static void opcode_mul(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[2])
	auto first = argv[0];
	GETREG(first)
	auto second = argv[1];
	GETREG(second)
	vecref regis = call.st.at(reg);

	if (first && first->type->opMul) { // first support op
		first->type->opMul(first, second, &regis);
	} else if (second && second->type->opMul) { // second support op
		second->type->opMul(second, first, &regis);
	} else if (first && second) {
		throw runtime_error("operation mul not supported on types " + string(first->type->name) + " and " + second->type->name);
	} else {
		klCopy(nullptr, &regis);
	}
}

static void opcode_sub(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[2])
	auto first = argv[0];
	GETREG(first)
	auto second = argv[1];
	GETREG(second)
	vecref regis = call.st.at(reg);

	if (first && first->type->opSub) { // first support op
		first->type->opSub(first, second, &regis);
	} else if (first && second) {
		throw runtime_error("operation sub not supported on types " + string(first->type->name) + " and " + second->type->name);
	} else if (first) {
		klCopy(first, &regis);
	} else if (second) {
		throw runtime_error("operation sub not supported on null and type " + string(second->type->name));
	} else {
		klCopy(nullptr, &regis);
	}
}

static void opcode_opne(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto first = argv[0];
	GETREG(first)
	auto second = argv[1];
	GETREG(second)

	int8_t operation;
	if (first && first->type->equal) {
		operation = first->type->equal(first, second);
	} else if (second && second->type->equal) {
		operation = second->type->equal(second, first);
	} else {
		operation = first == second ? 1 : 0;
	}

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, !operation);
}

static void opcode_opeq(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto first = argv[0];
	GETREG(first)
	auto second = argv[1];
	GETREG(second)

	int8_t operation;
	if (first && first->type->equal) {
		operation = first->type->equal(first, second);
	} else if (second && second->type->equal) {
		operation = second->type->equal(second, first);
	} else {
		operation = first == second ? 1 : 0;
	}

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, operation);
}

static void opcode_opge(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto first = argv[0];
	GETREG(first)
	auto second = argv[1];
	GETREG(second)

	int8_t operation = 0;
	if (first && first->type->comparer) {
		operation = first->type->comparer(first, second);
	} else if (second && second->type->comparer) {
		operation = second->type->comparer(second, first);
		operation *= -1;
	}

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, operation <= 0);
}

static void opcode_opgt(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto first = argv[0];
	GETREG(first)
	auto second = argv[1];
	GETREG(second)

	int8_t operation = 0;
	if (first && first->type->comparer) {
		operation = first->type->comparer(first, second);
	} else if (second && second->type->comparer) {
		operation = second->type->comparer(second, first);
		operation *= -1;
	}

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, operation < 0);
}

static void opcode_ople(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto first = argv[0];
	GETREG(first)
	auto second = argv[1];
	GETREG(second)

	int8_t operation = 0;
	if (first && first->type->comparer) {
		operation = first->type->comparer(first, second);
	} else if (second && second->type->comparer) {
		operation = second->type->comparer(second, first);
		operation *= -1;
	}

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, operation >= 0);
}

static void opcode_xor(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto a = getBool(argv[0], call);
	auto b = getBool(argv[1], call);

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, a != b);
}

static void opcode_or(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto a = getBool(argv[0], call);
	auto b = getBool(argv[1], call);

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, a || b);
}

static void opcode_and(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto a = getBool(argv[0], call);
	auto b = getBool(argv[1], call);

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, a && b);
}

static void opcode_ldarg(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto index = argv[0];
	GETREG(index)

	if (index && index->type == klint_t) {
		vecref current = call.st.at(KASINT(index) + CALL_REG_COUNT + call.locs);
		vecref toset = call.st.at(reg);

		klCopy(current, &toset);
	}
}

static void opcode_starg(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto obj = argv[1];
	GETREG(obj)

	auto st = argv[0];
	GETREG(st)

	if (st && st->type == klint_t) {
		vecref current = call.st.at(KASINT(st) + CALL_REG_COUNT + call.locs);
		klCopy(obj, &current);
	}
}

static void opcode_get(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	if (argv[0]->type == klstring_t) {
		// maybe try to check if variable exists now?
		throw runtime_error("Unable to resolve variable " + KSTRING(argv[0]));
	}
	auto var = KLCAST(KLVariable, argv[0]);
	REGORRET(argv[1])
	vecref current = call.st.at(reg);
	// we are working with a package variable
	auto value = klGetVariable(var, nullptr);
	klCopy(value, &current);
}

static void opcode_set(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	if (argv[0]->type == klstring_t) {
		// maybe try to check if variable exists now?
		throw runtime_error("Unable to resolve variable " + KSTRING(argv[0]));
	}
	auto var = KLCAST(KLVariable, argv[0]);
	REGORRET(argv[1])
	vecref current = call.st.at(reg);
	// we are working with a package variable
	klSetVariable(var, nullptr, current);
}

static void opcode_lflag(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto index = KASINT(argv[0]);
	auto val = CALL_HAS_FLAG(call, index);
	vecref current = call.st.at(reg);

	klCopy(KLBOOL(val), &current);
}

static void opcode_pop(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[0])
	vecref current = call.st.at(reg);
	klDeref(current);
	current = nullptr;
}

static void opcode_goif(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto op = CALL_HAS_FLAG(call, CALL_FLAG_CHECK);
	if (op) {
		call.next = KASINT(argv[0]);
	}
}

static void opcode_cl(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	GETREG(obj)
	vecref current = call.st.at(reg);

	klClone(obj, &current);
}

static void opcode_mv(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	GETREG(obj)
	vecref current = call.st.at(reg);

	klMove(obj, &current);
}

static void opcode_cp(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	GETREG(obj)
	vecref current = call.st.at(reg);

	klCopy(obj, &current);
}

static void opcode_ret(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto first = argv[0];
	GETREG(first)
	klCopy(first, &call.ret);
	CALL_SET_FLAG(call, CALL_FLAG_EXIT, true);
}

static void opcode_call(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	if (argv[0]->type == klstring_t) {
		// maybe try to check if function exists now?
		throw runtime_error("Unable to resolve function " + KSTRING(argv[0]));
	}
	auto function = argv[0];
	call_core(call, argv, argc, function);
}

static void opcode_go(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	call.next = KASINT(argv[0]);
}

static void opcode_add(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[2])
	auto first = argv[0];
	GETREG(first)
	auto second = argv[1];
	GETREG(second)
	vecref regis = call.st.at(reg);

	if (first && first->type->opAdd) { // first support op
		first->type->opAdd(first, second, &regis);
	} else if (second && second->type->opAdd) { // second support op
		second->type->opAdd(second, first, &regis);
	} else if (first && second) {
		throw runtime_error("operation add not supported on types " + string(first->type->name) + " and " + second->type->name);
	} else if (first) {
		klCopy(first, &regis);
	} else {
		klCopy(second, &regis);
	}
}

static void opcode_goifn(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto op = CALL_HAS_FLAG(call, CALL_FLAG_CHECK);
	if (!op) {
		call.next = KASINT(argv[0]);
	}
	CALL_SET_FLAG(call, CALL_FLAG_CHECK, !op);
}

static void opcode_oplt(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	auto first = argv[0];
	GETREG(first)
	auto second = argv[1];
	GETREG(second)

	int8_t operation = 0;
	if (first && first->type->comparer) {
		operation = first->type->comparer(first, second);
	} else if (second && second->type->comparer) {
		operation = second->type->comparer(second, first);
		operation *= -1;
	}

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, operation > 0);
}

static void opcode_push(const KLObject *caller, KLCall &call, KLObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	vecref current = call.st.at(reg);
	klCopy(obj, &current);
}

#pragma clang diagnostic pop

void kliFunction_setInstructionCall(KLInstruction *instruction) {
	switch (instruction->opcode) {
		case KLOpcode::noc:
			instruction->call = opcode_noc;
			break;
		case KLOpcode::go:
			instruction->call = opcode_go;
			break;
		case KLOpcode::goif:
			instruction->call = opcode_goif;
			break;
		case KLOpcode::goifn:
			instruction->call = opcode_goifn;
			break;
		case KLOpcode::push:
			instruction->call = opcode_push;
			break;
		case KLOpcode::pop:
			instruction->call = opcode_pop;
			break;
		case KLOpcode::cl:
			instruction->call = opcode_cl;
			break;
		case KLOpcode::cp:
			instruction->call = opcode_cp;
			break;
		case KLOpcode::mv:
			instruction->call = opcode_mv;
			break;
		case KLOpcode::lflag:
			instruction->call = opcode_lflag;
			break;
		case KLOpcode::set:
			instruction->call = opcode_set;
			break;
		case KLOpcode::get:
			instruction->call = opcode_get;
			break;
		case KLOpcode::starg:
			instruction->call = opcode_starg;
			break;
		case KLOpcode::ldarg:
			instruction->call = opcode_ldarg;
			break;
		case KLOpcode::andi:
			instruction->call = opcode_and;
			break;
		case KLOpcode::ori:
			instruction->call = opcode_or;
			break;
		case KLOpcode::xori:
			instruction->call = opcode_xor;
			break;
		case KLOpcode::oplt:
			instruction->call = opcode_oplt;
			break;
		case KLOpcode::ople:
			instruction->call = opcode_ople;
			break;
		case KLOpcode::opgt:
			instruction->call = opcode_opgt;
			break;
		case KLOpcode::opge:
			instruction->call = opcode_opge;
			break;
		case KLOpcode::opeq:
			instruction->call = opcode_opeq;
			break;
		case KLOpcode::opne:
			instruction->call = opcode_opne;
			break;
		case KLOpcode::add:
			instruction->call = opcode_add;
			break;
		case KLOpcode::sub:
			instruction->call = opcode_sub;
			break;
		case KLOpcode::mul:
			instruction->call = opcode_mul;
			break;
		case KLOpcode::div:
			instruction->call = opcode_div;
			break;
		case KLOpcode::mod:
			instruction->call = opcode_mod;
			break;
		case KLOpcode::tstr:
			instruction->call = opcode_tstr;
			break;
		case KLOpcode::tint:
			instruction->call = opcode_tint;
			break;
		case KLOpcode::tflt:
			instruction->call = opcode_tflt;
			break;
		case KLOpcode::tbit:
			instruction->call = opcode_tbit;
			break;
		case KLOpcode::tobj:
			instruction->call = opcode_tobj;
			break;
		case KLOpcode::cast:
			instruction->call = opcode_cast;
			break;
		case KLOpcode::ivk:
			instruction->call = opcode_ivk;
			break;
		case KLOpcode::call:
			instruction->call = opcode_call;
			break;
		case KLOpcode::argc:
			instruction->call = opcode_argc;
			break;
		case KLOpcode::ret:
			instruction->call = opcode_ret;
			break;
		case KLOpcode::aloc:
			instruction->call = opcode_aloc;
			break;
		case KLOpcode::freei:
			instruction->call = opcode_free;
			break;
		case KLOpcode::copy:
			instruction->call = opcode_copy;
			break;
		case KLOpcode::fill:
			instruction->call = opcode_fill;
			break;
		case KLOpcode::arr:
			instruction->call = opcode_arr;
			break;
		case KLOpcode::arl:
			instruction->call = opcode_arl;
			break;
		case KLOpcode::ard:
			instruction->call = opcode_ard;
			break;
		case KLOpcode::lde:
			instruction->call = opcode_lde;
			break;
		case KLOpcode::ste:
			instruction->call = opcode_ste;
			break;
		case KLOpcode::type:
			instruction->call = opcode_type;
			break;
		case KLOpcode::typeofi:
			instruction->call = opcode_typeof;
			break;
		case KLOpcode::is:
			instruction->call = opcode_is;
			break;
		case KLOpcode::newi:
			instruction->call = opcode_new;
			break;
		case KLOpcode::sizeofi:
			instruction->call = opcode_sizeof;
			break;
		case KLOpcode::stfld:
			instruction->call = opcode_stfld;
			break;
		case KLOpcode::ldfld:
			instruction->call = opcode_ldfld;
			break;
		case KLOpcode::ref:
			instruction->call = opcode_ref;
			break;
		case KLOpcode::deref:
			instruction->call = opcode_deref;
			break;
		case KLOpcode::ins:
			instruction->call = opcode_ins;
			break;
		case KLOpcode::ldfnd:
			instruction->call = opcode_ldfnd;
			break;
		default:
			break;
	}
}