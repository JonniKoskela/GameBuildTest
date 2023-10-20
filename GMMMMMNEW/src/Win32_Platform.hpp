#pragma once
#include <Windows.h>
#include "thirdParty/wglext.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "platform.h"
#include "input.h"
#include <assert.h>
#include <iostream>
#include "renderInterface.h"
// GLOBALS________________________________________________________________
static HWND window;
static HDC dc;
static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT_ptr;

#ifndef KEYLOOKUPTABLE
#define KEYLOOKUPTABLE
static KeyCodeID KeyCodeLookupTable[KEY_COUNT];
#endif

//UPDATE + CREATE WINDOW FUNCTIONS________________________________________
LRESULT CALLBACK window_callback_proc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (msg)
	{
	case WM_CLOSE:
	{
		running = false;
		break;
	}
	case WM_SIZE:
	{
		RECT rect = {};
		GetClientRect(window, &rect);
		input->screenSize.x = rect.right - rect.left;
		input->screenSize.y = rect.bottom - rect.top;
		break;
	}
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	{
		bool isDown = (msg == WM_KEYDOWN) || (msg == WM_SYSKEYDOWN) ||
			(msg == WM_LBUTTONDOWN);

		KeyCodeID keyCode = KeyCodeLookupTable[wParam];
		Key* key = &input->keys[keyCode];
		key->justPressed = !key->justPressed && !key->isDown && isDown;
		key->justReleased = !key->justReleased && key->isDown && !isDown;
		key->isDown = isDown;
		key->halfTransitionCount++;

		break;
	}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	{
		bool isDown = (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN);
		int mouseCode =
			(msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) ? VK_LBUTTON :
			(msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP) ? VK_MBUTTON : VK_RBUTTON;

		KeyCodeID keyCode = KeyCodeLookupTable[mouseCode];
		Key* key = &input->keys[keyCode];
		key->justPressed = !key->justPressed && !key->isDown && isDown;
		key->justReleased = !key->justReleased && key->isDown && !isDown;
		key->isDown = isDown;
		key->halfTransitionCount++;

		break;
	}
	default:
	{
		result = DefWindowProcA(window, msg, wParam, lParam);
	}
	}
	return result;
}






