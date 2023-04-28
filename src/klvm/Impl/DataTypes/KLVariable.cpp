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

}

KLType klBType_Variable = KLBASIC_TYPE("var", KLVariable, kvar_instantiator, kvar_destructor)};