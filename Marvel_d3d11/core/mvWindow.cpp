#include "mvWindow.h"
#include "imgui_impl_win32.h"
#include <iostream>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Marvel {

	mvWindow::mvWindow(const char* name, int width, int height)
		:
		m_width(width),
		m_height(height)
	{
		m_hInst = GetModuleHandle(nullptr);

		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMsgSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInst;
		wc.hIcon = nullptr;
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = s_wndClassName;
		wc.hIconSm = nullptr;
		RegisterClassEx(&wc);

		// calculate window size based on desired client region size
		RECT wr;
		wr.left = 100;
		wr.right = width + wr.left;
		wr.top = 100;
		wr.bottom = height + wr.top;
		AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		// create window & get hWnd
		m_hWnd = CreateWindow(
			s_wndClassName, name,
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			0, 0, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, m_hInst, this
		);


		ShowWindow(m_hWnd, SW_SHOWDEFAULT);

		// register mouse raw input device
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01; // mouse page
		rid.usUsage = 0x02; // mouse usage
		rid.dwFlags = 0;
		rid.hwndTarget = nullptr;
		RegisterRawInputDevices(&rid, 1, sizeof(rid));
	}

	mvWindow::~mvWindow()
	{
		DestroyWindow(m_hWnd);
		UnregisterClass(s_wndClassName, m_hInst);

	}

	LRESULT CALLBACK mvWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
		if (msg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			mvWindow* const pWnd = static_cast<mvWindow*>(pCreate->lpCreateParams);
			// set WinAPI-managed user data to store ptr to window instance
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&mvWindow::HandleMsgThunk));
			// forward message to window instance handler
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		// if we get a message before the WM_NCCREATE message, handle with default handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK mvWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// retrieve ptr to window instance
		mvWindow* const pWnd = reinterpret_cast<mvWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}


	std::optional<int> mvWindow::ProcessMessages()
	{
		MSG msg;
		// while queue has messages, remove and dispatch them (but do not block on empty queue)
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// check for quit because peekmessage does not signal this via return val
			if (msg.message == WM_QUIT)
			{
				// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
				return (int)msg.wParam;
			}

			// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// return empty optional when not quitting app
		return {};
	}

	LRESULT mvWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		switch (msg)
		{

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		case WM_KILLFOCUS:
			kbd.clearState();
			break;
		case WM_ACTIVATE:
			// confine/free cursor on window to foreground/background if cursor disabled
			if (!m_cursorEnabled)
			{
				if (wParam & WA_ACTIVE)
				{
					confineCursor();
					hideCursor();
				}
				else
				{
					freeCursor();
					showCursor();
				}
			}
			break;

			/*********** KEYBOARD MESSAGES ***********/
		case WM_KEYDOWN:
			// syskey commands need to be handled to track ALT key (VK_MENU) and F10
		case WM_SYSKEYDOWN:
			// stifle this keyboard message if imgui wants to capture
			//if (imio.WantCaptureKeyboard)
			//	break;

			if (!(lParam & 0x40000000) || kbd.autorepeatIsEnabled()) // filter autorepeat
			{
				kbd.onKeyPressed(static_cast<unsigned char>(wParam));
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:

			kbd.onKeyReleased(static_cast<unsigned char>(wParam));
			break;
		case WM_CHAR:
			// stifle this keyboard message if imgui wants to capture
			//if (imio.WantCaptureKeyboard)
			//	break;

			kbd.onChar(static_cast<unsigned char>(wParam));
			break;
			/*********** END KEYBOARD MESSAGES ***********/

			/************* MOUSE MESSAGES ****************/
		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			// cursorless exclusive gets first dibs
			if (!m_cursorEnabled)
			{
				if (!mouse.isInWindow())
				{
					SetCapture(hWnd);
					mouse.onMouseEnter();
					hideCursor();
				}
				break;
			}

			// stifle this mouse message if imgui wants to capture
			//if (imio.WantCaptureMouse)
			//	break;

			// in client region -> log move, and log enter + capture mouse (if not previously in window)
			if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
			{
				mouse.onMouseMove(pt.x, pt.y);
				if (!mouse.isInWindow())
				{
					SetCapture(hWnd);
					mouse.onMouseEnter();
				}
			}
			// not in client -> log move / maintain capture if button down
			else
			{
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					mouse.onMouseMove(pt.x, pt.y);
				}
				// button up -> release capture / log event for leaving
				else
				{
					ReleaseCapture();
					mouse.onMouseLeave();
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			SetForegroundWindow(hWnd);
			if (!m_cursorEnabled)
			{
				confineCursor();
				hideCursor();
			}

			// stifle this mouse message if imgui wants to capture
			//if (imio.WantCaptureMouse)
			//	break;

			const POINTS pt = MAKEPOINTS(lParam);
			mouse.onLeftPressed(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			// stifle this mouse message if imgui wants to capture
			//if (imio.WantCaptureMouse)
			//	break;

			const POINTS pt = MAKEPOINTS(lParam);
			mouse.onRightPressed(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			// stifle this mouse message if imgui wants to capture
			//if (imio.WantCaptureMouse)
			//	break;

			const POINTS pt = MAKEPOINTS(lParam);
			mouse.onLeftReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
			{
				ReleaseCapture();
				mouse.onMouseLeave();
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			// stifle this mouse message if imgui wants to capture
			//if (imio.WantCaptureMouse)
			//	break;

			const POINTS pt = MAKEPOINTS(lParam);
			mouse.onRightReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
			{
				ReleaseCapture();
				mouse.onMouseLeave();
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			// stifle this mouse message if imgui wants to capture
			//if (imio.WantCaptureMouse)
			//	break;

			const POINTS pt = MAKEPOINTS(lParam);
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			mouse.onWheelDelta(pt.x, pt.y, delta);
			break;
		}
		/************** END MOUSE MESSAGES **************/

		/************** RAW MOUSE MESSAGES **************/
		case WM_INPUT:
		{
			if (!mouse.rawEnabled())
				break;

			UINT size;
			// first get the size of the input data
			if (GetRawInputData(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				nullptr,
				&size,
				sizeof(RAWINPUTHEADER)) == -1)
			{
				// bail msg processing if error
				break;
			}
			m_rawBuffer.resize(size);
			// read in the input data
			if (GetRawInputData(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				m_rawBuffer.data(),
				&size,
				sizeof(RAWINPUTHEADER)) != size)
			{
				// bail msg processing if error
				break;
			}
			// process the raw input data
			auto& ri = reinterpret_cast<const RAWINPUT&>(*m_rawBuffer.data());
			if (ri.header.dwType == RIM_TYPEMOUSE &&
				(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
			{
				mouse.onRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
			}
			break;
		}
		/************** END RAW MOUSE MESSAGES **************/
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void mvWindow::confineCursor()
	{
		RECT rect;
		GetClientRect(m_hWnd, &rect);
		MapWindowPoints(m_hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
		ClipCursor(&rect);
	}

	void mvWindow::enableCursor()
	{
		m_cursorEnabled = true;
		showCursor();
		freeCursor();
	}

	void mvWindow::disableCursor()
	{
		m_cursorEnabled = false;
		hideCursor();
		confineCursor();
	}

}