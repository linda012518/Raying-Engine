#pragma once

namespace Raying
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define RAYING_KEY_SPACE           ::Raying::Key::Space
#define RAYING_KEY_APOSTROPHE      ::Raying::Key::Apostrophe    /* ' */
#define RAYING_KEY_COMMA           ::Raying::Key::Comma         /* , */
#define RAYING_KEY_MINUS           ::Raying::Key::Minus         /* - */
#define RAYING_KEY_PERIOD          ::Raying::Key::Period        /* . */
#define RAYING_KEY_SLASH           ::Raying::Key::Slash         /* / */
#define RAYING_KEY_0               ::Raying::Key::D0
#define RAYING_KEY_1               ::Raying::Key::D1
#define RAYING_KEY_2               ::Raying::Key::D2
#define RAYING_KEY_3               ::Raying::Key::D3
#define RAYING_KEY_4               ::Raying::Key::D4
#define RAYING_KEY_5               ::Raying::Key::D5
#define RAYING_KEY_6               ::Raying::Key::D6
#define RAYING_KEY_7               ::Raying::Key::D7
#define RAYING_KEY_8               ::Raying::Key::D8
#define RAYING_KEY_9               ::Raying::Key::D9
#define RAYING_KEY_SEMICOLON       ::Raying::Key::Semicolon     /* ; */
#define RAYING_KEY_EQUAL           ::Raying::Key::Equal         /* = */
#define RAYING_KEY_A               ::Raying::Key::A
#define RAYING_KEY_B               ::Raying::Key::B
#define RAYING_KEY_C               ::Raying::Key::C
#define RAYING_KEY_D               ::Raying::Key::D
#define RAYING_KEY_E               ::Raying::Key::E
#define RAYING_KEY_F               ::Raying::Key::F
#define RAYING_KEY_G               ::Raying::Key::G
#define RAYING_KEY_H               ::Raying::Key::H
#define RAYING_KEY_I               ::Raying::Key::I
#define RAYING_KEY_J               ::Raying::Key::J
#define RAYING_KEY_K               ::Raying::Key::K
#define RAYING_KEY_L               ::Raying::Key::L
#define RAYING_KEY_M               ::Raying::Key::M
#define RAYING_KEY_N               ::Raying::Key::N
#define RAYING_KEY_O               ::Raying::Key::O
#define RAYING_KEY_P               ::Raying::Key::P
#define RAYING_KEY_Q               ::Raying::Key::Q
#define RAYING_KEY_R               ::Raying::Key::R
#define RAYING_KEY_S               ::Raying::Key::S
#define RAYING_KEY_T               ::Raying::Key::T
#define RAYING_KEY_U               ::Raying::Key::U
#define RAYING_KEY_V               ::Raying::Key::V
#define RAYING_KEY_W               ::Raying::Key::W
#define RAYING_KEY_X               ::Raying::Key::X
#define RAYING_KEY_Y               ::Raying::Key::Y
#define RAYING_KEY_Z               ::Raying::Key::Z
#define RAYING_KEY_LEFT_BRACKET    ::Raying::Key::LeftBracket   /* [ */
#define RAYING_KEY_BACKSLASH       ::Raying::Key::Backslash     /* \ */
#define RAYING_KEY_RIGHT_BRACKET   ::Raying::Key::RightBracket  /* ] */
#define RAYING_KEY_GRAVE_ACCENT    ::Raying::Key::GraveAccent   /* ` */
#define RAYING_KEY_WORLD_1         ::Raying::Key::World1        /* non-US #1 */
#define RAYING_KEY_WORLD_2         ::Raying::Key::World2        /* non-US #2 */

