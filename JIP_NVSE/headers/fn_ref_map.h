#pragma once

DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetSize, RefMapSize, "None", 0, 1, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetType, RefMapGetType, "None", 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetFloat, RefMapGetFlt, "None", 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetRef, RefMapGetRef, "None", 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetString, RefMapGetStr, "None", 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetFirst, RefMapFirst, "None", 0, 1, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetNext, RefMapNext, "None", 0, 1, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetKeys, RefMapKeys, "None", 0, 1, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetAll, RefMapGetAll, "None", 0, 1, kParams_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArraySetFloat, RefMapSetFlt, "None", 0, 3, kParams_JIP_OneString_OneFloat_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArraySetRef, RefMapSetRef, "None", 0, 3, kParams_JIP_OneString_OneForm_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArraySetString, RefMapSetStr, "None", 0, 3, kParams_JIP_TwoStrings_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayErase, RefMapErase, "None", 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayValidate, RefMapValidate, "None", 0, 1, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayDestroy, RefMapDestroy, "None", 0, 1, kParams_OneString);

struct RefMapIDsMap : std::unordered_map<UInt32, AuxVariableValue> {};
struct RefMapVarsMap : std::unordered_map<std::string, RefMapIDsMap> {};
struct RefMapModsMap : std::unordered_map<UInt8, RefMapVarsMap>
{
	RefMapIDsMap *RMGetIDs(char *arrName)
	{
		auto findMod = find(g_avModIdx);
		if (findMod == end()) return NULL;
		NameToLower(arrName);
		auto findVar = findMod->second.find(arrName);
		if (findVar == findMod->second.end()) return NULL;
		return &findVar->second;
	}
}
g_refMapArraysPerm, g_refMapArraysTemp;

RefMapModsMap &RMTarget()
{
	return g_avIsPerm ? g_refMapArraysPerm : g_refMapArraysTemp;
}

bool Cmd_RefMapArrayGetSize_Execute(COMMAND_ARGS)
{
	*result = 0;
	char arrName[256] = {0};

	if (ExtractArgs(EXTRACT_ARGS, &arrName) && *arrName)
	{
		GetBaseParams(scriptObj, thisObj, arrName);
		RefMapIDsMap *refsArr = RMTarget().RMGetIDs(arrName);
		if (refsArr) *result = refsArr->size();
	}
	return true;
}

bool RefMapArrayGet_Execute(COMMAND_ARGS, UInt8 action)
{
	*result = 0;
	char arrName[256] = {0};
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &arrName, &form) && *arrName)
	{
		GetBaseParams(scriptObj, thisObj, arrName);
		RefMapIDsMap *refsArr = RMTarget().RMGetIDs(arrName);
		if (refsArr)
		{
			auto findID = refsArr->find(GetCallerID(form, thisObj));
			if (findID != refsArr->end())
			{
				if (!action) *result = findID->second.GetType();
				else if (action == 1) *result = findID->second.GetFlt();
				else if (action == 2) *(UInt32*)result = findID->second.GetRef();
				else g_string->Assign(PASS_COMMAND_ARGS, findID->second.GetStr());
				return true;
			}
		}
	}
	if (action == 4) g_string->Assign(PASS_COMMAND_ARGS, "");
	return true;
}

bool Cmd_RefMapArrayGetType_Execute(COMMAND_ARGS)
{
	return RefMapArrayGet_Execute(PASS_COMMAND_ARGS, 0);
}

bool Cmd_RefMapArrayGetFloat_Execute(COMMAND_ARGS)
{
	return RefMapArrayGet_Execute(PASS_COMMAND_ARGS, 1);
}

bool Cmd_RefMapArrayGetRef_Execute(COMMAND_ARGS)
{
	return RefMapArrayGet_Execute(PASS_COMMAND_ARGS, 2);
}

bool Cmd_RefMapArrayGetString_Execute(COMMAND_ARGS)
{
	return RefMapArrayGet_Execute(PASS_COMMAND_ARGS, 3);
}

bool RefMapArrayIterator_Execute(COMMAND_ARGS, bool getFirst)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	char arrName[256] = {0};

	if (ExtractArgs(EXTRACT_ARGS, &arrName) && *arrName)
	{
		GetBaseParams(scriptObj, thisObj, arrName);
		RefMapIDsMap *refsArr = RMTarget().RMGetIDs(arrName);
		if (refsArr)
		{
			static RefMapIDsMap *arrPtr = NULL;
			static RefMapIDsMap::iterator current;
			if (getFirst || (arrPtr != refsArr))
			{
				arrPtr = refsArr;
				current = refsArr->begin();
			}
			if (current != refsArr->end())
			{
				*refResult = current->first;
				++current;
			}
		}
	}
	return true;
}

bool Cmd_RefMapArrayGetFirst_Execute(COMMAND_ARGS)
{
	return RefMapArrayIterator_Execute(PASS_COMMAND_ARGS, true);
}

bool Cmd_RefMapArrayGetNext_Execute(COMMAND_ARGS)
{
	return RefMapArrayIterator_Execute(PASS_COMMAND_ARGS, false);
}

bool Cmd_RefMapArrayGetKeys_Execute(COMMAND_ARGS)
{
	NVSEArrayVarInterface::Array *keysArr = g_array->CreateArray(NULL, 0, scriptObj);
	g_array->AssignCommandResult(keysArr, result);

	char arrName[256] = {0};
	if (ExtractArgs(EXTRACT_ARGS, &arrName) && *arrName)
	{
		GetBaseParams(scriptObj, thisObj, arrName);
		RefMapIDsMap *refsArr = RMTarget().RMGetIDs(arrName);
		if (refsArr)
		{
			NVSEArrayVarInterface::Element *elem;
			for (auto idIter = refsArr->begin(); idIter != refsArr->end(); ++idIter)
			{
				elem = new NVSEArrayVarInterface::Element(LookupFormByID(idIter->first));
				g_array->AppendElement(keysArr, *elem);
			}
		}
	}
	return true;
}

