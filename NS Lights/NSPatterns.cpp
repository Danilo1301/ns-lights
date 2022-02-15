#include "NSPatterns.h"

std::list<NSPattern*> NSPatterns::ListPatterns;

void NSPatterns::Init()
{
	NSPattern* pattern1 = CreatePattern();
	pattern1->AddFrame(1, 1, 0, 160);
	pattern1->AddFrame(0, 0, 0, 50);
	pattern1->AddFrame(0, 0, 1, 160);
	pattern1->AddFrame(0, 0, 0, 50);

	NSPattern* pattern2 = CreatePattern();
	pattern2->AddFrame(1, 1, 1, 60);
	pattern2->AddFrame(0, 0, 0, 60);
	pattern2->AddFrame(1, 1, 1, 60);
	pattern2->AddFrame(0, 0, 0, 300);

	NSPattern* pattern3 = CreatePattern();
	pattern3->AddFrame(0, 0, 0, 150);
	pattern3->AddFrame(1, 0, 0, 200);
	pattern3->AddFrame(1, 1, 1, 200);
	pattern3->AddFrame(0, 1, 1, 200);
	pattern3->AddFrame(0, 0, 0, 150);
}

NSPattern* NSPatterns::CreatePattern()
{
	NSPattern* pattern = new NSPattern();
	ListPatterns.push_back(pattern);
	return pattern;
}

NSPattern* NSPatterns::GetPattern(int pattern_current)
{
	int n = 0;
	std::list<NSPattern*>::iterator item;
	for (item = ListPatterns.begin(); item != ListPatterns.end(); ++item)
	{
		if (n == pattern_current) {
			return (*item);
		}
		n++;
	}
}