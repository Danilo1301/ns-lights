#include "NSMenu.h"
#include "NS.h"

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
int MAX_LIGHTS_PER_VEHICLE = 5;

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
			CMessages::AddMessageJumpQ("Voce precisa estar em um veiculo!", 1000, 0, false);
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

void NSMenu::SetMenuTitle(char* newTitle) {
	sprintf(title, "%s", newTitle);
}

NSMenuItem* NSMenu::CreateMenuItemLabel(char* text)
{
	NSMenuItem* item = new NSMenuItem(eMenuItemType::ITEM_LABEL, text, 0.0f);
	ListMenuItems.push_back(item);
	return item;
}

NSMenuItem* NSMenu::CreateMenuItemButton(char* text, std::function<void()> callback)
{
	NSMenuItem* item = new NSMenuItem(eMenuItemType::ITEM_BUTTON, text, 0.0f);
	item->btn_callback = callback;
	ListMenuItems.push_back(item);
	return item;

}
NSMenuItem* NSMenu::CreateMenuItemOption(char* text, float displayWidth, int &store_to)
{
	NSMenuItem* item = new NSMenuItem(eMenuItemType::ITEM_OPTION, text, displayWidth);
	item->CreateOptionValueBinder(store_to);
	ListMenuItems.push_back(item);
	return item;
}

NSMenuItem* NSMenu::CreateMenuItemFloat(char* text, float displayWidth, float min, float max, float &store_to)
{
	NSMenuItem* item = new NSMenuItem(eMenuItemType::ITEM_FLOAT, text, displayWidth);
	item->CreateValueBinder(store_to, min, max);
	ListMenuItems.push_back(item);
	return item;
}

NSMenuItem* NSMenu::CreateMenuItemInt(char* text, float displayWidth, int min, int max, int &store_to)
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
	NSMenu::SetMenuTitle("Configurar Luzes (NS)");

	char text[256];
	sprintf(text, "EDITANDO: ID %i ( %i / %i LUZES )", onVehicleId, NSLights::GetNumberOfLightsOfVehicle(onVehicleId), MAX_LIGHTS_PER_VEHICLE);
	CreateMenuItemLabel(text);

	if (NSLights::LightDataExists(onVehicleId))
	{
		int lightsn = NSLights::GetNumberOfLightsOfVehicle(onVehicleId) - 1;

		for (int i = 0; i <= lightsn; i++)
		{
			NSLightData* lightData = NSLights::GetLightData(onVehicleId, i);

			char button_name[256];
			snprintf(button_name, 256, "( EDITAR LUZ %i )", i);
			NSMenuItem* btn = CreateMenuItemButton(button_name, NSMenu::EditLight);

			btn->CreateColorDisplay(lightData->red, lightData->green, lightData->blue, lightData->alpha);
			if (lightData->secondary_enabled)
			{
				btn->CreateColorDisplay(lightData->secondary_green, lightData->secondary_green, lightData->secondary_blue, lightData->secondary_alpha);
			}
		}
	}
	
	CreateMenuItemButton("CRIAR NOVA LUZ", NSMenu::CreateNewLight);
}

void NSMenu::ShowMenuGeneralSettings() {
	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle("Configuracoes Gerais");

	CreateMenuItemButton("VOLTAR", NSMenu::ShowMenuPrincipal);
}

void NSMenu::CreateNewLight()
{
	if (NSLights::GetNumberOfLightsOfVehicle(onVehicleId) >= MAX_LIGHTS_PER_VEHICLE)
	{
		return CMessages::AddMessageJumpQ("Voce atingiu o limite de luzes neste veiculo!", 1000, 0, false);
	}

	NSLightData* l = NSLights::AddLightData(onVehicleId);
	CMessages::AddMessageJumpQ("Nova luz criada!", 1000, 0, false);
	NSMenu::ShowMenuPrincipal();
}

void NSMenu::ShowMenuEditingLight()
{
	NSLightData* lightData = NSLights::GetLightData(onVehicleId, menu_editing_light);

	char str[256];
	sprintf(str, "Editando luz %i", menu_editing_light);

	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle(str);

	NSMenuItem* item0 = CreateMenuItemOption("CONGELAR LUZES (DURANTE EDICAO)", 100.0f, SETTINGS_FREEZE_LIGHTS);
	item0->AddOption(0, "Nao");
	item0->AddOption(1, "Sim");

	NSMenuItem* item1 = CreateMenuItemOption("QUANTIDADE DE LUZES", 100.0f, lightData->amount_of_lights);
	item1->AddOption(1, "1");
	item1->AddOption(2, "2");
	item1->AddOption(3, "3");
	item1->AddOption(4, "4");

	NSMenuItem* item2 = CreateMenuItemOption("DIRECAO DAS LUZES", 100.0f, lightData->direction);
	item2->AddOption(0, "Frente ou Tras");
	item2->AddOption(1, "Todas as direcoes");

	CreateMenuItemFloat("TAMANHO", 100.0f, 0.0f, 2.0f, lightData->size);
	CreateMenuItemButton("ALTERAR POSICAO", NSMenu::ShowMenuChangePosition);
	CreateMenuItemButton("ALTERAR COR PRINCIPAL", NSMenu::ShowMenuChangeColor);
	CreateMenuItemButton("ALTERAR COR SECUNDARIA", NSMenu::ShowMenuChangeSecondaryColor);
	CreateMenuItemButton("ALTERAR FLASH MAIOR", NSMenu::ShowMenuChangeBigFlash);

	NSMenuItem* item4 = CreateMenuItemOption("TIPO DA LUZ", 100.0f, lightData->type);
	item4->AddOption(0, "Tipo 1");
	item4->AddOption(3, "Tipo 2");
	item4->AddOption(4, "Tipo 3");
	item4->AddOption(9, "Tipo 4");
	item4->AddOption(2, "Tipo 5");

	CreateMenuItemFloat("NEAR CLIP", 100.0f, 0.0f, 2.0f, lightData->near_clip);

	NSMenuItem* item3 = CreateMenuItemOption("PADRAO DAS LUZES", 100.0f, lightData->initial_pattern);
	item3->AddOption(0, "Padrao 1");
	item3->AddOption(1, "Padrao 2");
	item3->AddOption(2, "Padrao 3");

	CreateMenuItemButton("EXCLUIR LUZ", NSMenu::ShowMenuConfirmRemoveLight);
	CreateMenuItemButton("VOLTAR", NSMenu::ShowMenuPrincipal);
}

