#include "DataType.h"
#include "Core/Global.h"

#include <string>

namespace Galactose {
    const std::array<DataType, size_t(DataType::NumTypes)> DataType::s_types = {
        DataType(DataType::Unknown, 0),
        DataType(DataType::Bool, sizeof(bool)),
        DataType(DataType::Float, sizeof(float)),
        { DataType::Vector2, 2, DataType::Float },
        { DataType::Vector3, 3, DataType::Float },
        { DataType::Vector4, 4, DataType::Float }
    };

    DataType::DataType() :
        type(DataType::NumTypes),
        byteSize(0),
        componentCount(0),
        componentType(type)
    {
        for (int i = 0; i < int(DataType::NumTypes); ++i)
            GT_ASSERT(&s_types[i] != this, GT_STRINGIFY(DataType) "(" + std::to_string(i) + ") is not initialized.");

        GT_ASSERT(false, GT_STRINGIFY(DataType) " is not initialized.");
    }

    DataType::DataType(const Type type, const int32_t a_byteSize) :
        type(type),
        byteSize(a_byteSize),
        componentCount(1),
        componentType(type)
    {}

    DataType::DataType(const Type type, const uint32_t a_componentCount, const Type a_componentType) :
        type(type),
        byteSize(info(a_componentType).byteSize * a_componentCount),
        componentCount(a_componentCount),
        componentType(a_componentType)
    {}

    const DataType& DataType::info(const Type a_type) {
        const auto& typeInfo = s_types[size_t(a_type)];
        GT_ASSERT(typeInfo.type == a_type, GT_STRINGIFY(DataType) " and type mismatch.");
        return typeInfo;
    }
}
