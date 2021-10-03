#ifndef _INPUT_H_
#define _INPUT_H_

#include "../Core/Maths/Vectors/Vec2/Vec2.h"
#include <CoreWindow.h>
#include "KeyCode.h"
#include <unordered_map>

namespace engine
{
	namespace input
	{
		class Input
		{
		public:
			Input();
			~Input();
			void Update();
			bool IsKeyDown(keys::EKeyCode code);
			bool IsKey(keys::EKeyCode code);
			void DisplayCursor(bool display);
			engine::core::maths::Vec2 GetMousePos(HWND window);
			void SetMousePos(HWND window, engine::core::maths::Vec2 pos);
		private:

			std::unordered_map<keys::EKeyCode, bool> m_inputs;
			std::unordered_map<keys::EKeyCode, bool> m_inputsDown;
			std::unordered_map<keys::EKeyCode, bool> m_inputsWasDown;
		};
	}
}

#endif // !_INPUT_H_
