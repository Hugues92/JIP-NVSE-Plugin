#pragma once

DEFINE_COMMAND_PLUGIN(GetBodyPartDataSkeleton, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetBodyPartDataSkeleton, "None", 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetBodyPartDataRagdoll, "None", 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetBodyPartDataRagdoll, "None", 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetBodyPartDataHasPart, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetBodyPartTraitNumeric, "None", 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetBodyPartTraitNumeric, "None", 0, 4, kParams_JIP_OneForm_TwoInts_OneFloat);
DEFINE_COMMAND_PLUGIN(GetBodyPartTraitForm, "None", 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetBodyPartTraitForm, "None", 0, 4, kParams_JIP_OneForm_TwoInts_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetBodyPartName, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetBodyPartName, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(GetBodyPartReplacementModel, "None", 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetBodyPartReplacementModel, "None", 0, 3, kParams_JIP_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(GetBodyPartFlag, "None", 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetBodyPartFlag, "None", 0, 4, kParams_JIP_OneForm_ThreeInts);

bool Cmd_GetBodyPartDataSkeleton_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData)
		{
			g_string->Assign(PASS_COMMAND_ARGS, bpData->model.GetModelPath());
			return true;
		}
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");
	return true;
}

bool Cmd_SetBodyPartDataSkeleton_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	char nifPath[512] = {0};
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &nifPath))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData) bpData->model.SetModelPath(nifPath);
	}
	return true;
}

bool Cmd_GetBodyPartDataRagdoll_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL;
	
	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData && bpData->ragDoll) *refResult = bpData->ragDoll->refID;
	}
	return true;
}

bool Cmd_SetBodyPartDataRagdoll_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL, *rdForm = NULL;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &rdForm))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData)
		{
			BGSRagdoll *ragDoll = rdForm ? DYNAMIC_CAST(rdForm, TESForm, BGSRagdoll) : NULL;
			if (bpData->ragDoll) *refResult = bpData->ragDoll->refID;
			bpData->ragDoll = ragDoll;
		}
	}
	return true;
}

bool Cmd_GetBodyPartDataHasPart_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 partID = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &partID) && ValidInput(partID, 14))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData && bpData->bodyParts[partID]) *result = 1;
	}
	return true;
}

const double g_radialConvertion = 0.0174532925;

bool Cmd_GetBodyPartTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 partID = 0, traitID = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &partID, &traitID) && ValidInput(partID, 14) && ValidInput(traitID, 18))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData)
		{
			BGSBodyPart *bodyPart = bpData->bodyParts[partID];
			if (bodyPart)
			{
				if (!traitID) *result = bodyPart->actorValue;
				else if (traitID == 1) *result = bodyPart->damageMult;
				else if (traitID == 2) *result = bodyPart->healthPercent;
				else if (traitID == 3) *result = bodyPart->toHitChance;
				else if (traitID == 4) *result = bodyPart->trackingMaxAngle;
				else if (traitID == 5) *result = bodyPart->sevrDebrisCount;
				else if (traitID == 6) *result = bodyPart->sevrDebrisScale;
				else if (traitID == 7) *result = bodyPart->sevrDecalCount;
				else if (traitID == 8) *result = bodyPart->explChance;
				else if (traitID == 9) *result = bodyPart->limbRepScale;
				else if (traitID == 10) *result = bodyPart->explDebrisCount;
				else if (traitID == 11) *result = bodyPart->explDebrisScale;
				else if (traitID == 12) *result = bodyPart->explDecalCount;
				else if (traitID < 16) *result = bodyPart->goreEffTranslate[traitID - 13];
				else *result = bodyPart->goreEffRotation[traitID - 16] / g_radialConvertion;
			}
		}
	}
	return true;
}

