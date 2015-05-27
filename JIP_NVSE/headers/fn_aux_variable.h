#pragma once

DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetSize, AuxVarSize, "None", 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetType, AuxVarType, "None", 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetFloat, AuxVarGetFlt, "None", 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetRef, AuxVarGetRef, "None", 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetString, AuxVarGetStr, "None", 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetAsArray, AuxVarGetAsArr, "None", 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetAll, AuxVarGetAll, "None", 0, 2, kParams_JIP_OneInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetFloat, AuxVarSetFlt, "None", 0, 4, kParams_JIP_OneString_OneFloat_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetRef, AuxVarSetRef, "None", 0, 4, kParams_JIP_OneString_OneForm_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetString, AuxVarSetStr, "None", 0, 4, kParams_JIP_TwoStrings_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetFromArray, AuxVarSetFromArr, "None", 0, 3, kParams_JIP_OneString_OneInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableErase, AuxVarErase, "None", 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableEraseAll, AuxVarEraseAll, "None", 0, 2, kParams_JIP_OneInt_OneOptionalForm);

struct AuxVarValsArr : std::vector<AuxVariableValue>
{
	void ClearValues()
	{
		for (auto valIter = begin(); valIter != end(); ++valIter) valIter->DelStr();
	}
	~AuxVarValsArr() {ClearValues();}
};
struct AuxVarVarsMap : std::unordered_map<std::string, AuxVarValsArr> {};
struct AuxVarOwnersMap : std::unordered_map<UInt32, AuxVarVarsMap> {};
struct AuxVarModsMap : std::unordered_map<UInt8, AuxVarOwnersMap>
{
	AuxVarValsArr *AVGetArray(UInt32 ownerID, char *varName, bool addArr = false)
	{
		NameToLower(varName);
		auto findMod = find(g_avModIdx);
		if (findMod == end()) return addArr ? &(*this)[g_avModIdx][ownerID][varName] : NULL;
		auto findOwner = findMod->second.find(ownerID);
		if (findOwner == findMod->second.end()) return addArr ? &findMod->second[ownerID][varName] : NULL;
		auto findVar = findOwner->second.find(varName);
		if (findVar == findOwner->second.end()) return addArr ? &findOwner->second[varName] : NULL;
		return &findVar->second;
	}
	AuxVariableValue *AVGetValue(UInt32 ownerID, char *varName, SInt32 idx, bool addVal = false)
	{
		AuxVarValsArr *valsArr = AVGetArray(ownerID, varName, addVal && (idx <= 0));
		if (!valsArr) return NULL;
		if (idx >= 0)
		{
			auto findElem = valsArr->begin() + idx;
			if (findElem < valsArr->end()) return &*findElem;
			if (!addVal || (findElem > valsArr->end())) return NULL;
		}
		return &*valsArr->insert(valsArr->end(), AuxVariableValue());
	}
}
g_auxVariablesPerm, g_auxVariablesTemp;

AuxVarModsMap &AVTarget()
{
	return g_avIsPerm ? g_auxVariablesPerm : g_auxVariablesTemp;
}

bool Cmd_AuxiliaryVariableGetSize_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[256] = {0};
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &varName, &form) && *varName)
	{
		UInt32 ownerID = GetCallerID(form, thisObj);
		if (ownerID)
		{
			GetBaseParams(scriptObj, thisObj, varName);
			AuxVarValsArr *valsArr = AVTarget().AVGetArray(ownerID, varName);
			if (valsArr) *result = valsArr->size();
		}
	}
	return true;
}

bool AuxiliaryVariableGet_Execute(COMMAND_ARGS, UInt8 action)
{
	*result = 0;
	char varName[256] = {0};
	SInt32 idx = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &varName, &idx, &form) && *varName && (idx >= 0))
	{
		UInt32 ownerID = GetCallerID(form, thisObj);
		if (ownerID)
		{
			GetBaseParams(scriptObj, thisObj, varName);
			AuxVariableValue *value = AVTarget().AVGetValue(ownerID, varName, idx);
			if (value)
			{
				if (!action) *result = value->GetType();
				else if (action == 1) *result = value->GetFlt();
				else if (action == 2) *(UInt32*)result = value->GetRef();
				else g_string->Assign(PASS_COMMAND_ARGS, value->GetStr());
				return true;
			}
		}
	}
	if (action == 4) g_string->Assign(PASS_COMMAND_ARGS, "");
	return true;
}

