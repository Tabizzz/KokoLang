#include <iostream>
#include "KokoLangInternal.h"
#include "KLFunctionImpl.h"


void opcode_noc(const KlObject& caller, KLCall& call,const KlObject** operands, size_t operandc){}
/*
void opcode_goifn(const KlObject& caller, KLCall& call,const KlObject** operands, size_t operandc)
{
	auto op = CALL_HAS_FLAG(call, CALL_FLAG_CHECK);
	if(!op) {
		call.next = KASINT(operands[0]);
	}
	CALL_SET_FLAG(call, CALL_FLAG_CHECK, !op);
}

void opcode_go(const KlObject& caller, KLCall& call,const KlObject** operands, size_t operandc)
{
	call.next = KASINT(operands);
}

void opcode_add(const KlObject& caller,KLCall& call,const KlObject** operands, size_t operandc)
{
	auto first = utilPopTop(call);
	auto second = utilPopTop(call);

	auto f = KLCAST(kl_int, first);
	auto s = KLCAST(kl_int, second);

	s->value = f->value + s->value;

	klDeref(second);
	klDeref(first);
}

void opcode_goif(const KlObject& caller,KLCall& call,const KlObject** operands, size_t operandc)
{
	auto op = utilPopTop(call);
	if(op && KASBOOL(op)) {
		call->next = KASINT(operands);
	}
	klDeref(op);
}

void opcode_oplt(const KlObject& caller,KLCall& call,const KlObject** operands, size_t operandc)
{
	auto second = utilPopTop(call);
	auto first = utilPopTop(call);
	auto operation = first->type->comparer(first, second);
	klDeref(second);
	klDeref(first);
	if(operation == 1) {
		utilPushTop(caller, call, KLBOOL(true));
		return;
	}

	utilPushTop(caller, call, nullptr);
}

void opcode_ldvar(const KlObject& caller,KLCall& call,const KlObject** operands, size_t operandc)
{
	auto index = KLCAST(kl_int, operands)->value;
	// get the local, ref and then push onto the stack
	auto local = call->st[index + CALL_REG_COUNT];
	klRef(local);
	utilPushTop(caller, call, local);
}

void opcode_stvar(const KlObject& caller,KLCall& call,const KlObject** operands, size_t operandc)
{
	auto val = utilPopTop(call);
	auto index = KLCAST(kl_int, operands)->value;
	// decrease the ref count of the current local.
	klDeref(call->st[index + CALL_REG_COUNT]);
	call->st[index + CALL_REG_COUNT] = val;
}

void opcode_push(const KlObject& caller,KLCall& call,const KlObject** operands, size_t operandc)
{
	klRef(operands);
	utilPushTop(caller, call, operands);
}

void opcode_ret(const KlObject& caller, KLCall& call, const KlObject** operands, size_t operandc) {
	CALL_SET_FLAG(call, CALL_FLAG_EXIT, true);
}
*/
void klFunction_setInstructionCall(KLInstruction *instruction) {
	switch (instruction->opcode) {
		case noc:
			break;
		case go:
			break;
		case goif:
			break;
		case goifn:
			break;
		case push:
			break;
		case pop:
			break;
		case dup:
			break;
		case cp:
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
			break;
		case calla:
			break;
		case argc:
			break;
		case ret:
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