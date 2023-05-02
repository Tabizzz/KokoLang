#include <iostream>
#include "../klvm_internal.h"
#include "KLFunctionImpl.h"

#define GETREG(y) \
if(y && y->type == &klreg_t) { \
y = call.st.at(KASINT(y));}
#define REGORRET(x) if(!x) return; auto reg = KASINT(x);

typedef vector<KlObject *>::reference vecref;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"

static inline bool getBool(KlObject *obj, KLCall &call) {
	GETREG(obj)
	if (obj) {
		if (obj->type == &klbool_t) {
			return KASBOOL(obj);
		}
		if (obj->type == &klstring_t) {
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

static void opcode_noc(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {}

static void opcode_cast(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	if (argv[0]->type == &klstring_t) {
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

static void opcode_tobj(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	if (argv[0]->type == &klstring_t) {
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

static void opcode_tbit(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_tflt(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_tint(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_tstr(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_mod(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_div(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_mul(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_sub(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_opne(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_opeq(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_opge(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_opgt(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_ople(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_xor(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto a = getBool(argv[0], call);
	auto b = getBool(argv[1], call);

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, a != b);
}

static void opcode_or(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto a = getBool(argv[0], call);
	auto b = getBool(argv[1], call);

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, a || b);
}

static void opcode_and(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto a = getBool(argv[0], call);
	auto b = getBool(argv[1], call);

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, a && b);
}

static void opcode_ldarg(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto st = KASINT(argv[0]);
	auto target = KASINT(argv[1]);

	vecref current = call.st.at(st + CALL_REG_COUNT + call.locs);
	vecref toset = call.st.at(target);

	klCopy(current, &toset);
}

static void opcode_starg(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto obj = argv[1];
	GETREG(obj)

	auto st = KASINT(argv[0]);
	vecref current = call.st.at(st + CALL_REG_COUNT + call.locs);

	klCopy(obj, &current);
}

static void opcode_get(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	if (argv[0]->type == &klstring_t) {
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

static void opcode_set(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	if (argv[0]->type == &klstring_t) {
		// maybe try to check if variable exists now?
		throw runtime_error("Unable to resolve variable " + KSTRING(argv[0]));
	}
	auto var = KLCAST(KLVariable, argv[0]);
	REGORRET(argv[1])
	vecref current = call.st.at(reg);
	// we are working with a package variable
	klSetVariable(var, nullptr, current);
}

static void opcode_lflag(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto index = KASINT(argv[0]);
	auto val = CALL_HAS_FLAG(call, index);
	vecref current = call.st.at(reg);

	klCopy(KLBOOL(val), &current);
}

static void opcode_pop(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	REGORRET(argv[0])
	vecref current = call.st.at(reg);
	klDeref(current);
	current = nullptr;
}

static void opcode_goif(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto op = CALL_HAS_FLAG(call, CALL_FLAG_CHECK);
	if (op) {
		call.next = KASINT(argv[0]);
	}
}

static void opcode_cl(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	GETREG(obj)
	vecref current = call.st.at(reg);

	klClone(obj, &current);
}

static void opcode_mv(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	GETREG(obj)
	vecref current = call.st.at(reg);

	klMove(obj, &current);
}

static void opcode_cp(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	GETREG(obj)
	vecref current = call.st.at(reg);

	klCopy(obj, &current);
}

static void opcode_ret(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto first = argv[0];
	GETREG(first)
	klCopy(first, &call.ret);
	CALL_SET_FLAG(call, CALL_FLAG_EXIT, true);
}

static void opcode_call(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	if (argv[0]->type == &klstring_t) {
		// maybe try to check if function exists now?
		throw runtime_error("Unable to resolve function " + KSTRING(argv[0]));
	}
	auto reg = argv[1] ? KASINT(argv[1]) : -1;
	auto val = argv[2];
	GETREG(val)
	vector<KlObject *> args;
	args.reserve(argc - 2);
	for (int i = 2; i < argc; ++i) {
		args.push_back(call.st.at(KASINT(argv[i])));
	}
	auto ret = klInvoke(argv[0], args.data(), args.size());
	if (reg >= 0) {
		vecref save = call.st.at(reg);
		klTransfer(&ret, &save);
	} else {
		// dismiss return value
		klDeref(ret);
	}
}

static void opcode_go(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	call.next = KASINT(argv[0]);
}

static void opcode_add(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_goifn(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto op = CALL_HAS_FLAG(call, CALL_FLAG_CHECK);
	if (!op) {
		call.next = KASINT(argv[0]);
	}
	CALL_SET_FLAG(call, CALL_FLAG_CHECK, !op);
}

static void opcode_oplt(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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

static void opcode_push(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
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
			break;
		case KLOpcode::ivka:
			break;
		case KLOpcode::call:
			instruction->call = opcode_call;
			break;
		case KLOpcode::calla:
			break;
		case KLOpcode::argc:
			break;
		case KLOpcode::ret:
			instruction->call = opcode_ret;
			break;
		case KLOpcode::aloc:
			break;
		case KLOpcode::freei:
			break;
		case KLOpcode::copy:
			break;
		case KLOpcode::fill:
			break;
		case KLOpcode::arr:
			break;
		case KLOpcode::arl:
			break;
		case KLOpcode::ard:
			break;
		case KLOpcode::lde:
			break;
		case KLOpcode::ste:
			break;
		case KLOpcode::type:
			break;
		case KLOpcode::typeofi:
			break;
		case KLOpcode::is:
			break;
		case KLOpcode::newi:
			break;
		case KLOpcode::newa:
			break;
		case KLOpcode::sizeofi:
			break;
		case KLOpcode::stfld:
			break;
		case KLOpcode::ldfld:
			break;
		case KLOpcode::ref:
			break;
		case KLOpcode::deref:
			break;
		case KLOpcode::ins:
			break;
		default:
			break;
	}
}