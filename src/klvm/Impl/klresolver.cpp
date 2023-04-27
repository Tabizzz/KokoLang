#include "klvm_internal.h"
#include "klresolver.h"

static klresolver packageResolver;
static klresolver typeResolver;
static klresolver functionResolver;
static klresolver variableResolver;

KlObject* defaultPackageResolver(const KlObject* fullname,
								 const KLPackage* package,
								 const KLType* type,
								 const KLFunction* function,
								 kbyte mode)
{
	auto str = KSTRING(fullname);
	if (str == "global"){
		return KLWRAP(klGlobalPackage());
	}

	for (const auto& p : *kliRootPackages()) {
		if (klBType_String.equal(KLWRAP(fullname), p.second->name)) {
            return KLWRAP(p.second);
        }
	}

	if(mode) {
		// todo: load packages from native or .kll files
		// search order is:
		// - working dir.
		// - entry file dir.
		// - runtime dir.
		// - KL_EXTRA_PACKAGE_DIR path.
	}

	throw runtime_error("unknown package " + str);
}

KlObject* defaultTypeResolver(const KlObject* fullname,
							  const KLPackage* package,
							  const KLType* type,
							  const KLFunction* function,
							  kbyte mode)
{
	return nullptr;
}

KlObject* defaultFunctionResolver(const KlObject* fullname,
								  const KLPackage* package,
								  const KLType* type,
								  const KLFunction* function,
								  kbyte mode)
{
	return nullptr;
}

inline KlObject* getVariable(const KLPackage* package, string& name)
{
	auto varfind = package->variables->find(name);
	return varfind != package->variables->end() ? varfind->second : nullptr;
}

KlObject* defaultVariableResolver(const KlObject* fullname,
								  const KLPackage* package,
								  const KLType* type,
								  const KLFunction* function,
								  kbyte mode)
{
	auto str = KSTRING(fullname);
	auto find = str.find(':');
	auto varname = str.substr(find + 1, str.size());
	KLPackage* source = nullptr;
	// if not package separator we need to find on the global var or in the package.
	if(find == std::string::npos) {
		auto varfind = getVariable(package, varname);
		if (varfind) return varfind;

		varfind = getVariable(klGlobalPackage(), varname);
		if (varfind) return varfind;

		if(mode) {
			auto var = klIns(&klBType_Variable);
			package->variables->insert(MetaPair(varname, var));
			return var;
		}
		throw std::runtime_error("Undefined variable " + varname);
	}
	else {
		auto packname = KLSTR(str.substr(0, find));
		source = KLCAST(KLPackage, KokoLang::KLDefaultResolvers::getPackageResolver()(packname, package, type, function, mode));
		klDestroy(packname);

		auto varfind = getVariable(source, varname);
		if (varfind != nullptr) return varfind;


		if (mode) {
			auto var = klIns(&klBType_Variable);
			source->variables->insert(MetaPair(varname, var));
			return var;
		}
		throw std::runtime_error("Undefined variable " + varname);

	}
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
