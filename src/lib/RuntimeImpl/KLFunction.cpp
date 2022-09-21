﻿#include <utility>
#include <cstring>
#include <iostream>
#include "KokoLangInternal.h"
#include "Runtime/KLFunction.h"
#include "KLFunctionImpl.h"


void initToNull(KlObject *pObject[], int count) {
	for (int i = 0; i < count; ++i) {
		pObject[i] = nullptr;
	}
}

KlObject* kliFunctionImpl(KlObject *caller, KlObject **argv, KlObject *argsc)
{
	auto func = KLCAST(KLFunction, caller);
	auto passedArgs = KLCAST(kl_int, argsc)->value;
	if(passedArgs < func->margs){
		cout << "Error calling function " << KLCAST(kl_string, func->name)->value <<
		" expected  at least" << (int)func->margs << " argv but received " << passedArgs << endl;
		exit(1);
	}
	else if(func->args >= 0 && passedArgs > func->args)
	{
		cout << "Error calling function " << KLCAST(kl_string, func->name)->value <<
		" expected maximum " << (int)func->args << " argv but received " << passedArgs << endl;
		exit(1);
	}
	int argc = func->args == -1 ? passedArgs : func->args;

	// the call of this function
	KLCall call{};
	call.next = 0;
	KlObject* args[argc];
	KlObject* locals[func->locals];
	// set args and locals to null
	initToNull(args, argc);
	initToNull(locals, func->locals);
	// copy passed args
	for (int i = 0; i < passedArgs; ++i) {
		args[i] = argv[i];
		klRef(argv[i]);
	}
	call.locals = locals;
	call.args = args;

	// execute the code
	while (!call.exit)
	{
		auto ins = (*func->body)[call.next++];
		ins->call(caller, &call, ins->foperand, ins->soperand);
	}

	// final cleanup


	return nullptr;
}

void kfunc_instantiator(KlObject * obj)
{
	auto func = KLCAST(KLFunction, obj);
	func->external = false;
	func->name = nullptr;
	func->invokable = nullptr;
	func->locals = 0;
	func->stack = 10;
	func->args = 0;
	func->margs = 0;
	func->size = 0;
	func->body = nullptr;
}

void kfunc_destructor(KlObject* obj)
{
	auto func = KLCAST(KLFunction, obj);
	klDeref(func->name);
	if(!func->external) {
		for (auto ins: *func->body) {
			klDeref(KLWRAP(ins));
		}
		delete func->body;
	}
}

void klFunction_reallocateLabels(KLFunction* function) {
	auto olsize = function->size;
	for (int i = 0; i < function->body->size(); i++)
	{
		auto instruction = (*function->body)[i];
		if(instruction->opcode == noc)
		{
			for (auto ref: *function->body) {
				auto reflabel = false;
				switch (ref->opcode) {
					case go:
					case goif:
					case goifn:
						reflabel = true;
						break;
					default:
						break;
				}
				if(reflabel) {
					auto label = KLCAST(kl_string, ref->foperand);
					auto label2 =  KLCAST(kl_string, instruction->label);
					if(strcmp(label->value, label2->value) == 0)
					{
						klDeref(instruction->foperand);
						instruction->foperand = KLINT(i);
					}
				}
			}
			function->body->erase(next(function->body->begin(), i--));
		}
		else
		{
			klFunction_setInstructionCall(instruction);
		}
	}
	function->size = function->body->size();
	// resize the body if we delete instructions
	if(function->size < olsize) function->body->shrink_to_fit();

	function->invokable = kliFunctionImpl;
}

KlType klBType_Func =
{
		KlObject(),
		"func",
		0,
		sizeof(KLFunction),
		kfunc_instantiator,
		nullptr,
		kfunc_destructor
};
