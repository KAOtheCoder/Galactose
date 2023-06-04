#pragma once

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>

namespace Galactose {
	namespace internal {
		template<typename T>
		class GlmExtend : public T {
		public:
			using T::T;
			GlmExtend(const T& a_base) { T::operator=(a_base); }
			std::string toString() const { return glm::to_string(base()); }
			const float* data() const { return glm::value_ptr(base()); }
			float* data() { return const_cast<float*>(glm::value_ptr(base())); }

		protected:
			const T& base() const { return *static_cast<const T*>(this); }
		};

		template<glm::length_t L>
		class Vector : public GlmExtend<glm::vec<L, float>> {
		public:
			static float dot(const Vector<L>& a_lhs, const Vector<L>& a_rhs) { return glm::dot(a_lhs.base(), a_rhs.base()); }

			using GlmExtend<glm::vec<L, float>>::GlmExtend;

			Vector<L> normalized() const { return glm::normalize(this->base()); }
			float magnitude() const { return glm::length(this->base()); }
		};
	}

	typedef internal::Vector<2> Vector2;
	typedef internal::Vector<4> Vector4;

	class Vector3 : public internal::Vector<3> {
	public:
		using Vector<3>::Vector;

		static Vector3 cross(const Vector3& a_lhs, const Vector3& a_rhs) { return glm::cross(a_lhs.base(), a_rhs.base()); }
		
		friend class Quaternion;
	};

	class Quaternion;

	class Matrix4x4 : public internal::GlmExtend<glm::mat4> {
	public:
		using GlmExtend<glm::mat4>::GlmExtend;

		static Matrix4x4 translate(const Vector3& a_translation) { return glm::translate(a_translation); }
		static Matrix4x4 scale(const Vector3& a_scale) { return glm::scale(a_scale); }

		Vector4 column(const int index) const { return operator[](index); }
		Matrix4x4 affineInverse() const { return glm::affineInverse(base()); }
		void decomposeAffine(Vector3* position = nullptr, Quaternion* rotation = nullptr, Vector3* scale = nullptr) const;
	};

	class Quaternion : public internal::GlmExtend<glm::quat> {
	public:
		using GlmExtend<glm::quat>::GlmExtend;

		static Quaternion fromEulerDegrees(const Vector3& a_angles) { return Quaternion(glm::radians(a_angles.base())); }

		Quaternion(const Matrix4x4& a_matrix) : GlmExtend<glm::quat>(glm::quat_cast(a_matrix)) {}

		Matrix4x4 toMatrix() const { return glm::toMat4(*this); }
		Vector3 eulerRadians() const { return glm::eulerAngles(*this); }
		Vector3 eulerDegrees() const { return glm::degrees(eulerRadians().base()); }
		Quaternion inverse() const { return glm::inverse(base()); }
	};

	namespace Math {
		inline float degreesToRadians(const float a_degrees) { return glm::radians(a_degrees); }
		inline float radiansToDegrees(const float a_radians) { return glm::degrees(a_radians); }
	}
}