bool Cmd_AuxiliaryVariableGetType_Execute(COMMAND_ARGS)
{
	return AuxiliaryVariableGet_Execute(PASS_COMMAND_ARGS, 0);
}

bool Cmd_AuxiliaryVariableGetFloat_Execute(COMMAND_ARGS)
{
	return AuxiliaryVariableGet_Execute(PASS_COMMAND_ARGS, 1);
}

bool Cmd_AuxiliaryVariableGetRef_Execute(COMMAND_ARGS)
{
	return AuxiliaryVariableGet_Execute(PASS_COMMAND_ARGS, 2);
}

bool Cmd_AuxiliaryVariableGetString_Execute(COMMAND_ARGS)
{
	return AuxiliaryVariableGet_Execute(PASS_COMMAND_ARGS, 3);
}

bool Cmd_AuxiliaryVariableGetAsArray_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[256] = {0};
	TESForm *form = NULL;

	NVSEArrayVarInterface::Array *resArr = g_array->CreateArray(NULL, 0, scriptObj);
	if (ExtractArgs(EXTRACT_ARGS, &varName, &form) && *varName)
	{
		UInt32 ownerID = GetCallerID(form, thisObj);
		if (ownerID)
		{
			GetBaseParams(scriptObj, thisObj, varName);
			AuxVarValsArr *valsArr = AVTarget().AVGetArray(ownerID, varName);
			if (valsArr)
			{
				NVSEArrayVarInterface::Element *elemVal;
				for (auto valsIter = valsArr->begin(); valsIter != valsArr->end(); ++valsIter)
				{
					elemVal = valsIter->GetAsElement();
					g_array->AppendElement(resArr, *elemVal);
				}
			}
		}
	}
	g_array->AssignCommandResult(resArr, result);
	return true;
}

bool Cmd_AuxiliaryVariableGetAll_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 type = 0;
	TESForm *form = NULL;

	NVSEArrayVarInterface::Array *varsMap = g_array->CreateStringMap(NULL, NULL, 0, scriptObj);
	if (ExtractArgs(EXTRACT_ARGS, &type, &form))
	{
		UInt32 ownerID = GetCallerID(form, thisObj);
		if (ownerID)
		{
			GetBaseParams(scriptObj, thisObj, type);
			AuxVarModsMap &target = AVTarget();
			auto findMod = target.find(g_avModIdx);
			if (findMod != target.end())
			{
				auto findOwner = findMod->second.find(ownerID);
				if (findOwner != findMod->second.end())
				{
					NVSEArrayVarInterface::Array *valsArr;
					NVSEArrayVarInterface::Element *elemVal, *elemKey, *elemArr;
					for (auto varIter = findOwner->second.begin(); varIter != findOwner->second.end(); ++varIter)
					{
						valsArr = g_array->CreateArray(NULL, 0, scriptObj);
						for (auto valIter = varIter->second.begin(); valIter != varIter->second.end(); ++valIter)
						{
							elemVal = valIter->GetAsElement();
							g_array->AppendElement(valsArr, *elemVal);
						}
						elemKey = new NVSEArrayVarInterface::Element(varIter->first.data());
						elemArr = new NVSEArrayVarInterface::Element(valsArr);
						g_array->SetElement(varsMap, *elemKey, *elemArr);
					}
				}
			}
		}
	}
	g_array->AssignCommandResult(varsMap, result);
	return true;
}

