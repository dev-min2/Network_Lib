#pragma once
#include "std.h"
class Window
{
public:
	Window();
	virtual ~Window();
public:
	bool	InitWindow(HINSTANCE hInstance, _In_ int nCmdShow, int width, int height);
	bool	WindowRun();

protected:
	virtual bool GameInit() abstract;
	virtual bool GameRun()  abstract;
	virtual bool GameRelease() abstract;

	virtual LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
protected:
	bool		m_windowInit;
	HINSTANCE	m_hInst;
	HWND		m_hWindowHandle;
	bool		m_gameRun;
	RECT		m_clientRect;
};

