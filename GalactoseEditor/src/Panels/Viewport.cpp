#include "Viewport.h"
#include "EditorContext.h"

#include <Galactose/Renderer/Framebuffer.h>
#include <Galactose/Scene/Components/Camera.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	Viewport::Viewport(const std::string& a_title, const std::shared_ptr<EditorContext>& a_editorContext) :
		Panel(a_title),
		m_editorContext(a_editorContext),
		m_framebuffer(Framebuffer::create(1, 1, { Texture::RGBA8, Texture::Depth24Stencil8 }))
	{
		setPadding({ 0, 0 });
	}

	void Viewport::onUpdate() {
		const auto& scene = m_editorContext->scene();
		if (scene) {
			const auto& viewportSize = ImGui::GetContentRegionAvail();

			if (viewportSize.x >= 1 && viewportSize.y >= 1) {
				m_framebuffer->resize(int32_t(viewportSize.x), int32_t(viewportSize.y));

				auto camera = getCamera();

				if (camera)
					camera->setAspectRatio(viewportSize.x / viewportSize.y);

				m_framebuffer->bind();
				scene->render(camera);
				m_framebuffer->unbind();

				ImGui::Image((void*)(intptr_t)m_framebuffer->texture(0)->rendererId(), viewportSize, { 0, 1 }, { 1, 0 });
			}
		}
	}
}
