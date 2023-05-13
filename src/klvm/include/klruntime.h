#pragma once

// Utility methods for creating packages in native code

#include "DataTypes/KLObject.h"
#include "DataTypes/KLVariable.h"

CPPAPI bool klHasFunc(const KlObject* obj, const std::string& name);

CPPAPI KlObject* klGetFunc(const KlObject* obj, const std::string& name);

CPPAPI bool klHasField(const KlObject* obj, const std::string& name);

CPPAPI KLVariable* klGetField(const KlObject* obj, const std::string& name);

CPPAPI bool klHasMeta(const KlObject* obj, const std::string& name);

CPPAPI bool klHasMeta(const KlObject* obj, const std::string& name, KLType* type);

CPPAPI KlObject* klGetMeta(const KlObject* obj, const std::string& name);

CPPAPI void klSetMeta(KlObject* obj, const std::string& name, KlObject* value);
