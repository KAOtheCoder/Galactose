#pragma once

#include "Renderer/Renderer.h"

namespace Galactose {
	class OpenGLRenderer : public Renderer {
	public:
		OpenGLRenderer(const std::shared_ptr<Window>& a_window);

		void setClearColor(const Vector4& color) override;
		void clear() override;
		void drawVertexArrayIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		void drawSprite(const Matrix4x4& transform, const Sprite& sprite) override;
		void drawQuad2D(const Vector2& topLeft, const Vector2& size, const std::shared_ptr<Texture>& texture, const Vector2& canvasSize) override;

	private:
		void drawSprite(const Matrix4x4& transform, const Vector2& size, const Vector2& pivot);

		int m_textureSlots = 0;
		std::shared_ptr<VertexArray> m_quadVertexArray;
		std::shared_ptr<VertexArray> m_quad2DVertexArray;
	};
}
