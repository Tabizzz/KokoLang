#include <utility>
#include <cstring>
#include "KokoLangInternal.h"
#include "Runtime/KLFunction.h"

KLFunction::KLFunction(string name)
{
	FunctionName = std::move(name);
	locals = 0;
	stack = 10;
	args = 0;
	margs = 0;
	size = 0;
}

KLFunction::~KLFunction()
{
	for (auto ins : body)
	{
		delete ins;
	}
}

string KLFunction::getName()
{
	return FunctionName;
}

void KLFunction::reallocateLabels() {
	auto olsize = size;
	for (int i = 0; i < body.size(); i++)
	{
		auto instruction = body[i];
		if(instruction->opcode == noc)
		{
			for (auto ref: body) {
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
					if(strcmp(label->value, instruction->label) == 0)
					{
						klDeref(instruction->foperand);
						instruction->foperand = KLINT(i);
					}
				}
			}
			body.erase(next(body.begin(), i--));
		}
	}
	size = body.size();
	// resize the body if we delete instructions
	if(size < olsize) body.shrink_to_fit();
}
