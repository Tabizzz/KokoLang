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

KLType* klinstruction_t = nullptr;

void global_klinstruction_t()
{
	klinstruction_t = new KLType {
		.klbase = {
			.flags = KLOBJ_FLAG_USE_DELETE
		},
		.name = "instruction",
		.size = sizeof(KLInstruction),
		.initializer = kins_init,
		.finalizer = kins_end,
		KLTYPE_METADATA
	};
}