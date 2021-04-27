#include "mvImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "mvGraphics.h"
#include <implot.h>

namespace Marvel {

	mvImGuiManager::mvImGuiManager(HWND hwnd, mvGraphics& graphics)
	{
		
		
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.IniFilename = nullptr;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(graphics.getDevice(), graphics.getContext());
	}

	mvImGuiManager::~mvImGuiManager()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}

	void mvImGuiManager::beginFrame() const
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void mvImGuiManager::endFrame() const
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

}