bool AuxiliaryVariableSet_Execute(COMMAND_ARGS, UInt8 action)
{
	*result = 0;
	char varName[256] = {0}, *strVal;
	float fltVal;
	SInt32 idx = 0;
	TESForm *refVal, *form = NULL;

	if (!action)
	{
		if (!ExtractArgs(EXTRACT_ARGS, &varName, &fltVal, &idx, &form) || !*varName) return true;
	}
	else if (action == 1)
	{
		if (!ExtractArgs(EXTRACT_ARGS, &varName, &refVal, &idx, &form) || !*varName) return true;
	}
	else
	{
		char buffer[kMaxMessageLength] = {0};
		if (!ExtractArgs(EXTRACT_ARGS, &varName, &buffer, &idx, &form) || !*varName) return true;
		strVal = buffer;
	}

	UInt32 ownerID = GetCallerID(form, thisObj);
	if (ownerID)
	{
		GetBaseParams(scriptObj, thisObj, varName);
		AuxVariableValue *value = AVTarget().AVGetValue(ownerID, varName, idx, true);
		if (value)
		{
			if (!action) value->SetFlt(fltVal);
			else if (action == 1) value->SetRef(refVal->refID);
			else value->SetStr(strVal);
			*result = 1;
			AVSetChanged(1);
		}
	}
	return true;
}

bool Cmd_AuxiliaryVariableSetFloat_Execute(COMMAND_ARGS)
{
	return AuxiliaryVariableSet_Execute(PASS_COMMAND_ARGS, 0);
}

bool Cmd_AuxiliaryVariableSetRef_Execute(COMMAND_ARGS)
{
	return AuxiliaryVariableSet_Execute(PASS_COMMAND_ARGS, 1);
}

bool Cmd_AuxiliaryVariableSetString_Execute(COMMAND_ARGS)
{
	return AuxiliaryVariableSet_Execute(PASS_COMMAND_ARGS, 2);
}

bool Cmd_AuxiliaryVariableSetFromArray_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[256] = {0};
	UInt32 arrID = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &varName, &arrID, &form) && *varName && arrID)
	{
		UInt32 ownerID = GetCallerID(form, thisObj);
		if (ownerID)
		{
			NVSEArrayVarInterface::Array *srcArr = g_array->LookupArrayByID(arrID);
			if (srcArr)
			{
				SInt32 srcSize;
				NVSEArrayVarInterface::Element *arrElems = GetArrayData(srcArr, srcSize);
				if (arrElems)
				{
					GetBaseParams(scriptObj, thisObj, varName);
					AuxVarValsArr *valsArr = AVTarget().AVGetArray(ownerID, varName, true);
					if (!valsArr->empty())
					{
						valsArr->ClearValues();
						valsArr->clear();
					}
					for (SInt32 iter = 0; iter < srcSize; iter++) valsArr->push_back(AuxVariableValue(arrElems[iter]));
					delete[] arrElems;
					*result = 1;
					AVSetChanged(1);
				}
			}
		}
	}
	return true;
}

bool Cmd_AuxiliaryVariableErase_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[256] = {0};
	SInt32 idx = -1;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &varName, &idx, &form) && *varName)
	{
		UInt32 ownerID = GetCallerID(form, thisObj);
		if (ownerID)
		{
			GetBaseParams(scriptObj, thisObj, varName);
			AuxVarModsMap &target = AVTarget();
			auto findMod = target.find(g_avModIdx);
			if (findMod != target.end())
			{
				auto findOwner = findMod->second.find(ownerID);
				if (findOwner != findMod->second.end())
				{
					NameToLower(varName);
					auto findVar = findOwner->second.find(varName);
					if (findVar != findOwner->second.end())
					{
						if (idx >= 0)
						{
							auto findElem = findVar->second.begin() + idx;
							if (findElem >= findVar->second.end()) return true;
							findElem->DelStr();
							findVar->second.erase(findElem);
							if (findVar->second.empty()) findOwner->second.erase(findVar);
						}
						else findOwner->second.erase(findVar);
						if (findOwner->second.empty())
						{
							findMod->second.erase(findOwner);
							if (findMod->second.empty()) target.erase(findMod);
						}
						*result = 1;
						AVSetChanged(1);
					}
				}
			}
		}
	}
	return true;
}

bool Cmd_AuxiliaryVariableEraseAll_Execute(COMMAND_ARGS)
{
	UInt32 type = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &type, &form))
	{
		UInt32 ownerID = GetCallerID(form, thisObj);
		if (ownerID)
		{
			GetBaseParams(scriptObj, thisObj, type);
			AuxVarModsMap &target = AVTarget();
			auto findMod = target.find(g_avModIdx);
			if ((findMod != target.end()) && findMod->second.erase(ownerID))
			{
				if (findMod->second.empty()) target.erase(findMod);
				AVSetChanged(1);
			}
		}
	}
	return true;
}
