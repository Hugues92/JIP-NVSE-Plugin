#pragma once

DEFINE_COMMAND_PLUGIN(GetLandTextureTextureSet, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetLandTextureTextureSet, "None", 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetLandTextureTraitNumeric, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetLandTextureTraitNumeric, "None", 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(GetLandTextureNumGrasses, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetLandTextureNthGrass, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(LandTextureAddGrass, "None", 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(LandTextureRemoveGrass, "None", 0, 2, kParams_JIP_TwoForms);

bool Cmd_GetLandTextureTextureSet_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		TESLandTexture *landTex = DYNAMIC_CAST(form, TESForm, TESLandTexture);
		if (landTex && landTex->textureSet) *refResult = landTex->textureSet->refID;
	}
	return true;
}

bool Cmd_SetLandTextureTextureSet_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *ltForm = NULL, *tsForm = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &ltForm, &tsForm))
	{
		TESLandTexture *landTex = DYNAMIC_CAST(ltForm, TESForm, TESLandTexture);
		BGSTextureSet *texSet = DYNAMIC_CAST(tsForm, TESForm, BGSTextureSet);
		if (landTex && texSet)
		{
			if (landTex->textureSet) *refResult = landTex->textureSet->refID;
			landTex->textureSet = texSet;
		}
	}
	return true;
}

bool Cmd_GetLandTextureTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID) && ValidInput(traitID, 3))
	{
		TESLandTexture *landTex = DYNAMIC_CAST(form, TESForm, TESLandTexture);
		if (landTex)
		{
			if (!traitID) *result = landTex->materialType;
			else if (traitID == 1) *result = landTex->friction;
			else if (traitID == 2) *result = landTex->restitution;
			else *result = landTex->specularExponent;
		}
	}
	return true;
}

bool Cmd_SetLandTextureTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 traitID = 0, val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &traitID, &val) && ValidInput(traitID, 3))
	{
		TESLandTexture *landTex = DYNAMIC_CAST(form, TESForm, TESLandTexture);
		if (landTex)
		{
			if (traitID == 1)
			{
				*result = landTex->friction;
				landTex->friction = val;
			}
			else if (traitID == 2)
			{
				*result = landTex->restitution;
				landTex->restitution = val;
			}
			else if (traitID == 3)
			{
				*result = landTex->specularExponent;
				landTex->specularExponent = val;
			}
			else if ((val >= 0) && (val < 32))
			{
				*result = landTex->materialType;
				landTex->materialType = val;
			}
		}
	}
	return true;
}

bool Cmd_GetLandTextureNumGrasses_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		TESLandTexture *landTex = DYNAMIC_CAST(form, TESForm, TESLandTexture);
		if (landTex) *result = landTex->grasses.Count();
	}
	return true;
}

bool Cmd_GetLandTextureNthGrass_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, idx = 0;
	*refResult = 0;
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form, &idx))
	{
		TESLandTexture *landTex = DYNAMIC_CAST(form, TESForm, TESLandTexture);
		if (landTex && (idx < landTex->grasses.Count()))
		{
			TESGrass *grass = landTex->grasses.GetNthItem(idx);
			if (grass) *refResult = grass->refID;
		}
	}
	return true;
}

SInt8 GetGrassIndex(tList<TESGrass> &lst, TESGrass *grass)
{
	for (SInt8 idx = 0; idx < lst.Count(); idx++)
	{
		if (lst.GetNthItem(idx) == grass) return idx;
	}
	return -1;
}

bool Cmd_LandTextureAddGrass_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *ltxForm = NULL, *grsForm = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &ltxForm, &grsForm))
	{
		TESLandTexture *landTex = DYNAMIC_CAST(ltxForm, TESForm, TESLandTexture);
		TESGrass *grass = DYNAMIC_CAST(grsForm, TESForm, TESGrass);
		if (landTex && grass && (GetGrassIndex(landTex->grasses, grass) == -1))
		{
			landTex->grasses.AddAt(grass, eListEnd);
			*result = 1;
		}
	}
	return true;
}

bool Cmd_LandTextureRemoveGrass_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *ltxForm = NULL, *grsForm = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &ltxForm, &grsForm))
	{
		TESLandTexture *landTex = DYNAMIC_CAST(ltxForm, TESForm, TESLandTexture);
		TESGrass *grass = DYNAMIC_CAST(grsForm, TESForm, TESGrass);
		if (landTex && grass)
		{
			SInt8 idx = GetGrassIndex(landTex->grasses, grass);
			if (idx >= 0)
			{
				landTex->grasses.RemoveNth(idx);
				*result = 1;
			}
		}
	}
	return true;
}