bool Cmd_SetBodyPartTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 partID = 0, traitID = 0;
	float val = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &partID, &traitID, &val) && ValidInput(partID, 14) && ValidInput(traitID, 18))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData)
		{
			BGSBodyPart *bodyPart = bpData->bodyParts[partID];
			if (bodyPart)
			{
				if (val < 0) val = 0;
				if (!traitID)
				{
					*result = bodyPart->actorValue;
					bodyPart->actorValue = val;
				}
				else if (traitID == 1)
				{
					*result = bodyPart->damageMult;
					bodyPart->damageMult = val;
				}
				else if (traitID == 2)
				{
					*result = bodyPart->healthPercent;
					bodyPart->healthPercent = val;
				}
				else if (traitID == 3)
				{
					*result = bodyPart->toHitChance;
					bodyPart->toHitChance = val;
				}
				else if (traitID == 4)
				{
					*result = bodyPart->trackingMaxAngle;
					bodyPart->trackingMaxAngle = val;
				}
				else if (traitID == 5)
				{
					*result = bodyPart->sevrDebrisCount;
					bodyPart->sevrDebrisCount = val;
				}
				else if (traitID == 6)
				{
					*result = bodyPart->sevrDebrisScale;
					bodyPart->sevrDebrisScale = val;
				}
				else if (traitID == 7)
				{
					*result = bodyPart->sevrDecalCount;
					bodyPart->sevrDecalCount = val;
				}
				else if (traitID == 8)
				{
					*result = bodyPart->explChance;
					bodyPart->explChance = val;
				}
				else if (traitID == 9)
				{
					*result = bodyPart->limbRepScale;
					bodyPart->limbRepScale = val;
				}
				else if (traitID == 10)
				{
					*result = bodyPart->explDebrisCount;
					bodyPart->explDebrisCount = val;
				}
				else if (traitID == 11)
				{
					*result = bodyPart->explDebrisScale;
					bodyPart->explDebrisScale = val;
				}
				else if (traitID == 12)
				{
					*result = bodyPart->explDecalCount;
					bodyPart->explDecalCount = val;
				}
				else if (traitID < 16)
				{
					*result = bodyPart->goreEffTranslate[traitID - 13];
					bodyPart->goreEffTranslate[traitID - 13] = val;
				}
				else
				{
					*result = bodyPart->goreEffRotation[traitID - 16] / g_radialConvertion;
					bodyPart->goreEffRotation[traitID - 16] = val * g_radialConvertion;
				}
			}
		}
	}
	return true;
}

bool Cmd_GetBodyPartTraitForm_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL;
	UInt32 partID = 0, traitID = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &partID, &traitID) && ValidInput(partID, 14) && ValidInput(traitID, 5))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData)
		{
			BGSBodyPart *bodyPart = bpData->bodyParts[partID];
			if (bodyPart)
			{
				if (!traitID)
				{
					if (bodyPart->sevrExplosion) *refResult = bodyPart->sevrExplosion->refID;
				}
				else if (traitID == 1)
				{
					if (bodyPart->explExplosion) *refResult = bodyPart->explExplosion->refID;
				}
				else if (traitID == 2)
				{
					if (bodyPart->sevrDebris) *refResult = bodyPart->sevrDebris->refID;
				}
				else if (traitID == 3)
				{
					if (bodyPart->explDebris) *refResult = bodyPart->explDebris->refID;
				}
				else if (traitID == 4)
				{
					if (bodyPart->sevrImpactDS) *refResult = bodyPart->sevrImpactDS->refID;
				}
				else if (bodyPart->explImpactDS) *refResult = bodyPart->explImpactDS->refID;
			}
		}
	}
	return true;
}

