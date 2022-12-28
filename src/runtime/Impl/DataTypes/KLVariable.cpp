#include "../KokoLangInternal.h"

void kvar_instantiator(KlObject* obj) {
	auto var = KLCAST(KLVariable, obj);
	var->type = false;
	var->data.value = nullptr;
	var->source = nullptr;
	var->defaultValue = nullptr;
	var->metadata = new std::map<std::string, KlObject*>();
}

void kvar_destructor(KlObject* obj) {
	auto var = KLCAST(KLVariable, obj);

	kliDerefAndDeleteMap(var->metadata);
	klDeref(var->defaultValue);
	// the source is a package or a type, so we don't ref the source.
	//klDeref(var->source);
	if(var->type) {
		klDeref(var->data.value);
	}
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
