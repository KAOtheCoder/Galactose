#include "EditorLayer.h"

#include <Renderer/Renderer.h>
#include <Renderer/Camera.h>

using namespace Galactose;

namespace GalactoseEditor {
	EditorLayer::EditorLayer() :
		m_camera(new Camera())
	{}

	void EditorLayer::onUpdate() {
		const auto& projection = m_camera->projectionMatrix();
		const Matrix4x4& view = glm::lookAt(Vector3(4, 3, 3), Vector3(0, 0, 0), Vector3(0, 1, 0));
		const Matrix4x4 model(1.f);
		const Matrix4x4 mvp = projection * view * model;

		const auto& renderer = Renderer::renderer();
		renderer->clear();
		renderer->shader()->setMatrix4x4("u_mvp", mvp);
		renderer->drawSprite(Vector3(0, 0, 0), { 1.0f, 1.0f });
	}
}
