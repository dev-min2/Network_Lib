#pragma once
#include "Window.h"
class Chat final : public Window 
{
public:
	Chat() = default;
	~Chat() override;

public:
	bool			GameInit() override;
	bool			GameRun()  override;
	bool			GameRelease() override;
public:
	LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
private:
	bool			Init();
	bool			Update();
	bool			Render();
	bool			Release();
private:
	HWND			m_hEdit;
	HWND			m_hButton;
	HWND			m_hListBox;
	std::wstring	m_name;

};

