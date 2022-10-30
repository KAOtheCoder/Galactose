#pragma once

#include "VertexArray.h"
#include "Core/Window.h"
#include "Math/Math.h"

namespace Galactose {
	class Texture;
	class Shader;
	class Camera;

	class Renderer {
	public:
		static std::shared_ptr<Renderer> create(const std::shared_ptr<Window>& a_window);
		static std::shared_ptr<Renderer> renderer(const int32_t a_id = 0) { return s_renderers[a_id]; }

		int32_t id() const { return m_id; }

		virtual void setClearColor(const Vector4& color) = 0;
		virtual void clear() = 0;
		virtual void drawVertexArrayIndexed(const std::shared_ptr<VertexArray>& a_vertexArray) = 0;

		virtual void drawQuad(const Vector3& center, const Vector2& size, const std::shared_ptr<Texture>& texture) = 0;
		virtual void drawQuad(const Vector3& center, const Vector2& size, const Vector4& color) = 0;
		virtual void drawQuad2D(const Vector2& topLeft, const Vector2& size, const std::shared_ptr<Texture>& texture, const Vector2& canvasSize) = 0;

		std::shared_ptr<Shader> textureShader() const { return m_textureShader; }
		std::shared_ptr<Shader> colorShader() const { return m_colorShader; }

		void setViewProjection(const Camera& camera);

	protected:
		Renderer(const std::shared_ptr<Window>& a_window);

		std::shared_ptr<Window> m_window;
		std::shared_ptr<Shader> m_textureShader;
		std::shared_ptr<Shader> m_colorShader;
		std::shared_ptr<Shader> m_quad2DShader;

	private:
		inline static std::vector<std::shared_ptr<Renderer>> s_renderers;
		
		int32_t m_id = -1;
	};
}