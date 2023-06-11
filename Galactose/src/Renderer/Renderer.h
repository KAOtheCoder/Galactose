#pragma once

#include "VertexArray.h"
#include "Sprite.h"
#include "Core/Window.h"

namespace Galactose {
	class Texture;
	class Shader;
	class Camera;

	class Renderer {
	public:
		GT_API static std::shared_ptr<Renderer> create(const std::shared_ptr<Window>& a_window);
		static std::shared_ptr<Renderer> renderer(const int32_t id = 0);

		int32_t id() const { return m_id; }

		virtual void setClearColor(const Vector4& color) = 0;
		virtual void clear() = 0;
		virtual void drawVertexArrayIndexed(const std::shared_ptr<VertexArray>& a_vertexArray) = 0;

		virtual void drawSprite(const Matrix4x4& transform, const Sprite& sprite) = 0;

		std::shared_ptr<Shader> textureShader() const { return m_textureShader; }

		void setViewProjection(const Matrix4x4& viewProjection);

	protected:
		Renderer(const std::shared_ptr<Window>& a_window);

		std::shared_ptr<Window> m_window;
		std::shared_ptr<Texture> m_defaultTexture;
		std::shared_ptr<Shader> m_textureShader;

	private:
		inline static std::vector<std::shared_ptr<Renderer>> s_renderers;
		
		int32_t m_id = -1;
	};
}
