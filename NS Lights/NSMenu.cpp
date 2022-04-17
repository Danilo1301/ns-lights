#include "NSMenu.h"
#include "NS.h"

#include "NSLangConfig.h"

using namespace plugin;

float NSMenu::x = 40.0f;
float NSMenu::y = 40.0f;
float NSMenu::width = 400.0f;
bool NSMenu::isOpen = false;
char NSMenu::title[256] = "";
int NSMenu::menu_editing_light = 0;
int NSMenu::onVehicleId = 0;
int NSMenu::SETTINGS_FREEZE_LIGHTS = 0;
CRGBA NSMenu::color_title = CRGBA(11, 54, 97, 255);
std::list<NSMenuItem*> ListMenuItems;

int menu_selection_index = 0;
int MAX_LIGHTS_PER_VEHICLE = 99999;

void NSMenu::ToggleMenu()
{


	if (!KeyPressed(17)) { return; }

	if (KeyPressed(16)) {

		NS::m_ShowDebug = !NS::m_ShowDebug;
		CMessages::AddMessageJumpQ(NS::m_ShowDebug ? "Debug enabled" : "Debug disabled", 1000, 0, false);

		

		return;
	}

	if (isOpen)
	{
		isOpen = false;
	}
	else {
		CVehicle* veh = FindPlayerVehicle(0, false);
		if (!(veh > 0))
		{
			NSLangConfig::PrintString("need_vehicle", 1000);
			return;
		}

		onVehicleId = veh->m_nModelIndex;
		isOpen = true;
		ShowMenuPrincipal();
	}
	NSConfig::SaveConfig();
}

void NSMenu::Draw()
{
	if (!isOpen) { return; }

	NSDraw::SetOrientation(ALIGN_CENTER);
	NSDraw::MDrawRect(CRect(x, y, x + width, y + 40.0f), color_title);
	NSDraw::MDrawText(title, x + width / 2, y + 10.0f, CRGBA(255, 255, 255, 255));

	int n = 0;
	std::list<NSMenuItem*>::iterator item;
	for (item = ListMenuItems.begin(); item != ListMenuItems.end(); ++item)
	{
		(*item)->isActive = (menu_selection_index == n);
		(*item)->DrawAt(x, y + 40.0f + (n * 30.0f), width);
		n++;
	}
}

void NSMenu::DeleteMenuItems() {
	ListMenuItems.clear();
	menu_selection_index = 0;
}

void NSMenu::SetMenuTitle(const char* newTitle) {
	sprintf(title, "%s", newTitle);
}

NSMenuItem* NSMenu::CreateMenuItemLabel(const char* text)
{
	NSMenuItem* item = new NSMenuItem(eMenuItemType::ITEM_LABEL, text, 0.0f);
	ListMenuItems.push_back(item);
	return item;
}

NSMenuItem* NSMenu::CreateMenuItemButton(const char* text, std::function<void()> callback)
{
	NSMenuItem* item = new NSMenuItem(eMenuItemType::ITEM_BUTTON, text, 0.0f);
	item->btn_callback = callback;
	ListMenuItems.push_back(item);
	return item;

}
NSMenuItem* NSMenu::CreateMenuItemOption(const char* text, float displayWidth, int &store_to)
{
	NSMenuItem* item = new NSMenuItem(eMenuItemType::ITEM_OPTION, text, displayWidth);
	item->CreateOptionValueBinder(store_to);
	ListMenuItems.push_back(item);
	return item;
}

NSMenuItem* NSMenu::CreateMenuItemFloat(const char* text, float displayWidth, float min, float max, float &store_to)
{
	NSMenuItem* item = new NSMenuItem(eMenuItemType::ITEM_FLOAT, text, displayWidth);
	item->CreateValueBinder(store_to, min, max);
	ListMenuItems.push_back(item);
	return item;
}

NSMenuItem* NSMenu::CreateMenuItemInt(const char* text, float displayWidth, int min, int max, int &store_to)
{
	NSMenuItem* item = new NSMenuItem(eMenuItemType::ITEM_INT, text, displayWidth);
	item->CreateValueBinder(store_to, min, max);
	ListMenuItems.push_back(item);
	return item;
}

NSMenuItem* NSMenu::GetMenuItemAt(int index)
{
	int n = 0;
	std::list<NSMenuItem*>::iterator item;
	for (item = ListMenuItems.begin(); item != ListMenuItems.end(); ++item)
	{
		if (index == n) {
			return (*item);
		}
		n++;
	}
}

