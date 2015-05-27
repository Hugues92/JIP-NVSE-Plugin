#pragma once

DEFINE_COMMAND_PLUGIN(SetPersistent, "None", 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetObjectDimensions, "None", 0, 2, kParams_JIP_OneAxis_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetIsItem, "None", 0, 1, kParams_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(SetLinkedReference, "None", 1, 1, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(GetEnableChildren, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetLinkedChildren, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetPrimitiveBound, "None", 1, 1, kParams_Axis);
DEFINE_COMMAND_PLUGIN(SetPrimitiveBound, "None", 1, 2, kParams_JIP_OneAxis_OneFloat);
DEFINE_COMMAND_PLUGIN(GetTeammates, "None", 0, 0, NULL);

bool Cmd_SetPersistent_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 flag = 0;
	if (thisObj && ExtractArgs(EXTRACT_ARGS, &flag))
	{
		*result = (thisObj->flags & 0x400) ? 1 : 0;
		flag ? (thisObj->flags |= 0x400) : (thisObj->flags &= ~0x400);
	}
	return true;
}

bool Cmd_GetObjectDimensions_Execute(COMMAND_ARGS)
{
	*result = 0;
	char axis = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &axis, &form))
	{
		float pScale = 1.0;

		if (thisObj)
		{
			form = thisObj->baseForm;
			pScale = thisObj->scale;
		}
		else if (form) form = form->TryGetREFRParent();
		
		if (form)
		{
			TESBoundObject *object = DYNAMIC_CAST(form, TESForm, TESBoundObject);
			if (object)
			{
				if (axis < 'x') axis += 32;
				if (axis == 'x') *result = abs(object->bounds[3] - object->bounds[0]) * pScale;
				else if (axis == 'y') *result = abs(object->bounds[4] - object->bounds[1]) * pScale;
				else *result = abs(object->bounds[5] - object->bounds[2]) * pScale;
			}
		}
		if (IsConsoleMode()) Console_Print("%s Dimension >> %.0f", axis, *result);
	}
	return true;
}

bool Cmd_GetIsItem_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	if (thisObj)
	{
		if (form = thisObj->baseForm) *result = form->IsInventoryObject();
	}
	else if (ExtractArgs(EXTRACT_ARGS, &form) && form) *result = 1;
	return true;
}

struct LinkedRefEntry
{
	UInt32	linkID;
	UInt8	modIdx;

	void Set(UInt32 pLinkID, UInt8 pModIdx)
	{
		linkID = pLinkID;
		modIdx = pModIdx;
	}
};
std::unordered_map<UInt32, LinkedRefEntry> g_linkedRefModified;
typedef std::unordered_map<UInt32, UInt32> LinkedRefMap;
LinkedRefMap g_linkedRefDefault;

bool SetLinkedRef(TESObjectREFR *thisObj, TESObjectREFR *linkObj = NULL, UInt8 modIdx = 255)
{
	ExtraLinkedRef *xLinkedRef = (ExtraLinkedRef*)GetByTypeCast(thisObj->extraDataList, LinkedRef);
	if (!linkObj)
	{
		auto findDefID = g_linkedRefDefault.find(thisObj->refID);
		if (findDefID != g_linkedRefDefault.end())
		{
			if (xLinkedRef)
			{
				if (findDefID->second)
				{
					TESForm *form = LookupFormByID(findDefID->second);
					if (form && (linkObj = DYNAMIC_CAST(form, TESForm, TESObjectREFR))) xLinkedRef->linkedRef = linkObj;
				}
				else thisObj->extraDataList.RemoveByType(kExtraData_LinkedRef, true);
			}
			g_linkedRefDefault.erase(findDefID);
		}
		auto findModID = g_linkedRefModified.find(thisObj->refID);
		if (findModID != g_linkedRefModified.end()) g_linkedRefModified.erase(findModID);
		return true;
	}
	if (!xLinkedRef)
	{
		xLinkedRef = (ExtraLinkedRef*)BSExtraData::Create(kExtraData_LinkedRef, sizeof(ExtraLinkedRef), s_ExtraLinkedRefVtbl);
		if (!thisObj->extraDataList.Add(xLinkedRef))
		{
			FormHeap_Free(xLinkedRef);
			return false;
		}
		g_linkedRefDefault[thisObj->refID] = 0;
	}
	else
	{
		if (!xLinkedRef->linkedRef) return false;
		g_linkedRefDefault[thisObj->refID] = xLinkedRef->linkedRef->refID;
	}
	g_linkedRefModified[thisObj->refID].Set(linkObj->refID, modIdx);
	xLinkedRef->linkedRef = linkObj;
	return true;
}

