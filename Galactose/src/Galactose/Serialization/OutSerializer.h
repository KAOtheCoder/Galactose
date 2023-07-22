#pragma once

#include "Galactose/Math/Math.h"

#include <memory>
#include <type_traits>

namespace YAML {
	class Emitter;
}

namespace std::filesystem {
	class path;
}

namespace Galactose {
	class Uuid;

	class OutSerializer {
	public:
		enum _Null {
			Null
		};

		enum Manip {
			// general manipulators
			Auto,
			TagByKind,
			Newline,

			// output character set
			EmitNonAscii,
			EscapeNonAscii,
			EscapeAsJson,

			// string manipulators
			// Auto, // duplicate
			SingleQuoted,
			DoubleQuoted,
			Literal,

			// null manipulators
			LowerNull,
			UpperNull,
			CamelNull,
			TildeNull,

			// bool manipulators
			YesNoBool,      // yes, no
			TrueFalseBool,  // true, false
			OnOffBool,      // on, off
			UpperCase,      // TRUE, N
			LowerCase,      // f, yes
			CamelCase,      // No, Off
			LongBool,       // yes, On
			ShortBool,      // y, t

			// int manipulators
			Dec,
			Hex,
			Oct,

			// document manipulators
			BeginDoc,
			EndDoc,

			// sequence manipulators
			BeginSeq,
			EndSeq,
			Flow,
			Block,

			// map manipulators
			BeginMap,
			EndMap,
			Key,
			Value,
			// Flow, // duplicate
			// Block, // duplicate
			// Auto, // duplicate
			LongKey
		};

		GT_API OutSerializer();
		OutSerializer(const OutSerializer&) = delete;
		OutSerializer& operator=(const OutSerializer&) = delete;

		GT_API void save(const std::string& filePath);

		// don't mess with string literals. remove type checks when you overload for them.
		template<typename T, typename = std::enable_if_t<std::is_pointer_v<T> 
			&& !std::is_same_v<T, const wchar_t*>
			&& !std::is_same_v<T, const char16_t*>
			&& !std::is_same_v<T, const char32_t*>
		>>
		OutSerializer& operator<<(const T a_rhs) { return a_rhs ? *this << (*a_rhs) : *this << Null; }

		OutSerializer& operator<<(const _Null rhs);
		GT_API OutSerializer& operator<<(const Manip rhs);
		GT_API OutSerializer& operator<<(const float& rhs);
		OutSerializer& operator<<(const char* a_rhs) { return *this << std::string(a_rhs); }
		GT_API OutSerializer& operator<<(const std::string& rhs);
		GT_API OutSerializer& operator<<(const std::filesystem::path& rhs);
		OutSerializer& operator<<(const Uuid& rhs);
		OutSerializer& operator<<(const Vector2& rhs);
		OutSerializer& operator<<(const Vector3& rhs);
		OutSerializer& operator<<(const Vector4& rhs);
		OutSerializer& operator<<(const Quaternion& rhs);

	private:
		std::shared_ptr<YAML::Emitter> m_emitter;
	};
}