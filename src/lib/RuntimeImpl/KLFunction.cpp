#include <utility>
#include <cstring>
#include "KokoLangInternal.h"
#include "Runtime/KLFunction.h"

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
	}
	function->size = function->body->size();
	// resize the body if we delete instructions
	if(function->size < olsize) function->body->shrink_to_fit();
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
