#include "KokoLangInternal.h"
#include "klresolver.h"

static klresolver packageResolver;
static klresolver typeResolver;
static klresolver functionResolver;
static klresolver variableResolver;


KlObject* defaultPackageResolver(KlObject* fullname, const KlObject* caller, kbyte mode)
{
	return nullptr;
}

KlObject* defaultTypeResolver(KlObject* fullname, const KlObject* caller, kbyte mode)
{
	return nullptr;
}

KlObject* defaultFunctionResolver(KlObject* fullname, const KlObject* caller, kbyte mode)
{
	return nullptr;
}

KlObject* defaultVariableResolver(KlObject* fullname, const KlObject* caller, kbyte mode)
{
	return KLINT(0);
}

void kliSetDefaultResolvers() {
	klConfigureResolvers(defaultPackageResolver, defaultTypeResolver, defaultFunctionResolver, defaultVariableResolver);
}

void klConfigureResolvers(klresolver package, klresolver type, klresolver function, klresolver variable) {
	if(package) packageResolver = package;
	if(type) typeResolver = type;
	if(function) functionResolver = function;
	if(variable) variableResolver = variable;
}

klresolver KokoLang::KLDefaultResolvers::getPackageResolver() {
	return packageResolver;
}

klresolver KokoLang::KLDefaultResolvers::getTypeResolver() {
	return typeResolver;
}

klresolver KokoLang::KLDefaultResolvers::getFunctionResolver() {
	return functionResolver;
}

klresolver KokoLang::KLDefaultResolvers::getVariableResolver() {
	return variableResolver;
}
