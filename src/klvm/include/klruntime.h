#pragma once

// Utility methods for creating packages in native code

#include "DataTypes/KLObject.h"
#include "DataTypes/KLVariable.h"

CPPAPI bool klGetFunc(const KLObject* obj, const std::string& name, KLFunction** out);

CPPAPI bool klGetType(const KLObject* obj, const std::string& name, KLType** out);

CPPAPI bool klGetPack(const KLObject* obj, const std::string& name, KLPackage** out);

CPPAPI bool klGetField(const KLObject* obj, const std::string& name, KLVariable** out);

CPPAPI bool klGetMeta(const KLObject* obj, const std::string& name, KLObject** out, KLType* type = nullptr);

CPPAPI void klSetMeta(KLObject* obj, const std::string& name, KLObject* value);
