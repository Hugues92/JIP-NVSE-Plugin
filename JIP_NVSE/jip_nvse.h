#pragma once

#include "nvse/PluginAPI.h"
#include "nvse/ParamInfos.h"
#include "nvse/GameObjects.h"
#include "nvse/GameData.h"
#include "nvse/GameEffects.h"
#include "nvse/GameRTTI.h"
#include "nvse/GameUI.cpp"
#include "nvse/InventoryInfo.h"
#include "common/IFileStream.h"

#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "headers/jip_internal.h"
#include "headers/fn_actor.h"
#include "headers/fn_ammo.h"
#include "headers/fn_ammo_effect.h"
#include "headers/fn_aux_variable.h"
#include "headers/fn_body_part_data.h"
#include "headers/fn_class.h"
#include "headers/fn_climate.h"
#include "headers/fn_combat_style.h"
#include "headers/fn_destruction_data.h"
#include "headers/fn_effect.h"
#include "headers/fn_explosion.h"
#include "headers/fn_faction.h"
#include "headers/fn_global_var.h"
#include "headers/fn_inventory.h"
#include "headers/fn_land_texture.h"
#include "headers/fn_light.h"
#include "headers/fn_miscellaneous.h"
#include "headers/fn_package.h"
#include "headers/fn_projectile.h"
#include "headers/fn_ref_map.h"
#include "headers/fn_script.h"
#include "headers/fn_sound.h"
#include "headers/fn_texture_set.h"
#include "headers/fn_ui.h"
#include "headers/fn_utility.h"
#include "headers/fn_weapon.h"
#include "headers/fn_weather.h"

#include "headers/fn_jip_ccc.h"

std::string g_lastLoadedPath;

void ReadDataDump(UInt16 type, UInt8 *dump)
{
	if (type == 4) g_serialization->ReadRecordData(&type, 2);
	else type = 4;
	g_serialization->ReadRecordData(dump, type);
}

void RestoreLinkedRefs(LinkedRefMap *tempMap = NULL)
{
	if (!g_linkedRefDefault.empty())
	{
		LinkedRefMap::iterator linkIter = g_linkedRefDefault.begin(), linkTemp;
		while (linkIter != g_linkedRefDefault.end())
		{
			linkTemp = linkIter++;
			if (!tempMap || !tempMap->count(linkTemp->first)) SetLinkedRefID(linkTemp->first);
		}
	}
}

