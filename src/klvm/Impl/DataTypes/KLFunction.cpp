#include "../klvm_internal.h"
#include "DataTypes/KLFunction.h"
#include "KLFunctionImpl.h"

KlObject* kliFunctionImpl(KlObject *caller, KlObject **argv, kbyte passedArgs)
{
	auto func = KLCAST(KLFunction, caller);
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
	auto argc = func->args == -1 ? passedArgs : func->args;

	// the call of this function
	KLCall call{};
	call.next = 0;
	call.st.reserve(argc + func->locals + CALL_REG_COUNT);
	call.argc = argc;
	call.st.push_back(KLINT(0));
	call.st.push_back(KLFLOAT(0));
	// locals by default to null
	for (int i = 0; i < func->locals; ++i) {
		call.st.push_back(nullptr);
	}
	// copy all the passed args
	for (int i = 0; i < passedArgs; ++i) {
		call.st.push_back(argv[i]);
		klRef(argv[i]);
	}
	// fill not passed args with null
	for (int i = 0; i < argc - passedArgs; ++i) {
		call.st.push_back(nullptr);
	}

	// execute the code
	while (!CALL_HAS_FLAG(call, CALL_FLAG_EXIT))
	{
		auto ins = (*func->body)[call.next++];

		ins->call(*caller, call, ins->operands, ins->operandc);
	}
	// final cleanup
	for (auto reg: call.st) {
		klDeref(reg);
	}

	return call.ret;
}

void kfunc_instantiator(KlObject * obj)
{
	auto func = KLCAST(KLFunction, obj);
	func->external = false;
	func->name = nullptr;
	func->invokable = nullptr;
	func->locals = 0;
	func->args = 0;
	func->margs = 0;
	func->size = 0;
	func->body = nullptr;
	func->metadata = new std::map<std::string, KlObject*>();

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
	kliDerefAndDeleteMap(func->metadata);
}

inline void klFunction_reallocateLabels(KLFunction* function) {
	auto olsize = function->size;
	for (int i = 0; i < function->body->size(); i++)
	{
		auto instruction = (*function->body)[i];
		if(instruction->opcode == KLOpcode::noc)
		{
			for (auto ref: *function->body) {
				auto reflabel = false;
				switch (ref->opcode) {
					case KLOpcode::go:
					case KLOpcode::goif:
					case KLOpcode::goifn:
						reflabel = true;
						break;
					default:
						break;
				}
				if(reflabel && ref->operands[0]->type == &klBType_String) {
					if(klBType_String.equal(ref->operands[0], instruction->label))
					{
						klDeref(ref->operands[0]);
						ref->operands[0] = KLINT(i);
					}
				}
			}
			auto removed = function->body->at(i);
			function->body->erase(next(function->body->begin(), i--));
			klDeref(KLWRAP(removed));
		}
	}
	function->size = function->body->size();
	// resize the body if we delete instructions
	if(function->size < olsize) function->body->shrink_to_fit();

	function->invokable = kliFunctionImpl;
}

void klBuildFunction(KLPackage* package, KLType* type, KLFunction* func) {

	for (auto instruction : *func->body) {
		klFunction_setInstructionCall(instruction);
		// replace identifiers with pointers
		switch (instruction->opcode) {
			case KLOpcode::set:
			case KLOpcode::get:
				if (instruction->operands[0]->type == &klBType_String) {
                    auto current = instruction->operands[0];
					klMove(KokoLang::KLDefaultResolvers::getVariableResolver()(current, package, type, func, true),
						   &instruction->operands[0]);
                }
                break;
			case KLOpcode::tobj:
			case KLOpcode::cast:
			case KLOpcode::type:
			case KLOpcode::is:
			case KLOpcode::newi:
			case KLOpcode::newa:
			case KLOpcode::sizeofi:
			case KLOpcode::ins:
				if (instruction->operands[0]->type == &klBType_String) {
					auto current = instruction->operands[0];
					klMove(KokoLang::KLDefaultResolvers::getTypeResolver()(current, package, type, func, true),
						   &instruction->operands[0]);
				}
				break;
			case KLOpcode::call:
			case KLOpcode::calla:
				if (instruction->operands[0]->type == &klBType_String) {
					auto current = instruction->operands[0];
					klMove(KokoLang::KLDefaultResolvers::getFunctionResolver()(current, package, type, func, true),
						   &instruction->operands[0]);
				}
				break;
			default:
				break;
		}
	}
	klFunction_reallocateLabels(func);
}

KLType klBType_Func = KLBASIC_TYPE("fn", KLFunction, kfunc_instantiator, kfunc_destructor)};