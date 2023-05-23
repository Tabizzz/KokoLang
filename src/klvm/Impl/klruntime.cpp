#include "klvm_internal.h"
#include "klruntime.h"

template<typename T>
static inline T mapfind(unordered_map<string, T> *map, const string &find) {
    auto varfind = map->find(find);
    return varfind != map->end() ? varfind->second : nullptr;
}

bool klGetField(const KLObject *obj, const string &name, KLVariable **out) {
    if (!obj) return false;
    MetaMap *map;
    if (obj->type == kltype_t) {
        map = KLCAST(KLType, obj)->variables;
    } else if (obj->type == klpack_t) {
        map = KLCAST(KLPackage, obj)->variables;
    } else {
        map = obj->type->variables;
    }

    *out = KLCAST(KLVariable, mapfind(map, name));
    return *out;
}

bool klGetFunc(const KLObject *obj, const string &name, KLFunction **out) {
    if (!obj) return false;
    MetaMap *map;
    if (obj->type == kltype_t) {
        map = KLCAST(KLType, obj)->functions;
    } else if (obj->type == klpack_t) {
        map = KLCAST(KLPackage, obj)->functions;
    } else {
        map = obj->type->functions;
    }

    *out = KLCAST(KLFunction, mapfind(map, name));
    return *out;
}
