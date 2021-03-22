#pragma once
#include "mvMarvelWin.h"
#include <optional>
#include <vector>
#include "mvKeyboard.h"
#include "mvMouse.h"

namespace Marvel {

	class mvWindow
	{

	public:

		static std::optional<int> ProcessMessages();

	public:

		mvWindow(const char* name, int width, int height);
		~mvWindow();

		HWND getHandle    () const { return m_hWnd; }
		void enableCursor ();
		void disableCursor();
		bool cursorEnabled() const { return m_cursorEnabled; }

	public:

		mvKeyboard kbd;
		mvMouse    mouse;

	private:

		void confineCursor();
		void freeCursor() { ClipCursor(nullptr); }
		void showCursor() { while (::ShowCursor(TRUE) < 0); }
		void hideCursor() { while (::ShowCursor(FALSE) >= 0); }

		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:

		static constexpr const char* s_wndClassName = "Marvel Window";

		bool              m_cursorEnabled = true;
		int               m_width;
		int               m_height;
		HWND              m_hWnd;
		HINSTANCE         m_hInst;
		std::vector<BYTE> m_rawBuffer;

	};


}