#pragma once

#include "list"
#include "algorithm"
#include "array"

class NSPattern {
public:
	std::list<std::array<int, 4>> Frames;

	void AddFrame(int left_active, int middle_active, int right_active, int time)
	{
		std::array arr = { left_active, middle_active, right_active, time };
		Frames.push_back(arr);
	}

	int GetNumberOfFrames()
	{
		return (int)Frames.size();
	}

	std::array<int, 4> GetFrame(int n)
	{
		int i = 0;
		std::list<std::array<int, 4>>::iterator item;
		for (item = Frames.begin(); item != Frames.end(); ++item)
		{
			if (i == n) {
				return (*item);
			}
			i++;
		}
	}
};

class NSPatterns {
public:
	static std::list<NSPattern*> ListPatterns;

	static void Init();

	static NSPattern* CreatePattern();
	static NSPattern* GetPattern(int pattern_current);
};