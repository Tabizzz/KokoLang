#include <iostream>
#include "KokoLangInternal.h"
#include "KLFunctionImpl.h"

#define GETREG(x, y) \
if(y->type == &klBType_Reg) { \
y = x.st.at(KASINT(y));\
}\

void opcode_noc(const KlObject& caller, KLCall& call, KlObject* operands[], size_t operandc) {}

void opcode_cp(const KlObject& caller, KLCall& call, KlObject* operands[], size_t operandc) {
	auto reg = KASINT(operands[1]);
	auto obj = operands[0];
	GETREG(call, obj);
	vector<KlObject*>::reference current = call.st.at(reg);

	klCopy(obj, &current);
}

void opcode_ret(const KlObject& caller, KLCall& call, KlObject* operands[], size_t operandc) {
	auto ret = operands[0];
	CALL_SET_FLAG(call, CALL_FLAG_EXIT, true);
}

void opcode_call(const KlObject& caller, KLCall& call, KlObject* operands[], size_t operandc) {
	auto reg = operands[2];
	GETREG(call, reg)

	cout << KASINT(reg) << endl;
}

void opcode_go(const KlObject& caller, KLCall& call, KlObject* operands[], size_t operandc) {
	call.next = KASINT(operands[0]);
}

void opcode_add(const KlObject& caller, KLCall& call, KlObject* operands[], size_t operandc) {
	auto first = operands[0];
	GETREG(call, first)
	auto second = operands[1];
	GETREG(call, second)
	auto out = KASINT(operands[2]);

	// todo change this
	auto f = KLCAST(kl_int, first);
	auto s = KLCAST(kl_int, second);

	auto res = f->value + s->value;

	vector<KlObject*>::reference regis = call.st.at(out);
	if(regis) {
		KASINT(regis) = res;
	} else {
		regis = KLINT(res);
	}
}

void opcode_goifn(const KlObject& caller, KLCall& call, KlObject* operands[], size_t operandc)
{
	auto op = CALL_HAS_FLAG(call, CALL_FLAG_CHECK);
	if(!op) {
		call.next = KASINT(operands[0]);
	}
	CALL_SET_FLAG(call, CALL_FLAG_CHECK, !op);
}

void opcode_oplt(const KlObject& caller, KLCall& call, KlObject* operands[], size_t operandc) {
	auto first = operands[0];
	GETREG(call, first)
	auto second = operands[1];
	GETREG(call, second)

	auto operation = first->type->comparer(first, second);

	CALL_SET_FLAG(call, CALL_FLAG_CHECK, operation == 1);

}

void opcode_push(const KlObject& caller, KLCall& call, KlObject* operands[], size_t operandc) {
	auto reg = KASINT(operands[1]);
	auto obj = operands[0];
	vector<KlObject*>::reference current = call.st.at(reg);
	klClone(obj, &current);
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
			break;
		case goifn:
			instruction->call = opcode_goifn;
			break;
		case push:
			instruction->call = opcode_push;
			break;
		case pop:
			break;
		case dup:
			break;
		case cp:
			instruction->call = opcode_cp;
			break;
		case mv:
			break;
		case lflag:
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