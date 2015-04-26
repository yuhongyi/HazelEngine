#include "InputManager.h"
InputManager::InputManager() :
	mIsMouseLeftButtonDown(false),
	mMousePosition(0.f, 0.f)
{
}

void InputManager::OnMouseMove(HWND hWnd, WORD mouseX, WORD mouseY, int comboState)
{
	mMousePosition.X = mouseX;
	mMousePosition.Y = mouseY;

	for(auto receiverIter = mInputReceivers.cbegin();
		receiverIter != mInputReceivers.cend();
		++receiverIter)
	{
		(*receiverIter)->OnMouseMove(hWnd, mouseX, mouseY, comboState);
	}
}

void InputManager::OnMouseLButtonDown(HWND hWnd, WORD mouseX, WORD mouseY, int comboState)
{
	mIsMouseLeftButtonDown = true;

	for(auto receiverIter = mInputReceivers.cbegin();
		receiverIter != mInputReceivers.cend();
		++receiverIter)
	{
		(*receiverIter)->OnMouseLButtonDown(hWnd, mouseX, mouseY, comboState);
	}
}

void InputManager::OnMouseLButtonUp(HWND hWnd, WORD mouseX, WORD mouseY, int comboState)
{
	mIsMouseLeftButtonDown = false;

	for(auto receiverIter = mInputReceivers.cbegin();
		receiverIter != mInputReceivers.cend();
		++receiverIter)
	{
		(*receiverIter)->OnMouseLButtonUp(hWnd, mouseX, mouseY, comboState);
	}
}

void InputManager::RegisterInputReceiver(IInputReceiver* inputReceiver)
{
	mInputReceivers.push_back(inputReceiver);
}