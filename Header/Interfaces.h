#pragma once


class IInputReceiver
{
public:
	virtual void OnMouseMove(HWND hWnd, WORD mouseX, WORD mouseY, int comboState) = 0;
	virtual void OnMouseLButtonDown(HWND hWnd, WORD mouseX, WORD mouseY, int comboState) = 0;
	virtual void OnMouseLButtonUp(HWND hWnd, WORD mouseX, WORD mouseY, int comboState) = 0;
};