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
#include <Core/Application.h>

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
		auto& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
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

	EditorLayer::~EditorLayer() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorLayer::onUpdate() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		m_camera.setView(m_position, m_direction, m_up);
		const auto& renderer = Renderer::renderer();
		m_framebuffer->bind();
		renderer->clear();
		renderer->setViewProjection(m_camera);
		renderer->drawQuad({ -1, 0, 1 }, { 1.0f, 1.0f }, m_texture);
		renderer->drawQuad({ 1, 0, 1 }, { 1.0f, 1.0f }, { 0.8f, 0.1f, 0.1f, 0.8 });
		m_framebuffer->unbind();
		renderer->drawQuad2D({ 0, 0 }, { 1, 1 }, m_framebuffer->texture(0), { 1, 1 });

		auto& io = ImGui::GetIO();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "ALT+F4")) 
				{
					Application::instance()->exit();
				}

				ImGui::EndMenu();
			}
			
			ImGui::EndMainMenuBar();
		}

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		ImGui::Begin("Viewport");
		const auto& viewportSize = ImGui::GetContentRegionAvail();
		m_framebuffer->resize(viewportSize.x, viewportSize.y);
		m_camera.setAspectRatio(viewportSize.x / viewportSize.y);
		ImGui::Image(ImTextureID(m_framebuffer->texture(0)->rendererId()), viewportSize, { 0, 1 }, { 1, 0 });
		ImGui::End();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			//GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			//glfwMakeContextCurrent(backup_current_context);
		}
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
