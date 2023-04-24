#include "../KokoLangInternal.h"
#include "DataTypes/KLVariable.h"


void kvar_instantiator(KlObject* obj) {
	auto var = KLCAST(KLVariable, obj);
	var->data.packvar.defined = false;
	var->metadata = new MetaMap();
}

void kvar_destructor(KlObject* obj) {
	auto var = KLCAST(KLVariable, obj);

	kliDerefAndDeleteMap(var->metadata);

	// we dont deref the value held by package variables.
}

void klSetVariable(KLVariable *variable, KlObject *target, KlObject *value) {

}

KLType klBType_Variable =
{
		KlObject(),
		"var",
		0,
		sizeof(KLVariable),
		kvar_instantiator,
		nullptr,
		kvar_destructor
};
