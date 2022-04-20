#pragma once

#include "eMenuItemType.h"
#include "map"
#include "array"

class NSMenuItemOption {
public:
	int val;
	char* display;

	NSMenuItemOption(int val, char* display)
	{
		this->val = val;
		this->display = display;
	}
};

class OptionValueBinder {
public:
	std::list<std::pair<int, std::string> > Options;
	int option_selection_index = 0;

	int& store_to_value;

	OptionValueBinder(int& a) : store_to_value(a) {}

	bool CanGoLeft()
	{
		return option_selection_index > 0;
	}

	bool CanGoRight()
	{
		return option_selection_index < ((int)Options.size() - 1);
	}

	void ChangeBy(int add_index)
	{
		option_selection_index += add_index;

		if (option_selection_index < 0) { option_selection_index = 0; }
		if (option_selection_index > (int)Options.size()-1) { option_selection_index = (int)Options.size() - 1; }

		store_to_value = GetCurrentOptionInt();
	}

	void AddOption(int value, std::string text)
	{
		//std::string option_display(text);
		Options.push_back(std::pair<int, std::string>(value, text));

		SetOptionByValue(store_to_value);
	}

	void SetOptionByValue(int value) {
		int n = 0;
		std::list<std::pair<int, std::string>>::iterator item;
		for (item = Options.begin(); item != Options.end(); ++item)
		{
			if (value == (*item).first) {
				option_selection_index = n;
				return;
			}
			n++;
		}
	}

	const char* GetCurrentOptionDisplay()
	{
		int n = 0;
		std::list<std::pair<int, std::string>>::iterator item;
		for (item = Options.begin(); item != Options.end(); ++item)
		{
			if (n == option_selection_index) {
				return (*item).second.c_str();
			}
			n++;
		}
	}

	int GetCurrentOptionInt()
	{
		int n = 0;
		std::list<std::pair<int, std::string>>::iterator item;
		for (item = Options.begin(); item != Options.end(); ++item)
		{
			if (n == option_selection_index) {
				return (*item).first;
			}
			n++;
		}
	}
};

template<class T>
class ValueBinder {
public:
	T& value;

	T min;
	T max;

	ValueBinder(T& a, T min, T max) : value(a)
	{
		this->min = min;
		this->max = max;
	}

	void SetValue(T newValue) {
		value = newValue;
	}

	void ChangeBy(T addValue) {
		value += addValue;
		
		if (value < min) { value = min; }
		if (value > max) { value = max; }
	}

};

class NSMenuItem {
public:
	eMenuItemType type;
	char text[256] = "";
	float displayWidth;
	
	std::function<void()> btn_callback;

	OptionValueBinder* optionValueBinder;

	bool isActive = false;

	ValueBinder<float>* valueBinder_float;
	ValueBinder<int>* valueBinder_int;

	std::list<std::array<int, 4>> ListColorDisplays;

	NSMenuItem(eMenuItemType type, const char* text, float displayWidth)
	{
		this->type = type;
		sprintf(this->text, "%s", text);
		this->displayWidth = displayWidth;
	}

	void CreateColorDisplay(int red, int green, int blue, int alpha)
	{
		std::array arr = { red, green, blue, alpha };
		ListColorDisplays.push_back(arr);
	}

	void CreateValueBinder(float &store_to, float min, float max)
	{
		valueBinder_float = new ValueBinder<float>(store_to, min, max);
	}

	void CreateValueBinder(int &store_to, int min, int max)
	{
		valueBinder_int = new ValueBinder<int>(store_to, min, max);
	}

	void CreateOptionValueBinder(int& store_to)
	{
		optionValueBinder = new OptionValueBinder(store_to);
	}

	void AddOption(int val, std::string text) {
		optionValueBinder->AddOption(val, text);
	}

	void DrawAt(float x, float y, float width)
	{
		CRGBA background_color = this->isActive ? CRGBA(255, 255, 255, 255) : CRGBA(0, 0, 0, 217);
		CRGBA text_color = this->isActive ? CRGBA(0, 0, 0, 255) : CRGBA(255, 255, 255, 255);

		NSDraw::SetOrientation(ALIGN_LEFT);
		NSDraw::MDrawRect(CRect(x, y, x + width, y + 30.0f), background_color);

		char str[256];
		sprintf(str, "%s%s", type == eMenuItemType::ITEM_BUTTON ? "> " : "", text);
		NSDraw::MDrawText(str, x + 5.0f, y + 5.0f, text_color);

		if (type == eMenuItemType::ITEM_FLOAT || type == eMenuItemType::ITEM_INT || type == eMenuItemType::ITEM_OPTION) {
			DrawDisplayAt(x + width - displayWidth - 25.0f, y);
		}

		int n = ListColorDisplays.size();
		for (std::array<int, 4> arr : ListColorDisplays)
		{
			n--;
			float px = (x + width) - ((n+1) * 30.0f);

			CRect rect(px + 5.0f, y + 5.0f, px + 20.0f, y + 25.0f);

			NSDraw::MDrawRect(rect, CRGBA(0, 0, 0, 255));
			NSDraw::MDrawRect(rect, CRGBA(arr[0], arr[1], arr[2], arr[3]));
		}
	}

	void DrawDisplayAt(float x, float y)
	{
		CRGBA text_color = this->isActive ? CRGBA(0, 0, 0, 255) : CRGBA(255, 255, 255, 255);

		NSDraw::SetOrientation(ALIGN_CENTER);

		char display_text[256];
		
		if (type == eMenuItemType::ITEM_FLOAT)
		{
			sprintf(display_text, "%.2f", valueBinder_float->value);
		}
		else if (type == eMenuItemType::ITEM_INT) {
			sprintf(display_text, "%i", valueBinder_int->value);
		}
		else if (type == eMenuItemType::ITEM_OPTION) {
	
			const char* option_text = optionValueBinder->GetCurrentOptionDisplay();
			
			sprintf(display_text, "%s", option_text);
		}
		NSDraw::MDrawText(display_text, x + displayWidth / 2, y + 5.0f, text_color);

		bool allowL = true, allowR = true;

		if (type == eMenuItemType::ITEM_FLOAT)
		{
			if (valueBinder_float->value == valueBinder_float->min) { allowL = false; }
			if (valueBinder_float->value == valueBinder_float->max) { allowR = false; }
		}

		if (type == eMenuItemType::ITEM_INT)
		{
			if (valueBinder_int->value == valueBinder_int->min) { allowL = false; }
			if (valueBinder_int->value == valueBinder_int->max) { allowR = false; }
		}

		if (type == eMenuItemType::ITEM_OPTION)
		{
			allowL = optionValueBinder->CanGoLeft();
			allowR = optionValueBinder->CanGoRight();
		}

		if (allowL) { NSDraw::MDrawText("<", x - 10.0f, y + 5.0f, text_color); }
		if (allowR) { NSDraw::MDrawText(">", x + displayWidth + 10.0f, y + 5.0f, text_color); }
	}
};