void NSMenu::EditLight()
{
	menu_editing_light = menu_selection_index - 1;

	ShowMenuEditingLight();
}

void NSMenu::ShowMenuConfirmRemoveLight()
{
	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle("Remover luz?");

	CreateMenuItemLabel("DESEJA REMOVER ESTA LUZ?");
	CreateMenuItemButton("SIM, REMOVER", NSMenu::RemoveLight);
	CreateMenuItemButton("NAO", NSMenu::ShowMenuEditingLight);
}

void NSMenu::RemoveLight()
{
	NSLights::RemoveLightData(onVehicleId, menu_editing_light);
	CMessages::AddMessageJumpQ("Luz removida!", 1000, 0, false);
	NSMenu::ShowMenuPrincipal();
}

void NSMenu::ShowMenuChangePosition()
{
	NSLightData* lightData = NSLights::GetLightData(onVehicleId, menu_editing_light);

	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle("Alterar Posicao");

	CreateMenuItemFloat("POSICAO X (lados)", 100.0f, -3.0f, 3.0f, lightData->pos_x);
	CreateMenuItemFloat("DISTANCIA ENTRE LUZES", 100.0f, -3.0f, 3.0f, lightData->light_distance);
	CreateMenuItemFloat("POSICAO Y (frente / tras)", 100.0f, -8.0f, 8.0f, lightData->pos_y);
	CreateMenuItemFloat("POSICAO Z (altura)", 100.0f, -2.0f, 4.0f, lightData->pos_z);

	CreateMenuItemButton("VOLTAR", NSMenu::ShowMenuEditingLight);
}

void NSMenu::ShowMenuChangeColor()
{
	NSLightData* lightData = NSLights::GetLightData(onVehicleId, menu_editing_light);

	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle("Alterar Cor");

	CreateMenuItemInt("R - VERMELHO", 100.0f, 0, 255, lightData->red);
	CreateMenuItemInt("G - VERDE", 100.0f, 0, 255, lightData->green);
	CreateMenuItemInt("B - AZUL", 100.0f, 0, 255, lightData->blue);
	CreateMenuItemInt("A - TRANSPARENCIA", 100.0f, 0, 255, lightData->alpha);

	CreateMenuItemButton("VOLTAR", NSMenu::ShowMenuEditingLight);
}

void NSMenu::ShowMenuChangeSecondaryColor()
{
	NSLightData* lightData = NSLights::GetLightData(onVehicleId, menu_editing_light);

	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle("Alterar Cor Secundaria");

	NSMenuItem* item1 = CreateMenuItemOption("USAR COR SECUNDARIA", 100.0f, lightData->secondary_enabled);
	item1->AddOption(0, "Nao");
	item1->AddOption(1, "Sim");

	CreateMenuItemInt("R - VERMELHO", 100.0f, 0, 255, lightData->secondary_red);
	CreateMenuItemInt("G - VERDE", 100.0f, 0, 255, lightData->secondary_green);
	CreateMenuItemInt("B - AZUL", 100.0f, 0, 255, lightData->secondary_blue);
	CreateMenuItemInt("A - TRANSPARENCIA", 100.0f, 0, 255, lightData->secondary_alpha);

	CreateMenuItemButton("VOLTAR", NSMenu::ShowMenuEditingLight);
}

void NSMenu::ShowMenuChangeBigFlash()
{
	NSLightData* lightData = NSLights::GetLightData(onVehicleId, menu_editing_light);

	NSMenu::DeleteMenuItems();
	NSMenu::SetMenuTitle("Alterar Flash maior");

	NSMenuItem* item1 = CreateMenuItemOption("ATIVAR FLASH MAIOR", 100.0f, lightData->big_light_enabled);
	item1->AddOption(0, "Desativado");
	item1->AddOption(1, "Ativado");

	CreateMenuItemFloat("TAMANHO", 100.0f, 2.0f, 8.0f, lightData->big_light_size);
	CreateMenuItemInt("INTENSIDADE", 100.0f, 0, 50, lightData->big_light_alpha);

	CreateMenuItemButton("VOLTAR", NSMenu::ShowMenuEditingLight);
}