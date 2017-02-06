#pragma once

#include <cstdint>
// OFL_IB WINSYS
#ifdef linux
#include <X11/Xutil.h>
#else
// TODO WINDOWS stuff
#endif
// OFL_IB

#ifdef linux


namespace ofl
{


class Key
{
public:
	enum KeyActions
	{
		RELEASED = -1,
		REPEAT = 0,
		PRESSED = 1,
	};

	enum KeyModifiers
	{
		MOD_SHIFT = 1,
		MOD_CTRL = 2,
		MOD_ALT = 4,
		MOD_SUPER = 8
	};
	enum KeyID
	{
		UNKNOWN			=	XK_VoidSymbol&0x01ff,
		SPACE			=	XK_space&0x01ff,
		APOSTROPHE		=	XK_apostrophe&0x01ff,		// '
		COMMA			=	XK_comma&0x01ff,		// ,
		MINUS			=	XK_minus&0x01ff,		// -
		PERIOD			=	XK_period&0x01ff,		// .
		SLASH			=	XK_slash&0x01ff,		// /
		ZERO			=	XK_0&0x01ff,
		ONE				=	XK_1&0x01ff,
		TWO				=	XK_2&0x01ff,
		THREE			=	XK_3&0x01ff,
		FOUR			=	XK_4&0x01ff,
		FIVE			=	XK_5&0x01ff,
		SIX				=	XK_6&0x01ff,
		SEVEN			=	XK_7&0x01ff,
		EIGHT			=	XK_8&0x01ff,
		NINE			=	XK_9&0x01ff,
		SEMICOLON		=	XK_semicolon&0x01ff,		// ;
		EQUAL			=	XK_equal&0x01ff,		// =
		A				=	XK_A&0x01ff,
		B				=	XK_B&0x01ff,
		C				=	XK_C&0x01ff,
		D				=	XK_D&0x01ff,
		E				=	XK_E&0x01ff,
		F				=	XK_F&0x01ff,
		G				=	XK_G&0x01ff,
		H				=	XK_H&0x01ff,
		I				=	XK_I&0x01ff,
		J				=	XK_J&0x01ff,
		K				=	XK_K&0x01ff,
		L				=	XK_L&0x01ff,
		M				=	XK_M&0x01ff,
		N				=	XK_N&0x01ff,
		O				=	XK_O&0x01ff,
		P				=	XK_P&0x01ff,
		Q				=	XK_Q&0x01ff,
		R				=	XK_R&0x01ff,
		S				=	XK_S&0x01ff,
		T				=	XK_T&0x01ff,
		U				=	XK_U&0x01ff,
		V				=	XK_V&0x01ff,
		W				=	XK_W&0x01ff,
		X				=	XK_X&0x01ff,
		Y				=	XK_Y&0x01ff,
		Z				=	XK_Z&0x01ff,
		LEFT_BRACKET	=	XK_bracketleft&0x01ff,		// [
		BACKSLASH		=	XK_backslash&0x01ff,
		RIGHT_BRACKET	=	XK_bracketright&0x01ff,		// ]
		GRAVE_ACCENT	=	XK_grave&0x01ff,		// `
		ESCAPE			=	XK_Escape&0x01ff,
		ENTER			=	XK_Return&0x01ff,
		TAB				=	XK_Tab&0x01ff,
		BACKSPACE		=	XK_BackSpace&0x01ff,
		INSERT			=	XK_Insert&0x01ff,
		DELETE			=	XK_Delete&0x01ff,
		RIGHT			=	XK_Right&0x01ff,
		LEFT			=	XK_Left&0x01ff,
		DOWN			=	XK_Down&0x01ff,
		UP				=	XK_Up&0x01ff,
		PAGE_UP			=	XK_Page_Up&0x01ff,
		PAGE_DOWN		=	XK_Page_Down&0x01ff,
		HOME			=	XK_Home&0x01ff,
		END				=	XK_End&0x01ff,
		CAPS_LOCK		=	XK_Caps_Lock&0x01ff,
		SCROLL_LOCK		=	XK_Scroll_Lock&0x01ff,
		NUM_LOCK		=	XK_Num_Lock&0x01ff,
		PRINT_SCREEN	=	XK_Print&0x01ff,
		PAUSE			=	XK_Pause&0x01ff,
		F1				=	XK_F1&0x01ff,
		F2				=	XK_F2&0x01ff,
		F3				=	XK_F3&0x01ff,
		F4				=	XK_F4&0x01ff,
		F5				=	XK_F5&0x01ff,
		F6				=	XK_F6&0x01ff,
		F7				=	XK_F7&0x01ff,
		F8				=	XK_F8&0x01ff,
		F9				=	XK_F9&0x01ff,
		F10				=	XK_F10&0x01ff,
		F11				=	XK_F11&0x01ff,
		F12				=	XK_F12&0x01ff,
		F13				=	XK_F13&0x01ff,
		F14				=	XK_F14&0x01ff,
		F15				=	XK_F15&0x01ff,
		F16				=	XK_F16&0x01ff,
		F17				=	XK_F17&0x01ff,
		F18				=	XK_F18&0x01ff,
		F19				=	XK_F19&0x01ff,
		F20				=	XK_F20&0x01ff,
		F21				=	XK_F21&0x01ff,
		F22				=	XK_F22&0x01ff,
		F23				=	XK_F23&0x01ff,
		F24				=	XK_F24&0x01ff,
		F25				=	XK_F25&0x01ff,
		NUM_ZERO		=	XK_KP_0&0x01ff,
		NUM_ONE			=	XK_KP_1&0x01ff,
		NUM_TWO			=	XK_KP_2&0x01ff,
		NUM_THREE		=	XK_KP_3&0x01ff,
		NUM_FOUR		=	XK_KP_4&0x01ff,
		NUM_FIVE		=	XK_KP_5&0x01ff,
		NUM_SIX			=	XK_KP_6&0x01ff,
		NUM_SEVEN		=	XK_KP_7&0x01ff,
		NUM_EIGHT		=	XK_KP_8&0x01ff,
		NUM_NEINE		=	XK_KP_9&0x01ff,
		NUM_DECIMAL		=	XK_KP_Decimal&0x01ff,
		NUM_DIVIDE		=	XK_KP_Divide&0x01ff,
		NUM_MULTIPLY	=	XK_KP_Multiply&0x01ff,
		NUM_SUBTRACT	=	XK_KP_Subtract&0x01ff,
		NUM_ADD			=	XK_KP_Add&0x01ff,
		NUM_ENTER		=	XK_KP_Enter&0x01ff,
		NUM_EQUAL		=	XK_KP_Equal&0x01ff,
		LEFT_SHIFT		=	XK_Shift_L&0x01ff,
		LEFT_CONTROL	=	XK_Control_L&0x01ff,
		LEFT_ALT		=	XK_Alt_L&0x01ff,
		LEFT_SUPER		=	XK_Super_L&0x01ff,
		RIGHT_SHIFT		=	XK_Shift_R&0x01ff,
		RIGHT_CONTROL	=	XK_Control_R&0x01ff,
		RIGHT_ALT		=	XK_Alt_R&0x01ff,
		RIGHT_SUPER		=	XK_Super_R&0x01ff,
		MENU			=	XK_Menu&0x01ff,
		MAX_KEYS		=   0x0200
	};

