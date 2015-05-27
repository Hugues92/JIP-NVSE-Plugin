#pragma once

DEFINE_COMMAND_PLUGIN(GetCombatStyleTraitNumeric, "None", 0, 2, kParams_JIP_OneCombatStyle_OneInt);
DEFINE_COMMAND_PLUGIN(SetCombatStyleTraitNumeric, "None", 0, 3, kParams_JIP_OneCombatStyle_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetCombatStyleFlag, "None", 0, 2, kParams_JIP_OneCombatStyle_OneInt);
DEFINE_COMMAND_PLUGIN(SetCombatStyleFlag, "None", 0, 3, kParams_JIP_OneCombatStyle_TwoInts);

bool Cmd_GetCombatStyleTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESCombatStyle *cStyle = NULL;
	UInt32 traitID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &cStyle, &traitID) && ValidInput(traitID, 63))
	{
		if (!traitID) *result = cStyle->coverSearchRadius;
		else if (traitID == 1) *result = cStyle->takeCoverChance;
		else if (traitID == 2) *result = cStyle->waitTimeMin;
		else if (traitID == 3) *result = cStyle->waitTimeMax;
		else if (traitID == 4) *result = cStyle->waitToFireTimerMin;
		else if (traitID == 5) *result = cStyle->waitToFireTimerMax;
		else if (traitID == 6) *result = cStyle->fireTimerMin;
		else if (traitID == 7) *result = cStyle->fireTimerMax;
		else if (traitID == 8) *result = cStyle->rangedWeapRangeMultMin;
		else if (traitID == 9) *result = cStyle->rangedWeapRangeMultMax;
		else if (traitID == 10) *result = cStyle->weaponRestrictions;
		else if (traitID == 11) *result = cStyle->maxTargetingFOV;
		else if (traitID == 12) *result = cStyle->combatRadius;
		else if (traitID == 13) *result = cStyle->semiAutoFiringDelayMultMin;
		else if (traitID == 14) *result = cStyle->semiAutoFiringDelayMultMax;
		else if (traitID == 15) *result = cStyle->dodgeChance;
		else if (traitID == 16) *result = cStyle->LRChance;
		else if (traitID == 17) *result = cStyle->dodgeLRTimerMin;
		else if (traitID == 18) *result = cStyle->dodgeLRTimerMax;
		else if (traitID == 19) *result = cStyle->dodgeFWTimerMin;
		else if (traitID == 20) *result = cStyle->dodgeFWTimerMax;
		else if (traitID == 21) *result = cStyle->dodgeBKTimerMin;
		else if (traitID == 22) *result = cStyle->dodgeBKTimerMax;
		else if (traitID == 23) *result = cStyle->idleTimerMin;
		else if (traitID == 24) *result = cStyle->idleTimerMax;
		else if (traitID == 25) *result = cStyle->blockChance;
		else if (traitID == 26) *result = cStyle->attackChance;
		else if (traitID == 27) *result = cStyle->staggerBonusToAttack;
		else if (traitID == 28) *result = cStyle->KOBonusToAttack;
		else if (traitID == 29) *result = cStyle->H2HBonusToAttack;
		else if (traitID == 30) *result = cStyle->powerAttackChance;
		else if (traitID == 31) *result = cStyle->staggerBonusToPower;
		else if (traitID == 32) *result = cStyle->KOBonusToPower;
		else if (traitID == 33) *result = cStyle->powerAttackN;
		else if (traitID == 34) *result = cStyle->powerAttackF;
		else if (traitID == 35) *result = cStyle->powerAttackB;
		else if (traitID == 36) *result = cStyle->powerAttackL;
		else if (traitID == 37) *result = cStyle->powerAttackR;
		else if (traitID == 38) *result = cStyle->holdTimerMin;
		else if (traitID == 39) *result = cStyle->holdTimerMax;
		else if (traitID == 40) *result = cStyle->acrobaticDodgeChance;
		else if (traitID == 41) *result = cStyle->rushAttackChance;
		else if (traitID == 42) *result = cStyle->rushAttackDistMult;
		else if (traitID == 43) *result = cStyle->dodgeFatigueModBase;
		else if (traitID == 44) *result = cStyle->dodgeFatigueModMult;
		else if (traitID == 45) *result = cStyle->encumSpeedModBase;
		else if (traitID == 46) *result = cStyle->encumSpeedModMult;
		else if (traitID == 47) *result = cStyle->dodgeUnderAttackMult;
		else if (traitID == 48) *result = cStyle->dodgeNotUnderAttackMult;
		else if (traitID == 49) *result = cStyle->dodgeBackUnderAttackMult;
		else if (traitID == 50) *result = cStyle->dodgeBackNotUnderAttackMult;
		else if (traitID == 51) *result = cStyle->dodgeFWAttackingMult;
		else if (traitID == 52) *result = cStyle->dodgeFWNotAttackingMult;
		else if (traitID == 53) *result = cStyle->blockSkillModBase;
		else if (traitID == 54) *result = cStyle->blockSkillModMult;
		else if (traitID == 55) *result = cStyle->blockUnderAttackMult;
		else if (traitID == 56) *result = cStyle->blockNotUnderAttackMult;
		else if (traitID == 57) *result = cStyle->attackSkillModBase;
		else if (traitID == 58) *result = cStyle->attackSkillModMult;
		else if (traitID == 59) *result = cStyle->attackUnderAttackMult;
		else if (traitID == 60) *result = cStyle->attackNotUnderAttackMult;
		else if (traitID == 61) *result = cStyle->attackDuringBlockMult;
		else if (traitID == 62) *result = cStyle->powerAttackFatigueModBase;
		else *result = cStyle->powerAttackFatigueModMult;
	}
	return true;
}

