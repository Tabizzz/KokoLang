#include "../klvm_internal.h"
#include "DataTypes/KLVariable.h"


static void kvar_instantiator(KLObject *obj) {
	auto var = KLCAST(KLVariable, obj);
	var->data.packvar.value = nullptr;
	var->data.packvar.type = true;
	var->data.packvar.defined = false;
	var->metadata = new MetaMap();
}

static void kvar_destructor(KLObject *obj) {
	auto var = KLCAST(KLVariable, obj);

	kliDerefAndDeleteMap(var->metadata);

	// we deref the current value or the default value in type vars
	klDeref(var->data.packvar.value);
}

void klSetVariable(KLVariable *variable, KLObject *target, KLObject *value) {
	if (variable->data.packvar.type) {
		// mark var as defined
		variable->data.packvar.defined = true;
		// directly copy the value to the var
		klCopy(value, &variable->data.packvar.value);
	} else {
		// copy the value to the address given by the offset.
		// we skip the header of KLObject with target + 1, then apply the offset
		//			  |		header				   |		offset					|
		klCopy(value, KLCAST(KLObject*, target + 1) + variable->data.typevar.offset);

	}
}

KLObject *klGetVariable(KLVariable *variable, KLObject *target) {
	if (variable->data.packvar.type) {
		if (variable->data.packvar.defined) {
			return variable->data.packvar.value;
		}
		throw runtime_error("Unable to read undefined variable");
	} else {
		return *(KLCAST(KLObject*, target + 1) + variable->data.typevar.offset);
	}
}

KLType* klvar_t = nullptr;

void global_klvar_t()
{
	klvar_t = new KLType {
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"var",
		0,
		sizeof(KLVariable),
		kvar_instantiator,
		kvar_destructor
	};
	KLTYPE_METADATA(klvar_t)
}