void NSMenu::OnUpPressed()
{
	if (!isOpen) { return; }

	if (menu_selection_index > 0) {
		menu_selection_index -= 1;
		Command<Commands::ADD_ONE_OFF_SOUND, float, float, float, int>(0.0, 0.0, 0.0, 1083);
	}
	else {
		if ((int)ListMenuItems.size() > 1) {
			menu_selection_index = (int)ListMenuItems.size() - 1;
			Command<Commands::ADD_ONE_OFF_SOUND, float, float, float, int>(0.0, 0.0, 0.0, 1083);
		}
	}
}

void NSMenu::OnDownPressed()
{
	if (!isOpen) { return; }

	if (menu_selection_index < (int)ListMenuItems.size()-1) {
		menu_selection_index += 1;
		Command<Commands::ADD_ONE_OFF_SOUND, float, float, float, int>(0.0, 0.0, 0.0, 1083);
	}
	else {
		if ((int)ListMenuItems.size() > 1) {
			menu_selection_index = 0;
			Command<Commands::ADD_ONE_OFF_SOUND, float, float, float, int>(0.0, 0.0, 0.0, 1083);
		}
	}
}

void NSMenu::OnLeftPressed()
{
	if (!isOpen) { return; }

	NSMenuItem* item = NSMenu::GetMenuItemAt(menu_selection_index);
	if (item->type == eMenuItemType::ITEM_OPTION)
	{
		item->optionValueBinder->ChangeBy(-1);
		Command<Commands::ADD_ONE_OFF_SOUND, float, float, float, int>(0.0, 0.0, 0.0, 1084);
	}
}

void NSMenu::OnLeftHold()
{
	if (!isOpen) { return; }

	NSMenuItem* item = NSMenu::GetMenuItemAt(menu_selection_index);
	if (item->type == eMenuItemType::ITEM_FLOAT)
	{
		item->valueBinder_float->ChangeBy(-0.01f);
	}
	if (item->type == eMenuItemType::ITEM_INT)
	{
		item->valueBinder_int->ChangeBy(-1);
	}
}

void NSMenu::OnRightPressed()
{
	if (!isOpen) { return; }

	NSMenuItem* item = NSMenu::GetMenuItemAt(menu_selection_index);
	if (item->type == eMenuItemType::ITEM_OPTION)
	{
		Command<Commands::ADD_ONE_OFF_SOUND, float, float, float, int>(0.0, 0.0, 0.0, 1084);
		item->optionValueBinder->ChangeBy(1);
	}
}

void NSMenu::OnRightHold()
{
	if (!isOpen) { return; }

	NSMenuItem* item = NSMenu::GetMenuItemAt(menu_selection_index);
	if (item->type == eMenuItemType::ITEM_FLOAT)
	{
		item->valueBinder_float->ChangeBy(0.01f);
	}
	if (item->type == eMenuItemType::ITEM_INT)
	{
		item->valueBinder_int->ChangeBy(1);
	}
}

void NSMenu::OnSpacePressed()
{
	if (!isOpen) { return; }

	NSMenuItem* item = NSMenu::GetMenuItemAt(menu_selection_index);
	if (item->type == eMenuItemType::ITEM_BUTTON) {
		Command<Commands::ADD_ONE_OFF_SOUND, float, float, float, int>(0.0, 0.0, 0.0, 1085);
		item->btn_callback();
	}
}

void NSMenu::ShowMenuPrincipal() {
	NSMenu::DeleteMenuItems();

	NSMenu::SetMenuTitle(NSLangConfig::GetLine("configure_lights"));

	char text[256];
	sprintf(text, NSLangConfig::GetLine("edit_vehicle"), onVehicleId, NSLights::GetNumberOfLightsOfVehicle(onVehicleId), MAX_LIGHTS_PER_VEHICLE);
	CreateMenuItemLabel(text);

	if (NSLights::LightDataExists(onVehicleId))
	{
		int lightsn = NSLights::GetNumberOfLightsOfVehicle(onVehicleId) - 1;

		for (int i = 0; i <= lightsn; i++)
		{
			NSLightData* lightData = NSLights::GetLightData(onVehicleId, i);

			char button_name[256];//NSLangConfig::GetText("edit_light")
			snprintf(button_name, 256, NSLangConfig::GetLine("edit_light"), i);
			NSMenuItem* btn = CreateMenuItemButton(button_name, NSMenu::EditLight);

			btn->CreateColorDisplay(lightData->red, lightData->green, lightData->blue, lightData->alpha);
			if (lightData->secondary_enabled)
			{
				btn->CreateColorDisplay(lightData->secondary_green, lightData->secondary_green, lightData->secondary_blue, lightData->secondary_alpha);
			}
		}
	}
	
	CreateMenuItemButton(NSLangConfig::GetLine("create_new_light"), NSMenu::CreateNewLight);
}

