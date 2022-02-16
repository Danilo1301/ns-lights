#include "NSVehicleController.h"

std::map<CVehicle*, NSVehicleController*> NSVehicles::ListVehicleControllers;

NSVehicleController* NSVehicles::CreateVehicleController(CVehicle* vehicle) {
	NSVehicleController* vehicleController = new NSVehicleController(vehicle);
	ListVehicleControllers.insert(std::pair<CVehicle*, NSVehicleController*>(vehicle, vehicleController));
	return vehicleController;
}

bool NSVehicles::VehicleHasController(CVehicle* vehicle) {
	return ListVehicleControllers.find(vehicle) != ListVehicleControllers.end();;
}

NSVehicleController* NSVehicles::GetVehicleController(CVehicle* vehicle) {
	if (!VehicleHasController(vehicle)) { CreateVehicleController(vehicle); }
	return (ListVehicleControllers.find(vehicle))->second;
}

void NSVehicles::CheckVehicles()
{
	for (std::pair<CVehicle*, NSVehicleController*> p : ListVehicleControllers)
	{
		bool found = false;

		for (CVehicle* vehicle : CPools::ms_pVehiclePool)
		{
			if (vehicle == p.first) { found = true; return; }
		}
		if (!found) { ListVehicleControllers.erase(p.first); }
	}
}

void NSVehicles::Draw()
{
	for (std::pair<CVehicle*, NSVehicleController*> p : ListVehicleControllers)
	{
		auto vehController = p.second;

		vehController->Draw();
	}
}

