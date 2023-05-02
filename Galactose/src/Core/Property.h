#pragma once

#include "Core/DataType.h"

#include <type_traits>

#define GT_PROPERTY(a_type, a_name, a_get, a_set) Galactose::Property<_ScriptType, a_type, &_ScriptType::a_get, &_ScriptType::a_set> a_name;
#define GT_READONLY_PROPERTY(a_type, a_name, a_get) Galactose::ReadOnlyProperty<_ScriptType, a_type, &_ScriptType::a_get> a_name;
#define GT_INIT_PROPERTY(a_name) a_name(this, GT_STRINGIFY(a_name))

namespace Galactose {
	class Script;

	class PropertyBase {
	public:
		PropertyBase() = default;

		virtual bool isReadOnly() const { return true; }
		virtual DataType::Type type() const = 0;

	protected:
		void registerProperty(Script* a_script, const char* a_name);
	};

	template <typename T>
	class AccessibleProperty : public PropertyBase {
	public:
		using PropertyBase::PropertyBase;

		DataType::Type type() const override { return DataType::toType<T>(); }

		virtual T get() = 0;
		virtual void set(const T& value) {}
	};

	template <typename O, typename T, auto _Get>
	class ReadOnlyProperty : public AccessibleProperty<T> {
	public:
		ReadOnlyProperty(O* a_object, const char* a_name) :
			m_object(a_object)
		{
			if constexpr (std::is_base_of_v<Script, O>)
				this->registerProperty(a_object, a_name);
		}

		operator T() {
			if constexpr (std::is_member_object_pointer_v<decltype(_Get)>) {
				return m_object->*_Get;
			}
			else {
				static_assert(std::is_member_function_pointer_v<decltype(_Get)>, "Getter is neighter member object or member function.");
				return (m_object->*_Get)();
			}
		}

		T get() override { return this->operator T(); }

	protected:
		O* m_object = nullptr;
	};

	template <typename O, typename T, auto _Get, auto _Set>
	class Property : public ReadOnlyProperty<O, T, _Get> {
	public:
		using ReadOnlyProperty<O, T, _Get>::ReadOnlyProperty;

		void operator=(const T& a_value) {
			if constexpr (std::is_member_object_pointer_v<decltype(_Set)>) {
				this->m_object->*_Set = a_value;
			}
			else {
				static_assert(std::is_member_function_pointer_v<decltype(_Set)>, "Setter is neighter member object or member function.");
				(this->m_object->*_Set)(a_value);
			}
		}

		void set(const T& a_value) override { this->operator=(a_value); }

		bool isReadOnly() const override { return false; }
	};
}
