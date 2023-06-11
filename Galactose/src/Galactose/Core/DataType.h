#pragma once

#include "Galactose/Math/Math.h"

#include <array>

namespace Galactose {
    class DataType {
    public:
        enum Type {
            Void,
            Bool,
            Float,
            Vector2,
            Vector3,
            Vector4,
            NumTypes
        };

        static const DataType& info(const Type a_type);

        template <typename T>
        static Type toType() { return DataType::Unknown; }

        template <> static Type toType<bool>() { return DataType::Bool; }
        template <> static Type toType<float>() { return DataType::Float; }
        template <> static Type toType<Galactose::Vector2>() { return DataType::Vector2; }
        template <> static Type toType<Galactose::Vector3>() { return DataType::Vector3; }
        template <> static Type toType<Galactose::Vector4>() { return DataType::Vector4; }

        const Type type;
        const int32_t byteSize;
        const uint32_t componentCount;
        const Type componentType;

    private:
        static const std::array<DataType, size_t(DataType::NumTypes)> s_types;

        DataType();
        DataType(const Type type, const int32_t byteSize);
        DataType(const Type type, const uint32_t componentCount, const Type componentType);
    };
}
