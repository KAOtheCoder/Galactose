#pragma once

#include "Renderer/IndexBuffer.h"

namespace Galactose {
	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(const uint32_t* a_indices, const uint32_t count);
		~OpenGLIndexBuffer() override;

		void bind() override;
		void unbind() override;

	private:
		uint32_t m_rendererId;
	};
}