bool createWindow(int width, int height, char* title)
{

	HINSTANCE instance = GetModuleHandleA(0);

	WNDCLASSA wc = {};
	wc.hInstance = instance;
	wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = title;
	wc.lpfnWndProc = window_callback_proc;

	if (!RegisterClassA(&wc))
	{
		return false;
	}
	int dwStyle = WS_OVERLAPPEDWINDOW;
	
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;

	{
		window = CreateWindowExA(0, title, title, dwStyle, 100, 100, width, height, NULL, NULL, instance, NULL);

		if (window == NULL)
		{
			return false;
		}


		HDC fakeDC = GetDC(window);
		if (!fakeDC)
		{
			assert("cant load fakeDC");
			return false;
		}

		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cAlphaBits = 8;
		pfd.cDepthBits = 24;

		int pixelFormat = ChoosePixelFormat(fakeDC, &pfd);

		if (!SetPixelFormat(fakeDC, pixelFormat, &pfd))
		{
			std::cerr << "failed to set pixelformat" << "\n";
			return false;
		}

		HGLRC fakeRC = wglCreateContext(fakeDC);
		if (!fakeRC)
		{
			std::cerr << "failed to make current" << "\n";
			return false;
		}

		if (!wglMakeCurrent(fakeDC, fakeRC))
		{
			std::cerr << "failed to make current" << "\n";
			return false;
		}


		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)platform_load_gl_function(const_cast<char*>("wglChoosePixelFormatARB"));
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)platform_load_gl_function(const_cast<char*>("wglCreateContextAttribsARB"));
		wglSwapIntervalEXT_ptr = (PFNWGLSWAPINTERVALEXTPROC)platform_load_gl_function(const_cast < char*>("wglSwapIntervalEXT"));


		if (!wglCreateContextAttribsARB || !wglChoosePixelFormatARB)
		{
			assert(" Failed to load opengl functions");
			return false;
		}


		wglMakeCurrent(fakeDC, 0);
		wglDeleteContext(fakeRC);
		ReleaseDC(window, fakeDC);


		DestroyWindow(window);
	}





	{
		{
			RECT borderRect = {};
			AdjustWindowRectEx(&borderRect, dwStyle, 0, 0);

			width += borderRect.right - borderRect.left;
			height += borderRect.bottom - borderRect.top;
		}


		window = CreateWindowExA(0, title, title, dwStyle, 100, 100, width, height, NULL, NULL, instance, NULL);
		if (!window)
		{
			std::cerr << "failed to create actual window" << "\n";
			return false;
		}


		dc = GetDC(window);
		if (!dc)
		{
			assert("failed to get dc");
			return false;
		}


		const int pixelAttribs[] =
		{
			WGL_DRAW_TO_WINDOW_ARB,		GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,		GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,		GL_TRUE,
			WGL_SWAP_METHOD_ARB,		WGL_SWAP_COPY_ARB,
			WGL_PIXEL_TYPE_ARB,			WGL_TYPE_RGBA_ARB,
			WGL_ACCELERATION_ARB,		WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,			32,
			WGL_ALPHA_BITS_ARB,			8,
			WGL_DEPTH_BITS_ARB,			24,
			0
		};


		UINT numPixelFormats;
		int pixelFormat = 0;


		if (!wglChoosePixelFormatARB(dc, pixelAttribs,
			0,
			1,
			&pixelFormat,
			&numPixelFormats))
		{
			assert(0 && "failed to get wglChoosePixelFormatARB");
			return false;
		}

		PIXELFORMATDESCRIPTOR pfd = { 0 };
		DescribePixelFormat(dc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

		if (!SetPixelFormat(dc, pixelFormat, &pfd))
		{
			std::cerr << "faield to set pixelformat" << "\n";
			return false;
		}

		const int contextAttribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB,		4,
			WGL_CONTEXT_MINOR_VERSION_ARB,		3,
			WGL_CONTEXT_PROFILE_MASK_ARB,		WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			WGL_CONTEXT_FLAGS_ARB,				WGL_CONTEXT_DEBUG_BIT_ARB,
			0
		};

		HGLRC rc = wglCreateContextAttribsARB(dc, 0, contextAttribs);
		if (!rc)
		{
			assert("failed to create rendering context win32_platform.h line 205");
			return false;
		}

		if (!wglMakeCurrent(dc, rc))
		{
			assert("failed to make current win32_platform.h line 205");
			return false;
		}
	}


	ShowWindow(window, SW_SHOW);

	return true;
}








void updateWindow()
{
	MSG msg;
	while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	{
		POINT point = {};
		GetCursorPos(&point);
		ScreenToClient(window, &point);

		input->mousePos.x = point.x;
		input->mousePos.y = point.y;

		input->mousePosWorld = screenToWorld(input->mousePos);
	}
}


void* platform_load_gl_function(char* fnName)
{
	PROC proc = wglGetProcAddress(fnName);

	if (!proc)
	{
		static HMODULE openglDLL = LoadLibraryA("opengl32.dll");
		proc = GetProcAddress(openglDLL, fnName);

		if (!proc)
			return nullptr;
	}

	return (void*)proc;

}

void platform_swap_buffers()
{
	SwapBuffers(dc);
}
void platformSetVsync(bool vsync)
{
	wglSwapIntervalEXT_ptr(vsync);
}



void* loadDynamicLibrary(char* dll)
{
	HMODULE result = LoadLibraryA(dll);
	assert(result && "failed to load dll: %d", dll);
	return result;
}

void* loadDynamicFunction(void* dll, char* fn)
{
	FARPROC proc = GetProcAddress((HMODULE)dll, fn);
	assert(proc && "failed to load function: %d", fn);

	return (void*)proc;

}
bool freeDLL(void* dll)
{
	BOOL freeResult = FreeLibrary((HMODULE)dll);
	assert(freeResult && "failed to free DLL");
	return (bool)freeResult;
}


