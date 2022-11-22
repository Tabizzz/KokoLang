#include <iostream>
#include "KokoLangInternal.h"
#include "KLFunctionImpl.h"

#define GETREG(x, y) \
if(y && y->type == &klBType_Reg) { \
y = x.st.at(KASINT(y));}
#define USEREGOP klRegOp regop = CALL_HAS_FLAG(call, CALL_FLAG_CLONE) ? klClone : klCopy;
#define REGORRET(x) if(!x) return; auto reg = KASINT(x);

typedef vector<KlObject*>::reference vecref;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"
void opcode_noc(const KlObject& caller, KLCall& call, KlObject* operands[], [[maybe_unused]] size_t operandc) {}
#pragma clang diagnostic pop

void opcode_lflag(const KlObject& caller, KLCall& call, KlObject* operands[], [[maybe_unused]] size_t operandc) {
	USEREGOP
	REGORRET(operands[1])
	auto index = KASINT(operands[0]);
	auto val = CALL_HAS_FLAG(call, index);
	vecref current = call.st.at(reg);

	regop(KLBOOL(val), &current);
}

void opcode_pop(const KlObject& caller, KLCall& call, KlObject* operands[], [[maybe_unused]] size_t operandc) {
	REGORRET(operands[0])
	vecref current = call.st.at(reg);
	klDeref(current);
	current = nullptr;
}

void opcode_goif(const KlObject& caller, KLCall& call, [[maybe_unused]] KlObject* operands[], [[maybe_unused]] size_t operandc) {
	auto op = CALL_HAS_FLAG(call, CALL_FLAG_CHECK);
	if(op) {
		call.next = KASINT(operands[0]);
	}
}

void opcode_cl(const KlObject& caller, KLCall& call, KlObject* operands[], [[maybe_unused]] size_t operandc) {
	REGORRET(operands[1])
	auto obj = operands[0];
	GETREG(call, obj)
	vecref current = call.st.at(reg);

	klClone(obj, &current);
}

void opcode_mv(const KlObject& caller, KLCall& call, KlObject* operands[], [[maybe_unused]] size_t operandc) {
	REGORRET(operands[1])
	auto obj = operands[0];
	GETREG(call, obj)
	vecref current = call.st.at(reg);

	klMove(obj, &current);
}

void opcode_cp(const KlObject& caller, KLCall& call, KlObject* operands[], [[maybe_unused]] size_t operandc) {
	REGORRET(operands[1])
	auto obj = operands[0];
	GETREG(call, obj)
	vecref current = call.st.at(reg);

	klCopy(obj, &current);
}

void opcode_ret(const KlObject& caller, KLCall& call, [[maybe_unused]] KlObject* operands[], [[maybe_unused]] size_t operandc) {
	CALL_SET_FLAG(call, CALL_FLAG_EXIT, true);
}

void opcode_call(const KlObject& caller, KLCall& call, KlObject* operands[], [[maybe_unused]] size_t operandc) {
	auto reg = operands[2];
	GETREG(call, reg)
	if(reg->type == &klBType_Int)
		cout << KASINT(reg) << endl;
	else if (reg->type == &klBType_Float)
		cout << KASFLOAT(reg) << endl;
}

void opcode_go(const KlObject& caller, KLCall& call, KlObject* operands[], [[maybe_unused]] size_t operandc) {
	call.next = KASINT(operands[0]);
}

void opcode_add(const KlObject& caller, KLCall& call, KlObject* operands[], [[maybe_unused]] size_t operandc) {
	USEREGOP
	REGORRET(operands[2])
	auto first = operands[0];
	GETREG(call, first)
	auto second = operands[1];
	GETREG(call, second)
	vecref regis = call.st.at(reg);

	if(first->type->opAdd) {
		first->type->opAdd(first, second, &regis, regop);
	} else {
		throw invalid_argument("Invalid object to operation add");
	}
}

void opcode_goifn(const KlObject& caller, KLCall& call, KlObject* operands[], [[maybe_unused]] size_t operandc)
{
	auto op = CALL_HAS_FLAG(call, CALL_FLAG_CHECK);
	if(!op) {
		call.next = KASINT(operands[0]);
	}
	CALL_SET_FLAG(call, CALL_FLAG_CHECK, !op);
}

void opcode_oplt(const KlObject& caller, KLCall& call, KlObject* operands[], [[maybe_unused]] size_t operandc) {
	auto first = operands[0];
	GETREG(call, first)
	auto second = operands[1];
	GETREG(call, second)

	auto operation = first->type->comparer(first, second);

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, operation == 1);

}

void opcode_push(const KlObject& caller, KLCall& call, KlObject* operands[], [[maybe_unused]] size_t operandc) {
	USEREGOP
	REGORRET(operands[1])
	auto obj = operands[0];
	vecref current = call.st.at(reg);
	regop(obj, &current);
}

void klFunction_setInstructionCall(KLInstruction *instruction) {
	switch (instruction->opcode) {
		case noc:
			instruction->call = opcode_noc;
			break;
		case go:
			instruction->call = opcode_go;
			break;
		case goif:
			instruction->call= opcode_goif;
			break;
		case goifn:
			instruction->call = opcode_goifn;
			break;
		case push:
			instruction->call = opcode_push;
			break;
		case pop:
			instruction->call = opcode_pop;
			break;
		case cl:
			instruction->call = opcode_cl;
			break;
		case cp:
			instruction->call = opcode_cp;
			break;
		case mv:
			instruction->call = opcode_mv;
			break;
		case lflag:
			instruction->call = opcode_lflag;
			break;
		case set:
			break;
		case KOpcode::get:
			break;
		case starg:
			break;
		case ldarg:
			break;
		case andi:
			break;
		case ori:
			break;
		case xori:
			break;
		case oplt:
			instruction->call = opcode_oplt;
			break;
		case ople:
			break;
		case opgt:
			break;
		case opge:
			break;
		case opeq:
			break;
		case opne:
			break;
		case add:
			instruction->call = opcode_add;
			break;
		case sub:
			break;
		case mul:
			break;
		case divi:
			break;
		case mod:
			break;
		case tstr:
			break;
		case tint:
			break;
		case tflt:
			break;
		case tbit:
			break;
		case tobj:
			break;
		case cast:
			break;
		case jump:
			break;
		case jumpa:
			break;
		case call:
			instruction->call = opcode_call;
			break;
		case calla:
			break;
		case argc:
			break;
		case ret:
			instruction->call = opcode_ret;
			break;
		case aloc:
			break;
		case freei:
			break;
		case KOpcode::copy:
			break;
		case KOpcode::fill:
			break;
		case arr:
			break;
		case arl:
			break;
		case ard:
			break;
		case lde:
			break;
		case ste:
			break;
		case type:
			break;
		case typeofi:
			break;
		case is:
			break;
		case newi:
			break;
		case newa:
			break;
		case sizeofi:
			break;
		case stfld:
			break;
		case ldfld:
			break;
		case KOpcode::ref:
			break;
		case deref:
			break;
		case ins:
			break;
		default:
			break;
	}
}

#undef GETREG
#undef REGORRET