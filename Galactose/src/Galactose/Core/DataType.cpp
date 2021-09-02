#include "GalactosePCH.h"

#include "DataType.h"

namespace Galactose {
    DataType::DataType(const std::string_view& a_name, const size_t a_byteSize)
        : m_ordinal(int32_t(s_types.size())),
        m_name(a_name),
        m_byteSize(a_byteSize),
        m_componentType(*this),
        m_componentCount(1)
    {
        s_types.emplace_back(*this);
    }

    DataType::DataType(const std::string_view& a_name, const DataType& a_componentType, const uint32_t a_componentCount)
        : m_ordinal(int32_t(s_types.size())),
        m_name(a_name),
        m_byteSize(a_componentType.m_byteSize* a_componentCount),
        m_componentType(a_componentType),
        m_componentCount(a_componentCount)
    {
        s_types.emplace_back(*this);
    }
}