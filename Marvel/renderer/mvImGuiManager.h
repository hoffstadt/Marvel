#pragma once
#include "mvMarvelWin.h"
#include "imgui.h"

namespace Marvel {

	class mvGraphics;

	class mvImGuiManager
	{

	public:

		mvImGuiManager(HWND hwnd, mvGraphics& graphics);
		~mvImGuiManager();

		void beginFrame() const;
		void endFrame() const;

	};

}