#include "Math.h"

namespace Galactose {
	// Inspired from cglm::glm_decompose
	void Matrix4x4::decomposeAffine(Vector3* a_position, Quaternion* a_rotation, Vector3* a_scale) const {
		if (a_position)
			*a_position = column(3);

		if (!a_rotation && !a_scale)
			return;

		Vector3 scale(column(0).magnitude(), column(1).magnitude(), column(2).magnitude());

		Matrix4x4 rotation(*this); // we only need 3x3 matrix, maybe change that later
		rotation[3] = { 0, 0, 0, 1 };

		for (int i = 0; i < 3; ++i) // avoid divide by zero
			rotation[i] /= scale[i] == 0 ? glm::epsilon<float>() : scale[i];

		// check for a coordinate system flip.If the determinant
		// is - 1, then negate the matrixand the scaling factors.
		const auto& v = Vector3::cross(column(0), column(1));
		if (Vector3::dot(v, column(2)) < 0) {
			rotation[0] = -rotation[0];
			rotation[1] = -rotation[1];
			rotation[2] = -rotation[2];
			scale = -scale;
		}

		if (a_rotation)
			*a_rotation = rotation;

		if (a_scale)
			*a_scale = scale;
	}

	Matrix4x4 Matrix4x4::rotate(const Quaternion& a_rotation) {
		return a_rotation.toMatrix();
	}

	Matrix4x4 Matrix4x4::transform(const Vector3& a_translation, const Quaternion& a_rotation, const Vector3& a_scale) {
		return translate(a_translation) * rotate(a_rotation) * scale(a_scale);
	}
}
