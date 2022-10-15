#pragma once

#include "GalactosePCH.h"

namespace Galactose {
    enum class DataType {
        Float,
        Vector2,
        Vector3,
        Vector4,
        NumTypes
    };

    class DataTypeInfo {
    public:
        static const DataTypeInfo& info(const DataType a_type);

        const DataType type;
        const int32_t byteSize;
        const uint32_t componentCount;
        const DataType componentType;

    private:
        static const std::array<DataTypeInfo, size_t(DataType::NumTypes)> s_types;

        DataTypeInfo();
        DataTypeInfo(const DataType type, const int32_t byteSize);
        DataTypeInfo(const DataType type, const uint32_t componentCount, const DataType componentType);
    };
}
