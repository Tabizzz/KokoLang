#include "klvm_internal.h"
#include "klresolver.h"

klresolver resolver;

static inline void split(vector<string> &vector, const string &source, char separator) {
	string::size_type start = 0;
	string::size_type index;
	while ((index = source.find(separator, start)) != string::npos) {
		vector.push_back(source.substr(start, index));
		start = index + 1;
	}
	vector.push_back(source.substr(start, index));
}

template<typename T>
static inline T mapfind(unordered_map<string, T> *map, string &find) {
	auto varfind = map->find(find);
	return varfind != map->end() ? varfind->second : nullptr;
}

static inline KlObject *internalPackageResolver(const string &str) {
	if (str == "global") return KLWRAP(klGlobalPackage());
	vector<string> list;
	split(list, str, '.');

	KLPackage *pack = nullptr;

	for (auto p: list) {
		if (pack == nullptr) {
			pack = mapfind(kliRootPackages(), p);
			if (pack == nullptr) return nullptr;
		} else {
			pack = KLCAST(KLPackage, mapfind(pack->packs, p));
			if (pack == nullptr) return nullptr;
		}
	}

	return KLWRAP(pack);
}

static inline KlObject *defaultPackageResolver(const KlObject *fullname) {
	return internalPackageResolver(KSTRING(fullname));
}

static inline KlObject *defaultTypeResolver(const KlObject *fullname,
									 const KLPackage *package) {
	auto str = KSTRING(fullname);
	auto index = str.rfind('.');

	if (index == string::npos) {
		// fisrt check on the current pack.
		auto var = mapfind(package->types, str);
		if (var) return var;

		// check on the global pack
		var = mapfind(klGlobalPackage()->types, str);
		if (var) return var;
	} else {
		auto packname = str.substr(0, index);
		auto varname = str.substr(index + 1);

		auto pack = KLCAST(KLPackage, internalPackageResolver(packname));
		if (pack == nullptr) return nullptr;

		auto var = mapfind(pack->types, varname);
		if (var) return var;
	}
	return nullptr;
}

static inline KlObject *defaultFunctionResolver(const KlObject *fullname,
										 const KLPackage *package) {
	auto str = KSTRING(fullname);
	auto index = str.rfind('.');

	if (index == string::npos) {
		// fisrt check on the current pack.
		auto var = mapfind(package->functions, str);
		if (var) return var;

		// check on the global pack
		var = mapfind(klGlobalPackage()->functions, str);
		if (var) return var;
	} else {
		auto packname = str.substr(0, index);
		auto varname = str.substr(index + 1);

		auto pack = KLCAST(KLPackage, internalPackageResolver(packname));
		if (pack == nullptr) return nullptr;

		auto var = mapfind(pack->functions, varname);
		if (var) return var;
	}
	return nullptr;
}

static inline KlObject *defaultVariableResolver(const KlObject *fullname,
										 const KLPackage *package,
										 kbyte mode) {
	auto str = KSTRING(fullname);
	auto index = str.rfind('.');
	auto global = klGlobalPackage();

	if (index == string::npos) {
		// the package to create the if not found
		auto pack = mode ? global : package;

		// fisrt check on the current pack.
		auto var = mapfind(package->variables, str);
		if (var) return var;

		// check on the global pack
		var = mapfind(global->variables, str);
		if (var) return var;

		// create and add to the package
		var = klIns(klvar_t);
		pack->variables->insert(MetaPair(str, var));
		return var;
	} else {
		auto packname = str.substr(0, index);
		auto varname = str.substr(index + 1);

		auto pack = KLCAST(KLPackage, internalPackageResolver(packname));
		if (pack == nullptr) return nullptr;

		auto var = mapfind(pack->variables, varname);
		if (var) return var;
		var = klIns(klvar_t);
		pack->variables->insert(MetaPair(str, var));
		return var;
	}
}

static KlObject *defaultResolver(const KlObject *fullname,
						  const KLPackage *package,
						  [[maybe_unused]] const KLType *type,
						  [[maybe_unused]] const KLFunction *function,
						  kbyte mode) {
	if (mode & KLRESOLVE_VARIABLE) {
		return defaultVariableResolver(fullname, package, mode & KLRESOLVE_GLOBAL);
	}
	if (mode & KLRESOLVE_PACKAGE) {
		return defaultPackageResolver(fullname);
	}
	if (mode & KLRESOLVE_FUNCTION) {
		return defaultFunctionResolver(fullname, package);
	}
	if (mode & KLRESOLVE_TYPE) {
		return defaultTypeResolver(fullname, package);
	}
	return nullptr;
}

void klRestoreResolver() {
	resolver = defaultResolver;
}

void klSetResolver(klresolver toset) {
	resolver = toset;
}

klresolver klGetResolver() {
	return resolver;
}