bool Cmd_RefMapArrayGetAll_Execute(COMMAND_ARGS)
{
	NVSEArrayVarInterface::Array *varsMap = g_array->CreateStringMap(NULL, NULL, 0, scriptObj);
	g_array->AssignCommandResult(varsMap, result);

	UInt32 type = 0;
	if (ExtractArgs(EXTRACT_ARGS, &type))
	{
		GetBaseParams(scriptObj, thisObj, type);
		RefMapModsMap &target = RMTarget();
		auto findMod = target.find(g_avModIdx);
		if (findMod != target.end())
		{
			NVSEArrayVarInterface::Array *keysArr;
			NVSEArrayVarInterface::Element *elemVar, *elemKeys, *elemRef;
			for (auto varIter = findMod->second.begin(); varIter != findMod->second.end(); ++varIter)
			{
				keysArr = g_array->CreateArray(NULL, 0, scriptObj);
				for (auto idIter = varIter->second.begin(); idIter != varIter->second.end(); ++idIter)
				{
					elemRef = new NVSEArrayVarInterface::Element(LookupFormByID(idIter->first));
					g_array->AppendElement(keysArr, *elemRef);
				}
				elemVar = new NVSEArrayVarInterface::Element(varIter->first.data());
				elemKeys = new NVSEArrayVarInterface::Element(keysArr);
				g_array->SetElement(varsMap, *elemVar, *elemKeys);
			}
		}
	}
	return true;
}

bool RefMapArraySet_Execute(COMMAND_ARGS, UInt8 action)
{
	char arrName[256] = {0}, *strVal;
	float fltVal;
	TESForm *refVal, *form = NULL;

	if (!action)
	{
		if (!ExtractArgs(EXTRACT_ARGS, &arrName, &fltVal, &form) || !*arrName) return true;
	}
	else if (action == 1)
	{
		if (!ExtractArgs(EXTRACT_ARGS, &arrName, &refVal, &form) || !*arrName) return true;
	}
	else
	{
		char buffer[kMaxMessageLength] = {0};
		if (!ExtractArgs(EXTRACT_ARGS, &arrName, &buffer, &form) || !*arrName) return true;
		strVal = buffer;
	}
	
	UInt32 keyID = GetCallerID(form, thisObj);
	if (keyID)
	{
		GetBaseParams(scriptObj, thisObj, arrName);
		NameToLower(arrName);
		if (!action) RMTarget()[g_avModIdx][arrName][keyID].SetFlt(fltVal);
		else if (action == 1) RMTarget()[g_avModIdx][arrName][keyID].SetRef(refVal->refID);
		else RMTarget()[g_avModIdx][arrName][keyID].SetStr(strVal);
		AVSetChanged(2);
	}
	return true;
}

bool Cmd_RefMapArraySetFloat_Execute(COMMAND_ARGS)
{
	return RefMapArraySet_Execute(PASS_COMMAND_ARGS, 0);
}

bool Cmd_RefMapArraySetRef_Execute(COMMAND_ARGS)
{
	return RefMapArraySet_Execute(PASS_COMMAND_ARGS, 1);
}

bool Cmd_RefMapArraySetString_Execute(COMMAND_ARGS)
{
	return RefMapArraySet_Execute(PASS_COMMAND_ARGS, 2);
}

bool RefMapArrayErase_Execute(COMMAND_ARGS, UInt8 action)
{
	*result = -1;
	char arrName[256] = {0};
	TESForm *form = NULL;

	if (action)
	{
		if (!ExtractArgs(EXTRACT_ARGS, &arrName) || !*arrName) return true;
	}
	else if (!ExtractArgs(EXTRACT_ARGS, &arrName, &form) || !*arrName) return true;

	GetBaseParams(scriptObj, thisObj, arrName);
	RefMapModsMap &target = RMTarget();
	auto findMod = target.find(g_avModIdx);
	if (findMod != target.end())
	{
		NameToLower(arrName);
		auto findVar = findMod->second.find(arrName);
		if (findVar != findMod->second.end())
		{
			if (!action)
			{
				auto findID = findVar->second.find(GetCallerID(form, thisObj));
				if (findID == findVar->second.end()) return true;
				findID->second.DelStr();
				findVar->second.erase(findID);
			}
			else if (action == 1)
			{
				auto refIter = findVar->second.begin();
				while (refIter != findVar->second.end())
				{
					if (!LookupFormByID(refIter->first))
					{
						refIter->second.DelStr();
						refIter = findVar->second.erase(refIter);
						action = 0;
					}
					else refIter++;
				}
			}
			if (action != 1) AVSetChanged(2);
			if ((action == 2) || findVar->second.empty())
			{
				findMod->second.erase(findVar);
				if (findMod->second.empty()) target.erase(findMod);
				*result = 0;
			}
			else *result = findVar->second.size();
		}
	}
	return true;
}

bool Cmd_RefMapArrayErase_Execute(COMMAND_ARGS)
{
	return RefMapArrayErase_Execute(PASS_COMMAND_ARGS, 0);
}

bool Cmd_RefMapArrayValidate_Execute(COMMAND_ARGS)
{
	return RefMapArrayErase_Execute(PASS_COMMAND_ARGS, 1);
}

bool Cmd_RefMapArrayDestroy_Execute(COMMAND_ARGS)
{
	return RefMapArrayErase_Execute(PASS_COMMAND_ARGS, 2);
}
