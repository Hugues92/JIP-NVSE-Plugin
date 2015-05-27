#include "jip_nvse.h"

bool NVSEPlugin_Query(const NVSEInterface *nvse, PluginInfo *info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "JIP NVSE Plugin";
	info->version = 13;
	_MESSAGE("JIP NVSE Plugin Query\nJIP version = %d\nNVSE version = %08X", info->version, nvse->nvseVersion);

	if(nvse->nvseVersion < 0x4050070)
	{
		_ERROR("ERROR: NVSE version is outdated. This plugin requires v4.5b7 (04050070) minimum.");
		return false;
	}
	if(!nvse->isEditor)
	{
#if RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525
		if(nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525)
		{
			_ERROR("ERROR: Incorrect runtime version. This plugin requires v1.4.0.525 minimum.");
			return false;
		}
#elif RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525ng
		if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525ng)
		{
			_ERROR("ERROR: Incorrect runtime version. This plugin requires v1.4.0.525 minimum.");
			return false;
		}
#endif
	}
	else if(nvse->editorVersion < CS_VERSION_1_4_0_518)
	{
		_ERROR("ERROR: Incorrect editor version. This plugin requires v1.4.0.518 minimum.");
		return false;
	}
	return true;
}

bool NVSEPlugin_Load(const NVSEInterface *nvse)
{
	nvse->SetOpcodeBase(0x2600);
	//	v1.00
	nvse->RegisterCommand(&kCommandInfo_SetPersistent);
	nvse->RegisterCommand(&kCommandInfo_GetActorTemplate);
	nvse->RegisterCommand(&kCommandInfo_GetLeveledActorBase);
	nvse->RegisterCommand(&kCommandInfo_GetCreatureDamage);
	nvse->RegisterCommand(&kCommandInfo_SetCreatureDamage);
	nvse->RegisterCommand(&kCommandInfo_GetIsPoisoned);
	nvse->RegisterCommand(&kCommandInfo_AddItemAlt);
	nvse->RegisterCommand(&kCommandInfo_GetValueAlt);
	nvse->RegisterCommand(&kCommandInfo_IsComponentLoaded);
	nvse->RegisterCommand(&kCommandInfo_InjectUIXML);
	nvse->RegisterCommand(&kCommandInfo_InjectUIComponent);
	nvse->RegisterCommand(&kCommandInfo_GetAmmoTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetAmmoTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetAmmoProjectile);
	nvse->RegisterCommand(&kCommandInfo_SetAmmoProjectile);
	nvse->RegisterCommand(&kCommandInfo_GetNumAmmoEffects);
	nvse->RegisterCommand(&kCommandInfo_GetNthAmmoEffect);
	nvse->RegisterCommand(&kCommandInfo_AddAmmoEffect);
	nvse->RegisterCommand(&kCommandInfo_RemoveAmmoEffect);
	nvse->RegisterCommand(&kCommandInfo_GetAmmoEffectTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetAmmoEffectTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetProjectileTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetProjectileTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetProjectileFlag);
	nvse->RegisterCommand(&kCommandInfo_SetProjectileFlag);
	nvse->RegisterCommand(&kCommandInfo_GetProjectileExplosion);
	nvse->RegisterCommand(&kCommandInfo_SetProjectileExplosion);
	nvse->RegisterCommand(&kCommandInfo_GetExplosionTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetExplosionTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetExplosionTraitForm);
	nvse->RegisterCommand(&kCommandInfo_SetExplosionTraitForm);
	nvse->RegisterCommand(&kCommandInfo_GetExplosionFlag);
	nvse->RegisterCommand(&kCommandInfo_SetExplosionFlag);
	nvse->RegisterCommand(&kCommandInfo_AddScriptVariable);
	nvse->RegisterCommand(&kCommandInfo_GetDelayElapsed);
	nvse->RegisterCommand(&kCommandInfo_SetDelayElapsed);
	nvse->RegisterCommand(&kCommandInfo_GetPackageFlag);
	nvse->RegisterCommand(&kCommandInfo_SetPackageFlag);
	nvse->RegisterCommand(&kCommandInfo_GetGlobalVariable);
	nvse->RegisterCommand(&kCommandInfo_SetGlobalVariable);
	nvse->RegisterCommand(&kCommandInfo_GetFactionReputationType);
	nvse->RegisterCommand(&kCommandInfo_SetFactionReputationType);
	nvse->RegisterTypedCommand(&kCommandInfo_RefToString, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_StringToRef);
	nvse->RegisterCommand(&kCommandInfo_GetObjectDimensions);
	nvse->RegisterCommand(&kCommandInfo_GetIsItem);
	nvse->RegisterCommand(&kCommandInfo_GetMinOf);
	nvse->RegisterCommand(&kCommandInfo_GetMaxOf);
	//	v2.00
	nvse->RegisterCommand(&kCommandInfo_GetCombatStyleTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetCombatStyleTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetCombatStyleFlag);
	nvse->RegisterCommand(&kCommandInfo_SetCombatStyleFlag);
	nvse->RegisterCommand(&kCommandInfo_SetLinkedReference);
	nvse->RegisterTypedCommand(&kCommandInfo_GetEnableChildren, kRetnType_Array);
	nvse->RegisterTypedCommand(&kCommandInfo_GetLinkedChildren, kRetnType_Array);
	nvse->RegisterTypedCommand(&kCommandInfo_GetFollowers, kRetnType_Array);
	nvse->RegisterCommand(&kCommandInfo_GetCursorPos);
	nvse->RegisterTypedCommand(&kCommandInfo_GetUIString, kRetnType_String);
	nvse->RegisterTypedCommand(&kCommandInfo_ReadArrayFromFile, kRetnType_Array);
	nvse->RegisterTypedCommand(&kCommandInfo_ReadStringFromFile, kRetnType_String);
	//	v3.00
	nvse->RegisterCommand(&kCommandInfo_WriteArrayToFile);
	nvse->RegisterCommand(&kCommandInfo_WriteStringToFile);
	nvse->RegisterCommand(&kCommandInfo_GetClassTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetClassTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetClassFlag);
	nvse->RegisterCommand(&kCommandInfo_SetClassFlag);
	nvse->RegisterCommand(&kCommandInfo_GetLightTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetLightTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetLightFlag);
	nvse->RegisterCommand(&kCommandInfo_SetLightFlag);
	nvse->RegisterCommand(&kCommandInfo_GetSoundTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetSoundTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetSoundFlag);
	nvse->RegisterCommand(&kCommandInfo_SetSoundFlag);
	//	v4.00
	nvse->RegisterCommand(&kCommandInfo_GetWeaponDetectionSoundLevel);
	nvse->RegisterCommand(&kCommandInfo_SetWeaponDetectionSoundLevel);
	nvse->RegisterCommand(&kCommandInfo_IsEquippedWeaponSilenced);
	nvse->RegisterCommand(&kCommandInfo_IsEquippedWeaponScoped);
	nvse->RegisterCommand(&kCommandInfo_GetWeaponSound);
	nvse->RegisterCommand(&kCommandInfo_SetWeaponSound);
	nvse->RegisterCommand(&kCommandInfo_SetWeaponItemMod);
	nvse->RegisterCommand(&kCommandInfo_SetWeaponItemModEffect);
	nvse->RegisterCommand(&kCommandInfo_SetWeaponItemModValue);
	nvse->RegisterCommand(&kCommandInfo_GetWeatherImageSpaceMod);
	nvse->RegisterCommand(&kCommandInfo_SetWeatherImageSpaceMod);
	nvse->RegisterTypedCommand(&kCommandInfo_GetWeatherTexture, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_SetWeatherTexture);
	nvse->RegisterTypedCommand(&kCommandInfo_GetWeatherPrecipitationModel, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_SetWeatherPrecipitationModel);
	nvse->RegisterCommand(&kCommandInfo_GetWeatherTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetWeatherTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetWeatherRGBColor);
	nvse->RegisterCommand(&kCommandInfo_SetWeatherRGBColor);
	nvse->RegisterCommand(&kCommandInfo_GetLandTextureTextureSet);
	nvse->RegisterCommand(&kCommandInfo_SetLandTextureTextureSet);
	nvse->RegisterCommand(&kCommandInfo_GetLandTextureTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetLandTextureTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetLandTextureNumGrasses);
	nvse->RegisterCommand(&kCommandInfo_GetLandTextureNthGrass);
	nvse->RegisterCommand(&kCommandInfo_LandTextureAddGrass);
	nvse->RegisterCommand(&kCommandInfo_LandTextureRemoveGrass);
	nvse->RegisterTypedCommand(&kCommandInfo_GetTextureSetTexture, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_SetTextureSetTexture);
	nvse->RegisterCommand(&kCommandInfo_GetTextureSetTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetTextureSetTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetTextureSetFlag);
	nvse->RegisterCommand(&kCommandInfo_SetTextureSetFlag);
	nvse->RegisterTypedCommand(&kCommandInfo_GetClimateSunTexture, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_SetClimateSunTexture);
	nvse->RegisterTypedCommand(&kCommandInfo_GetClimateSunGlareTexture, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_SetClimateSunGlareTexture);
	nvse->RegisterTypedCommand(&kCommandInfo_GetClimateNightSkyModel, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_SetClimateNightSkyModel);
	nvse->RegisterCommand(&kCommandInfo_GetClimateTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetClimateTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetClimateNumWeatherTypes);
	nvse->RegisterCommand(&kCommandInfo_GetClimateNthWeatherType);
	nvse->RegisterCommand(&kCommandInfo_GetClimateNthWeatherChance);
	nvse->RegisterCommand(&kCommandInfo_GetClimateNthWeatherGlobal);
	nvse->RegisterCommand(&kCommandInfo_ClimateAddWeatherType);
	nvse->RegisterCommand(&kCommandInfo_ClimateRemoveWeatherType);
	//	v5.00
	nvse->RegisterCommand(&kCommandInfo_GetPrimitiveBound);
	nvse->RegisterCommand(&kCommandInfo_SetPrimitiveBound);
	nvse->RegisterCommand(&kCommandInfo_GetNumEffects);
	nvse->RegisterCommand(&kCommandInfo_GetNthEffectBase);
	nvse->RegisterCommand(&kCommandInfo_GetNthEffectTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetNthEffectTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetNumActorEffects);
	nvse->RegisterCommand(&kCommandInfo_GetNthActorEffect);
	//	v6.00
	nvse->RegisterCommand(&kCommandInfo_GetActiveMenuMode);
	nvse->RegisterTypedCommand(&kCommandInfo_GetActiveUIComponentName, kRetnType_String);
	nvse->RegisterTypedCommand(&kCommandInfo_GetActiveUIComponentFullName, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_GetActiveUIComponentID);
	nvse->RegisterCommand(&kCommandInfo_GetMenuTargetRef);
	nvse->RegisterCommand(&kCommandInfo_GetMenuItemFilter);
	nvse->RegisterCommand(&kCommandInfo_ClickMenuButton);
	//	v7.00
	nvse->RegisterCommand(&kCommandInfo_RemoveItemTarget);
	nvse->RegisterCommand(&kCommandInfo_GetActorLevelingData);
	nvse->RegisterCommand(&kCommandInfo_SetActorLevelingData);
	//	v8.00
	nvse->RegisterCommand(&kCommandInfo_SetNthEffectBase);
	nvse->RegisterCommand(&kCommandInfo_HasVariableAdded);
	nvse->RegisterCommand(&kCommandInfo_RemoveScriptVariable);
	nvse->RegisterCommand(&kCommandInfo_GetLoadOrderChanged);
	nvse->RegisterCommand(&kCommandInfo_ValidateModIndex);
	nvse->RegisterCommand(&kCommandInfo_RemoveAllAddedVariables);
	//	v9.00
	nvse->RegisterCommand(&kCommandInfo_AuxiliaryVariableGetSize);
	nvse->RegisterCommand(&kCommandInfo_AuxiliaryVariableGetType);
	nvse->RegisterCommand(&kCommandInfo_AuxiliaryVariableGetFloat);
	nvse->RegisterCommand(&kCommandInfo_AuxiliaryVariableSetFloat);
	nvse->RegisterCommand(&kCommandInfo_AuxiliaryVariableGetRef);
	nvse->RegisterCommand(&kCommandInfo_AuxiliaryVariableSetRef);
	nvse->RegisterTypedCommand(&kCommandInfo_AuxiliaryVariableGetString, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_AuxiliaryVariableSetString);
	nvse->RegisterTypedCommand(&kCommandInfo_AuxiliaryVariableGetAll, kRetnType_Array);
	nvse->RegisterCommand(&kCommandInfo_AuxiliaryVariableErase);
	nvse->RegisterCommand(&kCommandInfo_AuxiliaryVariableEraseAll);
	nvse->RegisterCommand(&kCommandInfo_RefMapArrayGetSize);
	nvse->RegisterCommand(&kCommandInfo_RefMapArrayGetType);
	nvse->RegisterCommand(&kCommandInfo_RefMapArrayGetFirst);
	nvse->RegisterCommand(&kCommandInfo_RefMapArrayGetNext);
	nvse->RegisterTypedCommand(&kCommandInfo_RefMapArrayGetKeys, kRetnType_Array);
	nvse->RegisterTypedCommand(&kCommandInfo_RefMapArrayGetAll, kRetnType_Array);
	nvse->RegisterCommand(&kCommandInfo_RefMapArrayErase);
	nvse->RegisterCommand(&kCommandInfo_RefMapArrayDestroy);
	nvse->RegisterCommand(&kCommandInfo_RefMapArrayGetFloat);
	nvse->RegisterCommand(&kCommandInfo_RefMapArraySetFloat);
	nvse->RegisterCommand(&kCommandInfo_RefMapArrayGetRef);
	nvse->RegisterCommand(&kCommandInfo_RefMapArraySetRef);
	nvse->RegisterTypedCommand(&kCommandInfo_RefMapArrayGetString, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_RefMapArraySetString);
	nvse->RegisterCommand(&kCommandInfo_ClearJIPSavedData);
	//	9.10
	nvse->RegisterCommand(&kCommandInfo_SetValueAlt);
	//	10.00
	nvse->RegisterTypedCommand(&kCommandInfo_GetBodyPartDataSkeleton, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_SetBodyPartDataSkeleton);
	nvse->RegisterCommand(&kCommandInfo_GetBodyPartDataRagdoll);
	nvse->RegisterCommand(&kCommandInfo_SetBodyPartDataRagdoll);
	nvse->RegisterCommand(&kCommandInfo_GetBodyPartDataHasPart);
	nvse->RegisterCommand(&kCommandInfo_GetBodyPartTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_SetBodyPartTraitNumeric);
	nvse->RegisterCommand(&kCommandInfo_GetBodyPartTraitForm);
	nvse->RegisterCommand(&kCommandInfo_SetBodyPartTraitForm);
	nvse->RegisterTypedCommand(&kCommandInfo_GetBodyPartName, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_SetBodyPartName);
	nvse->RegisterTypedCommand(&kCommandInfo_GetBodyPartReplacementModel, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_SetBodyPartReplacementModel);
	nvse->RegisterCommand(&kCommandInfo_GetBodyPartFlag);
	nvse->RegisterCommand(&kCommandInfo_SetBodyPartFlag);
	nvse->RegisterCommand(&kCommandInfo_GetWeaponModReloadAnim);
	nvse->RegisterCommand(&kCommandInfo_SetWeaponModReloadAnim);
	//	11.00
	nvse->RegisterTypedCommand(&kCommandInfo_AuxiliaryVariableGetAsArray, kRetnType_Array);
	nvse->RegisterCommand(&kCommandInfo_AuxiliaryVariableSetFromArray);
	nvse->RegisterTypedCommand(&kCommandInfo_GetSoundSourceFile, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_SetSoundSourceFile);
	nvse->RegisterCommand(&kCommandInfo_GetDestructionDataHealth);
	nvse->RegisterCommand(&kCommandInfo_SetDestructionDataHealth);
	nvse->RegisterCommand(&kCommandInfo_GetDestructionDataTargetable);
	nvse->RegisterCommand(&kCommandInfo_SetDestructionDataTargetable);
	nvse->RegisterCommand(&kCommandInfo_GetDestructionDataNumStages);
	nvse->RegisterCommand(&kCommandInfo_GetNthDestructionStageTrait);
	nvse->RegisterCommand(&kCommandInfo_SetNthDestructionStageTrait);
	nvse->RegisterCommand(&kCommandInfo_GetNthDestructionStageExplosion);
	nvse->RegisterCommand(&kCommandInfo_SetNthDestructionStageExplosion);
	nvse->RegisterCommand(&kCommandInfo_GetNthDestructionStageDebris);
	nvse->RegisterCommand(&kCommandInfo_SetNthDestructionStageDebris);
	nvse->RegisterTypedCommand(&kCommandInfo_GetNthDestructionStageReplacement, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_SetNthDestructionStageReplacement);
	//	12.00
	nvse->RegisterTypedCommand(&kCommandInfo_GetTeammates, kRetnType_Array);
	nvse->RegisterCommand(&kCommandInfo_GetCurrentWeather);
	nvse->RegisterCommand(&kCommandInfo_GetCurrentClimate);
	nvse->RegisterCommand(&kCommandInfo_SetCurrentClimate);
	nvse->RegisterCommand(&kCommandInfo_RefreshCurrentClimate);
	nvse->RegisterCommand(&kCommandInfo_GetWorldspaceClimate);
	nvse->RegisterCommand(&kCommandInfo_SetWorldspaceClimate);
	nvse->RegisterCommand(&kCommandInfo_GetActorVoiceType);
	nvse->RegisterCommand(&kCommandInfo_SetActorVoiceType);
	//	13.00
	nvse->RegisterCommand(&kCommandInfo_GetCreatureReach);
	nvse->RegisterCommand(&kCommandInfo_GetIsImmobile);
	nvse->RegisterCommand(&kCommandInfo_PickFromList);
	nvse->RegisterCommand(&kCommandInfo_CCCOnRestart);
	nvse->RegisterCommand(&kCommandInfo_CCCOnLoad);
	nvse->RegisterCommand(&kCommandInfo_CCCSetFloat);
	nvse->RegisterCommand(&kCommandInfo_CCCSetString);
	nvse->RegisterCommand(&kCommandInfo_CCCSetTrait);
	nvse->RegisterCommand(&kCommandInfo_CCCGetDistance);
	nvse->RegisterCommand(&kCommandInfo_CCCGetCombatStyle);
	nvse->RegisterCommand(&kCommandInfo_CCCInfAmmoWeapon);
	nvse->RegisterCommand(&kCommandInfo_CCCInFaction);
	nvse->RegisterCommand(&kCommandInfo_CCCSetScript);
	nvse->RegisterCommand(&kCommandInfo_CCCIsModified);
	nvse->RegisterCommand(&kCommandInfo_CCCLoadNCCS);
	nvse->RegisterCommand(&kCommandInfo_CCCSavedForm);
	nvse->RegisterTypedCommand(&kCommandInfo_CCCLocationName, kRetnType_String);
	nvse->RegisterCommand(&kCommandInfo_CCCGetReputation);

	if(!nvse->isEditor)
	{
		g_pluginHandle = nvse->GetPluginHandle();
		g_serialization = (NVSESerializationInterface*)nvse->QueryInterface(kInterface_Serialization);
		g_serialization->SetLoadCallback(g_pluginHandle, ReceivedLoadCallback);
		g_serialization->SetSaveCallback(g_pluginHandle, ReceivedSaveCallback);
		g_serialization->SetNewGameCallback(g_pluginHandle, ReceivedNewGameCallback);
		g_script = (NVSEScriptInterface*)nvse->QueryInterface(kInterface_Script);
		g_string = (NVSEStringVarInterface*)nvse->QueryInterface(kInterface_StringVar);
		g_array = (NVSEArrayVarInterface*)nvse->QueryInterface(kInterface_ArrayVar);
	}
	return true;
}
