#include "NSConfig.h"

std::ifstream config_file;

std::map<std::string, NSConfigItem*> NSConfig::ConfigItems;
std::map<int, int> NSConfig::_models;

void NSConfig::LoadConfigs() {
	ConfigItems.clear();

	config_file.open("config.dat");

	std::string line;
	while (std::getline(config_file, line))
	{
		std::string key = line.substr(0, line.find("="));
		std::string value = line.substr(line.find("=") + 1);

		std::string modelid = line.substr(0, line.find_first_of("_"));
		std::string lightn = line.substr(line.find_first_of("_") + 1, 1);

		_models[std::atoi(modelid.c_str())] = std::atoi(lightn.c_str());

		int val_int = 0;
		float val_float = 0.0f;

		if (value.find(".") != std::string::npos) {
			val_float = std::stof(value.c_str());
		}
		else {
			val_int = std::atoi(value.c_str());
		}

		char key_nc[256];
		sprintf(key_nc, "%s", key.c_str());

		NSConfigItem* cfgItem = new NSConfigItem(key_nc);
		cfgItem->val_float = val_float;
		cfgItem->val_int = val_int;
		cfgItem->val_uchar = (unsigned char)val_int;

		ConfigItems.insert(std::pair<char*, NSConfigItem*>(key_nc, cfgItem));
	}
	config_file.close();
}

NSConfigItem* NSConfig::GetConfig(char* key)
{
	return ConfigItems.at(std::string(key));
}

NSConfigItem* NSConfig::GetConfigForModel(int modelid, int lightn, char* key)
{
	char str[256];
	sprintf(str, "%i_%i_%s", modelid, lightn, key);

	if (ConfigItems.find(str) == ConfigItems.end()) {
		NSConfigItem* cfgItem = new NSConfigItem(str);
		cfgItem->val_float = 0.0f;
		cfgItem->val_int = 0;
		cfgItem->val_uchar = (unsigned char)0;

		ConfigItems.insert(std::pair<char*, NSConfigItem*>(str, cfgItem));
	}

	return ConfigItems.at(std::string(str));
}

bool NSConfig::ConfigExists(char* key)
{
	return ConfigItems.find(std::string(key)) != ConfigItems.end();
}

void WriteToConfigFile(std::ofstream& file, char* key, int modelid, int lightn, int val)
{
	file << modelid << "_" << lightn << "_" << key << "=" << val << "\n";
}

void WriteToConfigFile(std::ofstream& file, char* key, int modelid, int lightn, float val)
{
	char f[16];
	sprintf(f, "%.2f", val);
	file << modelid << "_" << lightn << "_" << key << "=" << f << "\n";
}

void NSConfig::SaveConfig()
{
	std::ofstream out_cfg;
	out_cfg.open("config.dat");

	for (int modelid : NSLights::ListLights_modelid_used)
	{
		int lightsn = NSLights::GetNumberOfLightsOfVehicle(modelid);

		if (lightsn > 0)
		{
			for (int i = 0; i <= lightsn - 1; i++)
			{
				NSLightData* data = NSLights::GetLightData(modelid, i);

				WriteToConfigFile(out_cfg, "AMOUNT_OF_LIGHTS", modelid, i, data->amount_of_lights);
				WriteToConfigFile(out_cfg, "DIRECTION", modelid, i, data->direction);
				WriteToConfigFile(out_cfg, "SIZE", modelid, i, data->size);
				WriteToConfigFile(out_cfg, "TYPE", modelid, i, data->type);
				WriteToConfigFile(out_cfg, "POS_X", modelid, i, data->pos_x);
				WriteToConfigFile(out_cfg, "LIGHT_DISTANCE", modelid, i, data->light_distance);
				WriteToConfigFile(out_cfg, "POS_Y", modelid, i, data->pos_y);
				WriteToConfigFile(out_cfg, "POS_Z", modelid, i, data->pos_z);
				WriteToConfigFile(out_cfg, "RED", modelid, i, data->red);
				WriteToConfigFile(out_cfg, "GREEN", modelid, i, data->green);
				WriteToConfigFile(out_cfg, "BLUE", modelid, i, data->blue);
				WriteToConfigFile(out_cfg, "ALPHA", modelid, i, data->alpha);
				WriteToConfigFile(out_cfg, "SECONDARY_ENABLED", modelid, i, data->secondary_enabled);
				WriteToConfigFile(out_cfg, "SECONDARY_RED", modelid, i, data->secondary_red);
				WriteToConfigFile(out_cfg, "SECONDARY_GREEN", modelid, i, data->secondary_green);
				WriteToConfigFile(out_cfg, "SECONDARY_BLUE", modelid, i, data->secondary_blue);
				WriteToConfigFile(out_cfg, "SECONDARY_ALPHA", modelid, i, data->secondary_alpha);
				WriteToConfigFile(out_cfg, "BIG_LIGHT_ENABLED", modelid, i, data->big_light_enabled);
				WriteToConfigFile(out_cfg, "BIG_LIGHT_SIZE", modelid, i, data->big_light_size);
				WriteToConfigFile(out_cfg, "BIG_LIGHT_ALPHA", modelid, i, data->big_light_alpha);
				WriteToConfigFile(out_cfg, "NEAR_CLIP", modelid, i, data->near_clip);
				WriteToConfigFile(out_cfg, "INITIAL_PATTERN", modelid, i, data->initial_pattern);
				WriteToConfigFile(out_cfg, "REFLECT", modelid, i, data->reflect);

			}
		}
	}
	out_cfg.close();
}