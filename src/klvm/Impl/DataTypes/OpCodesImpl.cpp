#include <iostream>
#include "../klvm_internal.h"
#include "KLFunctionImpl.h"

#define GETREG(x, y) \
if(y && y->type == &klBType_Reg) { \
y = x.st.at(KASINT(y));}
#define REGORRET(x) if(!x) return; auto reg = KASINT(x);

typedef vector<KlObject *>::reference vecref;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"

void opcode_noc(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {}

void opcode_ldarg(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto st = KASINT(argv[0]);
	auto target = KASINT(argv[1]);

	vecref current = call.st.at(st + CALL_REG_COUNT + call.locs);
	vecref toset = call.st.at(target);

	klCopy(current, &toset);
}

void opcode_starg(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto obj = argv[1];
	GETREG(call, obj)
	
	auto st = KASINT(argv[0]);
	vecref current = call.st.at(st + CALL_REG_COUNT + call.locs);

	klCopy(obj, &current);
}

void opcode_get(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	if (argv[0]->type == &klBType_String) {
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

void opcode_set(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	if (argv[0]->type == &klBType_String) {
		// maybe try to check if variable exists now?
		throw runtime_error("Unable to resolve variable " + KSTRING(argv[0]));
	}
	auto var = KLCAST(KLVariable, argv[0]);
	REGORRET(argv[1])
	vecref current = call.st.at(reg);
	// we are working with a package variable
	klSetVariable(var, nullptr, current);
}

void opcode_lflag(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto index = KASINT(argv[0]);
	auto val = CALL_HAS_FLAG(call, index);
	vecref current = call.st.at(reg);

	klCopy(KLBOOL(val), &current);
}

void opcode_pop(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	REGORRET(argv[0])
	vecref current = call.st.at(reg);
	klDeref(current);
	current = nullptr;
}

void opcode_goif(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto op = CALL_HAS_FLAG(call, CALL_FLAG_CHECK);
	if (op) {
		call.next = KASINT(argv[0]);
	}
}

void opcode_cl(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	GETREG(call, obj)
	vecref current = call.st.at(reg);

	klClone(obj, &current);
}

void opcode_mv(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	GETREG(call, obj)
	vecref current = call.st.at(reg);

	klMove(obj, &current);
}

void opcode_cp(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	GETREG(call, obj)
	vecref current = call.st.at(reg);

	klCopy(obj, &current);
}

void opcode_ret(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto first = argv[0];
	GETREG(call, first)
	klCopy(first, &call.ret);
	CALL_SET_FLAG(call, CALL_FLAG_EXIT, true);
}

void opcode_call(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto reg = argv[2];
	GETREG(call, reg)
	if (reg->type == &klBType_Int)
		cout << KASINT(reg) << endl;
	else if (reg->type == &klBType_Float)
		cout << KASFLOAT(reg) << endl;
	else if (reg->type == &klBType_String)
		cout << KSTRING(reg) << endl;
}

void opcode_go(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	call.next = KASINT(argv[0]);
}

void opcode_add(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	REGORRET(argv[2])
	auto first = argv[0];
	GETREG(call, first)
	auto second = argv[1];
	GETREG(call, second)
	vecref regis = call.st.at(reg);

	if (first->type->opAdd) {
		first->type->opAdd(first, second, &regis);
	} else {
		throw invalid_argument("Invalid object to operation add");
	}
}

void opcode_goifn(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto op = CALL_HAS_FLAG(call, CALL_FLAG_CHECK);
	if (!op) {
		call.next = KASINT(argv[0]);
	}
	CALL_SET_FLAG(call, CALL_FLAG_CHECK, !op);
}

void opcode_oplt(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	auto first = argv[0];
	GETREG(call, first)
	auto second = argv[1];
	GETREG(call, second)

	auto operation = first->type->comparer(first, second);

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, operation == 1);

}

void opcode_push(const KlObject *caller, KLCall &call, KlObject *argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	vecref current = call.st.at(reg);
	klCopy(obj, &current);
}

#pragma clang diagnostic pop

void klFunction_setInstructionCall(KLInstruction *instruction) {
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
			break;
		case KLOpcode::ori:
			break;
		case KLOpcode::xori:
			break;
		case KLOpcode::oplt:
			instruction->call = opcode_oplt;
			break;
		case KLOpcode::ople:
			break;
		case KLOpcode::opgt:
			break;
		case KLOpcode::opge:
			break;
		case KLOpcode::opeq:
			break;
		case KLOpcode::opne:
			break;
		case KLOpcode::add:
			instruction->call = opcode_add;
			break;
		case KLOpcode::sub:
			break;
		case KLOpcode::mul:
			break;
		case KLOpcode::div:
			break;
		case KLOpcode::mod:
			break;
		case KLOpcode::tstr:
			break;
		case KLOpcode::tint:
			break;
		case KLOpcode::tflt:
			break;
		case KLOpcode::tbit:
			break;
		case KLOpcode::tobj:
			break;
		case KLOpcode::cast:
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

#undef GETREG
#undef REGORRET