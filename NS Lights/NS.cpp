#include "plugin.h"

#include "CSprite.h"

#include "NSMenu.h"
#include "NSKeys.h"
#include "NSConfig.h"
#include "NSVehicleController.h"
#include "NSPatterns.h"

using namespace plugin;

bool bIsSamp = true;

class NS {
public:
    static void Init()
    {
        //plugin::patch::SetUInt(0x70026C, 0x90909090);
        //plugin::patch::SetUChar(0x700270, 0x90);
        //plugin::patch::SetUChar(0x700271, 0x90);
        //plugin::patch::SetUInt(0x700261, 0x90909090);
        //plugin::patch::SetUChar(0x700265, 0x90);
        //plugin::patch::SetUChar(0x700266, 0x90);
        //plugin::patch::SetUInt(0x700257, 0x90909090);
        //plugin::patch::SetUChar(0x70025B, 0x90);
        //plugin::patch::SetUChar(0x70025C, 0x90);

        //0@ = 0xC3F12C //CPointLight => RGB
        //int pointLight = 0xC3F12C;
        //plugin::patch::SetUInt(pointLight, 0);
        //pointLight += 4;
        //plugin::patch::SetUInt(pointLight, 0);
        //pointLight += 4;
        //plugin::patch::SetUInt(pointLight, 0);

        //NOPs the function that draws the coronnas
        //plugin::patch::SetUInt(0x6ABA60, 0x90909090);
        //plugin::patch::SetUChar(0x6ABA64, 0x90);

        //NOPs the function that checks wether the siren was activated or not
        //plugin::patch::SetUChar(0x6FFDFC, 0x90);
        //plugin::patch::SetUChar(0x6FFDFD, 0x90);
        //plugin::patch::SetUChar(0x6FFDFE, 0x90);

        NSConfig::Init();

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

    static void Update()
    {
        bIsSamp = false;
        NSVehicles::CheckVehicles();
        NSKeys::Update();
    }

    static void ScriptUpdate()
    {
        Update();

        if (KeyPressed(83) && KeyPressed(16) && bIsSamp)
        {
            CMessages::AddMessageJumpQ("Voce precisa estar no modo OFFLINE para editar!", 1000, 0, false);
        }
    }

    static void UpdateVehicleControllers(CVehicle* vehicle)
    {
        NSVehicleController* vehicleController = NSVehicles::GetVehicleController(vehicle);
        vehicleController->Update();
    }

    static void Draw()
    {
        NSMenu::Draw();
    }

    NS() {
        Events::initGameEvent += Init;
        //Events::gameProcessEvent += Update;
        Events::processScriptsEvent += ScriptUpdate;
        Events::vehicleRenderEvent.before += UpdateVehicleControllers;
        Events::drawingEvent.after += Draw;
    }
} NS;
