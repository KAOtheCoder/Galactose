#pragma once

#include <type_traits>

#define GT_PROPERTY(_type, _name, _get, _set) Galactose::Property<ScriptType, _type, &ScriptType::_get, &ScriptType::_set> _name;
#define GT_READONLY_PROPERTY(_type, _name, _get) Galactose::ReadOnlyProperty<ScriptType, _type, &ScriptType::_get> _name;
#define GT_INIT_PROPERTY(_name) _name(this, GT_STRINGIFY(_name))

namespace Galactose {
	class Script;

	class PropertyBase {
	public:
		PropertyBase() = default;

		virtual bool isReadOnly() const { return true; }

	protected:
		void registerProperty(Script* a_script, const char* a_name);
	};

	template <typename T>
	class AccessibleProperty : public PropertyBase {
	public:
		using PropertyBase::PropertyBase;

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
				registerProperty(a_object, a_name);
		}

		operator T() {
			if constexpr (std::is_member_object_pointer_v<decltype(_Get)>)
				return m_object->*_Get;
			else if constexpr (std::is_member_function_pointer_v<decltype(_Get)>)
				return (m_object->*_Get)();
			else
				static_assert(false, "Getter is neighter member object or member function.");
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
			if constexpr (std::is_member_object_pointer_v<decltype(_Set)>)
				m_object->*_Set = a_value;
			else if constexpr (std::is_member_function_pointer_v<decltype(_Set)>)
				(m_object->*_Set)(a_value);
			else
				static_assert(false, "Setter is neighter member object or member function.");
		}

		void set(const T& a_value) override { this->operator=(a_value); }

		bool isReadOnly() const override { return false; }
	};
}
