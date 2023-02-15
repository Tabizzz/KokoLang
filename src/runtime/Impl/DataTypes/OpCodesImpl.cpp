#include <iostream>
#include "../KokoLangInternal.h"
#include "KLFunctionImpl.h"

#define GETREG(x, y) \
if(y && y->type == &klBType_Reg) { \
y = x.st.at(KASINT(y));}
#define REGORRET(x) if(!x) return; auto reg = KASINT(x);

typedef vector<KlObject*>::reference vecref;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"
void opcode_noc(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {}

void opcode_set(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	if(argv[0]->type == &klBType_String)
	{
		auto var = KokoLang::KLDefaultResolvers::getVariableResolver()(argv[0], &caller, true);
		klDeref(argv[0]);
		argv[0] = var;
	}
}

void opcode_lflag(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	REGORRET(argv[1])
	auto index = KASINT(argv[0]);
	auto val = CALL_HAS_FLAG(call, index);
	vecref current = call.st.at(reg);

	klCopy(KLBOOL(val), &current);
}

void opcode_pop(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	REGORRET(argv[0])
	vecref current = call.st.at(reg);
	klDeref(current);
	current = nullptr;
}

void opcode_goif(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	auto op = CALL_HAS_FLAG(call, CALL_FLAG_CHECK);
	if(op) {
		call.next = KASINT(argv[0]);
	}
}

void opcode_cl(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	GETREG(call, obj)
	vecref current = call.st.at(reg);

	klClone(obj, &current);
}

void opcode_mv(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	GETREG(call, obj)
	vecref current = call.st.at(reg);

	klMove(obj, &current);
}

void opcode_cp(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	GETREG(call, obj)
	vecref current = call.st.at(reg);

	klCopy(obj, &current);
}

void opcode_ret(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	auto first = argv[0];
	GETREG(call, first)
	klCopy(first, &call.ret);
	CALL_SET_FLAG(call, CALL_FLAG_EXIT, true);
}

void opcode_call(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	auto reg = argv[2];
	GETREG(call, reg)
	if(reg->type == &klBType_Int)
		cout << KASINT(reg) << endl;
	else if (reg->type == &klBType_Float)
		cout << KASFLOAT(reg) << endl;
}

void opcode_go(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	call.next = KASINT(argv[0]);
}

void opcode_add(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	REGORRET(argv[2])
	auto first = argv[0];
	GETREG(call, first)
	auto second = argv[1];
	GETREG(call, second)
	vecref regis = call.st.at(reg);

	if(first->type->opAdd) {
		first->type->opAdd(first, second, &regis);
	} else {
		throw invalid_argument("Invalid object to operation add");
	}
}

void opcode_goifn(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc)
{
	auto op = CALL_HAS_FLAG(call, CALL_FLAG_CHECK);
	if(!op) {
		call.next = KASINT(argv[0]);
	}
	CALL_SET_FLAG(call, CALL_FLAG_CHECK, !op);
}

void opcode_oplt(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	auto first = argv[0];
	GETREG(call, first)
	auto second = argv[1];
	GETREG(call, second)

	auto operation = first->type->comparer(first, second);

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, operation == 1);

}

void opcode_push(const KlObject& caller, KLCall& call, KlObject* argv[], size_t argc) {
	REGORRET(argv[1])
	auto obj = argv[0];
	vecref current = call.st.at(reg);
	klCopy(obj, &current);
}

#pragma clang diagnostic pop

void klFunction_setInstructionCall(KLInstruction *instruction) {
	switch (instruction->opcode) {
		case KOpcode::noc:
			instruction->call = opcode_noc;
			break;
		case KOpcode::go:
			instruction->call = opcode_go;
			break;
		case KOpcode::goif:
			instruction->call= opcode_goif;
			break;
		case KOpcode::goifn:
			instruction->call = opcode_goifn;
			break;
		case KOpcode::push:
			instruction->call = opcode_push;
			break;
		case KOpcode::pop:
			instruction->call = opcode_pop;
			break;
		case KOpcode::cl:
			instruction->call = opcode_cl;
			break;
		case KOpcode::cp:
			instruction->call = opcode_cp;
			break;
		case KOpcode::mv:
			instruction->call = opcode_mv;
			break;
		case KOpcode::lflag:
			instruction->call = opcode_lflag;
			break;
		case KOpcode::set:
			instruction->call = opcode_set;
			break;
		case KOpcode::get:
			break;
		case KOpcode::starg:
			break;
		case KOpcode::ldarg:
			break;
		case KOpcode::andi:
			break;
		case KOpcode::ori:
			break;
		case KOpcode::xori:
			break;
		case KOpcode::oplt:
			instruction->call = opcode_oplt;
			break;
		case KOpcode::ople:
			break;
		case KOpcode::opgt:
			break;
		case KOpcode::opge:
			break;
		case KOpcode::opeq:
			break;
		case KOpcode::opne:
			break;
		case KOpcode::add:
			instruction->call = opcode_add;
			break;
		case KOpcode::sub:
			break;
		case KOpcode::mul:
			break;
		case KOpcode::div:
			break;
		case KOpcode::mod:
			break;
		case KOpcode::tstr:
			break;
		case KOpcode::tint:
			break;
		case KOpcode::tflt:
			break;
		case KOpcode::tbit:
			break;
		case KOpcode::tobj:
			break;
		case KOpcode::cast:
			break;
		case KOpcode::jump:
			break;
		case KOpcode::jumpa:
			break;
		case KOpcode::call:
			instruction->call = opcode_call;
			break;
		case KOpcode::calla:
			break;
		case KOpcode::argc:
			break;
		case KOpcode::ret:
			instruction->call = opcode_ret;
			break;
		case KOpcode::aloc:
			break;
		case KOpcode::freei:
			break;
		case KOpcode::copy:
			break;
		case KOpcode::fill:
			break;
		case KOpcode::arr:
			break;
		case KOpcode::arl:
			break;
		case KOpcode::ard:
			break;
		case KOpcode::lde:
			break;
		case KOpcode::ste:
			break;
		case KOpcode::type:
			break;
		case KOpcode::typeofi:
			break;
		case KOpcode::is:
			break;
		case KOpcode::newi:
			break;
		case KOpcode::newa:
			break;
		case KOpcode::sizeofi:
			break;
		case KOpcode::stfld:
			break;
		case KOpcode::ldfld:
			break;
		case KOpcode::ref:
			break;
		case KOpcode::deref:
			break;
		case KOpcode::ins:
			break;
		default:
			break;
	}
}

#undef GETREG
#undef REGORRET