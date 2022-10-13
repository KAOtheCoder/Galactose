#pragma once

#include "VertexArray.h"
#include "Core/Window.h"
#include "Core/Math.h"
#include "Renderer/Shader.h"

namespace Galactose {
	class Renderer {
	public:
		static std::shared_ptr<Renderer> create(const std::shared_ptr<Window>& a_window);
		static std::shared_ptr<Renderer> renderer(const int32_t a_id = 0) { return s_renderers[a_id]; }

		int32_t id() const { return m_id; }

		virtual void setClearColor(const float a_r, const float a_g, const float a_b, const float a_a) = 0;
		virtual void clear() = 0;
		virtual void drawVertexArrayIndexed(const std::shared_ptr<VertexArray>& a_vertexArray) = 0;

		virtual void drawSprite(const Vector3 a_center, const Vector2 a_size) = 0;

		std::shared_ptr<Shader> shader() const { return m_shader; }

	protected:
		std::shared_ptr<Shader> m_shader;

	private:
		inline static std::vector<std::shared_ptr<Renderer>> s_renderers;
		
		int32_t m_id = -1;
	};
}