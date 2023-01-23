#include "EditorLayer.h"
#include "EditorSceneData.h"

#include <Core/Events/KeyEvent.h>
#include <Core/Events/MouseEvent.h>
#include <Scene/Entity.h>
#include <Scene/Components/SpriteRenderer.h>
#include <Core/Application.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <nfd.hpp>

using namespace Galactose;

namespace GalactoseEditor {
	EditorLayer::EditorLayer(Window* a_window) :
		//m_texture(Texture::create("assets/textures/SSwithPistol.gif")),
		m_sceneData(std::make_shared<EditorSceneData>()),
		m_sceneViewport(m_sceneData),
		m_sceneHierarchy(m_sceneData),
		m_inspector(m_sceneData)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		auto& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
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

		//ImGui::SetWindowFontScale(2);

		GT_ASSERT(NFD_Init() == NFD_OKAY, "Failed to initialze NFD");

		const auto entity = Entity::create(m_sceneData->scene().get(), "parent");
		SceneObject::Ptr<Entity> entityPtr(entity);
		auto component = entityPtr->addComponent<Component>();
		GT_ASSERT(entityPtr.isValid() && component->entity() == entityPtr.get(), "");
		auto child = Entity::create(entity, "child");
		child->addComponent<SpriteRenderer>()->sprite = Sprite(std::shared_ptr<Texture>(), {1.0f, 0.0f, 0.0f, 0.5f});
		GT_ASSERT(child->parent() == entityPtr.get(), "");
		//child->setParent(nullptr);
		//GT_ASSERT(entity->getChildren().empty(), "");
		//m_sceneData->scene()->save("scene.yaml");
	}

	EditorLayer::~EditorLayer() {
		NFD_Quit();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorLayer::onUpdate() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		drawMenuBar();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		m_sceneHierarchy.update();
		m_inspector.update();
		m_sceneViewport.update();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			//GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			//glfwMakeContextCurrent(backup_current_context);
		}
	}

	void EditorLayer::drawMenuBar() {
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "ALT+F4"))
					Application::instance()->exit();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Panels"))
			{
				if (ImGui::MenuItem("Scene"))
					m_sceneViewport.setVisible(true);

				if (ImGui::MenuItem("Scene Hierarchy"))
					m_sceneHierarchy.setVisible(true);

				if (ImGui::MenuItem("Inspector"))
					m_inspector.setVisible(true);

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void EditorLayer::onEvent(const std::shared_ptr<Event>& a_event) {
		//std::cout << a_event->toString() << std::endl;

		if (Panel::focusedPanel() == &m_sceneViewport)
			m_sceneViewport.onEvent(a_event);
	}
}
