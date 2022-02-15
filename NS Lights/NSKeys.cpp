#include "NSKeys.h"

using namespace plugin;

std::map<int, std::list<std::function<void()>> > press_callbacks;
std::list<int> press_keys_used;
std::list<int> press_keys_down;

std::map<int, std::list<std::function<void()>> > hold_callbacks;
std::list<int> hold_keys_used;

void NSKeys::AddKeyPressListener(int i, std::function<void()> fn) {
	bool exists = (std::find(press_keys_used.begin(), press_keys_used.end(), i) != press_keys_used.end());
	if (!exists) {
		press_callbacks.insert(std::pair<int, std::list<std::function<void()>>>(i, std::list<std::function<void()>>()));
		press_keys_used.push_back(i);
	}
	press_callbacks.at(i).push_back(fn);
}

void NSKeys::AddKeyHoldListener(int i, std::function<void()> fn) {
	bool exists = (std::find(hold_keys_used.begin(), hold_keys_used.end(), i) != hold_keys_used.end());
	if (!exists) {
		hold_callbacks.insert(std::pair<int, std::list<std::function<void()>>>(i, std::list<std::function<void()>>()));
		hold_keys_used.push_back(i);
	}
	hold_callbacks.at(i).push_back(fn);
}

void NSKeys::Update() {
	for (std::list<int>::iterator it = press_keys_used.begin(); it != press_keys_used.end(); it++) {
		bool alreadyPressed = (std::find(press_keys_down.begin(), press_keys_down.end(), *it) != press_keys_down.end());
		if (KeyPressed(*it) && !alreadyPressed) {
			press_keys_down.push_back(*it);
			std::list<std::function<void()>> cbs = press_callbacks.at(*it);
			for (std::list<std::function<void()>>::iterator fn = cbs.begin(); fn != cbs.end(); fn++) {
				(*fn)();
			}
		}
		if (!KeyPressed(*it) && alreadyPressed) {
			press_keys_down.erase(std::find(press_keys_down.begin(), press_keys_down.end(), *it));
		}
	}

	for (std::list<int>::iterator it = hold_keys_used.begin(); it != hold_keys_used.end(); it++) {
		if (KeyPressed(*it)) {
			std::list<std::function<void()>> cbs = hold_callbacks.at(*it);
			for (std::list<std::function<void()>>::iterator fn = cbs.begin(); fn != cbs.end(); fn++) {
				(*fn)();
			}
		}
	}
}