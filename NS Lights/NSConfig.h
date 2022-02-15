#pragma once

#include "NSLightData.h"

#include "fstream"
#include "sstream"
#include "string"
#include "list"
#include "map"
#include "string"

class NSConfigItem {
public:
	char* key;
	float val_float = 0.0f;
	int val_int = 0;
	unsigned char val_uchar = 0;

	NSConfigItem(char* key) {
		this->key = key;
	}
};

class NSConfig {
public:
	static std::map<std::string, NSConfigItem*> ConfigItems;
	static std::map<int, int> NSConfig::_models;

	static void Init();
	static void NSConfig::LoadConfigs();
	static NSConfigItem* GetConfig(char* key);
	static NSConfigItem* GetConfigForModel(int modelid, int lightn, char* key);
	static bool NSConfig::ConfigExists(char* key);
	static void SaveConfig();
};