#include <utility>

#include "KokoLangInternal.h"

void kins_init(KlObject* pack)
{
	auto ins = KLCAST(KLInstruction, pack);
	ins->label = nullptr;
	ins->opcode = noc;
	ins->foperand = nullptr;
	ins->soperand = nullptr;
	ins->call = nullptr;
}

void kins_end(KlObject* pack)
{
	auto ins = KLCAST(KLInstruction, pack);
	klDeref(ins->label);
	klDeref(ins->foperand);
	klDeref(ins->soperand);
}

KlType klBType_Instruction =
{
		KlObject(),
		"instruction",
		0,
		sizeof(KLInstruction),
		kins_init,
		nullptr,
		kins_end
};
