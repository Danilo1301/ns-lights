#include "NSLangConfig.h"

std::ifstream NSLangConfig::langconfig_file;

std::map<std::string, std::string> NSLangConfig::LanguageLines;

void NSLangConfig::LoadConfig() {
	langconfig_file.open("lang.txt");

	std::string line;
	while (std::getline(langconfig_file, line))
	{
		std::string key = line.substr(0, line.find("="));
		std::string value = line.substr(line.find("=") + 1);

		LanguageLines.insert(std::pair<std::string, std::string>(key, value));
	}
	langconfig_file.close();
}

const char* NSLangConfig::GetLine(const char* line) {
	if (LanguageLines.find(line) == LanguageLines.end()) {
		return line;
	}

	return LanguageLines.at(line).c_str();
}

const char* NSLangConfig::FormatLine(const char* line, const char* format, int num) {
	static char message[256];
	snprintf(message, 256, format, line, num);

	if (LanguageLines.find(line) != LanguageLines.end()) {
		snprintf(message, 256, format, GetLine(line), num);
	}

	return message;
}

void NSLangConfig::PrintString(const char* line, int time) {

	static char message[256];
	snprintf(message, 256, "%s", line);

	if (LanguageLines.find(line) != LanguageLines.end()) {
		snprintf(message, 256, "%s", GetLine(line));
	}

	CMessages::AddMessageJumpQ(message, time, 0, false);
}
