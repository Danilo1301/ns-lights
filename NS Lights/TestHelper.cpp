#include "TestHelper.h"

std::vector<TestFunction*> TestHelper::m_Steps;

void TestHelper::AddStep(std::string name, std::function<void(void)> fn) {
	TestFunction* testFunction = new TestFunction{ name, fn };
	m_Steps.push_back(testFunction);
}

void TestHelper::RemoveStep(int step) {
	auto testFunction = m_Steps[0];
	auto it = std::find(m_Steps.begin(), m_Steps.end(), testFunction);
	if (it == m_Steps.end()) return;
	m_Steps.erase(it);
	delete testFunction;
}

void TestHelper::RunNextStep() {
	if (m_Steps.size() == 0) return;

	auto testFunction = m_Steps[0];

	auto fn = testFunction->fn;

	RemoveStep(0);

	fn();
}


void TestHelper::RunAllSteps() {
	while (m_Steps.size() > 0)
	{
		RunNextStep();
	}
}