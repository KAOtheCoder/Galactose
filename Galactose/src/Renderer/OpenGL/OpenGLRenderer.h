#pragma once

#include "Renderer/Renderer.h"

namespace Galactose {
	class OpenGLRenderer : public Renderer {
	public:
		OpenGLRenderer(const std::shared_ptr<Window>& a_window);

		void setClearColor(const Vector4& color) override;
		void clear() override;
		void drawVertexArrayIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		void drawQuad(const Vector3& center, const Vector2& size, const std::shared_ptr<Texture>& texture) override;
		void drawQuad(const Vector3& center, const Vector2& size, const Vector4& color) override;
		void drawQuad2D(const Vector2& topLeft, const Vector2& size, const std::shared_ptr<Texture>& texture, const Vector2& canvasSize) override;

	private:
		void drawQuad(const Vector3& a_center, const Vector2& a_size);

		int m_textureSlots = 0;
		std::shared_ptr<VertexArray> m_quadVertexArray;
		std::shared_ptr<VertexArray> m_quad2DVertexArray;
	};
}
