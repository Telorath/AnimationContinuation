#pragma once
namespace InputManager
{
	enum Button
	{
		keypad_tilde, keypad_1, keypad_2, keypad_3, keypad_4, keypad_5, keypad_6, keypad_7, keypad_8, keypad_9, keypad_0, keypad_minus, keypad_equals, backspace,
		keypad_tab, keypad_q, keypad_w, keypad_e, keypad_r, keypad_t, keypad_y, keypad_u, keypad_i, keypad_o, keypad_p,
		keypad_a, keypad_s, keypad_d,
		arrow_up, arrow_left, arrow_right, arrow_down,
		numlock, numpad_forewardslash,
		mouse_1, mouse_2, mouse_3
	};

	void RegisterKey(const char*, Button keybind);

	void setstate(Button Buttoncode, bool newstate);

	bool getkeystate(Button Buttoncode);

	bool getkeydown(Button buttoncode);

	void update();


	//Cleans Keyup/Keydown states for next frame.
	void finalupdate();

	void GetCursorDifference(float * x, float * y);

};

