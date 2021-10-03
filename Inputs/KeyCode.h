#ifndef _KEY_CODE_H_
#define _KEY_CODE_H_

namespace engine
{
	namespace input
	{
		namespace keys
		{
			enum EKeyCode : int
			{
				INPUT_BUTTON_MOUSE_LEFT = 0,
				INPUT_BUTTON_MOUSE_RIGHT = 1,
				INPUT_BUTTON_MOUSE_MIDDLE = 2,
				INPUT_KEY_CANCEL = 3,
				INPUT_KEY_BACKSPACE = 4,
				INPUT_KEY_TAB = 5,
				INPUT_KEY_CLEAR = 6,
				INPUT_KEY_ENTER = 7,
				INPUT_KEY_SHIFT = 8,
				INPUT_KEY_CONTROL = 9,
				INPUT_KEY_ALT = 10,
				INPUT_KEY_PAUSE = 11,
				INPUT_KEY_CAPSLOCK = 12,
				INPUT_KEY_ESCAPE = 13,
				INPUT_KEY_SPACE = 14,
				INPUT_KEY_PAGE_UP = 15,
				INPUT_KEY_PAGE_DOWN = 16,
				INPUT_KEY_END = 17,
				INPUT_KEY_HOME = 18,
				INPUT_KEY_LEFT_ARROW = 19,
				INPUT_KEY_UP_ARROW = 20,
				INPUT_KEY_RIGHT_ARROW = 21,
				INPUT_KEY_DOWN_ARROW = 22,
				INPUT_KEY_SELECT = 23,
				INPUT_KEY_PRINT = 24,
				INPUT_KEY_EXECUTE = 25,
				INPUT_KEY_PRINT_SCREEN = 26,
				INPUT_KEY_INSERT = 27,
				INPUT_KEY_DELETE = 28,
				INPUT_KEY_HELP = 29,
				INPUT_KEY_NUM_0 = 30,
				INPUT_KEY_NUM_1 = 31,
				INPUT_KEY_NUM_2 = 32,
				INPUT_KEY_NUM_3 = 33,
				INPUT_KEY_NUM_4 = 34,
				INPUT_KEY_NUM_5 = 35,
				INPUT_KEY_NUM_6 = 36,
				INPUT_KEY_NUM_7 = 37,
				INPUT_KEY_NUM_8 = 38,
				INPUT_KEY_NUM_9 = 39,
				INPUT_KEY_A = 40,
				INPUT_KEY_B = 41,
				INPUT_KEY_C = 42,
				INPUT_KEY_D = 43,
				INPUT_KEY_E = 44,
				INPUT_KEY_F = 45,
				INPUT_KEY_G = 46,
				INPUT_KEY_H = 47,
				INPUT_KEY_I = 48,
				INPUT_KEY_J = 49,
				INPUT_KEY_K = 50,
				INPUT_KEY_L = 51,
				INPUT_KEY_M = 52,
				INPUT_KEY_N = 53,
				INPUT_KEY_O = 54,
				INPUT_KEY_P = 55,
				INPUT_KEY_Q = 56,
				INPUT_KEY_R = 57,
				INPUT_KEY_S = 58,
				INPUT_KEY_T = 59,
				INPUT_KEY_U = 60,
				INPUT_KEY_V = 61,
				INPUT_KEY_W = 62,
				INPUT_KEY_X = 63,
				INPUT_KEY_Y = 64,
				INPUT_KEY_Z = 65,
				INPUT_KEY_NUM_PAD_0 = 66,
				INPUT_KEY_NUM_PAD_1 = 67,
				INPUT_KEY_NUM_PAD_2 = 68,
				INPUT_KEY_NUM_PAD_3 = 69,
				INPUT_KEY_NUM_PAD_4 = 70,
				INPUT_KEY_NUM_PAD_5 = 71,
				INPUT_KEY_NUM_PAD_6 = 72,
				INPUT_KEY_NUM_PAD_7 = 73,
				INPUT_KEY_NUM_PAD_8 = 74,
				INPUT_KEY_NUM_PAD_9 = 75,
				INPUT_KEY_MULTIPLY = 76,
				INPUT_KEY_ADD = 77,
				INPUT_KEY_SEPARATOR = 78,
				INPUT_KEY_SUBSTRACT = 79,
				INPUT_KEY_DECIMAL = 80,
				INPUT_KEY_DEVIDE = 81,
				INPUT_KEY_F1 = 82,
				INPUT_KEY_F2 = 83,
				INPUT_KEY_F3 = 84,
				INPUT_KEY_F4 = 85,
				INPUT_KEY_F5 = 86,
				INPUT_KEY_F6 = 87,
				INPUT_KEY_F7 = 88,
				INPUT_KEY_F8 = 89,
				INPUT_KEY_F9 = 90,
				INPUT_KEY_F10 = 91,
				INPUT_KEY_F11 = 92,
				INPUT_KEY_F12 = 93,
				INPUT_KEY_NUM_LOCK = 94,
				INPUT_KEY_SCROLL_LOCK = 95,
				INPUT_KEY_LEFT_SHIFT = 96,
				INPUT_KEY_RIGHT_SHIFT = 97,
				INPUT_KEY_LEFT_CONTROL = 98,
				INPUT_KEY_RIGHT_CONTROL = 99,
				INPUT_KEY_LEFT_MENU = 100,
				INPUT_KEY_RIGHT_MENU = 101,
				COUNT = 102
			};

