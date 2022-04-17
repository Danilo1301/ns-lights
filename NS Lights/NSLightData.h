#pragma once


#include "plugin.h"

#include "CMessages.h"
#include "CFont.h"
#include "CSprite.h"
#include "CCoronas.h"
#include "CGeneral.h"

#include "extensions/ScriptCommands.h"

#include "map"

#include "CCamera.h"


class NSLightData {
public:
	int vehicle_modelid;

	int amount_of_lights = 2;
	int direction = 1;
	float size = 0.5f;
	int type = 0;

	float pos_x = 0.0f;
	float light_distance = 0.6f;
	float pos_y = 0.0f;
	float pos_z = 1.2f;

	int red = 255;
	int green = 0;
	int blue = 0;
	int alpha = 255;

	int secondary_enabled = 1;
	int secondary_red = 0;
	int secondary_green = 0;
	int secondary_blue = 255;
	int secondary_alpha = 255;

	int big_light_enabled = 0;
	float big_light_size = 6.0f;
	int big_light_alpha = 30;

	float near_clip = 0.3f;
	int initial_pattern = 0;

	int reflect = 0;

	NSLightData(int vehicle_modelid) {
		this->vehicle_modelid = vehicle_modelid;
	}

	float NSGetAngleBetweenVectors(CVector v1, CVector v2, CVector v3) {
		float v12 = sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
		float v13 = sqrt(pow(v1.x - v3.x, 2) + pow(v1.y - v3.y, 2));
		float v23 = sqrt(pow(v2.x - v3.x, 2) + pow(v2.y - v3.y, 2));
		return acos((pow(v12, 2) + pow(v13, 2) - pow(v23, 2)) / (2 * v12 * v13));
	}

	void RegisterOnVehicle(CVehicle* vehicle, unsigned int id, bool left, bool middle, bool right, bool freeze) {
		int light_id = id;

	
		for (int i = 0; i < amount_of_lights; i++)
		{
			float radius = size;
			float fl_radius = big_light_size;

			bool isLeftSide = (i < floor(amount_of_lights / 2));
			bool isMiddle = amount_of_lights == 1;
			bool isRightSide = (i >= floor(amount_of_lights / 2) + (amount_of_lights % 2 == 0 ? 0 : 1));

			bool showLight = false;

			if (left == 1 && isLeftSide) { showLight = true; }
			if (right == 1 && isRightSide) { showLight = true; }
			if (middle == 1 && isMiddle) { showLight = true; }

			CVector posn = CVector(0, pos_y, pos_z);
			posn.x = pos_x - (amount_of_lights - 1) * light_distance / 2 + (i)*light_distance;

		
			float dir = NSGetAngleBetweenVectors(vehicle->GetPosition(), vehicle->TransformFromObjectSpace(posn), TheCamera.GetPosition());
			if (isnan(dir)) {
				dir = 0.01f;
			}
			const float start_fadeout = 1.0f;
			const float end_fadeout = 1.6f;

			if ((pos_y >= 0.1f || pos_y <= -0.1f) && direction != 1) {
				if (dir >= start_fadeout && dir <= end_fadeout) {
					float r = (dir - start_fadeout) / (end_fadeout - start_fadeout);

					radius *= (1.0f - r);
					fl_radius *= (1.0f - r);
				}

				if (dir >= end_fadeout && direction == 1) {
					showLight = false;
				}

				if (direction == 0 && dir >= end_fadeout) {
					showLight = false;
				}
			}

			

			if (!showLight & !freeze) {
				radius = 0.0f;
				fl_radius = 0.0f;
			}

			bool useSec = (secondary_enabled && isRightSide);

			CCoronas::RegisterCorona(light_id, vehicle, (unsigned char)(useSec ? secondary_red : red), (unsigned char)(useSec ? secondary_green : green), (unsigned char)(useSec ? secondary_blue : blue), (unsigned char)(useSec ? secondary_alpha : alpha), posn, radius, 400.0f, (eCoronaType)type, eCoronaFlareType::FLARETYPE_NONE, false, false, 0, 0.0f, false, near_clip, 0, 100.0f, false, false);

			
			light_id++;

			if (big_light_enabled)
			{
				CCoronas::RegisterCorona(light_id, vehicle, (unsigned char)(useSec ? secondary_red : red), (unsigned char)(useSec ? secondary_green : green), (unsigned char)(useSec ? secondary_blue : blue), (unsigned char)big_light_alpha, posn, fl_radius, 400.0f, eCoronaType::CORONATYPE_SHINYSTAR, eCoronaFlareType::FLARETYPE_NONE, false, false, 0, 0.0f, false, 1.5f, 0, 100.0f, false, false);
				light_id++;
			}

			if (radius > 0 && reflect) {
				CVector fposition = vehicle->TransformFromObjectSpace(posn);
				plugin::Command< 0x09E5 >(fposition.x, fposition.y, fposition.z, (useSec ? secondary_red : red), (useSec ? secondary_green : green), (useSec ? secondary_blue : blue), 30.0f);

			}

		}


	}
};

class NSLights {
public:
	static std::map<int, std::list<NSLightData*>> ListLights;
	static std::list<int> ListLights_modelid_used;
	static std::list<std::pair<int, int>> _models;

	static NSLightData* AddLightData(int vehicle_modelid);
	static bool LightDataExists(int vehicle_modelid);
	static NSLightData* GetLightData(int vehicle_modelid, int lightn);
	static void RemoveLightData(int vehicle_modelid, int index);

	static int GetNumberOfLightsOfVehicle(int vehicle_modelid);
	//static bool LightDataExists(int vehicle_modelid);
	//static std::list<NSLightData*> GetLightData(int vehicle_modelid);
};