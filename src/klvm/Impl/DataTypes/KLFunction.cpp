#include "../klvm_internal.h"
#include "DataTypes/KLFunction.h"
#include "KLFunctionImpl.h"

KLCall::~KLCall() {
	for (auto reg: st) {
		klDeref(reg);
	}
}

static KLObject *kliFunctionImpl(KLObject *caller, KLObject **argv, kbyte passedArgs) {
	auto func = KLCAST(KLFunction, caller);
	auto argc = func->args == -1 ? passedArgs : func->args;

	// the call of this function
	KLCall call;
	call.next = 0;
	call.st.reserve(argc + func->locals + CALL_REG_COUNT);
	call.argc = argc;
	call.locs = func->locals;
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
	while (!CALL_HAS_FLAG(call, CALL_FLAG_EXIT)) {
		auto ins = (*func->body)[call.next++];

		ins->call(caller, call, ins->operands, ins->operandc);
	}

	return call.ret;
}

static void kfunc_instantiator(KLObject *obj) {
	auto func = KLCAST(KLFunction, obj);
	func->external = false;
	func->name = nullptr;
	func->invokable = nullptr;
	func->locals = 0;
	func->args = -1;
	func->margs = 0;
	func->size = 0;
	func->body = nullptr;
	func->metadata = new MetaMap();

}

static void kfunc_destructor(KLObject *obj) {
	auto func = KLCAST(KLFunction, obj);
	klDeref(func->name);
	if (!func->external) {
		for (auto ins: *func->body) {
			klDeref(KLWRAP(ins));
		}
		delete func->body;
	}
	kliDerefAndDeleteMap(func->metadata);
}

static inline void klFunction_reallocateLabels(KLFunction *function) {
	auto olsize = function->size;
	for (int i = 0; i < function->body->size(); i++) {
		auto instruction = (*function->body)[i];
		if (instruction->opcode == KLOpcode::noc) {
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
				if (reflabel && ref->operands[0]->type == klstring_t) {
					if (klstring_t->KLComparerFunctions.equal(ref->operands[0], instruction->label)) {
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
	if (function->size < olsize) function->body->shrink_to_fit();

	function->invokable = kliFunctionImpl;
}

void klBuildFunction(KLPackage *package, KLType *type, KLFunction *func) {

	if(func->external) return;
	auto flag = 0;
	if (KSTRING(func->name) == KLENTRY_NAME || KSTRING(package->name) == KLPROGRAM_NAME) {
		flag = KLRESOLVE_GLOBAL;
	}

	for (auto instruction: *func->body) {
		kliFunction_setInstructionCall(instruction);
		// replace identifiers with pointers
		switch (instruction->opcode) {
			case KLOpcode::set:
			case KLOpcode::get:
				if (instruction->operands[0]->type == klstring_t) {
					auto current = instruction->operands[0];
					auto toset = klGetResolver()(current, package, type, func, KLRESOLVE_VARIABLE | flag);
					if (toset)
						klMove(toset, &instruction->operands[0]);
				}
				break;
			case KLOpcode::tobj:
			case KLOpcode::cast:
			case KLOpcode::type:
			case KLOpcode::is:
			case KLOpcode::newi:
			case KLOpcode::sizeofi:
			case KLOpcode::ins:
				if (instruction->operands[0]->type == klstring_t) {
					auto current = instruction->operands[0];
					auto toset = klGetResolver()(current, package, type, func, KLRESOLVE_TYPE | flag);
					if (toset)
						klMove(toset, &instruction->operands[0]);
				}
				break;
			case KLOpcode::call:
				if (instruction->operands[0]->type == klstring_t) {
					auto current = instruction->operands[0];
					auto toset = klGetResolver()(current, package, type, func, KLRESOLVE_FUNCTION | flag);
					if (toset)
						klMove(toset, &instruction->operands[0]);
				}
				break;
			default:
				break;
		}
	}
	klFunction_reallocateLabels(func);
}

KLType* klfunc_t = nullptr;

void global_klfunc_t()
{
	klfunc_t = new KLType {
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"fn",
		sizeof(KLFunction),
		0, 0,
		kfunc_instantiator,
		kfunc_destructor
	};

	KLTYPE_METADATA(klfunc_t)
}