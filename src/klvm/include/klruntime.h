#pragma once

// Utility methods for creating packages in native code

#include "DataTypes/KLObject.h"
#include "DataTypes/KLVariable.h"

CPPAPI bool klGetFunc(const KlObject* obj, const std::string& name, KLFunction** out);

CPPAPI bool klGetType(const KlObject* obj, const std::string& name, KLType** out);

CPPAPI bool klGetPack(const KlObject* obj, const std::string& name, KLPackage** out);

CPPAPI bool klGetField(const KlObject* obj, const std::string& name, KLVariable** out);

CPPAPI bool klGetMeta(const KlObject* obj, const std::string& name, KlObject** out, KLType* type = nullptr);

CPPAPI void klSetMeta(KlObject* obj, const std::string& name, KlObject* value);
