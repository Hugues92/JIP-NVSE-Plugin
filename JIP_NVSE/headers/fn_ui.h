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
DEFINE_COMMAND_PLUGIN(GetSelectedItemRef, "None", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetBarterItems, "None", 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetBarterGoldAlt, "None", 0, 0, NULL);

Tile *GetTargetComponent(const char *componentPath, TileMenu **parentMenu = NULL)
{
	const char *slashPos = strpbrk(componentPath, "\\/");
	std::string menuName("&");
	if (slashPos) menuName.append(componentPath, slashPos - componentPath);
	else menuName.append(componentPath);
	menuName.append(";");

	UInt32 menuType = Tile::TraitNameToID(menuName.data());
	if ((menuType >= kMenuType_Min) && (menuType <= kMenuType_Max))
	{
		Tile *component = g_TileMenuArray->Get(menuType - kMenuType_Min);
		if (component)
		{
			if (parentMenu) *parentMenu = (TileMenu*)component;
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
		if (InterfaceManager::GetMenuComponentValue(componentPath) || GetTargetComponent(componentPath)) *result = 1;
	}
	if (IsConsoleMode()) Console_Print("Component Loaded >> %.0f", *result);
	return true;
}

bool InjectXML(Tile *component, const char *xmlPath)
{
	return ThisStdCall(s_InjectXML, component, xmlPath) ? true : false;
}

bool Cmd_InjectUIXML_Execute(COMMAND_ARGS)
{
	*result = 0;
	char componentPath[512] = {0};
	char xmlPath[512] = {0};

	if (ExtractArgs(EXTRACT_ARGS, &componentPath, &xmlPath))
	{
		Tile *component = GetTargetComponent(componentPath);
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
		Tile *component = GetTargetComponent(componentPath);
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
	InterfaceManager *intfc = *g_interfaceManager;
	if (intfc && intfc->activeMenu) *result = intfc->activeMenu->id;
	return true;
}

bool Cmd_GetActiveUIComponentName_Execute(COMMAND_ARGS)
{
	InterfaceManager *intfc = *g_interfaceManager;
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
	InterfaceManager *intfc = *g_interfaceManager;
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
	InterfaceManager *intfc = *g_interfaceManager;
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

	if (!IsMenuMode() || !ExtractArgs(EXTRACT_ARGS, &menuID)) return true;
	TileMenu *tileMenu = g_TileMenuArray->Get(menuID - kMenuType_Min);
	if (!tileMenu || !tileMenu->menu) return true;
	TESObjectREFR *menuRef = NULL;
	if (menuID == 1008)
		menuRef = ((ContainerMenu*)tileMenu->menu)->containerRef;
	else if (menuID == 1009)
		menuRef = ((DialogMenu*)tileMenu->menu)->partnerRef;
	else if (menuID == 1014)
		menuRef = ((LockPickMenu*)tileMenu->menu)->targetRef;
	else if (menuID == 1053)
		menuRef = ((BarterMenu*)tileMenu->menu)->merchantRef;
	else if (menuID == 1055)
		menuRef = ((HackingMenu*)tileMenu->menu)->targetRef;
	else if (menuID == 1056)
		menuRef = ((VATSMenu*)tileMenu->menu)->targetRef;
	else if (menuID == 1057)
		menuRef = ((ComputersMenu*)tileMenu->menu)->targetRef;
	else if (menuID == 1075)
		menuRef = ((CompanionWheelMenu*)tileMenu->menu)->companionRef;
	if (menuRef->TryGetREFRParent()) *refResult = menuRef->refID;
	return true;
}

bool Cmd_GetMenuItemFilter_Execute(COMMAND_ARGS)
{
	*result = -1;
	UInt32 menuID = 0, useRef = 0;

	if (!IsMenuMode() || !ExtractArgs(EXTRACT_ARGS, &menuID, &useRef)) return true;
	TileMenu *tileMenu = g_TileMenuArray->Get(menuID - kMenuType_Min);
	if (!tileMenu || !tileMenu->menu) return true;
	if (menuID == 1002)
		*result = ((InventoryMenu*)tileMenu->menu)->filter + 1;
	else if (menuID == 1008)
		*result = useRef ? ((ContainerMenu*)tileMenu->menu)->rightFilter : ((ContainerMenu*)tileMenu->menu)->leftFilter;
	else if (menuID == 1053)
		*result = useRef ? ((BarterMenu*)tileMenu->menu)->rightFilter : ((BarterMenu*)tileMenu->menu)->leftFilter;
	return true;
}

bool Cmd_ClickMenuButton_Execute(COMMAND_ARGS)
{
	*result = 0;
	char componentPath[512] = {0};

	if (IsMenuMode() && ExtractArgs(EXTRACT_ARGS, &componentPath))
	{
		TileMenu *parentMenu;
		Tile *component = GetTargetComponent(componentPath, &parentMenu);
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

bool Cmd_GetSelectedItemRef_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;

	if (!IsMenuMode() || !s_nvseInvRefGetRef) return true;
	InterfaceManager *intfc = *g_interfaceManager;
	if (!intfc || !intfc->activeMenu) return true;
	UInt32 menuID = intfc->activeMenu->id;
	TESObjectREFR *target = NULL, *itemRef = NULL;

	if (menuID == 1002)
		itemRef = CreateRefForStack(*g_thePlayer, *g_inventoryMenuSelection);
	else if (menuID == 1008)
	{
		ContainerMenu *conMenu = (ContainerMenu*)intfc->activeMenu;
		if (conMenu->rightActive)
		{
			target = conMenu->containerRef;
			if (!target) return true;
		}
		itemRef = CreateRefForStack(target ? target : *g_thePlayer, *g_containerMenuSelection);
	}
	else if (menuID == 1053)
	{
		BarterMenu *brtMenu = (BarterMenu*)intfc->activeMenu;
		if (brtMenu->rightActive)
		{
			target = GetMerchantContainer(brtMenu->merchantRef);
			if (!target) return true;
		}
		itemRef = CreateRefForStack(target ? target : *g_thePlayer, *g_barterMenuSelection);
	}
	if (itemRef) *refResult = itemRef->refID;
	return true;
}

bool Cmd_GetBarterItems_Execute(COMMAND_ARGS)
{
	NVSEArrayVarInterface::Array *barterItems = g_array->CreateArray(NULL, 0, scriptObj);
	g_array->AssignCommandResult(barterItems, result);

	UInt32 sold = 0;
	if (!IsMenuMode() || !s_nvseInvRefGetRef || !ExtractArgs(EXTRACT_ARGS, &sold)) return true;

	TileMenu *tileMenu = g_TileMenuArray->Get(52);
	if (!tileMenu || !tileMenu->menu) return true;

	BarterMenu *brtMenu = (BarterMenu*)tileMenu->menu;

	TESObjectREFR *target, *itemRef;
	BarterItemList *barterList;
	if (sold)
	{
		target = GetMerchantContainer(brtMenu->merchantRef);
		if (!target) return true;
		barterList = &brtMenu->rightBarter;
	}
	else
	{
		target = *g_thePlayer;
		barterList = &brtMenu->leftBarter;
	}
	NVSEArrayVarInterface::Element *elem;
	for (auto iter = barterList->Begin(); !iter.End(); ++iter)
	{
		if (itemRef = CreateRefForStack(target, iter.Get()))
		{
			elem = new NVSEArrayVarInterface::Element(itemRef);
			g_array->AppendElement(barterItems, *elem);
		}
	}
	return true;
}

bool Cmd_GetBarterGoldAlt_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (IsMenuMode())
	{
		TileMenu *tileMenu = g_TileMenuArray->Get(52);
		if (tileMenu && tileMenu->menu) *result = ((BarterMenu*)tileMenu->menu)->merchantGold;
	}
	return true;
}
