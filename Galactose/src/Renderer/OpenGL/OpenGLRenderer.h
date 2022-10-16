#pragma once

#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"

namespace Galactose {
	class OpenGLRenderer : public Renderer {
	public:
		OpenGLRenderer(const std::shared_ptr<Window>& a_window);

		void setClearColor(const float r, const float g, const float b, const float a) override;
		void clear() override;
		void drawVertexArrayIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		void drawSprite(const Vector3& center, const Vector2& size, const std::shared_ptr<Texture>& texture) override;
	};
}