/*
void NSMenu::ShowMenuGeneralSettings() {
	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle("Configuracoes gerais");

	CreateMenuItemButton("VOLTAR", NSMenu::ShowMenuPrincipal);
}
*/

void NSMenu::CreateNewLight()
{
	if (NSLights::GetNumberOfLightsOfVehicle(onVehicleId) >= MAX_LIGHTS_PER_VEHICLE)
	{
		return NSLangConfig::PrintString("reached_lights_limit", 1000);
	}

	NSLightData* l = NSLights::AddLightData(onVehicleId);
	NSLangConfig::PrintString("new_light_created", 1000);
	NSMenu::ShowMenuPrincipal();
}

void NSMenu::ShowMenuEditingLight()
{
	NSLightData* lightData = NSLights::GetLightData(onVehicleId, menu_editing_light);

	char str[256];
	sprintf(str, NSLangConfig::GetLine("edit_light_title"), menu_editing_light);

	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle(str);

	NSMenuItem* item0 = CreateMenuItemOption(NSLangConfig::GetLine("freeze_lights"), 100.0f, SETTINGS_FREEZE_LIGHTS);
	item0->AddOption(0, NSLangConfig::GetLine("no"));
	item0->AddOption(1, NSLangConfig::GetLine("yes"));

	NSMenuItem* item1 = CreateMenuItemOption(NSLangConfig::GetLine("light_amount"), 100.0f, lightData->amount_of_lights);
	item1->AddOption(1, "1");
	item1->AddOption(2, "2");
	item1->AddOption(3, "3");
	item1->AddOption(4, "4");

	NSMenuItem* item2 = CreateMenuItemOption(NSLangConfig::GetLine("light_direction"), 100.0f, lightData->direction);
	item2->AddOption(0, NSLangConfig::GetLine("light_direction_front_back"));
	item2->AddOption(1, NSLangConfig::GetLine("light_direction_all"));

	CreateMenuItemFloat(NSLangConfig::GetLine("light_size"), 100.0f, 0.0f, 2.0f, lightData->size);
	CreateMenuItemButton(NSLangConfig::GetLine("light_position"), NSMenu::ShowMenuChangePosition);
	CreateMenuItemButton(NSLangConfig::GetLine("light_color_primary"), NSMenu::ShowMenuChangeColor);
	CreateMenuItemButton(NSLangConfig::GetLine("light_color_secondary"), NSMenu::ShowMenuChangeSecondaryColor);
	CreateMenuItemButton(NSLangConfig::GetLine("light_flash"), NSMenu::ShowMenuChangeBigFlash);

	NSMenuItem* item4 = CreateMenuItemOption(NSLangConfig::GetLine("light_type"), 100.0f, lightData->type);
	item4->AddOption(0, NSLangConfig::FormatLine("light_type_option", "%s %i", 1));
	item4->AddOption(3, NSLangConfig::FormatLine("light_type_option", "%s %i", 2));
	item4->AddOption(4, NSLangConfig::FormatLine("light_type_option", "%s %i", 3));
	item4->AddOption(9, NSLangConfig::FormatLine("light_type_option", "%s %i", 4));
	item4->AddOption(2, NSLangConfig::FormatLine("light_type_option", "%s %i", 5));

	CreateMenuItemFloat(NSLangConfig::GetLine("light_near_clip"), 100.0f, 0.0f, 2.0f, lightData->near_clip);

	NSMenuItem* item3 = CreateMenuItemOption(NSLangConfig::GetLine("light_pattern"), 100.0f, lightData->initial_pattern);
	item3->AddOption(0, NSLangConfig::FormatLine("light_pattern_option", "%s %i", 1));
	item3->AddOption(1, NSLangConfig::FormatLine("light_pattern_option", "%s %i", 2));
	item3->AddOption(2, NSLangConfig::FormatLine("light_pattern_option", "%s %i", 3));

	NSMenuItem* itemReflect = CreateMenuItemOption(NSLangConfig::GetLine("reflect_lights"), 100.0f, lightData->reflect);
	itemReflect->AddOption(0, NSLangConfig::GetLine("no"));
	itemReflect->AddOption(1, NSLangConfig::GetLine("yes"));

	CreateMenuItemButton(NSLangConfig::GetLine("remove_light"), NSMenu::ShowMenuConfirmRemoveLight);
	CreateMenuItemButton(NSLangConfig::GetLine("back"), NSMenu::ShowMenuPrincipal);
}

void NSMenu::EditLight()
{
	menu_editing_light = menu_selection_index - 1;

	ShowMenuEditingLight();
}

