#pragma once

#include "plugin.h"

struct TestFunction {
	std::string name;
	std::function<void(void)> fn;
};

class TestHelper {
public:
	static std::vector<TestFunction*> m_Steps;

	static void AddStep(std::string name, std::function<void(void)> fn);
	static void RemoveStep(int index);
	static void RunNextStep();
	static void RunAllSteps();
};