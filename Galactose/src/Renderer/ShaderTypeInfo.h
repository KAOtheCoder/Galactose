#pragma once

#include "GalactosePCH.h"

namespace Galactose {
    class ShaderTypeInfo {
    public:
        enum Type {
            Float,
            Vector2,
            Vector3,
            Vector4,
            NumTypes
        };

        static const ShaderTypeInfo& info(const Type a_type);

        const Type type;
        const int32_t byteSize;
        const uint32_t componentCount;

    private:
        static const std::array<ShaderTypeInfo, NumTypes> s_types;

        ShaderTypeInfo();
        ShaderTypeInfo(const Type type, const int32_t byteSize);
        ShaderTypeInfo(const Type type, const Type componentType, const uint32_t componentCount);
    };
}