void ReceivedLoadCallback(void *p)
{
	UInt32 type, version, length, buffer4, refID;
	UInt8 buffer1, modIdx, dump[kMaxMessageLength];
	UInt16 nRecs, nRefs, nVars;
	char varName[256];
	LinkedRefMap linkedRefsTemp;

	std::string currentPath(g_serialization->GetSavePath());
	if (g_lastLoadedPath != currentPath)
	{
		g_lastLoadedPath = currentPath;
		g_dataChangedFlags = 7;
	}
	g_scriptVariablesBuffer.clear();
	if (g_dataChangedFlags & 1) g_auxVariablesPerm.clear();
	if (g_dataChangedFlags & 2) g_refMapArraysPerm.clear();
	if (g_dataChangedFlags & 4) g_linkedRefModified.clear();

	while (g_serialization->GetNextRecordInfo(&type, &version, &length))
	{
		g_serialization->ReadRecordData(&nRecs, 2);
		if (type == 'VSPJ')
		{
			TESForm *form;
			Script *pScript;
			ScriptEventList *pEventList;
			double value;
			ScriptEventList::Var *var;
			UInt32 *pVal;
			while (nRecs)
			{
				nRecs--;
				g_serialization->ReadRecordData(&buffer4, 4);
				if (!g_serialization->ResolveRefID(buffer4, &refID) || !(form = LookupFormByID(refID)) || !GetScriptAndEventList(form, &pScript, &pEventList)) continue;
				g_serialization->ReadRecordData(&nVars, 2);
				while (nVars)
				{
					nVars--;
					g_serialization->ReadRecordData(&modIdx, 1);
					g_serialization->ReadRecordData(&buffer1, 1);
					g_serialization->ReadRecordData(varName, buffer1);
					varName[buffer1] = 0;
					g_serialization->ReadRecordData(&value, 8);
					if (!g_serialization->ResolveRefID(modIdx << 24, &buffer4)) continue;
					if (var = AddScriptVariable(pScript, pEventList, refID, buffer4 >> 24, varName))
					{
						pVal = (UInt32*)&value;
						if (*(pVal + 1) || !*pVal) var->data = value;
						else if (g_serialization->ResolveRefID(*pVal, pVal)) *(UInt64*)&var->data = *pVal;
						else var->data = 0;
					}
				}
			}
		}
		else if (type == 'VAPJ')
		{
			if (!(g_dataChangedFlags & 1)) continue;
			AuxVarOwnersMap *ownersMap;
			AuxVarVarsMap *aVarsMap;
			AuxVarValsArr *valsArr;
			UInt16 nElems;
			while (nRecs)
			{
				nRecs--;
				g_serialization->ReadRecordData(&buffer1, 1);
				if (!g_serialization->ResolveRefID(buffer1 << 24, &buffer4)) continue;
				ownersMap = NULL;
				modIdx = buffer4 >> 24;
				g_serialization->ReadRecordData(&nRefs, 2);
				while (nRefs)
				{
					nRefs--;
					g_serialization->ReadRecordData(&refID, 4);
					if (g_serialization->ResolveRefID(refID, &refID) && LookupFormByID(refID))
					{
						if (!ownersMap) ownersMap = &g_auxVariablesPerm[modIdx];
						aVarsMap = &(*ownersMap)[refID];
					}
					else aVarsMap = NULL;
					g_serialization->ReadRecordData(&nVars, 2);
					while (nVars)
					{
						nVars--;
						valsArr = NULL;
						g_serialization->ReadRecordData(&buffer1, 1);
						g_serialization->ReadRecordData(varName, buffer1);
						varName[buffer1] = 0;
						g_serialization->ReadRecordData(&nElems, 2);
						while (nElems)
						{
							nElems--;
							g_serialization->ReadRecordData(&buffer1, 1);
							if (aVarsMap)
							{
								if (!valsArr) valsArr = &(*aVarsMap)[varName];
								valsArr->push_back(AuxVariableValue(buffer1));
							}
							else ReadDataDump(buffer1, dump);
						}
					}
				}
			}
		}
		else if (type == 'MRPJ')
		{
			if (!(g_dataChangedFlags & 2)) continue;
			RefMapVarsMap *rVarsMap;
			RefMapIDsMap *idsMap;
			while (nRecs)
			{
				nRecs--;
				g_serialization->ReadRecordData(&buffer1, 1);
				if (!g_serialization->ResolveRefID(buffer1 << 24, &buffer4)) continue;
				rVarsMap = NULL;
				modIdx = buffer4 >> 24;
				g_serialization->ReadRecordData(&nVars, 2);
				while (nVars)
				{
					nVars--;
					idsMap = NULL;
					g_serialization->ReadRecordData(&buffer1, 1);
					g_serialization->ReadRecordData(varName, buffer1);
					varName[buffer1] = 0;
					g_serialization->ReadRecordData(&nRefs, 2);
					while (nRefs)
					{
						nRefs--;
						g_serialization->ReadRecordData(&refID, 4);
						g_serialization->ReadRecordData(&buffer1, 1);
						if (g_serialization->ResolveRefID(refID, &refID))
						{
							if (!idsMap)
							{
								if (!rVarsMap) rVarsMap = &g_refMapArraysPerm[modIdx];
								idsMap = &(*rVarsMap)[varName];
							}
							(*idsMap)[refID].ReadValData(buffer1);
						}
						else ReadDataDump(buffer1, dump);
					}
				}
			}
		}
		else if ((type == 'RLPJ') && (g_dataChangedFlags & 4))
		{
			UInt32 lnkID;
			while (nRecs)
			{
				nRecs--;
				g_serialization->ReadRecordData(&refID, 4);
				g_serialization->ReadRecordData(&lnkID, 4);
				g_serialization->ReadRecordData(&buffer1, 1);
				if (g_serialization->ResolveRefID(refID, &refID) && g_serialization->ResolveRefID(lnkID, &lnkID) && 
					g_serialization->ResolveRefID(buffer1 << 24, &buffer4) && SetLinkedRefID(refID, lnkID, buffer4 >> 24))
					linkedRefsTemp[refID] = lnkID;
			}
		}
	}
	if (g_dataChangedFlags & 4) RestoreLinkedRefs(&linkedRefsTemp);
	g_dataChangedFlags = 0;
}

