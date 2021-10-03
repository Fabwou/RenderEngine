#include "Input.h"

 const engine::input::keys::EKeyCode engine::input::keys::KeyCodeHelper::keyCodes[] = 
 { INPUT_BUTTON_MOUSE_LEFT, INPUT_BUTTON_MOUSE_RIGHT, INPUT_BUTTON_MOUSE_MIDDLE, INPUT_KEY_CANCEL,
INPUT_KEY_BACKSPACE, INPUT_KEY_TAB, INPUT_KEY_CLEAR, INPUT_KEY_ENTER, INPUT_KEY_SHIFT, INPUT_KEY_CONTROL, INPUT_KEY_ALT,
INPUT_KEY_PAUSE, INPUT_KEY_CAPSLOCK, INPUT_KEY_ESCAPE, INPUT_KEY_SPACE, INPUT_KEY_PAGE_UP, INPUT_KEY_PAGE_DOWN, INPUT_KEY_END,
INPUT_KEY_HOME, INPUT_KEY_LEFT_ARROW, INPUT_KEY_UP_ARROW, INPUT_KEY_RIGHT_ARROW, INPUT_KEY_DOWN_ARROW, INPUT_KEY_SELECT, INPUT_KEY_PRINT,
INPUT_KEY_EXECUTE, INPUT_KEY_PRINT_SCREEN, INPUT_KEY_INSERT, INPUT_KEY_DELETE, INPUT_KEY_HELP, INPUT_KEY_NUM_0, INPUT_KEY_NUM_1,
INPUT_KEY_NUM_2, INPUT_KEY_NUM_3, INPUT_KEY_NUM_4, INPUT_KEY_NUM_5, INPUT_KEY_NUM_6, INPUT_KEY_NUM_7, INPUT_KEY_NUM_8, INPUT_KEY_NUM_9,
INPUT_KEY_A, INPUT_KEY_B, INPUT_KEY_C, INPUT_KEY_D, INPUT_KEY_E, INPUT_KEY_F, INPUT_KEY_G, INPUT_KEY_H, INPUT_KEY_I, INPUT_KEY_J,
INPUT_KEY_K, INPUT_KEY_L, INPUT_KEY_M, INPUT_KEY_N, INPUT_KEY_O, INPUT_KEY_P, INPUT_KEY_Q, INPUT_KEY_R, INPUT_KEY_S, INPUT_KEY_T,
INPUT_KEY_U, INPUT_KEY_V, INPUT_KEY_W, INPUT_KEY_X, INPUT_KEY_Y, INPUT_KEY_Z, INPUT_KEY_NUM_PAD_0, INPUT_KEY_NUM_PAD_1, INPUT_KEY_NUM_PAD_2,
INPUT_KEY_NUM_PAD_3, INPUT_KEY_NUM_PAD_4, INPUT_KEY_NUM_PAD_5, INPUT_KEY_NUM_PAD_6, INPUT_KEY_NUM_PAD_7, INPUT_KEY_NUM_PAD_8,
INPUT_KEY_NUM_PAD_9, INPUT_KEY_MULTIPLY, INPUT_KEY_ADD, INPUT_KEY_SEPARATOR, INPUT_KEY_SUBSTRACT, INPUT_KEY_DECIMAL, INPUT_KEY_DEVIDE,
INPUT_KEY_F1, INPUT_KEY_F2, INPUT_KEY_F3, INPUT_KEY_F4, INPUT_KEY_F5, INPUT_KEY_F6, INPUT_KEY_F7, INPUT_KEY_F8, INPUT_KEY_F9,
INPUT_KEY_F10, INPUT_KEY_F11, INPUT_KEY_F12, INPUT_KEY_NUM_LOCK, INPUT_KEY_SCROLL_LOCK, INPUT_KEY_LEFT_SHIFT, INPUT_KEY_RIGHT_SHIFT,
INPUT_KEY_LEFT_CONTROL, INPUT_KEY_RIGHT_CONTROL, INPUT_KEY_LEFT_MENU, INPUT_KEY_RIGHT_MENU };


 engine::input::Input::Input()
{
	for (keys::EKeyCode keyCode : keys::KeyCodeHelper::keyCodes)
	{
		m_inputs.emplace(keyCode, false);
		m_inputsDown.emplace(keyCode, false);
		m_inputsWasDown.emplace(keyCode, false);
	}
}

 engine::input::Input::~Input()
{
	m_inputs.clear();
	m_inputsDown.clear();
	m_inputsWasDown.clear();
}

void engine::input::Input::Update() // TODO: Optimization -> avoid calling useless inputs
{
	for (std::unordered_map<keys::EKeyCode, bool>::iterator it{ m_inputs.begin() }; it != m_inputs.end(); ++it)
	{
		bool result{ static_cast<bool>(GetAsyncKeyState(keys::KeyCodeHelper::KeyCodeToBinary(it->first))) };
		if (result)
		{
			it->second = true;
		}
		else
		{
			it->second = false;
			m_inputsWasDown.find(it->first)->second = false;
		}
	}
	for (std::unordered_map<keys::EKeyCode, bool>::iterator it{ m_inputsDown.begin() }; it != m_inputsDown.end(); ++it)
	{
		if (it->second == true)
		{
			it->second = false;
			m_inputsWasDown.find(it->first)->second = true;
		}
		else if(m_inputsWasDown.find(it->first)->second == false)
		{
			it->second = m_inputs.find(it->first)->second;
		}
		else
		{
			it->second = false;
		}
	}
}

bool engine::input::Input::IsKeyDown(keys::EKeyCode keyCode)
{
	std::unordered_map<keys::EKeyCode, bool>::const_iterator input = m_inputs.find(keyCode);
	if (input == m_inputs.end())
	{
		bool result{ static_cast<bool>(GetAsyncKeyState(keys::KeyCodeHelper::KeyCodeToBinary(keyCode))) };
		m_inputs.emplace(keyCode, result);
		m_inputsDown.emplace(keyCode, result);
		m_inputsWasDown.emplace(keyCode, false);
		return result;
	}
	return m_inputsDown.at(keyCode);
}

bool engine::input::Input::IsKey(keys::EKeyCode keyCode)
{
	std::unordered_map<keys::EKeyCode, bool>::const_iterator input = m_inputs.find(keyCode);
	if (input == m_inputs.end())
	{
		bool result{ static_cast<bool>(GetAsyncKeyState(keys::KeyCodeHelper::KeyCodeToBinary(keyCode))) };
		m_inputs.emplace(keyCode, result);
		m_inputsDown.emplace(keyCode, result);
		m_inputsWasDown.emplace(keyCode, false);
		return result;
	}
	return m_inputs.at(keyCode);
}

void engine::input::Input::DisplayCursor(bool display)
{
	ShowCursor(display);
}

engine::core::maths::Vec2 engine::input::Input::GetMousePos(HWND window)
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(window, &point);
	engine::core::maths::Vec2 pos{static_cast<float>(point.x), static_cast<float>(point.y) };
	return pos;
}

void engine::input::Input::SetMousePos(HWND window, engine::core::maths::Vec2 pos)
{
	POINT point{(LONG)pos.m_x, (LONG)pos.m_y};
	ClientToScreen(window, &point);
	SetCursorPos(point.x, point.y);
}
