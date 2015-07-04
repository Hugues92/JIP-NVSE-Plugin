#pragma once

DEFINE_COMMAND_PLUGIN(AddItemAlt, "None", 1, 3, kParams_JIP_OneObjectIDorList_OneInt_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(GetValueAlt, "None", 0, 1, kParams_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(SetValueAlt, "None", 0, 2, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(RemoveItemTarget, "None", 1, 3, kParams_JIP_OneItemOrList_OneObjectRef_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetWeaponRefModFlags, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetWeaponRefModFlags, "None", 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetItemRefCurrentHealth, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetItemRefCurrentHealth, "None", 1, 1, kParams_OneFloat);

void AddItemAlt(TESObjectREFR *target, TESForm *item, UInt32 count, float condition)
{
	ExtraDataList *xData = NULL;
	ExtraScript *xScript = NULL;
	
	TESScriptableForm *pScript = DYNAMIC_CAST(item, TESForm, TESScriptableForm);
	if (pScript && !pScript->script) pScript = NULL;

	TESHealthForm *pHealth = DYNAMIC_CAST(item, TESForm, TESHealthForm);
	if (pHealth)
	{
		ExtraHealth *xHealth = NULL;
		condition *= pHealth->GetHealth();
		while (count--)
		{
			xData = ExtraDataList::Create();
			if (xHealth = ExtraHealth::Create())
			{
				xHealth->health = condition;
				xData->Add(xHealth);
			}
			if (pScript && (xScript = ExtraScript::Create(item))) xData->Add(xScript);
			target->AddItem(item, xData, 1);
		}
	}
	else
	{
		if (pScript && (xScript = ExtraScript::Create(item)))
		{
			xData = ExtraDataList::Create();
			xData->Add(xScript);
		}
		target->AddItem(item, xData, count);
	}
}

bool Cmd_AddItemAlt_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	UInt32 count = 0;
	float condition = 100.0;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &form, &count, &condition) && (count > 0))
	{
		if (condition > 100) condition = 1.0;
		else if (condition < 0) condition = 0.0;
		else condition /= 100;
		BGSListForm *lst = DYNAMIC_CAST(form, TESForm, BGSListForm);
		if (lst)
		{
			for (UInt32 idx = 0; idx < lst->Count(); idx++)
			{
				if ((form = lst->GetNthForm(idx)) && form->IsInventoryObject()) AddItemAlt(thisObj, form, count, condition);
			}
		}
		else AddItemAlt(thisObj, form, count, condition);
	}
	return true;
}

bool Cmd_GetValueAlt_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;

	if (thisObj)
	{
		form = thisObj->baseForm;
		if (!form || !form->IsInventoryObject()) return true;
	}
	else if (!ExtractArgs(EXTRACT_ARGS, &form) || !form) return true;

	TESValueForm *valForm = DYNAMIC_CAST(form, TESForm, TESValueForm);
	if (valForm) *result = valForm->value;
	else
	{
		AlchemyItem *ingestible = DYNAMIC_CAST(form, TESForm, AlchemyItem);
		if (ingestible) *result = ingestible->value;
	}
	return true;
}

bool Cmd_SetValueAlt_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 newVal = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &newVal))
	{
		TESValueForm *valForm = DYNAMIC_CAST(form, TESForm, TESValueForm);
		if (valForm)
		{
			*result = valForm->value;
			valForm->value = newVal;
			return true;
		}
		AlchemyItem *ingestible = DYNAMIC_CAST(form, TESForm, AlchemyItem);
		if (ingestible)
		{
			*result = ingestible->value;
			ingestible->value = newVal;
		}
	}
	return true;
}

std::unordered_set<TESForm*> g_processedForms;

void RemoveItemTarget(TESObjectREFR *source, TESContainer *cont, ExtraContainerChanges *xChanges, TESForm *form, TESObjectREFR *target, SInt32 quantity)
{
	if (!form || g_processedForms.count(form)) return;
	g_processedForms.insert(form);

	if (form->IsInventoryObject())
	{
		SInt32 total = 0;
		TESContainer::FormCount *found = cont->formCountList.Find(ContainerFindType(form));
		if (found && (found->count > 0)) total = found->count;
		if (xChanges && xChanges->data && xChanges->data->objList)
		{
			ExtraContainerChanges::EntryData *entry = xChanges->data->objList->Find(ItemInEntryDataListMatcher(form));
			if (entry) total += entry->countDelta;
		}
		if (total > 0)
		{
			if ((quantity > 0) && (quantity < total)) total = quantity;
			source->RemoveItem(form, NULL, total, 0, 0, target, 0, 0, 1, 0);
		}
	}
	else if (form->typeID == kFormType_ListForm)
	{
		BGSListForm *lst = DYNAMIC_CAST(form, TESForm, BGSListForm);
		if (lst)
		{
			for (UInt32 idx = 0; idx < lst->Count(); idx++)
				RemoveItemTarget(source, cont, xChanges, lst->GetNthForm(idx), target, quantity);
		}
	}
}

