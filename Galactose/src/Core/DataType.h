#pragma once

#include "GalactosePCH.h"

namespace Galactose {
    class DataType {
    public:
        DataType(const std::string_view& a_name, const size_t a_byteSize);
        DataType(const std::string_view& a_name, const DataType& a_componentType, const uint32_t a_componentCount);

    private:
        inline static std::vector<DataType> s_types;

        int32_t m_ordinal;
        std::string_view m_name;
        size_t m_byteSize;
        const DataType& m_componentType;
        uint32_t m_componentCount;
    };

    namespace DataTypes {
        extern inline const DataType FLOAT(GT_STRINGIFY(FLOAT), sizeof(float));
        extern inline const DataType VECTOR2(GT_STRINGIFY(VECTOR2), FLOAT, 2);
        extern inline const DataType VECTOR3(GT_STRINGIFY(VECTOR3), FLOAT, 3);
        extern inline const DataType VECTOR4(GT_STRINGIFY(VECTOR4), FLOAT, 4);
    };
}