#include "NS.h"

#include "plugin.h"


#include "CSprite.h"

#include "NSMenu.h"
#include "NSKeys.h"

#include "NSConfig.h"
#include "NSLangConfig.h"

#include "NSVehicleController.h"
#include "NSPatterns.h"

#include "TestHelper.h"

int lastToggledLights = 0;

bool NS::m_ShowDebug = false;

using namespace plugin;

void NS::Init()
{
    //https://gtaforums.com/topic/757430-block-siren-lights-memory-address-for-it/

    /*
    TestHelper::AddStep("", []() {
         //0A8C: write_memory 0x70026C size 4 value 0x90909090 virtual_protect 0
         plugin::patch::SetUInt(0x70026C, 0x90909090);

         //0A8C : write_memory 0x700270 size 1 value 0x90 virtual_protect 0
         plugin::patch::SetUChar(0x700270, 0x90);

         //0A8C : write_memory 0x700271 size 1 value 0x90 virtual_protect 0
         plugin::patch::SetUChar(0x700271, 0x90);

         //0A8C : write_memory 0x700261 size 4 value 0x90909090 virtual_protect 0
         plugin::patch::SetUInt(0x700261, 0x90909090);

         //0A8C : write_memory 0x700265 size 1 value 0x90 virtual_protect 0
         plugin::patch::SetUChar(0x700265, 0x90);

         //0A8C : write_memory 0x700266 size 1 value 0x90 virtual_protect 0
         plugin::patch::SetUChar(0x700266, 0x90);

         //0A8C : write_memory 0x700257 size 4 value 0x90909090 virtual_protect 0
         plugin::patch::SetUInt(0x700257, 0x90909090);

         //0A8C : write_memory 0x70025B size 1 value 0x90 virtual_protect 0
         plugin::patch::SetUChar(0x70025B, 0x90);

         //0A8C : write_memory 0x70025C size 1 value 0x90 virtual_protect 0
         plugin::patch::SetUChar(0x70025C, 0x90);

        CMessages::AddMessageJumpQ("1", 1000, 0, false);
    });
    */

    TestHelper::AddStep("", []() {
        //0@ = 0xC3F12C //CPointLight => RGB
        int pointLight = 0xC3F12C;

        //0A8C: write_memory 0@ size 4 value 0.0 virtual_protect 0 // R
        plugin::patch::SetUInt(pointLight, 0);

        //0@ += 4
        pointLight += 4;

        //0A8C: write_memory 0@ size 4 value 0.0 virtual_protect 0  // G
        plugin::patch::SetUInt(pointLight, 0);

        //0@ += 4
        pointLight += 4;

        //0A8C: write_memory 2@ size 4 value 0.0 virtual_protect 0 
        plugin::patch::SetUInt(pointLight, 0);

        //CMessages::AddMessageJumpQ("2", 1000, 0, false);
    });

    TestHelper::AddStep("", []() {
        //disable siren on police car

        //NOPs the function that draws the coronnas
        //0A8C: write_memory 0x6ABA60 size 4 value 0x90909090 virtual_protect 0
        plugin::patch::SetUInt(0x6ABA60, 0x90909090);

        //0A8C: write_memory 0x6ABA64 size 1 value 0x90 virtual_protect 0
        plugin::patch::SetUChar(0x6ABA64, 0x90);

        //CMessages::AddMessageJumpQ("3", 1000, 0, false);
    });

    /*
    TestHelper::AddStep("", []() {
        //NOPs the function that checks wether the siren was activated or not
       //0A8C: write_memory 0x6FFDFC size 1 value 0x90 virtual_protect 0
       plugin::patch::SetUChar(0x6FFDFC, 0x90);

       //0A8C: write_memory 0x6FFDFD size 1 value 0x90 virtual_protect 0
       plugin::patch::SetUChar(0x6FFDFD, 0x90);

       //0A8C: write_memory 0x6FFDFE size 1 value 0x90 virtual_protect 0
       plugin::patch::SetUChar(0x6FFDFE, 0x90);

        CMessages::AddMessageJumpQ("4", 1000, 0, false);
    });
    */


    TestHelper::AddStep("", []() {
        //NOPs the function that activates the shadow drawing under the vehicle
        //0A8C: write_memory 0x70802D size 4 value 0x90909090 virtual_protect 0
        plugin::patch::SetUInt(0x70802D, 0x90909090);

        //CMessages::AddMessageJumpQ("5", 1000, 0, false);
    });
    
    TestHelper::RunAllSteps();

    NSKeys::AddKeyPressListener(83, []() {
        if(KeyPressed(17)) TestHelper::RunNextStep();
    });
   



    NSLangConfig::LoadConfig();
    NSConfig::LoadConfigs();

    for (std::pair<int, int> m : NSConfig::_models)
    {
        int modelid = m.first;
        int lightsn = m.second;

        for (int i = 0; i <= lightsn; i++)
        {
            NSLightData* lightData = NSLights::AddLightData(modelid);

            lightData->amount_of_lights = NSConfig::GetConfigForModel(modelid, i, "AMOUNT_OF_LIGHTS")->val_int;
            lightData->direction = NSConfig::GetConfigForModel(modelid, i, "DIRECTION")->val_int;
            lightData->size = NSConfig::GetConfigForModel(modelid, i, "SIZE")->val_float;
            lightData->type = NSConfig::GetConfigForModel(modelid, i, "TYPE")->val_int;
            lightData->pos_x = NSConfig::GetConfigForModel(modelid, i, "POS_X")->val_float;
            lightData->light_distance = NSConfig::GetConfigForModel(modelid, i, "LIGHT_DISTANCE")->val_float;
            lightData->pos_y = NSConfig::GetConfigForModel(modelid, i, "POS_Y")->val_float;
            lightData->pos_z = NSConfig::GetConfigForModel(modelid, i, "POS_Z")->val_float;
            lightData->red = NSConfig::GetConfigForModel(modelid, i, "RED")->val_int;
            lightData->green = NSConfig::GetConfigForModel(modelid, i, "GREEN")->val_int;
            lightData->blue = NSConfig::GetConfigForModel(modelid, i, "BLUE")->val_int;
            lightData->alpha = NSConfig::GetConfigForModel(modelid, i, "ALPHA")->val_int;
            lightData->secondary_enabled = NSConfig::GetConfigForModel(modelid, i, "SECONDARY_ENABLED")->val_int;
            lightData->secondary_red = NSConfig::GetConfigForModel(modelid, i, "SECONDARY_RED")->val_int;
            lightData->secondary_green = NSConfig::GetConfigForModel(modelid, i, "SECONDARY_GREEN")->val_int;
            lightData->secondary_blue = NSConfig::GetConfigForModel(modelid, i, "SECONDARY_BLUE")->val_int;
            lightData->secondary_alpha = NSConfig::GetConfigForModel(modelid, i, "SECONDARY_ALPHA")->val_int;
            lightData->big_light_enabled = NSConfig::GetConfigForModel(modelid, i, "BIG_LIGHT_ENABLED")->val_int;
            lightData->big_light_size = NSConfig::GetConfigForModel(modelid, i, "BIG_LIGHT_SIZE")->val_float;
            lightData->big_light_alpha = NSConfig::GetConfigForModel(modelid, i, "BIG_LIGHT_ALPHA")->val_int;
            lightData->near_clip = NSConfig::GetConfigForModel(modelid, i, "NEAR_CLIP")->val_float;
            lightData->initial_pattern = NSConfig::GetConfigForModel(modelid, i, "INITIAL_PATTERN")->val_int;
            lightData->reflect = NSConfig::GetConfigForModel(modelid, i, "REFLECT")->val_int;
        }
    }

    NSPatterns::Init();

    NSKeys::AddKeyPressListener(38, NSMenu::OnUpPressed);
    NSKeys::AddKeyPressListener(40, NSMenu::OnDownPressed);
    NSKeys::AddKeyPressListener(37, NSMenu::OnLeftPressed);
    NSKeys::AddKeyHoldListener(37, NSMenu::OnLeftHold);
    NSKeys::AddKeyPressListener(39, NSMenu::OnRightPressed);
    NSKeys::AddKeyHoldListener(39, NSMenu::OnRightHold);
    NSKeys::AddKeyPressListener(32, NSMenu::OnSpacePressed);
    NSKeys::AddKeyPressListener(76, NSMenu::ToggleMenu);

  

}

void NS::Update() {
    CVehicle* vehicle = FindPlayerVehicle(0, false);

    if (vehicle)
    {
        NSVehicleController* vehicleController = NSVehicles::GetVehicleController(vehicle);

        if (KeyPressed(VK_LCONTROL) && KeyPressed(74) && CTimer::m_snTimeInMilliseconds - lastToggledLights > 300)
        {
            lastToggledLights = CTimer::m_snTimeInMilliseconds;

            vehicleController->m_bEnabled = !vehicleController->m_bEnabled;
        }

        vehicleController->RegisterCoronas();
    }


    NSVehicles::CheckVehicles();
    NSKeys::Update();
}

void NS::UpdateVehicleControllers(CVehicle* vehicle) {
    NSVehicleController* vehicleController = NSVehicles::GetVehicleController(vehicle);

    vehicleController->Update();
}

void NS::Draw() {
    NSMenu::Draw();
    NSVehicles::Draw();
}

NS::NS() {
    Events::initGameEvent += Init;
    //Events::gameProcessEvent += Update;
    Events::processScriptsEvent += Update;
    Events::vehicleRenderEvent.before += UpdateVehicleControllers;
    Events::drawingEvent.after += Draw;
}

NS ns;