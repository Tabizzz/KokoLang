#include "../klvm_internal.h"

void kins_init(KlObject* pack)
{
	auto ins = KLCAST(KLInstruction, pack);
	ins->label = nullptr;
	ins->opcode = KLOpcode::noc;
	ins->operands = nullptr;
	ins->operandc = 0;
	ins->call = nullptr;
}

void kins_end(KlObject* pack)
{
	auto ins = KLCAST(KLInstruction, pack);
	klDeref(ins->label);
	for (int i = 0; i < ins->operandc; ++i) {
		klDeref(ins->operands[i]);
	}
	delete[] ins->operands;
}

KLType klBType_Instruction =
{
		KlObject(),
		"instruction",
		0,
		sizeof(KLInstruction),
		kins_init,
		nullptr,
		kins_end
};
