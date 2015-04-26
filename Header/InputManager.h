#pragma once
#include <vector>
using namespace std;


class InputManager : public Singleton<InputManager>, public IInputReceiver
{
public:
	// ctor & dtor
	InputManager();

	// IInputReceiver
	virtual void OnMouseMove(HWND hWnd, WORD mouseX, WORD mouseY, int comboState);
	virtual void OnMouseLButtonDown(HWND hWnd, WORD mouseX, WORD mouseY, int comboState);
	virtual void OnMouseLButtonUp(HWND hWnd, WORD mouseX, WORD mouseY, int comboState);

	// Methods
	void RegisterInputReceiver(IInputReceiver* inputReceiver);
	inline bool GetIsMouseLeftButtonDown() { return mIsMouseLeftButtonDown; }

private:
	bool mIsMouseLeftButtonDown;
	Vector2D mMousePosition;
	vector<IInputReceiver*> mInputReceivers;
};