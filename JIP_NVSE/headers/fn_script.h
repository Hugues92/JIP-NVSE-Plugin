#pragma once

DEFINE_COMMAND_PLUGIN(HasVariableAdded, "None", 0, 2, kParams_JIP_OneString_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(AddScriptVariable, "None", 0, 2, kParams_JIP_OneString_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(RemoveScriptVariable, "None", 0, 2, kParams_JIP_OneString_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(RemoveAllAddedVariables, "None", 0, 1, kParams_JIP_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(GetDelayElapsed, "None", 0, 1, kParams_OneQuest);
DEFINE_COMMAND_PLUGIN(SetDelayElapsed, "None", 0, 2, kParams_JIP_OneQuest_OneFloat);

struct ScriptVariableEntry
{
	ScriptEventList::Var	*value;
	UInt8					modIdx;

	void Set(ScriptEventList::Var *pValue, UInt8 pModIdx)
	{
		value = pValue;
		modIdx = pModIdx;
	}
};
struct ScriptVariablesMap : std::unordered_map<std::string, ScriptVariableEntry> {};
std::unordered_map<UInt32, ScriptVariablesMap> g_scriptVariablesBuffer;

struct VariableNames : std::unordered_set<std::string> {};
std::unordered_map<UInt32, VariableNames> g_addedVariables;

bool GetScriptAndEventList(TESForm *form, Script *&pScript, ScriptEventList *&pEventList)
{
	TESScriptableForm *scriptable = NULL;
	if (form->typeID == kFormType_Quest)
	{
		TESQuest *quest = DYNAMIC_CAST(form, TESForm, TESQuest);
		if (quest)
		{
			scriptable = DYNAMIC_CAST(quest, TESQuest, TESScriptableForm);
			pEventList = quest->scriptEventList;
		}
	}
	else
	{
		TESObjectREFR *ref = DYNAMIC_CAST(form, TESForm, TESObjectREFR);
		if (ref)
		{
			scriptable = DYNAMIC_CAST(ref->baseForm, TESForm, TESScriptableForm);
			pEventList = ref->GetEventList();
		}
	}
	if (scriptable && pEventList)
	{
		pScript = scriptable->script;
		if (pScript) return true;
	}
	return false;
}

ScriptEventList::Var *AddScriptVariable(Script *pScript, ScriptEventList *pEventList, UInt32 ownerID, UInt8 modIdx, char *varName)
{
	VariableInfo *varInfo = pScript->GetVariableByName(varName);
	if (!varInfo)
	{
		Script::VarInfoEntry *infoEnt(&pScript->varList);
		varInfo = (VariableInfo*)FormHeap_Allocate(sizeof(VariableInfo));
		memset(varInfo, 0, sizeof(VariableInfo));
		varInfo->idx = Script::VarListVisitor(infoEnt).Count() + 1;
		varInfo->name.m_bufLen = varInfo->name.m_dataLen = strlen(varName);
		varInfo->name.m_data = (char*)FormHeap_Allocate(varInfo->name.m_dataLen + 1);
		strcpy_s(varInfo->name.m_data, varInfo->name.m_dataLen + 1, varName);

		g_addedVariables[pScript->refID].insert(varName);

		if (infoEnt->data)
		{
			while (infoEnt->next) infoEnt = infoEnt->next;
			infoEnt = infoEnt->next = (Script::VarInfoEntry*)FormHeap_Allocate(sizeof(Script::VarInfoEntry));
		}
		infoEnt->data = varInfo;
		infoEnt->next = NULL;

		pScript->info.varCount++;
	}
	else
	{
		auto findOwner = g_addedVariables.find(pScript->refID);
		if (findOwner == g_addedVariables.end()) return NULL;
		auto findVar = findOwner->second.find(varName);
		if (findVar == findOwner->second.end()) return NULL;
	}

	ScriptEventList::Var *var = pEventList->GetVariable(varInfo->idx);
	if (!var)
	{
		var = (ScriptEventList::Var*)FormHeap_Allocate(sizeof(ScriptEventList::Var));
		memset(var, 0, sizeof(ScriptEventList::Var));
		var->id = varInfo->idx;

		ScriptEventList::VarEntry *varEnt = pEventList->m_vars;
		if (varEnt->var)
		{
			while (varEnt->next) varEnt = varEnt->next;
			varEnt = varEnt->next = (ScriptEventList::VarEntry*)FormHeap_Allocate(sizeof(ScriptEventList::VarEntry));
		}
		varEnt->var = var;
		varEnt->next = NULL;
	}

	g_scriptVariablesBuffer[ownerID][varName].Set(var, modIdx);

	return var;
}

bool GetVariableAdded(UInt32 ownerID, const char *varName, bool delVar = false)
{
	auto findOwner = g_scriptVariablesBuffer.find(ownerID);
	if (findOwner != g_scriptVariablesBuffer.end())
	{
		auto findVar = findOwner->second.find(varName);
		if (findVar != findOwner->second.end())
		{
			if (delVar)
			{
				findOwner->second.erase(findVar);
				if (findOwner->second.empty()) g_scriptVariablesBuffer.erase(findOwner);
			}
			return true;
		}
	}
	return false;
}

bool ScriptVariableAction_Execute(COMMAND_ARGS, UInt8 action)
{
	*result = 0;
	char varName[256] = {0};
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &varName, &form) && (form || (form = thisObj)) && *varName)
	{
		NameToLower(varName);
		if (action < 2)
		{
			Script *pScript;
			ScriptEventList *pEventList;
			if (GetScriptAndEventList(form, pScript, pEventList) && (!action == GetVariableAdded(form->refID, varName)))
			{
				if (!action)
				{
					VariableInfo *varInfo = pScript->GetVariableByName(varName);
					if (varInfo && pEventList->GetVariable(varInfo->idx)) *result = 1;
				}
				else if (AddScriptVariable(pScript, pEventList, form->refID, GetScriptModIdx(scriptObj, thisObj), varName)) *result = 1;
			}
		}
		else if (GetVariableAdded(form->refID, varName, true)) *result = 1;
	}
	return true;
}

bool Cmd_HasVariableAdded_Execute(COMMAND_ARGS)
{
	return ScriptVariableAction_Execute(PASS_COMMAND_ARGS, 0);
}

bool Cmd_AddScriptVariable_Execute(COMMAND_ARGS)
{
	return ScriptVariableAction_Execute(PASS_COMMAND_ARGS, 1);
}

bool Cmd_RemoveScriptVariable_Execute(COMMAND_ARGS)
{
	return ScriptVariableAction_Execute(PASS_COMMAND_ARGS, 2);
}

bool Cmd_RemoveAllAddedVariables_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		if (!form)
		{
			if (!thisObj) return true;
			form = thisObj;
		}
		UInt8 modIdx = GetScriptModIdx(scriptObj, thisObj);
		auto findOwner = g_scriptVariablesBuffer.find(form->refID);
		if (findOwner != g_scriptVariablesBuffer.end())
		{
			auto varIter = findOwner->second.begin();
			while (varIter != findOwner->second.end())
			{
				if (varIter->second.modIdx == modIdx) varIter = findOwner->second.erase(varIter);
				else ++varIter;
			}
			if (findOwner->second.empty()) g_scriptVariablesBuffer.erase(findOwner);
		}
	}
	return true;
}

bool Cmd_GetDelayElapsed_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESQuest *quest = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &quest))
	{
		TESScriptableForm *scriptable = DYNAMIC_CAST(quest, TESQuest, TESScriptableForm);
		Script *script = scriptable->script;
		if (script) *result = script->questDelayTimeCounter;
	}
	return true;
}

bool Cmd_SetDelayElapsed_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESQuest *quest = NULL;
	float delay = 0.0;

	if (ExtractArgs(EXTRACT_ARGS, &quest, &delay))
	{
		TESScriptableForm *scriptable = DYNAMIC_CAST(quest, TESQuest, TESScriptableForm);
		Script *script = scriptable->script;
		if (script)
		{
			*result = script->questDelayTimeCounter;
			script->questDelayTimeCounter = delay;
		}
	}
	return true;
}
