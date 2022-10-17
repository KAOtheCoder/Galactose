#pragma once

#include "Renderer/Renderer.h"

namespace Galactose {
	class OpenGLRenderer : public Renderer {
	public:
		OpenGLRenderer(const std::shared_ptr<Window>& a_window);

		void setClearColor(const float r, const float g, const float b, const float a) override;
		void clear() override;
		void drawVertexArrayIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		void drawSprite(const Vector3& center, const Vector2& size, const std::shared_ptr<Texture>& texture) override;
		void drawQuad(const Vector3& center, const Vector2& size, const Vector4& color) override;

	private:
		void drawQuad(const Vector3& a_center, const Vector2& a_size);

		int m_textureSlots = 0;
		std::shared_ptr<VertexBuffer> m_quadVertexBuffer;
		std::shared_ptr<VertexArray> m_quadVertexArray;
	};
}
