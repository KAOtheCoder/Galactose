#include "GalactosePCH.h"

#include "ShaderTypeInfo.h"

namespace Galactose {
    const std::array<ShaderTypeInfo, ShaderTypeInfo::NumTypes> ShaderTypeInfo::s_types = {
        ShaderTypeInfo(ShaderTypeInfo::Float, sizeof(float)),
        { ShaderTypeInfo::Vector2, ShaderTypeInfo::Float, 2 },
        { ShaderTypeInfo::Vector3, ShaderTypeInfo::Float, 3 },
        { ShaderTypeInfo::Vector4, ShaderTypeInfo::Float, 4 }
    };

    ShaderTypeInfo::ShaderTypeInfo() :
        type(NumTypes),
        byteSize(0),
        componentCount(0)
    {
        for (int i = 0; i < ShaderTypeInfo::NumTypes; ++i)
            if (&s_types[i] == this)
                GT_ASSERT(false, GT_STRINGIFY(ShaderTypeInfo) "(" + std::to_string(i) + ") is not initialized.");

        GT_ASSERT(false, GT_STRINGIFY(ShaderTypeInfo) " is not initialized.");
    }

    ShaderTypeInfo::ShaderTypeInfo(const Type type, const int32_t a_byteSize) :
        type(type),
        byteSize(a_byteSize),
        componentCount(1)
    {}

    ShaderTypeInfo::ShaderTypeInfo(const Type type, const Type a_componentType, const uint32_t a_componentCount) : 
        type(type),
        byteSize(info(a_componentType).byteSize * a_componentCount),
        componentCount(a_componentCount)
    {}

    const ShaderTypeInfo& ShaderTypeInfo::info(const ShaderTypeInfo::Type a_type) {
        GT_ASSERT(s_types[a_type].type == a_type, GT_STRINGIFY(ShaderTypeInfo) " and type mismatch.");
        return s_types[a_type];
    }
}