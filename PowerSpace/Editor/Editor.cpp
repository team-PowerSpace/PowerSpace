#include <Windows.h>
#include <iostream>
#include "stdafx.h"
#include "Editor.h"
#include "resource.h"

CEditor::CEditor()
{
	menu = 0;
	renderingWindow = CEditorRenderingWindow();
}

CEditor::~CEditor()
{
}

bool CEditor::RegisterClass()
{
	WNDCLASSEX windowClass;
	::ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = CEditor::windowProc;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.lpszClassName = L"CEditor";
	windowClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	return (::RegisterClassEx(&windowClass) != 0);
}

bool CEditor::Create()
{
	CreateWindowEx(0, L"CEditor", L"My Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), this);
	return (handle != 0);
}

void CEditor::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
	renderingWindow.Show();
}

void CEditor::OnDestroy()
{
	DestroyMenu(menu);
	PostQuitMessage(0);
}

void CEditor::OnNCCreate(HWND _handle) {
	handle = _handle;
}

void CEditor::OnCreate()
{
	menu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));
	renderingWindow.Create(L"CEditor");
}


void CEditor::OnSize()
{
	RECT rect;
	int middleX, nWidth, nHeight;
	::GetClientRect(handle, &rect);
	middleX = (rect.left + rect.right) / 2;
	nWidth = (rect.right - rect.left) / 2;
	nHeight = (rect.bottom - rect.top);
	SetWindowPos(renderingWindow.GetHandle(), HWND_TOP, middleX, rect.top, nWidth, nHeight, 0);
}

void CEditor::OnCommandMenu(WPARAM wParam)
{
	switch LOWORD(wParam)
	{
	case ID_ADD_RECTANGLE:
	{
		//add rectangle
		break;
	}
	case ID_ADD_ELLIPSE:
	{
		//add ellipse
		break;
	}
	case ID_PLAY_LAUNCHPLAYER:
	{
		//launch player
		break;
	}
	}
}

void CEditor::OnCommand(WPARAM wParam)
{
	OnCommandMenu(wParam);
}

LRESULT CEditor::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_NCCREATE:
	{
		CEditor* window = (CEditor*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetLastError(0);
		SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)window);
		if (GetLastError() != 0) {
			return GetLastError();
		}
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CREATE:
	{
		CEditor* window = (CEditor*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_SIZE:
	{
		CEditor* window = (CEditor*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnSize();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_COMMAND:
	{
		CEditor* window = (CEditor*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnCommand(wParam);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY:
	{
		CEditor* window = (CEditor*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnDestroy();
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}
