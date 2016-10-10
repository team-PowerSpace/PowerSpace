#pragma once
#include <Windows.h>

class CEllipseWindow {
public:
	CEllipseWindow();
	~CEllipseWindow();

	// ���������������� ����� ����
	static bool RegisterClass();

	// ������� ��������� ���� ������ ������������� � ������� hWndParent � ������ ����� ������ ��������
	bool Create(HWND hWndParent);
	// �������� ����
	void Show(int cmdShow);
	// �������� ����� ����
	HWND getHandle();

protected:
	// �������, ���������� ��� �������� ��������� ���� (������ �������� WM_NCCREATE)
	void OnNCCreate(HWND handle);
	// �������, ���������� ��� �������� ���� (������ �������� WM_CREATE)
	void OnCreate();
	// �������, ���������� ��� ������� � ���� ��������� WM_PAINT
	void OnPaint();
	// �������, ���������� ��� ������� ��������� WM_TIMER
	void OnTimer();
	// �������, ���������� ��� ����������� ���� (������ ��������� WM_DESTROY)
	void OnDestroy();
	// �������, ���������� ��� ������� �� ���� ����� ������� ���� (������ ��������� WM_LBUTTONDOWN)
	void OnLButtonDown();

private:
	HWND handle; // ����� ����
	UINT_PTR timerId; // ������������� ������� ��������� �� ���������� ��������
	double time; // �������� �������. ������������� ��������� �������
	static const double deltaTime; // ��������� ������� ��� ������ ������� WM_TIMER
	static const int r; // ������ �������

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void drawEllipse(HDC targetDC, int x, int y);
};