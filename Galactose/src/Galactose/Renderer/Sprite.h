#pragma once

#include "Galactose/Math/Math.h"

#include <memory>

namespace Galactose {
	class Texture;

	class Sprite {
	public:
		Sprite() = default;

		Sprite(const std::shared_ptr<Texture>& texture,
			const Vector4& color = { 1, 1, 1, 1 }, 
			const Vector2& pivot = { 0.5f, 0.5f }
		);

		std::shared_ptr<Texture> texture() const { return m_texture; }
		void setTexture(const std::shared_ptr<Texture>& texture) { m_texture = texture; }

		Vector4 color() const { return m_color; }
		void setColor(const Vector4& a_color) { m_color = a_color; }

		Vector2 pivot() const { return m_pivot; }
		void setPivot(const Vector2& a_pivot) { m_pivot = a_pivot; }

		Vector2 size() const { return m_size; }
		void setSize(const Vector2& a_size) { m_size = a_size; }

		void autoResize();

	private:
		std::shared_ptr<Texture> m_texture;
		Vector4 m_color = { 1, 1, 1, 1 };
		Vector2 m_pivot = { 0.5f, 0.5f };
		Vector2 m_size = { 1, 1 };
	};
}