bool Cmd_SetBodyPartTraitForm_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL, *object = NULL;
	UInt32 partID = 0, traitID = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &partID, &traitID, &object) && ValidInput(partID, 14) && ValidInput(traitID, 5))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData)
		{
			BGSBodyPart *bodyPart = bpData->bodyParts[partID];
			if (bodyPart)
			{
				if (traitID < 2)
				{
					BGSExplosion *explosion = object ? DYNAMIC_CAST(object, TESForm, BGSExplosion) : NULL;
					if (!traitID)
					{
						if (bodyPart->sevrExplosion) *refResult = bodyPart->sevrExplosion->refID;
						bodyPart->sevrExplosion = explosion;
					}
					else
					{
						if (bodyPart->explExplosion) *refResult = bodyPart->explExplosion->refID;
						bodyPart->explExplosion = explosion;
					}
				}
				else if (traitID < 4)
				{
					BGSDebris *debris = object ? DYNAMIC_CAST(object, TESForm, BGSDebris) : NULL;
					if (traitID == 2)
					{
						if (bodyPart->sevrDebris) *refResult = bodyPart->sevrDebris->refID;
						bodyPart->sevrDebris = debris;
					}
					else
					{
						if (bodyPart->explDebris) *refResult = bodyPart->explDebris->refID;
						bodyPart->explDebris = debris;
					}
				}
				else
				{
					BGSImpactDataSet *ids = object ? DYNAMIC_CAST(object, TESForm, BGSImpactDataSet) : NULL;
					if (traitID == 4)
					{
						if (bodyPart->sevrImpactDS) *refResult = bodyPart->sevrImpactDS->refID;
						bodyPart->sevrImpactDS = ids;
					}
					else
					{
						if (bodyPart->explImpactDS) *refResult = bodyPart->explImpactDS->refID;
						bodyPart->explImpactDS = ids;
					}
				}
			}
		}
	}
	return true;
}

bool Cmd_GetBodyPartName_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	UInt32 partID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &partID) && ValidInput(partID, 14))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData)
		{
			BGSBodyPart *bodyPart = bpData->bodyParts[partID];
			if (bodyPart)
			{
				g_string->Assign(PASS_COMMAND_ARGS, bodyPart->partName.CStr());
				return true;
			}
		}
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");
	return true;
}

bool Cmd_SetBodyPartName_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	UInt32 partID = 0;
	char newName[64] = {0};
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &partID, &newName) && ValidInput(partID, 14))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData)
		{
			BGSBodyPart *bodyPart = bpData->bodyParts[partID];
			if (bodyPart) bodyPart->partName.Set(newName);
		}
	}
	return true;
}

bool Cmd_GetBodyPartReplacementModel_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	UInt32 partID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &form, &partID) && ValidInput(partID, 14))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData)
		{
			BGSBodyPart *bodyPart = bpData->bodyParts[partID];
			if (bodyPart)
			{
				g_string->Assign(PASS_COMMAND_ARGS, bodyPart->limbReplacement.GetModelPath());
				return true;
			}
		}
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");
	return true;
}

bool Cmd_SetBodyPartReplacementModel_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	UInt32 partID = 0;
	char nifPath[512] = {0};
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &partID, &nifPath) && ValidInput(partID, 14))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData)
		{
			BGSBodyPart *bodyPart = bpData->bodyParts[partID];
			if (bodyPart) bodyPart->limbReplacement.SetModelPath(nifPath);
		}
	}
	return true;
}

bool Cmd_GetBodyPartFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 partID = 0, flagID = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &partID, &flagID) && ValidInput(partID, 14) && ValidInput(flagID, 6))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData)
		{
			BGSBodyPart *bodyPart = bpData->bodyParts[partID];
			if (bodyPart) *result = (bodyPart->flags & (1 << flagID)) ? 1 : 0;
		}
	}
	return true;
}

bool Cmd_SetBodyPartFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	UInt32 partID = 0, flagID = 0, val = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &form, &partID, &flagID, &val) && ValidInput(partID, 14) && ValidInput(flagID, 6))
	{
		BGSBodyPartData *bpData = DYNAMIC_CAST(form, TESForm, BGSBodyPartData);
		if (bpData)
		{
			BGSBodyPart *bodyPart = bpData->bodyParts[partID];
			if (bodyPart)
			{
				*result = (bodyPart->flags & (1 << flagID)) ? 1 : 0;
				bodyPart->SetFlag(1 << flagID, val != 0);
			}
		}
	}
	return true;
}
