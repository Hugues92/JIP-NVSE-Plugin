#pragma once

DEFINE_COMMAND_PLUGIN(GetTextureSetTexture, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetTextureSetTexture, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(GetTextureSetTraitNumeric, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetTextureSetTraitNumeric, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetTextureSetFlag, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetTextureSetFlag, "None", 0, 3, kParams_JIP_OneForm_TwoInts);

bool Cmd_GetTextureSetTexture_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	UInt32 mapID = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &mapID) && ValidInput(mapID, 5))
	{
		BGSTextureSet *texSet = DYNAMIC_CAST(form, TESForm, BGSTextureSet);
		if (texSet)
		{
			g_string->Assign(PASS_COMMAND_ARGS, texSet->textures[mapID].ddsPath.CStr());
			return true;
		}
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");

	return true;
}

bool Cmd_SetTextureSetTexture_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	UInt32 mapID = 0;
	char ddsPath[512] = {0};
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &mapID, &ddsPath) && ValidInput(mapID, 5))
	{
		BGSTextureSet *texSet = DYNAMIC_CAST(form, TESForm, BGSTextureSet);
		if (texSet) texSet->textures[mapID].ddsPath.Set(ddsPath);
	}
	return true;
}

bool Cmd_GetTextureSetTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID) && ValidInput(traitID, 8))
	{
		BGSTextureSet *texSet = DYNAMIC_CAST(form, TESForm, BGSTextureSet);
		if (texSet && texSet->decalInfo)
		{
			if (!traitID) *result = texSet->decalInfo->minWidth;
			else if (traitID == 1) *result = texSet->decalInfo->maxWidth;
			else if (traitID == 2) *result = texSet->decalInfo->minHeight;
			else if (traitID == 3) *result = texSet->decalInfo->maxHeight;
			else if (traitID == 4) *result = texSet->decalInfo->depth;
			else if (traitID == 5) *result = texSet->decalInfo->shininess;
			else if (traitID == 6) *result = texSet->decalInfo->parallaxScale;
			else if (traitID == 7) *result = texSet->decalInfo->parallaxPasses;
			else *result = RGBHexToDec(texSet->decalInfo->color);
		}
	}
	return true;
}

bool Cmd_SetTextureSetTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;
	float val = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID, &val) && ValidInput(traitID, 8) && (val >= 0))
	{
		BGSTextureSet *texSet = DYNAMIC_CAST(form, TESForm, BGSTextureSet);
		if (texSet && texSet->decalInfo)
		{
			if (!traitID)
			{
				*result = texSet->decalInfo->minWidth;
				texSet->decalInfo->minWidth = val;
			}
			else if (traitID == 1)
			{
				*result = texSet->decalInfo->maxWidth;
				texSet->decalInfo->maxWidth = val;
			}
			else if (traitID == 2)
			{
				*result = texSet->decalInfo->minHeight;
				texSet->decalInfo->minHeight = val;
			}
			else if (traitID == 3)
			{
				*result = texSet->decalInfo->maxHeight;
				texSet->decalInfo->maxHeight = val;
			}
			else if (traitID == 4)
			{
				*result = texSet->decalInfo->depth;
				texSet->decalInfo->depth = val;
			}
			else if (traitID == 5)
			{
				*result = texSet->decalInfo->shininess;
				texSet->decalInfo->shininess = val;
			}
			else if (traitID == 6)
			{
				*result = texSet->decalInfo->parallaxScale;
				texSet->decalInfo->parallaxScale = val;
			}
			else if (traitID == 7)
			{
				*result = texSet->decalInfo->parallaxPasses;
				texSet->decalInfo->parallaxPasses = val;
			}
			else if (val <= 255255255)
			{
				*result = RGBHexToDec(texSet->decalInfo->color);
				texSet->decalInfo->color = RGBDecToHex((UInt32)val);
			}
		}
	}
	return true;
}

bool Cmd_GetTextureSetFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 flag = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &flag) && ValidInput(flag, 3))
	{
		BGSTextureSet *texSet = DYNAMIC_CAST(form, TESForm, BGSTextureSet);
		if (texSet)
		{
			if (!flag) *result = texSet->texFlags ? 1 : 0;
			else if (texSet->decalInfo) *result = (texSet->decalInfo->flags & (1 << (flag - 1))) ? 1 : 0;
		}
	}
	return true;
}

bool Cmd_SetTextureSetFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 flag = 0, val = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &flag, &val) && ValidInput(flag, 3))
	{
		BGSTextureSet *texSet = DYNAMIC_CAST(form, TESForm, BGSTextureSet);
		if (texSet)
		{
			if (!flag)
			{
				*result = texSet->texFlags ? 1 : 0;
				texSet->texFlags = val ? 1 : 0;
			}
			else if (texSet->decalInfo)
			{
				*result = (texSet->decalInfo->flags & (1 << (flag - 1))) ? 1 : 0;
				val ? (texSet->decalInfo->flags |= (1 << (flag - 1))) : (texSet->decalInfo->flags &= ~(1 << (flag - 1)));
			}
		}
	}
	return true;
}