	KeyID id;
	Key(KeyID id){this->id = id;}
	Key(uint32_t id){this->id = static_cast<KeyID>(id);}
	Key(KeySym id){this->id = static_cast<KeyID>(id);}

	operator KeyID(){return id;}
	operator uint32_t(){return static_cast<uint32_t>(id);}
	operator KeySym(){return static_cast<KeySym>(id);}

	bool operator ==(const Key o)const{return id == o.id;}
};
#else
TODO Windows
#endif


class UserIOListeners
{
public:
	virtual void event_keyboard(const Key /*key*/,const int /*action*/,const  uint /*mods*/,const  uint /*scancode*/){}
	virtual void event_character(const unsigned int /*c*/,const  Key::KeyActions /*action*/,const  uint /*mods*/){}
	virtual void event_mouse_button(const uint /*button*/, const int /*action*/,const uint /*mods*/,const double /*x*/, const double /*y*/){}
	virtual void event_mouse_scroll(double /*xScroll*/, double /*yScroll*/){}
	virtual void event_mouse_move(double /*x*/, double /*y*/){}
};

class UserIODevice
{
public:
	virtual void add_listener(UserIOListeners* l) = 0;
	virtual bool is_key_down(Key k) = 0;
	virtual uint modifyer_keys() = 0;
	virtual bool is_button_down(uint button) = 0;
};

}
