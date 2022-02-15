#pragma once

#include "plugin.h"

#include "map"

class NSKeys {
public:
	static void AddKeyPressListener(int, std::function<void()> fn);
	static void AddKeyHoldListener(int, std::function<void()> fn);
	static void Update();
};