#include <BNM/FieldBase.hpp>
#include <BNM/DebugMessages.hpp>
#include <Internals.hpp>

using namespace BNM;

// For static fields of streams
namespace BNM::PRIVATE_FieldUtils {
    void GetStaticValue(IL2CPP::FieldInfo *info, void *value) { return Internal::il2cppMethods.il2cpp_field_static_get_value(info, value); }

    void SetStaticValue(IL2CPP::FieldInfo *info, void *value) { return Internal::il2cppMethods.il2cpp_field_static_set_value(info, value); }
}

static bool CheckIsFieldStatic(IL2CPP::FieldInfo *field) {
    if (!field->type) return false;
    // 0x0010 = FIELD_ATTRIBUTE_STATIC
    return (field->type->attrs & 0x0010) != 0 && field->offset != -1;
}

FieldBase::FieldBase(IL2CPP::FieldInfo *info) {
    if (!info) return;

    _isConst = (info->type->attrs & 0x0040) != 0; // FIELD_ATTRIBUTE_LITERAL
    _isStatic = !_isConst && CheckIsFieldStatic(info);
    _data = info;
    _isThreadStatic = _data->offset == -1;
    _isInStruct = Class(info->parent).GetIl2CppType()->type == IL2CPP::IL2CPP_TYPE_VALUETYPE;
}

FieldBase &FieldBase::SetInstance(IL2CPP::Il2CppObject *val)  {
    if (_isStatic || _isConst) {
        BNM_LOG_WARN(DBG_BNM_MSG_FieldBase_SetInstance_Warn, str().c_str());
        return *this;
    }
#ifdef BNM_CHECK_INSTANCE_TYPE
    if (BNM::IsA(val, _data->parent)) _instance = val;
    else BNM_LOG_ERR(DBG_BNM_MSG_FieldBase_SetInstance_Wrong_Instance_Error, BNM::Class(val).str().c_str(), str().c_str());
#else
    _instance = val;
#endif
    return *this;
}

void *FieldBase::GetFieldPointer() const {
    if (!_data) return nullptr;
    if (!_isStatic && !IsAllocated(_instance)) {
        BNM_LOG_ERR(DBG_BNM_MSG_FieldBase_GetFieldPointer_Error_instance_dead_instance, str().c_str());
        return nullptr;
    } else if (_isStatic && !IsAllocated(_data->parent)) {
        BNM_LOG_ERR(DBG_BNM_MSG_FieldBase_GetFieldPointer_Error_static_dead_parent, str().c_str());
        return nullptr;
    } else if (_isThreadStatic) {
        BNM_LOG_ERR(DBG_BNM_MSG_FieldBase_GetFieldPointer_Error_thread_static_unsupported, str().c_str());
        return nullptr;
    } else if (_isConst) {
        BNM_LOG_ERR(DBG_BNM_MSG_FieldBase_GetFieldPointer_Error_const_impossible, str().c_str());
        return nullptr;
    }
    if (_isStatic) return (void *) ((BNM_PTR) _data->parent->static_fields + _data->offset);
    return (void *) ((BNM_PTR) _instance + _data->offset - (_isInStruct ? sizeof(IL2CPP::Il2CppObject) : 0x0));
}

BNM::Class FieldBase::GetType() const {
    if (!_data) return {};
    return _data->type;
}

BNM::Class FieldBase::GetParentClass() const {
    if (!_data) return {};
    return _data->parent;
}
