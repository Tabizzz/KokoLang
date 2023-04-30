#include "../klvm_internal.h"
#include "DataTypes/KLVariable.h"


void kvar_instantiator(KlObject* obj) {
	auto var = KLCAST(KLVariable, obj);
	var->data.packvar.value = nullptr;
	var->data.packvar.type = true;
	var->data.packvar.defined = false;
	var->metadata = new MetaMap();
}

void kvar_destructor(KlObject* obj) {
	auto var = KLCAST(KLVariable, obj);

	kliDerefAndDeleteMap(var->metadata);

	klDeref(var->data.packvar.value);
}

void klSetVariable(KLVariable *variable, KlObject *target, KlObject *value) {
	if(variable->data.packvar.type) {
		// directly copy the value to the var
		klCopy(value, &variable->data.packvar.value);
	} else {
		// copy the value to the address given by the offset.
		// we skip the header of KLObject with target + 1, then apply the offset
		//					  |		header				   |		offset					|
		klCopy(value, KLCAST(KlObject*, target + 1) + variable->data.typevar.offset);

	}
}

KLType klBType_Variable = KLBASIC_TYPE("var", KLVariable, kvar_instantiator, kvar_destructor)};