#include <iostream>
#include "KokoLangInternal.h"
#include "KLFunctionImpl.h"


void opcode_noc(const KlObject& caller,const KLCall& call,const KlObject** foperand, size_t operandc){}

void opcode_goifn(KlObject * caller, KLCall *call, KlObject *foperand, KlObject *soperand)
{
	auto op = utilPopTop(call);
	if(!op || !KASBOOL(op)) {
		call->next = KASINT(foperand);
	}
	klDeref(op);
}

void opcode_go(KlObject * caller, KLCall *call, KlObject *foperand, KlObject *soperand)
{
	call->next = KASINT(foperand);
}

void opcode_add(KlObject * caller, KLCall *call, KlObject *foperand, KlObject *soperand)
{
	auto first = utilPopTop(call);
	auto second = utilPopTop(call);

	auto f = KLCAST(kl_int, first);
	auto s = KLCAST(kl_int, second);

	s->value = f->value + s->value;

	klDeref(second);
	klDeref(first);
}

void opcode_goif(KlObject * caller, KLCall *call, KlObject *foperand, KlObject *soperand)
{
	auto op = utilPopTop(call);
	if(op && KASBOOL(op)) {
		call->next = KASINT(foperand);
	}
	klDeref(op);
}

void opcode_oplt(KlObject * caller, KLCall *call, KlObject *foperand, KlObject *soperand)
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

void opcode_ldvar(KlObject * caller, KLCall *call, KlObject *foperand, KlObject *soperand)
{
	auto index = KLCAST(kl_int, foperand)->value;
	// get the local, ref and then push onto the stack
	auto local = call->st[index + CALL_REG_COUNT];
	klRef(local);
	utilPushTop(caller, call, local);
}

void opcode_stvar(KlObject * caller, KLCall *call, KlObject *foperand, KlObject *soperand)
{
	auto val = utilPopTop(call);
	auto index = KLCAST(kl_int, foperand)->value;
	// decrease the ref count of the current local.
	klDeref(call->st[index + CALL_REG_COUNT]);
	call->st[index + CALL_REG_COUNT] = val;
}

void opcode_push(KlObject * caller, KLCall *call, KlObject *foperand, KlObject *soperand)
{
	klRef(foperand);
	utilPushTop(caller, call, foperand);
}

void opcode_ret(KlObject * caller, KLCall *call, KlObject *foperand, KlObject *soperand)
{
	call->exit = true;
}

void klFunction_setInstructionCall(KLInstruction *instruction)
{
	switch (instruction->opcode) {
		case noc:
			instruction->call = opcode_noc;
			break;
		case go:
			instruction->call = opcode_go;
			break;
		case goif:
			instruction->call = opcode_goif;
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
		case clear:
			break;
		case stackl:
			break;
		case stvar:
			instruction->call = opcode_stvar;
			break;
		case ldvar:
			instruction->call = opcode_ldvar;
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
		case ret:
			instruction->call = opcode_ret;
			break;
		case call:
			break;
		case argc:
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
		case KOpcode::size:
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
		case reserved_ext:
			break;
	}
}