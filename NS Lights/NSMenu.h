#pragma once

#include "plugin.h"

#include "CMessages.h"

#include "NSDraw.h"
#include "NSMenuItem.h"
#include "NSConfig.h"
#include "NSLightData.h"

#include "extensions/ScriptCommands.h"
#include "eMenuItemType.h"

class NSMenu {
public:
	static float x;
	static float y;
	static float width;

	static bool isOpen;

	static CRGBA color_title;

	static char title[256];

	static int onVehicleId;
	static int menu_editing_light;
	static int SETTINGS_FREEZE_LIGHTS;

	static void ToggleMenu();
	static void Draw();
	static void DeleteMenuItems();
	static void SetMenuTitle(char* newTitle);
	static NSMenuItem* NSMenu::GetMenuItemAt(int index);

	static NSMenuItem* CreateMenuItemLabel(char* text);
	static NSMenuItem* CreateMenuItemButton(char* text, std::function<void()> callback);
	static NSMenuItem* CreateMenuItemOption(char* text, float displayWidth, int& store_to);
	static NSMenuItem* CreateMenuItemFloat(char* text, float displayWidth, float min, float max, float& store_to);
	static NSMenuItem* CreateMenuItemInt(char* text, float displayWidth, int min, int max, int& store_to);

	static void OnUpPressed();
	static void OnDownPressed();
	static void OnLeftPressed();
	static void OnLeftHold();
	static void OnRightPressed();
	static void OnRightHold();
	static void OnSpacePressed();

	static void ShowMenuPrincipal();
	static void ShowMenuGeneralSettings();
	static void CreateNewLight();
	static void EditLight();
	static void ShowMenuEditingLight();
	static void ShowMenuConfirmRemoveLight();
	static void RemoveLight();
	static void ShowMenuChangePosition();
	static void ShowMenuChangeColor();
	static void ShowMenuChangeSecondaryColor();
	static void ShowMenuChangeBigFlash();
};