bool Cmd_SetCombatStyleTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESCombatStyle *cStyle = NULL;
	UInt32 traitID = 0;
	float val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &cStyle, &traitID, &val) && ValidInput(traitID, 63))
	{
		if (!traitID)
		{
			*result = cStyle->coverSearchRadius;
			cStyle->coverSearchRadius = val;
		}
		else if (traitID == 1)
		{
			*result = cStyle->takeCoverChance;
			cStyle->takeCoverChance = val;
		}
		else if (traitID == 2)
		{
			*result = cStyle->waitTimeMin;
			cStyle->waitTimeMin = val;
		}
		else if (traitID == 3)
		{
			*result = cStyle->waitTimeMax;
			cStyle->waitTimeMax = val;
		}
		else if (traitID == 4)
		{
			*result = cStyle->waitToFireTimerMin;
			cStyle->waitToFireTimerMin = val;
		}
		else if (traitID == 5)
		{
			*result = cStyle->waitToFireTimerMax;
			cStyle->waitToFireTimerMax = val;
		}
		else if (traitID == 6)
		{
			*result = cStyle->fireTimerMin;
			cStyle->fireTimerMin = val;
		}
		else if (traitID == 7)
		{
			*result = cStyle->fireTimerMax;
			cStyle->fireTimerMax = val;
		}
		else if (traitID == 8)
		{
			*result = cStyle->rangedWeapRangeMultMin;
			cStyle->rangedWeapRangeMultMin = val;
		}
		else if (traitID == 9)
		{
			*result = cStyle->rangedWeapRangeMultMax;
			cStyle->rangedWeapRangeMultMax = val;
		}
		else if (traitID == 10)
		{
			if ((val >= 0) && (val <= 2))
			{
				*result = cStyle->weaponRestrictions;
				cStyle->weaponRestrictions = val;
			}
		}
		else if (traitID == 11)
		{
			*result = cStyle->maxTargetingFOV;
			cStyle->maxTargetingFOV = val;
		}
		else if (traitID == 12)
		{
			*result = cStyle->combatRadius;
			cStyle->combatRadius = val;
		}
		else if (traitID == 13)
		{
			*result = cStyle->semiAutoFiringDelayMultMin;
			cStyle->semiAutoFiringDelayMultMin = val;
		}
		else if (traitID == 14)
		{
			*result = cStyle->semiAutoFiringDelayMultMax;
			cStyle->semiAutoFiringDelayMultMax = val;
		}
		else if (traitID == 15)
		{
			*result = cStyle->dodgeChance;
			cStyle->dodgeChance = val;
		}
		else if (traitID == 16)
		{
			*result = cStyle->LRChance;
			cStyle->LRChance = val;
		}
		else if (traitID == 17)
		{
			*result = cStyle->dodgeLRTimerMin;
			cStyle->dodgeLRTimerMin = val;
		}
		else if (traitID == 18)
		{
			*result = cStyle->dodgeLRTimerMax;
			cStyle->dodgeLRTimerMax = val;
		}
		else if (traitID == 19)
		{
			*result = cStyle->dodgeFWTimerMin;
			cStyle->dodgeFWTimerMin = val;
		}
		else if (traitID == 20)
		{
			*result = cStyle->dodgeFWTimerMax;
			cStyle->dodgeFWTimerMax = val;
		}
		else if (traitID == 21)
		{
			*result = cStyle->dodgeBKTimerMin;
			cStyle->dodgeBKTimerMin = val;
		}
		else if (traitID == 22)
		{
			*result = cStyle->dodgeBKTimerMax;
			cStyle->dodgeBKTimerMax = val;
		}
		else if (traitID == 23)
		{
			*result = cStyle->idleTimerMin;
			cStyle->idleTimerMin = val;
		}
		else if (traitID == 24)
		{
			*result = cStyle->idleTimerMax;
			cStyle->idleTimerMax = val;
		}
		else if (traitID == 25)
		{
			*result = cStyle->blockChance;
			cStyle->blockChance = val;
		}
		else if (traitID == 26)
		{
			*result = cStyle->attackChance;
			cStyle->attackChance = val;
		}
		else if (traitID == 27)
		{
			*result = cStyle->staggerBonusToAttack;
			cStyle->staggerBonusToAttack = val;
		}
		else if (traitID == 28)
		{
			*result = cStyle->KOBonusToAttack;
			cStyle->KOBonusToAttack = val;
		}
		else if (traitID == 29)
		{
			*result = cStyle->H2HBonusToAttack;
			cStyle->H2HBonusToAttack = val;
		}
		else if (traitID == 30)
		{
			*result = cStyle->powerAttackChance;
			cStyle->powerAttackChance = val;
		}
		else if (traitID == 31)
		{
			*result = cStyle->staggerBonusToPower;
			cStyle->staggerBonusToPower = val;
		}
		else if (traitID == 32)
		{
			*result = cStyle->KOBonusToPower;
			cStyle->KOBonusToPower = val;
		}
		else if (traitID == 33)
		{
			*result = cStyle->powerAttackN;
			cStyle->powerAttackN = val;
		}
		else if (traitID == 34)
		{
			*result = cStyle->powerAttackF;
			cStyle->powerAttackF = val;
		}
		else if (traitID == 35)
		{
			*result = cStyle->powerAttackB;
			cStyle->powerAttackB = val;
		}
		else if (traitID == 36)
		{
			*result = cStyle->powerAttackL;
			cStyle->powerAttackL = val;
		}
		else if (traitID == 37)
		{
			*result = cStyle->powerAttackR;
			cStyle->powerAttackR = val;
		}
		else if (traitID == 38)
		{
			*result = cStyle->holdTimerMin;
			cStyle->holdTimerMin = val;
		}
		else if (traitID == 39)
		{
			*result = cStyle->holdTimerMax;
			cStyle->holdTimerMax = val;
		}
		else if (traitID == 40)
		{
			*result = cStyle->acrobaticDodgeChance;
			cStyle->acrobaticDodgeChance = val;
		}
		else if (traitID == 41)
		{
			*result = cStyle->rushAttackChance;
			cStyle->rushAttackChance = val;
		}
		else if (traitID == 42)
		{
			*result = cStyle->rushAttackDistMult;
			cStyle->rushAttackDistMult = val;
		}
		else if (traitID == 43)
		{
			*result = cStyle->dodgeFatigueModBase;
			cStyle->dodgeFatigueModBase = val;
		}
		else if (traitID == 44)
		{
			*result = cStyle->dodgeFatigueModMult;
			cStyle->dodgeFatigueModMult = val;
		}
		else if (traitID == 45)
		{
			*result = cStyle->encumSpeedModBase;
			cStyle->encumSpeedModBase = val;
		}
		else if (traitID == 46)
		{
			*result = cStyle->encumSpeedModMult;
			cStyle->encumSpeedModMult = val;
		}
		else if (traitID == 47)
		{
			*result = cStyle->dodgeUnderAttackMult;
			cStyle->dodgeUnderAttackMult = val;
		}
		else if (traitID == 48)
		{
			*result = cStyle->dodgeNotUnderAttackMult;
			cStyle->dodgeNotUnderAttackMult = val;
		}
		else if (traitID == 49)
		{
			*result = cStyle->dodgeBackUnderAttackMult;
			cStyle->dodgeBackUnderAttackMult = val;
		}
		else if (traitID == 50)
		{
			*result = cStyle->dodgeBackNotUnderAttackMult;
			cStyle->dodgeBackNotUnderAttackMult = val;
		}
		else if (traitID == 51)
		{
			*result = cStyle->dodgeFWAttackingMult;
			cStyle->dodgeFWAttackingMult = val;
		}
		else if (traitID == 52)
		{
			*result = cStyle->dodgeFWNotAttackingMult;
			cStyle->dodgeFWNotAttackingMult = val;
		}
		else if (traitID == 53)
		{
			*result = cStyle->blockSkillModBase;
			cStyle->blockSkillModBase = val;
		}
		else if (traitID == 54)
		{
			*result = cStyle->blockSkillModMult;
			cStyle->blockSkillModMult = val;
		}
		else if (traitID == 55)
		{
			*result = cStyle->blockUnderAttackMult;
			cStyle->blockUnderAttackMult = val;
		}
		else if (traitID == 56)
		{
			*result = cStyle->blockNotUnderAttackMult;
			cStyle->blockNotUnderAttackMult = val;
		}
		else if (traitID == 57)
		{
			*result = cStyle->attackSkillModBase;
			cStyle->attackSkillModBase = val;
		}
		else if (traitID == 58)
		{
			*result = cStyle->attackSkillModMult;
			cStyle->attackSkillModMult = val;
		}
		else if (traitID == 59)
		{
			*result = cStyle->attackUnderAttackMult;
			cStyle->attackUnderAttackMult = val;
		}
		else if (traitID == 60)
		{
			*result = cStyle->attackNotUnderAttackMult;
			cStyle->attackNotUnderAttackMult = val;
		}
		else if (traitID == 61)
		{
			*result = cStyle->attackDuringBlockMult;
			cStyle->attackDuringBlockMult = val;
		}
		else if (traitID == 62)
		{
			*result = cStyle->powerAttackFatigueModBase;
			cStyle->powerAttackFatigueModBase = val;
		}
		else
		{
			*result = cStyle->powerAttackFatigueModMult;
			cStyle->powerAttackFatigueModMult = val;
		}
	}
	return true;
}

bool Cmd_GetCombatStyleFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESCombatStyle *cStyle = NULL;
	UInt32 flag = 0;

	if (ExtractArgs(EXTRACT_ARGS, &cStyle, &flag) && ValidInput(flag, 8))
		*result = (cStyle->csFlags & (1 << flag)) ? 1 : 0;

	return true;
}

bool Cmd_SetCombatStyleFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESCombatStyle *cStyle = NULL;
	UInt32 flag = 0, val = 0;

	if (ExtractArgs(EXTRACT_ARGS, &cStyle, &flag, &val) && ValidInput(flag, 8))
	{
		*result = (cStyle->csFlags & (1 << flag)) ? 1 : 0;
		cStyle->SetFlag(1 << flag, val != 0);
	}
	return true;
}