void platform_fill_keycode_lookup_table()
{
	KeyCodeLookupTable[VK_LBUTTON] = KEY_MOUSE_LEFT;
	KeyCodeLookupTable[VK_MBUTTON] = KEY_MOUSE_MIDDLE;
	KeyCodeLookupTable[VK_RBUTTON] = KEY_MOUSE_RIGHT;

	KeyCodeLookupTable['A'] = KEY_A;
	KeyCodeLookupTable['B'] = KEY_B;
	KeyCodeLookupTable['C'] = KEY_C;
	KeyCodeLookupTable['D'] = KEY_D;
	KeyCodeLookupTable['E'] = KEY_E;
	KeyCodeLookupTable['F'] = KEY_F;
	KeyCodeLookupTable['G'] = KEY_G;
	KeyCodeLookupTable['H'] = KEY_H;
	KeyCodeLookupTable['I'] = KEY_I;
	KeyCodeLookupTable['J'] = KEY_J;
	KeyCodeLookupTable['K'] = KEY_K;
	KeyCodeLookupTable['L'] = KEY_L;
	KeyCodeLookupTable['M'] = KEY_M;
	KeyCodeLookupTable['N'] = KEY_N;
	KeyCodeLookupTable['O'] = KEY_O;
	KeyCodeLookupTable['P'] = KEY_P;
	KeyCodeLookupTable['Q'] = KEY_Q;
	KeyCodeLookupTable['R'] = KEY_R;
	KeyCodeLookupTable['S'] = KEY_S;
	KeyCodeLookupTable['T'] = KEY_T;
	KeyCodeLookupTable['U'] = KEY_U;
	KeyCodeLookupTable['V'] = KEY_V;
	KeyCodeLookupTable['W'] = KEY_W;
	KeyCodeLookupTable['X'] = KEY_X;
	KeyCodeLookupTable['Y'] = KEY_Y;
	KeyCodeLookupTable['Z'] = KEY_Z;
	KeyCodeLookupTable['0'] = KEY_0;
	KeyCodeLookupTable['1'] = KEY_1;
	KeyCodeLookupTable['2'] = KEY_2;
	KeyCodeLookupTable['3'] = KEY_3;
	KeyCodeLookupTable['4'] = KEY_4;
	KeyCodeLookupTable['5'] = KEY_5;
	KeyCodeLookupTable['6'] = KEY_6;
	KeyCodeLookupTable['7'] = KEY_7;
	KeyCodeLookupTable['8'] = KEY_8;
	KeyCodeLookupTable['9'] = KEY_9;

	KeyCodeLookupTable[VK_SPACE] = KEY_SPACE,
		KeyCodeLookupTable[VK_OEM_3] = KEY_TICK,
		KeyCodeLookupTable[VK_OEM_MINUS] = KEY_MINUS,

		KeyCodeLookupTable[VK_OEM_PLUS] = KEY_EQUAL,
		KeyCodeLookupTable[VK_OEM_4] = KEY_LEFT_BRACKET,
		KeyCodeLookupTable[VK_OEM_6] = KEY_RIGHT_BRACKET,
		KeyCodeLookupTable[VK_OEM_1] = KEY_SEMICOLON,
		KeyCodeLookupTable[VK_OEM_7] = KEY_QUOTE,
		KeyCodeLookupTable[VK_OEM_COMMA] = KEY_COMMA,
		KeyCodeLookupTable[VK_OEM_PERIOD] = KEY_PERIOD,
		KeyCodeLookupTable[VK_OEM_2] = KEY_FORWARD_SLASH,
		KeyCodeLookupTable[VK_OEM_5] = KEY_BACKWARD_SLASH,
		KeyCodeLookupTable[VK_TAB] = KEY_TAB,
		KeyCodeLookupTable[VK_ESCAPE] = KEY_ESCAPE,
		KeyCodeLookupTable[VK_PAUSE] = KEY_PAUSE,
		KeyCodeLookupTable[VK_UP] = KEY_UP,
		KeyCodeLookupTable[VK_DOWN] = KEY_DOWN,
		KeyCodeLookupTable[VK_LEFT] = KEY_LEFT,
		KeyCodeLookupTable[VK_RIGHT] = KEY_RIGHT,
		KeyCodeLookupTable[VK_BACK] = KEY_BACKSPACE,
		KeyCodeLookupTable[VK_RETURN] = KEY_RETURN,
		KeyCodeLookupTable[VK_DELETE] = KEY_DELETE,
		KeyCodeLookupTable[VK_INSERT] = KEY_INSERT,
		KeyCodeLookupTable[VK_HOME] = KEY_HOME,
		KeyCodeLookupTable[VK_END] = KEY_END,
		KeyCodeLookupTable[VK_PRIOR] = KEY_PAGE_UP,
		KeyCodeLookupTable[VK_NEXT] = KEY_PAGE_DOWN,
		KeyCodeLookupTable[VK_CAPITAL] = KEY_CAPS_LOCK,
		KeyCodeLookupTable[VK_NUMLOCK] = KEY_NUM_LOCK,
		KeyCodeLookupTable[VK_SCROLL] = KEY_SCROLL_LOCK,
		KeyCodeLookupTable[VK_APPS] = KEY_MENU,

		KeyCodeLookupTable[VK_SHIFT] = KEY_SHIFT,
		KeyCodeLookupTable[VK_LSHIFT] = KEY_SHIFT,
		KeyCodeLookupTable[VK_RSHIFT] = KEY_SHIFT,

		KeyCodeLookupTable[VK_CONTROL] = KEY_CONTROL,
		KeyCodeLookupTable[VK_LCONTROL] = KEY_CONTROL,
		KeyCodeLookupTable[VK_RCONTROL] = KEY_CONTROL,

		KeyCodeLookupTable[VK_MENU] = KEY_ALT,
		KeyCodeLookupTable[VK_LMENU] = KEY_ALT,
		KeyCodeLookupTable[VK_RMENU] = KEY_ALT,

		KeyCodeLookupTable[VK_F1] = KEY_F1;
	KeyCodeLookupTable[VK_F2] = KEY_F2;
	KeyCodeLookupTable[VK_F3] = KEY_F3;
	KeyCodeLookupTable[VK_F4] = KEY_F4;
	KeyCodeLookupTable[VK_F5] = KEY_F5;
	KeyCodeLookupTable[VK_F6] = KEY_F6;
	KeyCodeLookupTable[VK_F7] = KEY_F7;
	KeyCodeLookupTable[VK_F8] = KEY_F8;
	KeyCodeLookupTable[VK_F9] = KEY_F9;
	KeyCodeLookupTable[VK_F10] = KEY_F10;
	KeyCodeLookupTable[VK_F11] = KEY_F11;
	KeyCodeLookupTable[VK_F12] = KEY_F12;

	KeyCodeLookupTable[VK_NUMPAD0] = KEY_NUMPAD_0;
	KeyCodeLookupTable[VK_NUMPAD1] = KEY_NUMPAD_1;
	KeyCodeLookupTable[VK_NUMPAD2] = KEY_NUMPAD_2;
	KeyCodeLookupTable[VK_NUMPAD3] = KEY_NUMPAD_3;
	KeyCodeLookupTable[VK_NUMPAD4] = KEY_NUMPAD_4;
	KeyCodeLookupTable[VK_NUMPAD5] = KEY_NUMPAD_5;
	KeyCodeLookupTable[VK_NUMPAD6] = KEY_NUMPAD_6;
	KeyCodeLookupTable[VK_NUMPAD7] = KEY_NUMPAD_7;
	KeyCodeLookupTable[VK_NUMPAD8] = KEY_NUMPAD_8;
	KeyCodeLookupTable[VK_NUMPAD9] = KEY_NUMPAD_9;
}