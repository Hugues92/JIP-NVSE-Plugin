#pragma once

DEFINE_COMMAND_PLUGIN(SetPersistent, "None", 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetObjectDimensions, "None", 0, 2, kParams_JIP_OneAxis_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetIsItem, "None", 0, 1, kParams_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(SetLinkedReference, "None", 1, 1, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(GetEnableChildren, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetLinkedChildren, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(HasPrimitive, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetPrimitiveBound, "None", 1, 1, kParams_Axis);
DEFINE_COMMAND_PLUGIN(SetPrimitiveBound, "None", 1, 2, kParams_JIP_OneAxis_OneFloat);
DEFINE_COMMAND_PLUGIN(AddPrimitive, "None", 1, 4, kParams_JIP_OneInt_ThreeFloats);
DEFINE_COMMAND_PLUGIN(GetTeammates, "None", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetStageAlt, "None", 0, 2, kParams_JIP_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(HasEffectShader, "None", 1, 1, kParams_JIP_OneEffectShader);
DEFINE_COMMAND_PLUGIN(SetScaleEx, "None", 1, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetWorldspaceFlag, "None", 0, 2, kParams_JIP_OneWorldspace_OneInt);
DEFINE_COMMAND_PLUGIN(SetWorldspaceFlag, "None", 0, 3, kParams_JIP_OneWorldspace_TwoInts);
DEFINE_COMMAND_PLUGIN(DisableNavMeshAlt, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(EnableNavMeshAlt, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetTerrainHeight, "None", 0, 2, kParams_TwoFloats);

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
	NVSEArrayVarInterface::Array *childrenArr = g_array->CreateArray(NULL, 0, scriptObj);
	g_array->AssignCommandResult(childrenArr, result);
	if (thisObj)
	{
		ExtraEnableStateChildren *xEnableChildren = (ExtraEnableStateChildren*)GetByTypeCast(thisObj->extraDataList, EnableStateChildren);
		if (xEnableChildren)
		{
			NVSEArrayVarInterface::Element *elem;
			for (auto iter = xEnableChildren->children.Begin(); !iter.End(); ++iter)
			{
				elem = new NVSEArrayVarInterface::Element(iter.Get());
				g_array->AppendElement(childrenArr, *elem);
			}
		}
	}
	return true;
}

bool Cmd_GetLinkedChildren_Execute(COMMAND_ARGS)
{
	NVSEArrayVarInterface::Array *childrenArr = g_array->CreateArray(NULL, 0, scriptObj);
	g_array->AssignCommandResult(childrenArr, result);
	if (thisObj)
	{
		ExtraLinkedRefChildren *xLinkedChildren = (ExtraLinkedRefChildren*)GetByTypeCast(thisObj->extraDataList, LinkedRefChildren);
		if (xLinkedChildren)
		{
			NVSEArrayVarInterface::Element *elem;
			for (auto iter = xLinkedChildren->children.Begin(); !iter.End(); ++iter)
			{
				elem = new NVSEArrayVarInterface::Element(iter.Get());
				g_array->AppendElement(childrenArr, *elem);
			}
		}
	}
	return true;
}

bool Cmd_HasPrimitive_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj)
	{
		ExtraPrimitive *xPrimitive = (ExtraPrimitive*)GetByTypeCast(thisObj->extraDataList, Primitive);
		if (xPrimitive && xPrimitive->primitive) *result = 1;
	}
	return true;
}

bool Cmd_GetPrimitiveBound_Execute(COMMAND_ARGS)
{
	*result = -1;
	char axis = 0;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &axis))
	{
		ExtraPrimitive *xPrimitive = (ExtraPrimitive*)GetByTypeCast(thisObj->extraDataList, Primitive);
		if (xPrimitive && xPrimitive->primitive)
		{
			axis -= (axis > 'Z') ? 120 : 88;
			*result = xPrimitive->primitive->bounds[axis] * 2;
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
			BGSPrimitive *primitive = xPrimitive->primitive;
			if (primitive)
			{
				axis -= (axis > 'Z') ? 120 : 88;
				*result = primitive->bounds[axis] * 2;
				if (primitive->type == 2) primitive->bounds[2] = primitive->bounds[1] = primitive->bounds[0] = val / 2;
				else primitive->bounds[axis] = val / 2;
				thisObj->Update3D();
			}
		}
	}
	return true;
}

bool Cmd_AddPrimitive_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 type = 0;
	float boundX = 0, boundY = 0, boundZ = 0;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &type, &boundX, &boundY, &boundZ) && ValidInput(type, 3, 1) &&
		((thisObj->baseForm->refID == 0x21) || ((thisObj->baseForm->typeID == kFormType_Activator) && (type != 3))) &&
		!thisObj->extraDataList.HasType(kExtraData_Primitive))
	{
		ExtraPrimitive *xPrimitive = (ExtraPrimitive*)BSExtraData::Create(kExtraData_Primitive, 0x10, s_ExtraPrimitiveVtbl);
		if (thisObj->extraDataList.Add(xPrimitive))
		{
			UInt32 size = (type == 1) ? 0x4C : 0x34;
			xPrimitive->primitive = (BGSPrimitive*)FormHeap_Allocate(size);
			memset(xPrimitive->primitive, 0, size);
			xPrimitive->primitive->vtbl = s_BGSPrimitiveVtbl[type - 1];
			xPrimitive->primitive->type = type;
			xPrimitive->primitive->bounds[0] = boundX / 2;
			if (type == 2) xPrimitive->primitive->bounds[2] = xPrimitive->primitive->bounds[1] = boundX / 2;
			else
			{
				xPrimitive->primitive->bounds[1] = (type == 1) ? (boundY / 2) : 1.0;
				xPrimitive->primitive->bounds[2] = boundZ / 2;
			}
			thisObj->Update3D();
			*result = 1;
		}
		else FormHeap_Free(xPrimitive);
	}
	return true;
}