bool SetLinkedRefID(UInt32 thisID, UInt32 linkID = 0, UInt8 modIdx = 255)
{
	TESForm *form;
	TESObjectREFR *thisObj, *linkObj;

	if ((form = LookupFormByID(thisID)) && (thisObj = DYNAMIC_CAST(form, TESForm, TESObjectREFR)))
	{
		if (!linkID) return SetLinkedRef(thisObj);
		if ((form = LookupFormByID(linkID)) && (linkObj = DYNAMIC_CAST(form, TESForm, TESObjectREFR)) && SetLinkedRef(thisObj, linkObj, modIdx))
			return true;
	}
	return false;
}

bool Cmd_SetLinkedReference_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectREFR *linkRef = NULL;
	if (thisObj && ExtractArgs(EXTRACT_ARGS, &linkRef) && SetLinkedRef(thisObj, linkRef, GetScriptModIdx(scriptObj, thisObj)))
	{
		*result = 1;
		g_dataChangedFlags |= 4;
	}
	return true;
}

bool Cmd_GetEnableChildren_Execute(COMMAND_ARGS)
{
	*result = 0;
	NVSEArrayVarInterface::Array *childrenArr = g_array->CreateArray(NULL, 0, scriptObj);
	if (thisObj)
	{
		ExtraEnableStateChildren *xEnableChildren = (ExtraEnableStateChildren*)GetByTypeCast(thisObj->extraDataList, EnableStateChildren);
		if (xEnableChildren)
		{
			NVSEArrayVarInterface::Element *elem;
			for (UInt32 idx = 0; idx < xEnableChildren->children.Count(); idx++)
			{
				elem = new NVSEArrayVarInterface::Element(xEnableChildren->children.GetNthItem(idx));
				g_array->AppendElement(childrenArr, *elem);
			}
		}
	}
	g_array->AssignCommandResult(childrenArr, result);
	return true;
}

bool Cmd_GetLinkedChildren_Execute(COMMAND_ARGS)
{
	*result = 0;
	NVSEArrayVarInterface::Array *childrenArr = g_array->CreateArray(NULL, 0, scriptObj);
	if (thisObj)
	{
		ExtraLinkedRefChildren *xLinkedChildren = (ExtraLinkedRefChildren*)GetByTypeCast(thisObj->extraDataList, LinkedRefChildren);
		if (xLinkedChildren)
		{
			NVSEArrayVarInterface::Element *elem;
			for (UInt32 idx = 0; idx < xLinkedChildren->children.Count(); idx++)
			{
				elem = new NVSEArrayVarInterface::Element(xLinkedChildren->children.GetNthItem(idx));
				g_array->AppendElement(childrenArr, *elem);
			}
		}
	}
	g_array->AssignCommandResult(childrenArr, result);
	return true;
}

bool Cmd_GetPrimitiveBound_Execute(COMMAND_ARGS)
{
	*result = -1;
	char axis = 0;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &axis))
	{
		ExtraPrimitive *xPrimitive = (ExtraPrimitive*)GetByTypeCast(thisObj->extraDataList, Primitive);
		if (xPrimitive)
		{
			if (axis > 'Z') axis -= 32;
			*result = xPrimitive->data->bounds[axis - 88] * 2;
		}
	}
	return true;
}

bool Cmd_SetPrimitiveBound_Execute(COMMAND_ARGS)
{
	*result = -1;
	char axis = 0;
	float val = 0;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &axis, &val))
	{
		ExtraPrimitive *xPrimitive = (ExtraPrimitive*)GetByTypeCast(thisObj->extraDataList, Primitive);
		if (xPrimitive)
		{
			if (axis > 'Z') axis -= 32;
			*result = xPrimitive->data->bounds[axis - 88] * 2;
			if (val < 0) val = 0;
			else val /= 2;
			if (xPrimitive->data->type == 2)
			{
				xPrimitive->data->bounds[0] = val;
				xPrimitive->data->bounds[1] = val;
				xPrimitive->data->bounds[2] = val;
			}
			else xPrimitive->data->bounds[axis - 88] = val;
			thisObj->Update3D();
		}
	}
	return true;
}

bool Cmd_GetTeammates_Execute(COMMAND_ARGS)
{
	*result = 0;
	NVSEArrayVarInterface::Array *teammatesArr = g_array->CreateArray(NULL, 0, scriptObj);
	PlayerCharacter *pc = PlayerCharacter::GetSingleton();
	if (pc)
	{
		NVSEArrayVarInterface::Element *elem;
		for (UInt32 idx = 0; idx < pc->teammates.Count(); idx++)
		{
			elem = new NVSEArrayVarInterface::Element(pc->teammates.GetNthItem(idx));
			g_array->AppendElement(teammatesArr, *elem);
		}
	}
	g_array->AssignCommandResult(teammatesArr, result);
	return true;
}
