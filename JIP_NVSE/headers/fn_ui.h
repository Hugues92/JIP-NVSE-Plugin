#pragma once

DEFINE_COMMAND_PLUGIN(IsComponentLoaded, "None", 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(InjectUIXML, "None", 0, 2, kParams_TwoStrings);
DEFINE_COMMAND_PLUGIN(InjectUIComponent, "None", 0, 22, kParams_JIP_OneString_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetCursorPos, "None", 0, 1, kParams_Axis);
DEFINE_COMMAND_PLUGIN(GetUIString, "None", 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(GetActiveMenuMode, "None", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActiveUIComponentName, "None", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActiveUIComponentFullName, "None", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActiveUIComponentID, "None", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetMenuTargetRef, "None", 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMenuItemFilter, "None", 0, 2, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ClickMenuButton, "None", 0, 1, kParams_OneString);

NiTArray<TileMenu*>* g_tileMenuArray = (NiTArray<TileMenu*>*)0x11F3508;

Tile *GetTargetComponent(const char *componentPath, TileMenu *&parentMenu)
{
	const char *slashPos = strpbrk(componentPath, "\\/");
	std::string menuName("&");
	if (slashPos) menuName.append(componentPath, slashPos - componentPath);
	else menuName.append(componentPath);
	menuName.append(";");

	UInt32 menuType = Tile::TraitNameToID(menuName.data());
	if ((menuType >= kMenuType_Min) && (menuType <= kMenuType_Max))
	{
		Tile *component = g_tileMenuArray->Get(menuType - kMenuType_Min);
		if (component)
		{
			parentMenu = (TileMenu*)component;
			if (slashPos) component = component->GetComponentTile(slashPos + 1);
			return component;
		}
	}
	return NULL;
}

bool Cmd_IsComponentLoaded_Execute(COMMAND_ARGS)
{
	*result = 0;
	char componentPath[512] = {0};

	if (ExtractArgs(EXTRACT_ARGS, &componentPath))
	{
		TileMenu *parentMenu;
		if (InterfaceManager::GetMenuComponentValue(componentPath) || GetTargetComponent(componentPath, parentMenu)) *result = 1;
	}
	if (IsConsoleMode()) Console_Print("Component Loaded >> %.0f", *result);
	return true;
}

bool InjectXML(Tile *component, const char *xmlPath)
{
#if RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525
	if (ThisStdCall(0xA01B00, component, xmlPath)) return true;
#elif RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525ng
	if (ThisStdCall(0xA019D0, component, xmlPath)) return true;
#endif
	return false;
}

bool Cmd_InjectUIXML_Execute(COMMAND_ARGS)
{
	*result = 0;
	char componentPath[512] = {0};
	char xmlPath[512] = {0};

	if (ExtractArgs(EXTRACT_ARGS, &componentPath, &xmlPath))
	{
		TileMenu *parentMenu;
		Tile *component = GetTargetComponent(componentPath, parentMenu);
		if (component) *result = InjectXML(component, xmlPath);
	}
	return true;
}

std::string g_tempFilePath = g_gamePath + "jip_temp.xml";

bool Cmd_InjectUIComponent_Execute(COMMAND_ARGS)
{
	*result = 0;
	char componentPath[512] = {0};
	char tileString[kMaxMessageLength] = {0};

	if (ExtractFormatStringArgs(1, tileString, paramInfo, scriptData, opcodeOffsetPtr, scriptObj, eventList, kCommandInfo_InjectUIComponent.numParams, &componentPath))
	{
		TileMenu *parentMenu;
		Tile *component = GetTargetComponent(componentPath, parentMenu);
		if (component)
		{
			IFileStream tempFile;
			if (tempFile.Create(g_tempFilePath.data()))
			{
				tempFile.WriteString(tileString);
				tempFile.Close();
				*result = InjectXML(component, "jip_temp.xml");
				remove(g_tempFilePath.data());
			}
		}
	}
	return true;
}

typedef bool (*_IsMenuMode)(void);
#if RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525
const _IsMenuMode IsMenuMode = (_IsMenuMode)0x702360;
#elif RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525ng
const _IsMenuMode IsMenuMode = (_IsMenuMode)0x7022C0;
#endif

bool Cmd_GetCursorPos_Execute(COMMAND_ARGS)
{
	*result = 0;
	char axis = 0;

	if (IsMenuMode() && ExtractArgs(EXTRACT_ARGS, &axis))
	{
		POINT mouseCoords;
		if (GetCursorPos(&mouseCoords))
		{
			if (axis < 'x') axis += 32;
			if (axis == 'x') *result = mouseCoords.x;
			else if (axis == 'y') *result = mouseCoords.y;
		}
	}
	return true;
}

bool Cmd_GetUIString_Execute(COMMAND_ARGS)
{
	char component[512] = {0};

	if (ExtractArgs(EXTRACT_ARGS, &component))
	{
		Tile::Value *val = InterfaceManager::GetMenuComponentValue(component);
		if (val && val->str)
		{
			g_string->Assign(PASS_COMMAND_ARGS, val->str);
			return true;
		}
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");

	return true;
}

bool Cmd_GetActiveMenuMode_Execute(COMMAND_ARGS)
{
	*result = 0;
	InterfaceManager *intfc = InterfaceManager::GetSingleton();
	if (intfc && intfc->activeMenu) *result = intfc->activeMenu->id;
	return true;
}

bool Cmd_GetActiveUIComponentName_Execute(COMMAND_ARGS)
{
	InterfaceManager *intfc = InterfaceManager::GetSingleton();
	if (intfc && intfc->activeTile)
	{
		g_string->Assign(PASS_COMMAND_ARGS, intfc->activeTile->name.CStr());
		return true;
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");
	return true;
}

bool Cmd_GetActiveUIComponentFullName_Execute(COMMAND_ARGS)
{
	InterfaceManager *intfc = InterfaceManager::GetSingleton();
	if (intfc && intfc->activeTile)
	{
		g_string->Assign(PASS_COMMAND_ARGS, intfc->activeTile->GetQualifiedName().data());
		return true;
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");
	return true;
}

bool Cmd_GetActiveUIComponentID_Execute(COMMAND_ARGS)
{
	*result = -1;
	InterfaceManager *intfc = InterfaceManager::GetSingleton();
	if (intfc && intfc->activeTile)
	{
		Tile::Value *val = intfc->activeTile->GetValue(4010);
		if (val) *result = val->num;
	}
	return true;
}

bool Cmd_GetMenuTargetRef_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, menuID = 0;
	*refResult = 0;

	if (IsMenuMode() && ExtractArgs(EXTRACT_ARGS, &menuID))
	{
		UInt32 refOffset;
		if (menuID == 1008) refOffset = 29;
		else if ((menuID == 1009) || (menuID == 1053)) refOffset = 32;
		else if (menuID == 1014) refOffset = 27;
		else if (menuID == 1055) refOffset = 102;
		else if (menuID == 1057) refOffset = 43;
		else if (menuID == 1075) refOffset = 26;
		else return true;

		TileMenu *tileMenu = g_tileMenuArray->Get(menuID - kMenuType_Min);
		if (tileMenu && tileMenu->menu)
		{
			TESForm *menuRef = (TESForm*)*((UInt32*)tileMenu->menu + refOffset);
			if (menuRef->TryGetREFRParent()) *refResult = menuRef->refID;
		}
	}
	return true;
}

bool Cmd_GetMenuItemFilter_Execute(COMMAND_ARGS)
{
	*result = -1;
	UInt32 menuID = 0, useRef = 0;

	if (IsMenuMode() && ExtractArgs(EXTRACT_ARGS, &menuID, &useRef))
	{
		UInt32 valOffset;
		if (menuID == 1002) valOffset = 33;
		else if (menuID == 1008) valOffset = 35 + (useRef != 0);
		else if (menuID == 1053) valOffset = 39 + (useRef != 0);
		else return true;

		TileMenu *tileMenu = g_tileMenuArray->Get(menuID - kMenuType_Min);
		if (tileMenu && tileMenu->menu) *result = *((UInt32*)tileMenu->menu + valOffset) + (menuID == 1002);
	}
	return true;
}

bool Cmd_ClickMenuButton_Execute(COMMAND_ARGS)
{
	*result = 0;
	char componentPath[512] = {0};

	if (IsMenuMode() && ExtractArgs(EXTRACT_ARGS, &componentPath))
	{
		TileMenu *parentMenu;
		Tile *component = GetTargetComponent(componentPath, parentMenu);
		if (component)
		{
			Tile::Value *val = component->GetValue(4010);
			if (val)
			{
				parentMenu->menu->HandleClick(val->num, component);
				*result = 1;
			}
		}
	}
	return true;
}