			struct KeyCodeHelper
			{
				static const EKeyCode keyCodes[];
				static int KeyCodeToBinary(EKeyCode code)
				{
					switch (code)
					{
					case EKeyCode::INPUT_BUTTON_MOUSE_LEFT:  return 0x01;
					case EKeyCode::INPUT_BUTTON_MOUSE_RIGHT: return 0x02;
					case EKeyCode::INPUT_BUTTON_MOUSE_MIDDLE: return 0x04;
					case EKeyCode::INPUT_KEY_CANCEL: return 0x03;
					case EKeyCode::INPUT_KEY_BACKSPACE: return 0x08;
					case EKeyCode::INPUT_KEY_TAB: return 0x09;
					case EKeyCode::INPUT_KEY_CLEAR: return 0x0C;
					case EKeyCode::INPUT_KEY_ENTER: return 0x0D;
					case EKeyCode::INPUT_KEY_SHIFT: return 0x10;
					case EKeyCode::INPUT_KEY_CONTROL: return 0x11;
					case EKeyCode::INPUT_KEY_ALT: return 0x12;
					case EKeyCode::INPUT_KEY_PAUSE: return 0x13;
					case EKeyCode::INPUT_KEY_CAPSLOCK: return 0x14;
					case EKeyCode::INPUT_KEY_ESCAPE: return 0x1B;
					case EKeyCode::INPUT_KEY_SPACE: return 0x20;
					case EKeyCode::INPUT_KEY_PAGE_UP: return 0x21;
					case EKeyCode::INPUT_KEY_PAGE_DOWN: return 0x22;
					case EKeyCode::INPUT_KEY_END: return 0x23;
					case EKeyCode::INPUT_KEY_HOME: return 0x24;
					case EKeyCode::INPUT_KEY_LEFT_ARROW: return 0x25;
					case EKeyCode::INPUT_KEY_UP_ARROW: return 0x26;
					case EKeyCode::INPUT_KEY_RIGHT_ARROW: return 0x27;
					case EKeyCode::INPUT_KEY_DOWN_ARROW: return 0x28;
					case EKeyCode::INPUT_KEY_SELECT: return 0x29;
					case EKeyCode::INPUT_KEY_PRINT: return 0x2A;
					case EKeyCode::INPUT_KEY_EXECUTE: return 0x2B;
					case EKeyCode::INPUT_KEY_PRINT_SCREEN: return 0x2C;
					case EKeyCode::INPUT_KEY_INSERT: return 0x2D;
					case EKeyCode::INPUT_KEY_DELETE: return 0x2E;
					case EKeyCode::INPUT_KEY_HELP: return 0x2F;
					case EKeyCode::INPUT_KEY_NUM_0: return 0x30;
					case EKeyCode::INPUT_KEY_NUM_1: return 0x31;
					case EKeyCode::INPUT_KEY_NUM_2: return 0x32;
					case EKeyCode::INPUT_KEY_NUM_3: return 0x33;
					case EKeyCode::INPUT_KEY_NUM_4: return 0x34;
					case EKeyCode::INPUT_KEY_NUM_5: return 0x35;
					case EKeyCode::INPUT_KEY_NUM_6: return 0x36;
					case EKeyCode::INPUT_KEY_NUM_7: return 0x37;
					case EKeyCode::INPUT_KEY_NUM_8: return 0x38;
					case EKeyCode::INPUT_KEY_NUM_9: return 0x39;
					case EKeyCode::INPUT_KEY_A: return 0x41;
					case EKeyCode::INPUT_KEY_B: return 0x42;
					case EKeyCode::INPUT_KEY_C: return 0x43;
					case EKeyCode::INPUT_KEY_D: return 0x44;
					case EKeyCode::INPUT_KEY_E: return 0x45;
					case EKeyCode::INPUT_KEY_F: return 0x46;
					case EKeyCode::INPUT_KEY_G: return 0x47;
					case EKeyCode::INPUT_KEY_H: return 0x48;
					case EKeyCode::INPUT_KEY_I: return 0x49;
					case EKeyCode::INPUT_KEY_J: return 0x4A;
					case EKeyCode::INPUT_KEY_K: return 0x4B;
					case EKeyCode::INPUT_KEY_L: return 0x4C;
					case EKeyCode::INPUT_KEY_M: return 0x4D;
					case EKeyCode::INPUT_KEY_N: return 0x4E;
					case EKeyCode::INPUT_KEY_O: return 0x4F;
					case EKeyCode::INPUT_KEY_P: return 0x50;
					case EKeyCode::INPUT_KEY_Q: return 0x51;
					case EKeyCode::INPUT_KEY_R: return 0x52;
					case EKeyCode::INPUT_KEY_S: return 0x53;
					case EKeyCode::INPUT_KEY_T: return 0x54;
					case EKeyCode::INPUT_KEY_U: return 0x55;
					case EKeyCode::INPUT_KEY_V: return 0x56;
					case EKeyCode::INPUT_KEY_W: return 0x57;
					case EKeyCode::INPUT_KEY_X: return 0x58;
					case EKeyCode::INPUT_KEY_Y: return 0x59;
					case EKeyCode::INPUT_KEY_Z: return 0x5A;
					case EKeyCode::INPUT_KEY_NUM_PAD_0: return 0x60;
					case EKeyCode::INPUT_KEY_NUM_PAD_1: return 0x61;
					case EKeyCode::INPUT_KEY_NUM_PAD_2: return 0x62;
					case EKeyCode::INPUT_KEY_NUM_PAD_3: return 0x63;
					case EKeyCode::INPUT_KEY_NUM_PAD_4: return 0x64;
					case EKeyCode::INPUT_KEY_NUM_PAD_5: return 0x65;
					case EKeyCode::INPUT_KEY_NUM_PAD_6: return 0x66;
					case EKeyCode::INPUT_KEY_NUM_PAD_7: return 0x67;
					case EKeyCode::INPUT_KEY_NUM_PAD_8: return 0x68;
					case EKeyCode::INPUT_KEY_NUM_PAD_9: return 0x69;
					case EKeyCode::INPUT_KEY_MULTIPLY: return 0x6A;
					case EKeyCode::INPUT_KEY_ADD: return 0x6B;
					case EKeyCode::INPUT_KEY_SEPARATOR: return 0x6C;
					case EKeyCode::INPUT_KEY_SUBSTRACT: return 0x6D;
					case EKeyCode::INPUT_KEY_DECIMAL: return 0x6E;
					case EKeyCode::INPUT_KEY_DEVIDE: return 0x6F;
					case EKeyCode::INPUT_KEY_F1: return 0x70;
					case EKeyCode::INPUT_KEY_F2: return 0x71;
					case EKeyCode::INPUT_KEY_F3: return 0x72;
					case EKeyCode::INPUT_KEY_F4: return 0x73;
					case EKeyCode::INPUT_KEY_F5: return 0x74;
					case EKeyCode::INPUT_KEY_F6: return 0x75;
					case EKeyCode::INPUT_KEY_F7: return 0x76;
					case EKeyCode::INPUT_KEY_F8: return 0x77;
					case EKeyCode::INPUT_KEY_F9: return 0x78;
					case EKeyCode::INPUT_KEY_F10: return 0x79;
					case EKeyCode::INPUT_KEY_F11: return 0x7A;
					case EKeyCode::INPUT_KEY_F12: return 0x7B;
					case EKeyCode::INPUT_KEY_NUM_LOCK: return 0x90;
					case EKeyCode::INPUT_KEY_SCROLL_LOCK: return 0x91;
					case EKeyCode::INPUT_KEY_LEFT_SHIFT: return 0xA0;
					case EKeyCode::INPUT_KEY_RIGHT_SHIFT: return 0xA1;
					case EKeyCode::INPUT_KEY_LEFT_CONTROL: return 0xA2;
					case EKeyCode::INPUT_KEY_RIGHT_CONTROL: return 0xA3;
					case EKeyCode::INPUT_KEY_LEFT_MENU: return 0xA4;
					case EKeyCode::INPUT_KEY_RIGHT_MENU: return 0xA5;
					default: return -1;

					}
				}
			};
		}
	}
}

#endif // !_KEY_CODE_H_

