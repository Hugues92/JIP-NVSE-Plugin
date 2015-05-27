#pragma once

DEFINE_COMMAND_PLUGIN(AddItemAlt, "None", 1, 3, kParams_JIP_OneObjectIDorList_OneInt_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(GetValueAlt, "None", 0, 1, kParams_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(SetValueAlt, "None", 0, 2, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(RemoveItemTarget, "None", 1, 3, kParams_JIP_OneItemOrList_OneObjectRef_OneOptionalInt);

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
			ExtraContainerChanges *xChanges = (ExtraContainerChanges*)thisObj->extraDataList.GetByType(kExtraData_ContainerChanges);
			RemoveItemTarget(thisObj, cont, xChanges, form, target, quantity);
			g_processedForms.clear();
		}
	}
	return true;
}
