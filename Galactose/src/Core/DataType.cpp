#include "GalactosePCH.h"

#include "DataType.h"

namespace Galactose {
    const std::array<DataTypeInfo, size_t(DataType::NumTypes)> DataTypeInfo::s_types = {
        DataTypeInfo(DataType::Float, sizeof(float)),
        { DataType::Vector2, 2, DataType::Float },
        { DataType::Vector3, 3, DataType::Float },
        { DataType::Vector4, 4, DataType::Float }
    };

    DataTypeInfo::DataTypeInfo() :
        type(DataType::NumTypes),
        byteSize(0),
        componentCount(0),
        componentType(type)
    {
        for (int i = 0; i < int(DataType::NumTypes); ++i)
            GT_ASSERT(&s_types[i] != this, GT_STRINGIFY(DataTypeInfo) "(" + std::to_string(i) + ") is not initialized.");

        GT_ASSERT(false, GT_STRINGIFY(DataTypeInfo) " is not initialized.");
    }

    DataTypeInfo::DataTypeInfo(const DataType type, const int32_t a_byteSize) :
        type(type),
        byteSize(a_byteSize),
        componentCount(1),
        componentType(type)
    {}

    DataTypeInfo::DataTypeInfo(const DataType type, const uint32_t a_componentCount, const DataType a_componentType) :
        type(type),
        byteSize(info(a_componentType).byteSize * a_componentCount),
        componentCount(a_componentCount),
        componentType(a_componentType)
    {}

    const DataTypeInfo& DataTypeInfo::info(const DataType a_type) {
        const auto& typeInfo = s_types[size_t(a_type)];
        GT_ASSERT(typeInfo.type == a_type, GT_STRINGIFY(DataTypeInfo) " and type mismatch.");
        return typeInfo;
    }
}