/* Function keys */
#define RAYINGZ_KEY_ESCAPE          ::Raying::Key::Escape
#define RAYINGZ_KEY_ENTER           ::Raying::Key::Enter
#define RAYINGZ_KEY_TAB             ::Raying::Key::Tab
#define RAYINGZ_KEY_BACKSPACE       ::Raying::Key::Backspace
#define RAYINGZ_KEY_INSERT          ::Raying::Key::Insert
#define RAYINGZ_KEY_DELETE          ::Raying::Key::Delete
#define RAYINGZ_KEY_RIGHT           ::Raying::Key::Right
#define RAYINGZ_KEY_LEFT            ::Raying::Key::Left
#define RAYINGZ_KEY_DOWN            ::Raying::Key::Down
#define RAYINGZ_KEY_UP              ::Raying::Key::Up
#define RAYINGZ_KEY_PAGE_UP         ::Raying::Key::PageUp
#define RAYINGZ_KEY_PAGE_DOWN       ::Raying::Key::PageDown
#define RAYINGZ_KEY_HOME            ::Raying::Key::Home
#define RAYINGZ_KEY_END             ::Raying::Key::End
#define RAYINGZ_KEY_CAPS_LOCK       ::Raying::Key::CapsLock
#define RAYINGZ_KEY_SCROLL_LOCK     ::Raying::Key::ScrollLock
#define RAYINGZ_KEY_NUM_LOCK        ::Raying::Key::NumLock
#define RAYINGZ_KEY_PRINT_SCREEN    ::Raying::Key::PrintScreen
#define RAYINGZ_KEY_PAUSE           ::Raying::Key::Pause
#define RAYINGZ_KEY_F1              ::Raying::Key::F1
#define RAYINGZ_KEY_F2              ::Raying::Key::F2
#define RAYINGZ_KEY_F3              ::Raying::Key::F3
#define RAYINGZ_KEY_F4              ::Raying::Key::F4
#define RAYINGZ_KEY_F5              ::Raying::Key::F5
#define RAYINGZ_KEY_F6              ::Raying::Key::F6
#define RAYINGZ_KEY_F7              ::Raying::Key::F7
#define RAYINGZ_KEY_F8              ::Raying::Key::F8
#define RAYINGZ_KEY_F9              ::Raying::Key::F9
#define RAYINGZ_KEY_F10             ::Raying::Key::F10
#define RAYINGZ_KEY_F11             ::Raying::Key::F11
#define RAYINGZ_KEY_F12             ::Raying::Key::F12
#define RAYINGZ_KEY_F13             ::Raying::Key::F13
#define RAYINGZ_KEY_F14             ::Raying::Key::F14
#define RAYINGZ_KEY_F15             ::Raying::Key::F15
#define RAYINGZ_KEY_F16             ::Raying::Key::F16
#define RAYINGZ_KEY_F17             ::Raying::Key::F17
#define RAYINGZ_KEY_F18             ::Raying::Key::F18
#define RAYINGZ_KEY_F19             ::Raying::Key::F19
#define RAYINGZ_KEY_F20             ::Raying::Key::F20
#define RAYINGZ_KEY_F21             ::Raying::Key::F21
#define RAYINGZ_KEY_F22             ::Raying::Key::F22
#define RAYINGZ_KEY_F23             ::Raying::Key::F23
#define RAYINGZ_KEY_F24             ::Raying::Key::F24
#define RAYINGZ_KEY_F25             ::Raying::Key::F25

/* Keypad */
#define RAYING_KEY_KP_0            ::Raying::Key::KP0
#define RAYING_KEY_KP_1            ::Raying::Key::KP1
#define RAYING_KEY_KP_2            ::Raying::Key::KP2
#define RAYING_KEY_KP_3            ::Raying::Key::KP3
#define RAYING_KEY_KP_4            ::Raying::Key::KP4
#define RAYING_KEY_KP_5            ::Raying::Key::KP5
#define RAYING_KEY_KP_6            ::Raying::Key::KP6
#define RAYING_KEY_KP_7            ::Raying::Key::KP7
#define RAYING_KEY_KP_8            ::Raying::Key::KP8
#define RAYING_KEY_KP_9            ::Raying::Key::KP9
#define RAYING_KEY_KP_DECIMAL      ::Raying::Key::KPDecimal
#define RAYING_KEY_KP_DIVIDE       ::Raying::Key::KPDivide
#define RAYING_KEY_KP_MULTIPLY     ::Raying::Key::KPMultiply
#define RAYING_KEY_KP_SUBTRACT     ::Raying::Key::KPSubtract
#define RAYING_KEY_KP_ADD          ::Raying::Key::KPAdd
#define RAYING_KEY_KP_ENTER        ::Raying::Key::KPEnter
#define RAYING_KEY_KP_EQUAL        ::Raying::Key::KPEqual

#define RAYING_KEY_LEFT_SHIFT      ::Raying::Key::LeftShift
#define RAYING_KEY_LEFT_CONTROL    ::Raying::Key::LeftControl
#define RAYING_KEY_LEFT_ALT        ::Raying::Key::LeftAlt
#define RAYING_KEY_LEFT_SUPER      ::Raying::Key::LeftSuper
#define RAYING_KEY_RIGHT_SHIFT     ::Raying::Key::RightShift
#define RAYING_KEY_RIGHT_CONTROL   ::Raying::Key::RightControl
#define RAYING_KEY_RIGHT_ALT       ::Raying::Key::RightAlt
#define RAYING_KEY_RIGHT_SUPER     ::Raying::Key::RightSuper
#define RAYING_KEY_MENU            ::Raying::Key::Menu
