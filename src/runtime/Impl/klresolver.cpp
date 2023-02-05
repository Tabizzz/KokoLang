#include "KokoLangInternal.h"
#include "klresolver.h"

static klresolver packageResolver;
static klresolver typeResolver;
static klresolver functionResolver;
static klresolver variableResolver;


KlObject* defaultPackageResolver(const char* fullname, KLPackage* callerPackage, KLType* callerType, KLFunction* callerFunction, kbyte mode)
{
	return nullptr;
}

KlObject* defaultTypeResolver(const char* fullname, KLPackage* callerPackage, KLType* callerType, KLFunction* callerFunction, kbyte mode)
{
	return nullptr;
}

KlObject* defaultFunctionResolver(const char* fullname, KLPackage* callerPackage, KLType* callerType, KLFunction* callerFunction, kbyte mode)
{
	return nullptr;
}

KlObject* defaultVariableResolver(const char* fullname, KLPackage* callerPackage, KLType* callerType, KLFunction* callerFunction, kbyte mode)
{
	return nullptr;
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
