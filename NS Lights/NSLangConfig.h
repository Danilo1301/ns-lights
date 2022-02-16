#pragma once

#include "NSLightData.h"

#include "fstream"
#include "sstream"
#include "string"
#include "list"
#include "map"
#include "string"


class NSLangConfig {
public:
	static std::ifstream langconfig_file;
	static std::map<std::string, std::string> LanguageLines;

	static void LoadConfig();
	static const char* GetLine(const char* line);
	static const char* FormatLine(const char* line, const char* format, int num);
	static void PrintString(const char* line, int time);
};