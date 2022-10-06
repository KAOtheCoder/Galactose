#pragma once

#include "Renderer/Renderer.h"

#include <glad/glad.h>

namespace Galactose {
	class OpenGLRenderer : public Renderer {
	public:
		OpenGLRenderer(const std::shared_ptr<Window>& a_window);

		void setClearColor(const float a_r, const float a_g, const float a_b, const float a_a) override { glClearColor(a_r, a_g, a_b, a_a); }
		void clear() override { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
		void drawVertexArrayIndexed(const std::shared_ptr<VertexArray>& a_vertexArray) override;
		void drawSprite(const Vector3 a_center, const Vector2 a_size) override;
	};
}