#include "EditorLayer.h"

#include <Core/Events/KeyEvent.h>
#include <Core/Events/MouseEvent.h>
#include <Renderer/Renderer.h>
#include <Renderer/Camera.h>
#include <Renderer/Shader.h>
#include <Renderer/FrameBuffer.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components/Component.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

using namespace Galactose;

namespace GalactoseEditor {
	EditorLayer::EditorLayer(Window* a_window) :
		m_framebuffer(Framebuffer::create(a_window->width(), a_window->height(), { Texture::RGBA8, Texture::Depth24Stencil8 })),
		m_scene(std::make_shared<Scene>("scene")),
		m_texture(Texture::create("assets/textures/SSwithPistol.gif")),
		m_position(0, 0, -1),
		m_direction(0, 0, 1),
		m_up(0, 1, 0)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(a_window->nativeWindow()), true);
		ImGui_ImplOpenGL3_Init("#version 410");

		const auto entity = Entity::create(m_scene.get());
		Object::Ptr<Entity> entityPtr(entity);
		auto component = entityPtr->addComponent<Component>();
		GT_ASSERT(entityPtr.isValid() && component->entity() == entityPtr.get(), "");
		auto child = Entity::create(entity);
		GT_ASSERT(child->parent() == entityPtr.get(), "");
		child->setParent(nullptr);
		GT_ASSERT(entity->getChildren().empty(), "");
	}

	void EditorLayer::onUpdate() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();

		m_camera.setView(m_position, m_direction, m_up);
		const auto& renderer = Renderer::renderer();
		m_framebuffer->bind();
		renderer->clear();
		renderer->setViewProjection(m_camera);
		renderer->drawQuad({ -1, 0, 1 }, { 1.0f, 1.0f }, m_texture);
		renderer->drawQuad({ 1, 0, 1 }, { 1.0f, 1.0f }, { 0.8f, 0.1f, 0.1f, 0.8 });
		m_framebuffer->unbind();
		renderer->drawQuad2D({ 0, 0 }, { 1, 1 }, m_framebuffer->texture(0), { 1, 1 });

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorLayer::onEvent(const std::shared_ptr<Event>& a_event) {
		std::cout << a_event->toString() << std::endl;

		switch (a_event->type()) {
		case Event::KeyPress: {
			const auto key = static_cast<KeyEvent*>(a_event.get())->key();
			const float speed = 0.05f;
			const auto& right = glm::cross(m_direction, m_up);

			switch (key)
			{
			case KeyEvent::KeyS: m_position -= m_direction * speed;
				break;
			case KeyEvent::KeyW: m_position += m_direction * speed;
				break;
			case KeyEvent::KeyA: m_position -= right * speed;
				break;
			case KeyEvent::KeyD: m_position += right * speed;
				break;
			case KeyEvent::KeyQ: m_position -= m_up * speed;
				break;
			case KeyEvent::KeyE: m_position += m_up * speed;
				break;
			}
			break;
		}
		case Event::MousePress: m_rotate = true;
			m_cursorPos = static_cast<MouseEvent*>(a_event.get())->cursorPosition();
			break;
		case Event::MouseRelease: m_rotate = false;
			break;
		case Event::MouseMove: 
			if (m_rotate) {
				 const auto& cursorPos = static_cast<MouseEvent*>(a_event.get())->cursorPosition();
				 const auto& move = cursorPos - m_cursorPos;
				 m_cursorPos = cursorPos;
				 const float speed = 0.1f;
				 Matrix4x4 rotationMatrix(1);
				 const auto& right = glm::cross(m_direction, m_up);
				 rotationMatrix = glm::rotate(rotationMatrix, Math::degreesToRadians(move.x * speed), m_up);
				 rotationMatrix = glm::rotate(rotationMatrix, Math::degreesToRadians(move.y * speed), right);
				 m_direction = rotationMatrix * Vector4(m_direction, 1);
			}
			break;
		}

		a_event->setHandled();
	}
}