void ReceivedSaveCallback(void *p)
{
	UInt8 buffer1;
	UInt16 buffer2;

	g_lastLoadedPath = g_serialization->GetSavePath();
	g_dataChangedFlags = 0;

	if (buffer2 = g_scriptVariablesBuffer.size())
	{
		g_serialization->WriteRecord('VSPJ', 9, &buffer2, 2);
		for (auto svOwnerIt = g_scriptVariablesBuffer.begin(); svOwnerIt != g_scriptVariablesBuffer.end(); ++svOwnerIt)
		{
			g_serialization->WriteRecordData(&svOwnerIt->first, 4);
			buffer2 = svOwnerIt->second.size();
			g_serialization->WriteRecordData(&buffer2, 2);
			for (auto svVarIt = svOwnerIt->second.begin(); svVarIt != svOwnerIt->second.end(); ++svVarIt)
			{
				g_serialization->WriteRecordData(&svVarIt->second.modIdx, 1);
				buffer1 = svVarIt->first.size();
				g_serialization->WriteRecordData(&buffer1, 1);
				g_serialization->WriteRecordData(svVarIt->first.data(), buffer1);
				g_serialization->WriteRecordData(&svVarIt->second.value->data, 8);
			}
		}
	}
	if (buffer2 = g_auxVariablesPerm.size())
	{
		g_serialization->WriteRecord('VAPJ', 9, &buffer2, 2);
		for (auto avModIt = g_auxVariablesPerm.begin(); avModIt != g_auxVariablesPerm.end(); ++avModIt)
		{
			g_serialization->WriteRecordData(&avModIt->first, 1);
			buffer2 = avModIt->second.size();
			g_serialization->WriteRecordData(&buffer2, 2);
			for (auto avOwnerIt = avModIt->second.begin(); avOwnerIt != avModIt->second.end(); ++avOwnerIt)
			{
				g_serialization->WriteRecordData(&avOwnerIt->first, 4);
				buffer2 = avOwnerIt->second.size();
				g_serialization->WriteRecordData(&buffer2, 2);
				for (auto avVarIt = avOwnerIt->second.begin(); avVarIt != avOwnerIt->second.end(); ++avVarIt)
				{
					buffer1 = avVarIt->first.size();
					g_serialization->WriteRecordData(&buffer1, 1);
					g_serialization->WriteRecordData(avVarIt->first.data(), buffer1);
					buffer2 = avVarIt->second.size();
					g_serialization->WriteRecordData(&buffer2, 2);
					for (auto avValIt = avVarIt->second.begin(); avValIt != avVarIt->second.end(); ++avValIt) avValIt->WriteValData();
				}
			}
		}
	}
	if (buffer2 = g_refMapArraysPerm.size())
	{
		g_serialization->WriteRecord('MRPJ', 9, &buffer2, 2);
		for (auto rmModIt = g_refMapArraysPerm.begin(); rmModIt != g_refMapArraysPerm.end(); ++rmModIt)
		{
			g_serialization->WriteRecordData(&rmModIt->first, 1);
			buffer2 = rmModIt->second.size();
			g_serialization->WriteRecordData(&buffer2, 2);
			for (auto rmVarIt = rmModIt->second.begin(); rmVarIt != rmModIt->second.end(); ++rmVarIt)
			{
				buffer1 = rmVarIt->first.size();
				g_serialization->WriteRecordData(&buffer1, 1);
				g_serialization->WriteRecordData(rmVarIt->first.data(), buffer1);
				buffer2 = rmVarIt->second.size();
				g_serialization->WriteRecordData(&buffer2, 2);
				for (auto rmRefIt = rmVarIt->second.begin(); rmRefIt != rmVarIt->second.end(); ++rmRefIt)
				{
					g_serialization->WriteRecordData(&rmRefIt->first, 4);
					rmRefIt->second.WriteValData();
				}
			}
		}
	}
	if (buffer2 = g_linkedRefModified.size())
	{
		g_serialization->WriteRecord('RLPJ', 9, &buffer2, 2);
		for (auto lrRefIt = g_linkedRefModified.begin(); lrRefIt != g_linkedRefModified.end(); ++lrRefIt)
		{
			g_serialization->WriteRecordData(&lrRefIt->first, 4);
			g_serialization->WriteRecordData(&lrRefIt->second, 5);
		}
	}
}

void ReceivedNewGameCallback(void *p)
{
	g_scriptVariablesBuffer.clear();
	g_auxVariablesPerm.clear();
	g_refMapArraysPerm.clear();
	g_linkedRefModified.clear();
	RestoreLinkedRefs();
	g_lastLoadedPath.clear();
	g_dataChangedFlags = 0;
}

DEFINE_COMMAND_PLUGIN(ClearJIPSavedData, "None", 0, 4, kParams_JIP_FourInts);

bool Cmd_ClearJIPSavedData_Execute(COMMAND_ARGS)
{
	UInt32 scrVars = 0, lnkRefs = 0, auxVars = 0, refMaps = 0;

	if (ExtractArgs(EXTRACT_ARGS, &scrVars, &lnkRefs, &auxVars, &refMaps))
	{
		UInt8 modIdx = GetScriptModIdx(scriptObj, thisObj);
		if (scrVars)
		{
			for (auto ownerIter = g_scriptVariablesBuffer.begin(); ownerIter != g_scriptVariablesBuffer.end(); ++ownerIter)
			{
				auto varIter = ownerIter->second.begin();
				while (varIter != ownerIter->second.end())
				{
					if (varIter->second.modIdx == modIdx) varIter = ownerIter->second.erase(varIter);
					else ++varIter;
				}
				if (ownerIter->second.empty()) g_scriptVariablesBuffer.erase(ownerIter);
			}
		}
		if (lnkRefs && !g_linkedRefModified.empty())
		{
			for (auto refIter = g_linkedRefModified.begin(); refIter != g_linkedRefModified.end(); ++refIter)
			{
				if (refIter->second.modIdx == modIdx) refIter = g_linkedRefModified.erase(refIter);
				else ++refIter;
			}
			g_dataChangedFlags |= 4;
		}
		if (auxVars && g_auxVariablesPerm.erase((auxVars == 2) ? 255 : modIdx)) g_dataChangedFlags |= 1;
		if (refMaps && g_refMapArraysPerm.erase((refMaps == 2) ? 255 : modIdx)) g_dataChangedFlags |= 2;
	}
	return true;
}