bool Cmd_RemoveItemTarget_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	TESObjectREFR *target = NULL;
	SInt32 quantity = 0;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &form, &target, &quantity))
	{
		TESContainer *cont = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESContainer);
		if (cont && DYNAMIC_CAST(target->baseForm, TESForm, TESContainer))
		{
			ExtraContainerChanges *xChanges = (ExtraContainerChanges*)GetByTypeCast(thisObj->extraDataList, ContainerChanges);
			RemoveItemTarget(thisObj, cont, xChanges, form, target, quantity);
			g_processedForms.clear();
		}
	}
	return true;
}

bool Cmd_GetWeaponRefModFlags_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj)
	{
		InventoryRef *iref = InventoryRefGetForID(thisObj->refID);
		if (!iref || !iref->data.xData) return true;
		ExtraWeaponModFlags *xModFlags = (ExtraWeaponModFlags*)iref->data.xData->GetByType(kExtraData_WeaponModFlags);
		if (xModFlags) *result = xModFlags->flags;
	}
	return true;
}

bool Cmd_SetWeaponRefModFlags_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 flags = 0;

	if (!thisObj || !ExtractArgs(EXTRACT_ARGS, &flags) || !ValidInput(flags, 7)) return true;
	InventoryRef *iref = InventoryRefGetForID(thisObj->refID);
	if (!iref || !iref->data.type || (iref->data.type->typeID != 40) || !iref->data.entry) return true;
	ExtraWeaponModFlags *xModFlags;
	if (!iref->data.xData)
	{
		if (flags)
		{
			xModFlags = ExtraWeaponModFlags::Create();
			xModFlags->flags = flags;
			if (iref->CreateExtraData(xModFlags)) *result = 1;
			else FormHeap_Free(xModFlags);
		}
		return true;
	}
	xModFlags = (ExtraWeaponModFlags*)iref->data.xData->GetByType(kExtraData_WeaponModFlags);
	if (!flags)
	{
		if (xModFlags)
		{
			iref->data.xData->Remove(xModFlags);
			FormHeap_Free(xModFlags);
		}
		*result = 1;
		return true;
	}
	if (!xModFlags)
	{
		xModFlags = ExtraWeaponModFlags::Create();
		if (!iref->data.xData->Add(xModFlags))
		{
			FormHeap_Free(xModFlags);
			return true;
		}
	}
	xModFlags->flags = flags;
	*result = 1;
	return true;
}

bool Cmd_GetItemRefCurrentHealth_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj)
	{
		InventoryRef *iref = InventoryRefGetForID(thisObj->refID);
		if (!iref || !iref->data.type) return true;
		TESHealthForm *healthForm = DYNAMIC_CAST(iref->data.type, TESForm, TESHealthForm);
		if (!healthForm) return true;
		if (iref->data.xData)
		{
			ExtraHealth *xHealth = (ExtraHealth*)iref->data.xData->GetByType(kExtraData_Health);
			if (xHealth)
			{
				*result = xHealth->health;
				return true;
			}
		}
		*result = healthForm->health;
	}
	return true;
}

bool Cmd_SetItemRefCurrentHealth_Execute(COMMAND_ARGS)
{
	*result = 0;
	float health = 0;

	if (!thisObj || !ExtractArgs(EXTRACT_ARGS, &health)) return true;
	InventoryRef *iref = InventoryRefGetForID(thisObj->refID);
	if (!iref || !iref->data.type || !iref->data.entry) return true;
	TESHealthForm *healthForm = DYNAMIC_CAST(iref->data.type, TESForm, TESHealthForm);
	if (!healthForm) return true;
	ExtraHealth *xHealth;
	if (!iref->data.xData)
	{
		if (health < healthForm->health)
		{
			xHealth = ExtraHealth::Create();
			xHealth->health = health;
			if (iref->CreateExtraData(xHealth)) *result = 1;
			else FormHeap_Free(xHealth);
		}
		return true;
	}
	xHealth = (ExtraHealth*)iref->data.xData->GetByType(kExtraData_Health);
	if (health >= healthForm->health)
	{
		if (xHealth)
		{
			iref->data.xData->Remove(xHealth);
			FormHeap_Free(xHealth);
		}
		return true;
	}
	if (!xHealth)
	{
		xHealth = ExtraHealth::Create();
		if (!iref->data.xData->Add(xHealth))
		{
			FormHeap_Free(xHealth);
			return true;
		}
	}
	xHealth->health = health;
	*result = 1;
	return true;
}
