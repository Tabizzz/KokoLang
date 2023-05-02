#include "../klvm_internal.h"

static void kins_init(KlObject* pack)
{
	auto ins = KLCAST(KLInstruction, pack);
	ins->label = nullptr;
	ins->opcode = KLOpcode::noc;
	ins->operands = nullptr;
	ins->operandc = 0;
	ins->call = nullptr;
}

static void kins_end(KlObject* pack)
{
	auto ins = KLCAST(KLInstruction, pack);
	klDeref(ins->label);
	for (int i = 0; i < ins->operandc; ++i) {
		klDeref(ins->operands[i]);
	}
	delete[] ins->operands;
}

KLType klinstruction_t = KLBASIC_TYPE("instruction", KLInstruction, kins_init, kins_end)};