bool Cmd_GetTeammates_Execute(COMMAND_ARGS)
{
	NVSEArrayVarInterface::Array *teammatesArr = g_array->CreateArray(NULL, 0, scriptObj);
	g_array->AssignCommandResult(teammatesArr, result);
	NVSEArrayVarInterface::Element *elem;
	for (auto iter = (*g_thePlayer)->teammates.Begin(); !iter.End(); ++iter)
	{
		elem = new NVSEArrayVarInterface::Element(iter.Get());
		g_array->AppendElement(teammatesArr, *elem);
	}
	return true;
}

bool Cmd_SetStageAlt_Execute(COMMAND_ARGS)
{
	TESQuest *quest = NULL;
	UInt32 newStage = 0;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &newStage) && quest)
	{
		for (auto iter = quest->stages.Begin(); !iter.End(); ++iter)
		{
			if (iter->stage > newStage) return true;
			if (iter->stage < newStage) continue;
			Cmd_SetStage_Execute(PASS_COMMAND_ARGS);
			quest->currentStage = newStage;
			return true;
		}
	}
	return true;
}

class EffectShaderFinder
{
	TESEffectShader	*m_shader;
	TESObjectREFR	*m_refr;
public:
	EffectShaderFinder(TESObjectREFR *refr, TESEffectShader *shader) : m_refr(refr), m_shader(shader) {}

	bool Accept(BSTempEffect *effect)
	{
		MagicShaderHitEffect *mgsh = DYNAMIC_CAST(effect, BSTempEffect, MagicShaderHitEffect);
		return mgsh && (mgsh->target == m_refr) && (mgsh->effectShader == m_shader);
	}
};

bool Cmd_HasEffectShader_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESEffectShader *shader = NULL;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &shader))
		*result = g_actorProcessManager->tempEffects.CountIf(EffectShaderFinder(thisObj, shader));

	return true;
}

bool Cmd_SetScaleEx_Execute(COMMAND_ARGS)
{
	float newScale = 0;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &newScale))
	{
		thisObj->scale = newScale;
		thisObj->MarkAsModified(0x10);
	}
	return true;
}

bool Cmd_GetWorldspaceFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWorldSpace *wspc = NULL;
	UInt32 flagID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &wspc, &flagID) && ValidInput(flagID, 7))
		*result = (wspc->flags & (1 << flagID)) ? 1 : 0;

	return true;
}

bool Cmd_SetWorldspaceFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWorldSpace *wspc = NULL;
	UInt32 flagID = 0, val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &wspc, &flagID, &val) && ValidInput(flagID, 7))
	{
		flagID = 1 << flagID;
		*result = (wspc->flags & flagID) ? 1 : 0;
		val ? (wspc->flags |= flagID) : (wspc->flags &= ~flagID);
	}
	return true;
}

bool ToggleNavMeshAlt_Execute(COMMAND_ARGS, bool disable)
{
	TESForm *form = NULL;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && form && (form->typeID == kFormType_NavMesh))
	{
		UInt8 *newData = (UInt8*)FormHeap_Allocate(scriptObj->info.dataLength + 7);
		memcpy(newData, scriptData, *opcodeOffsetPtr + 2);
		newData[*opcodeOffsetPtr + 2] = 8;
		newData[*opcodeOffsetPtr + 3] = 0;
		sprintf_s((char*)newData + *opcodeOffsetPtr + 4, 9, "%08X", form->refID);
		memcpy(newData + *opcodeOffsetPtr + 12, (UInt8*)scriptData + *opcodeOffsetPtr + 5, scriptObj->info.dataLength - *opcodeOffsetPtr - 5);
		scriptData = newData;
		paramInfo->typeID = 0;
		if (disable) Cmd_DisableNavMesh_Execute(PASS_COMMAND_ARGS);
		else Cmd_EnableNavMesh_Execute(PASS_COMMAND_ARGS);
		FormHeap_Free(newData);
		paramInfo->typeID = 61;
	}
	return true;
}

bool Cmd_DisableNavMeshAlt_Execute(COMMAND_ARGS)
{
	return ToggleNavMeshAlt_Execute(PASS_COMMAND_ARGS, true);
}

bool Cmd_EnableNavMeshAlt_Execute(COMMAND_ARGS)
{
	return ToggleNavMeshAlt_Execute(PASS_COMMAND_ARGS, false);
}

bool Cmd_GetTerrainHeight_Execute(COMMAND_ARGS)
{
	*result = 0;
	float posVec[3] = {0, 0, 0};

	if (ExtractArgs(EXTRACT_ARGS, &posVec[0], &posVec[1]))
	{
		float height = 0;
		if (GetTerrainHeight(posVec, &height)) *result = height;
	}
	return true;
}
