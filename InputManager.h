#pragma once
#include <Windows.h>
#include <unordered_map>
#include <string>

// abstraction layer for the keyboard
// allows for the user to rebind keys easily, no hardcoded keybinds.

class InputManager
{
private:
	// dictionary of different keys binded to actions.
	std::unordered_map<std::string, int> m_keyBindings;

public:
	// configurate any key for any action.
	void BindKey(const std::string& actionName, int keyCode)
	{
		m_keyBindings[actionName] = keyCode;
	}

	// check if any action is happening.
	bool IsActionPressed(const std::string& actionName)
	{
		// check if it exists in our dictionary.
		if (m_keyBindings.count(actionName))
		{
			int physicalKey = m_keyBindings[actionName];
			return (GetAsyncKeyState(physicalKey) & 0x8000) != 0; // check the high-order bit to see if the key is currently held down.
		}
		return false;
	}
};