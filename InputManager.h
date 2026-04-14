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

	// remember key state
	std::unordered_map<int, bool> m_currentKeyState;
	std::unordered_map<int, bool> m_previousKeyState;

public:
	// configurate any key for any action.
	void BindKey(const std::string& actionName, int keyCode)
	{
		m_keyBindings[actionName] = keyCode;
		m_currentKeyState[keyCode] = false;
		m_previousKeyState[keyCode] = false;
	}

	void Update() {

		// we copy the current states to the previous states as the frames go by
		for (auto const& [keyCode, isPressed] : m_currentKeyState) {
			m_previousKeyState[keyCode] = isPressed;

			// update the current state
			m_currentKeyState[keyCode] = (GetAsyncKeyState(keyCode) & 0x8000) != 0;
		}
	}

	// if (m_keyBindings.count(actionName)) is used to check if the input exists in binded keys

	bool IsActionPressed(const std::string& actionName) {
		if (m_keyBindings.count(actionName)) {
			return m_currentKeyState[m_keyBindings[actionName]];
		}
		return false;
	}

	bool IsActionJustPressed(const std::string& actionName) {
		if (m_keyBindings.count(actionName)) {
			int key = m_keyBindings[actionName];
			return m_currentKeyState[key] && !m_previousKeyState[key];
		}
		return false;
	}

	bool IsActionJustReleased(const std::string& actionName) {
		if (m_keyBindings.count(actionName)) {
			int key = m_keyBindings[actionName];
			return !m_currentKeyState[key] && m_previousKeyState[key];
		}
		return false;
	}
};