#include "NSLightData.h"


std::map<int, std::list<NSLightData*>> NSLights::ListLights;
std::list<int> NSLights::ListLights_modelid_used;

NSLightData* NSLights::AddLightData(int vehicle_modelid)
{
	NSLightData* lightData = new NSLightData(vehicle_modelid);

	bool exists = LightDataExists(vehicle_modelid);
	if (!exists) {
		ListLights.insert(std::pair<int, std::list<NSLightData*>>(vehicle_modelid, std::list<NSLightData*>()));
		ListLights_modelid_used.push_back(vehicle_modelid);
	}
	ListLights.at(vehicle_modelid).push_back(lightData);
	return lightData;
}

void NSLights::RemoveLightData(int vehicle_modelid, int index)
{
	int n = 0;
	std::list<NSLightData*>::iterator item;
	for (item = ListLights.at(vehicle_modelid).begin(); item != ListLights.at(vehicle_modelid).end(); ++item)
	{
		if (n == index) {
			ListLights.at(vehicle_modelid).erase(item);
		}
		n++;
	}
}

bool NSLights::LightDataExists(int vehicle_modelid)
{
	return (std::find(ListLights_modelid_used.begin(), ListLights_modelid_used.end(), vehicle_modelid) != ListLights_modelid_used.end());
}

NSLightData* NSLights::GetLightData(int vehicle_modelid, int lightn)
{
	int n = 0;
	std::list<NSLightData*>::iterator item;
	for (item = ListLights.at(vehicle_modelid).begin(); item != ListLights.at(vehicle_modelid).end(); ++item)
	{
		if (n == lightn) {
			return (*item);
		}
		n++;
	}
}

int NSLights::GetNumberOfLightsOfVehicle(int vehicle_modelid)
{
	if (!LightDataExists(vehicle_modelid)) { return 0; }
	return (int)ListLights.at(vehicle_modelid).size();
}