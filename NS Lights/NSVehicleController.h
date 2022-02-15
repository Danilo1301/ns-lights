#pragma once

#include "plugin.h"

#include "NSConfig.h"
#include "NSMenu.h"
#include "NSLightData.h"
#include "NSPatterns.h"

#include "CTimer.h"
#include "CFont.h"
#include "CSprite.h"

#include "map"

class NSVechiclePatternController {
public:
	int current_pattern = 0;
	int current_frame = 0;
	int last_changeFrame = 0;
	int last_changePattern = 0;

	bool leftActive = 0;
	bool middleActive = 0;
	bool rightActive = 0;

	NSVechiclePatternController() {};
};

class NSVehicleController {
public:
	CVehicle* vehicle;

	std::map<int, NSVechiclePatternController*> PatternControllers;

	std::map<int, int> _patternCheck;

	NSVehicleController(CVehicle* vehicle) {
		this->vehicle = vehicle;
	}

	bool firstReset = true;

	NSVechiclePatternController* GetPatternController(int lightn)
	{
		bool exists = PatternControllers.find(lightn) != PatternControllers.end();

		if (!exists) {
			PatternControllers.insert(std::pair<int, NSVechiclePatternController*>(lightn, new NSVechiclePatternController()));
		}
		return PatternControllers.at(lightn);
	}

	void ResetPatterns()
	{
		int lightsn = NSLights::GetNumberOfLightsOfVehicle(vehicle->m_nModelIndex) - 1;

		bool canReset = false;

		for (int i = 0; i <= lightsn; i++)
		{
			NSLightData* lightData = NSLights::GetLightData(vehicle->m_nModelIndex, i);

			if (!_patternCheck.count(i))
			{
				_patternCheck.insert(std::pair<int, int>(i, lightData->initial_pattern));
			}
			if (_patternCheck.at(i) != lightData->initial_pattern)
			{
				canReset = true;
			}
		}

		if (firstReset)
		{
			firstReset = false;
			canReset = true;
		}

		if (canReset)
		{
			for (int i = 0; i <= lightsn; i++)
			{
				NSLightData* lightData = NSLights::GetLightData(vehicle->m_nModelIndex, i);

				_patternCheck[i] = lightData->initial_pattern;

				NSVechiclePatternController* patternController = GetPatternController(i);
				patternController->current_pattern = lightData->initial_pattern;
				patternController->current_frame = 0;
				patternController->last_changeFrame = CTimer::m_snTimeInMilliseconds;
				patternController->last_changePattern = CTimer::m_snTimeInMilliseconds;
			}
		}
	}
	
	
	void UpdatePatterns()
	{
		ResetPatterns();

		for (std::pair<int, NSVechiclePatternController*>c : PatternControllers)
		{
			int i = c.first;
			NSVechiclePatternController* patternController = c.second;
			
			NSPattern* pattern = NSPatterns::GetPattern(patternController->current_pattern);

			std::array patternData = pattern->GetFrame(patternController->current_frame);
			
			patternController->leftActive = patternData[0];
			patternController->middleActive = patternData[1];
			patternController->rightActive = patternData[2];
			int time = patternData[3];

			if ((int)CTimer::m_snTimeInMilliseconds - patternController->last_changeFrame >= time)
			{
				patternController->last_changeFrame = CTimer::m_snTimeInMilliseconds;
				patternController->current_frame += 1;

				if (patternController->current_frame == pattern->GetNumberOfFrames())
				{
					patternController->current_frame = 0;

					if ((int)CTimer::m_snTimeInMilliseconds - patternController->last_changePattern >= 9000) {
						patternController->last_changePattern = CTimer::m_snTimeInMilliseconds;
						patternController->current_pattern += 1;

						if (patternController->current_pattern == NSPatterns::ListPatterns.size()) {
							patternController->current_pattern = 0;
						}
					}
		
				}
			}
		}
	}

	void Update()
	{
		if (!NSLights::LightDataExists(vehicle->m_nModelIndex)) { return; }

		if (!NSMenu::isOpen && !(vehicle->m_nVehicleFlags.bSirenOrAlarm && vehicle->UsesSiren()))
		{
			return;
		}

		UpdatePatterns();

		int lightsn = NSLights::GetNumberOfLightsOfVehicle(vehicle->m_nModelIndex) - 1;

		unsigned int light_id = reinterpret_cast<unsigned int>(vehicle) + 10;

		for (int i = 0; i <= lightsn; i++)
		{
			NSVechiclePatternController* patternController = GetPatternController(i);
			NSLightData* lightData = NSLights::GetLightData(vehicle->m_nModelIndex, i);

			bool freeze = NSMenu::SETTINGS_FREEZE_LIGHTS && NSMenu::isOpen;

			lightData->RegisterOnVehicle(vehicle, light_id, patternController->leftActive, patternController->middleActive, patternController->rightActive, freeze);

			light_id += lightData->amount_of_lights*2;
		}
	}
};

class NSVehicles {
public:
	static std::map<CVehicle*, NSVehicleController*> ListVehicleControllers;

	static NSVehicleController* CreateVehicleController(CVehicle* vehicle);
	static NSVehicleController* GetVehicleController(CVehicle* vehicle);

	static bool VehicleHasController(CVehicle* vehicle);
	static void CheckVehicles();
};