void NSMenu::ShowMenuConfirmRemoveLight()
{
	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle(NSLangConfig::GetLine("confirm_remove_light_title"));

	CreateMenuItemLabel(NSLangConfig::GetLine("confirm_remove_light_text"));
	CreateMenuItemButton(NSLangConfig::GetLine("confirm_remove_light_yes"), NSMenu::RemoveLight);
	CreateMenuItemButton(NSLangConfig::GetLine("confirm_remove_light_no"), NSMenu::ShowMenuEditingLight);
}

void NSMenu::RemoveLight()
{
	NSLights::RemoveLightData(onVehicleId, menu_editing_light);
	NSLangConfig::PrintString("light_removed", 1000);
	NSMenu::ShowMenuPrincipal();
}

void NSMenu::ShowMenuChangePosition()
{
	NSLightData* lightData = NSLights::GetLightData(onVehicleId, menu_editing_light);

	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle(NSLangConfig::GetLine("light_position_title"));

	CreateMenuItemFloat(NSLangConfig::GetLine("light_position_x"), 100.0f, -3.0f, 3.0f, lightData->pos_x);
	CreateMenuItemFloat(NSLangConfig::GetLine("light_position_distance"), 100.0f, -3.0f, 3.0f, lightData->light_distance);
	CreateMenuItemFloat(NSLangConfig::GetLine("light_position_y"), 100.0f, -8.0f, 8.0f, lightData->pos_y);
	CreateMenuItemFloat(NSLangConfig::GetLine("light_position_z"), 100.0f, -2.0f, 4.0f, lightData->pos_z);

	CreateMenuItemButton("VOLTAR", NSMenu::ShowMenuEditingLight);
}

void NSMenu::ShowMenuChangeColor()
{
	NSLightData* lightData = NSLights::GetLightData(onVehicleId, menu_editing_light);

	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle(NSLangConfig::GetLine("light_color_primary_title"));

	CreateMenuItemInt(NSLangConfig::GetLine("light_color_primary_r"), 100.0f, 0, 255, lightData->red);
	CreateMenuItemInt(NSLangConfig::GetLine("light_color_primary_g"), 100.0f, 0, 255, lightData->green);
	CreateMenuItemInt(NSLangConfig::GetLine("light_color_primary_b"), 100.0f, 0, 255, lightData->blue);
	CreateMenuItemInt(NSLangConfig::GetLine("light_color_primary_a"), 100.0f, 0, 255, lightData->alpha);

	CreateMenuItemButton(NSLangConfig::GetLine("back"), NSMenu::ShowMenuEditingLight);
}

void NSMenu::ShowMenuChangeSecondaryColor()
{
	NSLightData* lightData = NSLights::GetLightData(onVehicleId, menu_editing_light);

	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle(NSLangConfig::GetLine("light_color_secondary_title"));

	NSMenuItem* item1 = CreateMenuItemOption(NSLangConfig::GetLine("light_color_secondary_enable"), 100.0f, lightData->secondary_enabled);
	item1->AddOption(0, NSLangConfig::GetLine("no"));
	item1->AddOption(1, NSLangConfig::GetLine("yes"));

	CreateMenuItemInt(NSLangConfig::GetLine("light_color_secondary_r"), 100.0f, 0, 255, lightData->secondary_red);
	CreateMenuItemInt(NSLangConfig::GetLine("light_color_secondary_g"), 100.0f, 0, 255, lightData->secondary_green);
	CreateMenuItemInt(NSLangConfig::GetLine("light_color_secondary_b"), 100.0f, 0, 255, lightData->secondary_blue);
	CreateMenuItemInt(NSLangConfig::GetLine("light_color_secondary_a"), 100.0f, 0, 255, lightData->secondary_alpha);

	CreateMenuItemButton(NSLangConfig::GetLine("back"), NSMenu::ShowMenuEditingLight);
}

void NSMenu::ShowMenuChangeBigFlash()
{
	NSLightData* lightData = NSLights::GetLightData(onVehicleId, menu_editing_light);

	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle(NSLangConfig::GetLine("light_flash_title"));

	NSMenuItem* item1 = CreateMenuItemOption(NSLangConfig::GetLine("light_flash_enable"), 100.0f, lightData->big_light_enabled);
	item1->AddOption(0, NSLangConfig::GetLine("disabled"));
	item1->AddOption(1, NSLangConfig::GetLine("enabled"));

	CreateMenuItemFloat(NSLangConfig::GetLine("light_flash_size"), 100.0f, 2.0f, 8.0f, lightData->big_light_size);
	CreateMenuItemInt(NSLangConfig::GetLine("light_flash_intensity"), 100.0f, 0, 50, lightData->big_light_alpha);

	CreateMenuItemButton(NSLangConfig::GetLine("back"), NSMenu::ShowMenuEditingLight);
}