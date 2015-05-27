#pragma once

PluginHandle g_pluginHandle = kPluginHandle_Invalid;
NVSESerializationInterface *g_serialization;
NVSEScriptInterface *g_script;
NVSEStringVarInterface *g_string;
NVSEArrayVarInterface *g_array;

#define ExtractFormatStringArgs(...) g_script->ExtractFormatStringArgs(__VA_ARGS__)

class ExtraFollower : public BSExtraData
{
public:
	ExtraFollower();
	virtual ~ExtraFollower();

	struct FollowerNode
	{
		Character		*character;
		FollowerNode	*next;
	};

	FollowerNode	*followers;
};
STATIC_ASSERT(sizeof(ExtraFollower) == 0x10);

class ExtraEnableStateChildren : public BSExtraData
{
public:
	ExtraEnableStateChildren();
	virtual ~ExtraEnableStateChildren();

	tList<TESObjectREFR>	children;
};
STATIC_ASSERT(sizeof(ExtraEnableStateChildren) == 0x14);

class ExtraLinkedRef : public BSExtraData
{
public:
	ExtraLinkedRef();
	virtual ~ExtraLinkedRef();

    TESObjectREFR	*linkedRef;
};
STATIC_ASSERT(sizeof(ExtraLinkedRef) == 0x10);

static const UInt32 s_ExtraLinkedRefVtbl = 0x1015CC0;

class ExtraLinkedRefChildren : public BSExtraData
{
public:
	ExtraLinkedRefChildren();
	virtual ~ExtraLinkedRefChildren();

	tList<TESObjectREFR>	children;
};
STATIC_ASSERT(sizeof(ExtraLinkedRefChildren) == 0x14);

class ExtraPersistentCell : public BSExtraData
{
public:
	ExtraPersistentCell();
	virtual ~ExtraPersistentCell();

	TESObjectCELL	*persistentCell;
};
STATIC_ASSERT(sizeof(ExtraPersistentCell) == 0x10);

class ExtraPrimitive : public BSExtraData
{
public:
	ExtraPrimitive();
	virtual ~ExtraPrimitive();

	struct PrimitiveData
	{
		UInt32		unk00;
		UInt32		type;
		float		unk08[3];
		UInt32		unk14;
		float		bounds[3];
	};

	PrimitiveData	*data;
};
STATIC_ASSERT(sizeof(ExtraPrimitive) == 0x10);

class Sky
{
public:
	Sky();
	~Sky();

	virtual void Destructor(void);

//	void			**_vtbl;			// 00
	NiNode			*niNode04;			// 04
	NiNode			*niNode08;			// 08
	TESClimate		*firstClimate;		// 0C
	TESWeather		*firstWeather;		// 10
	TESWeather		*weather14;			// 14
	TESWeather		*weather18;			// 18
	TESWeather		*weatherOverride;	// 1C
	UInt32			atmosphere;			// 20	* class Atmosphere
	UInt32			stars;				// 24	* class Stars
	UInt32			sun;				// 28	* class Sun
	UInt32			clouds;				// 2C	* class Clouds
	UInt32			masserMoon;			// 30	* class Moon
	UInt32			secundaMoon;		// 34	* class Moon
	UInt32			precipitation;		// 38	* class Precipitation
	UInt32			unk3C[50];			// 3C

	static Sky *GetSingleton();
	void RefreshClimate(TESClimate *climate);
};
STATIC_ASSERT(sizeof(Sky) == 0x104);

typedef Sky *(*_Sky_GetSingleton)(void);
#if RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525
static const _Sky_GetSingleton Sky_GetSingleton = (_Sky_GetSingleton)0x46DD00;
static const UInt32 s_Sky_RefreshClimate = 0x63C8F0;
#elif RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525ng
static const _Sky_GetSingleton Sky_GetSingleton = (_Sky_GetSingleton)0x639B60;
static const UInt32 s_Sky_RefreshClimate = 0x63C6C0;
#endif

Sky *Sky::GetSingleton()
{
	return Sky_GetSingleton();
}

void Sky::RefreshClimate(TESClimate *climate)
{
	ThisStdCall(s_Sky_RefreshClimate, this, climate, 1);
}

std::string g_gamePath = GetFalloutDirectory();
UInt8 g_dataChangedFlags;

bool ValidInput(UInt32 inVal, UInt32 maxVal, UInt32 minVal = 0)
{
	return (inVal <= maxVal) && (inVal >= minVal);
}

UInt8 GetScriptModIdx(Script *scriptObj, TESObjectREFR *thisObj)
{
	if (scriptObj->refID) return scriptObj->refID >> 24;
	if (scriptObj->quest) return scriptObj->quest->refID >> 24;
	if (thisObj) return thisObj->TryGetREFRParent()->refID >> 24;
	return 0;
}

UInt32 GetCallerID(TESForm *form, TESObjectREFR *thisObj)
{
	if (form) return form->TryGetREFRParent()->refID;
	if (thisObj) return thisObj->refID;
	return 0;
}

void NameToLower(char *name)
{
	name[255] = 0;
	UInt8 pos = 0;
	while (name[pos])
	{
		if ((name[pos] >= 'A') && (name[pos] <= 'Z')) name[pos] += 32;
		pos++;
	}
}

class AuxVariableValue
{
	union
	{
		float	flt;
		UInt32	ref;
		char	*str;
	};
	UInt16		size;
	UInt16		buffer;
	UInt8		type;

public:
	AuxVariableValue() : size(0), buffer(0) {}

	void ReadValData(UInt8 pType)
	{
		type = pType;
		if (type == 4)
		{
			g_serialization->ReadRecordData(&size, 2);
			if (size)
			{
				buffer = size;
				str = new char[size + 1];
				str[size] = 0;
				g_serialization->ReadRecordData(str, size);
			}
		}
		else
		{
			g_serialization->ReadRecordData(&ref, 4);
			if (type == 2) g_serialization->ResolveRefID(ref, &ref);
		}
	}

	AuxVariableValue(UInt8 pType) : size(0), buffer(0) {ReadValData(pType);}

	void WriteValData()
	{
		g_serialization->WriteRecordData(&type, 1);
		if (type == 4)
		{
			g_serialization->WriteRecordData(&size, 2);
			if (size) g_serialization->WriteRecordData(str, size);
		}
		else g_serialization->WriteRecordData(&flt, 4);
	}

	void SetFlt(float value)
	{
		if (buffer)
		{
			delete[] str;
			buffer = size = 0;
		}
		type = 1;
		flt = value;
	}
	void SetRef(UInt32 value)
	{
		if (buffer)
		{
			delete[] str;
			buffer = size = 0;
		}
		type = 2;
		ref = value;
	}
	void SetStr(const char *value)
	{
		type = 4;
		if (size = strlen(value))
		{
			if (buffer < size)
			{
				if (buffer) delete[] str;
				buffer = size;
				str = new char[size + 1];
			}
			strcpy_s(str, size + 1, value);
		}
	}

	AuxVariableValue(NVSEArrayVarInterface::Element &elem) : size(0), buffer(0)
	{
		type = elem.GetType();
		if (type == 1) flt = elem.Number();
		else if (type == 2) ref = elem.Form() ? elem.Form()->refID : 0;
		else if (type == 3) SetStr(elem.String());
		else
		{
			type = 1;
			flt = 0;
		}
	}

	UInt8 GetType() {return type;}
	float GetFlt() {return (type == 1) ? flt : 0;}
	UInt32 GetRef() {return (type == 2) ? ref : 0;}
	const char *GetStr() {return size ? str : "";}

	NVSEArrayVarInterface::Element *GetAsElement()
	{
		if (type == 2) return new NVSEArrayVarInterface::Element(LookupFormByID(ref));
		if (type == 4) return new NVSEArrayVarInterface::Element(GetStr());
		return new NVSEArrayVarInterface::Element(flt);
	}

	void DelStr() {if (buffer) delete[] str;}
};

bool g_avIsPerm;
UInt8 g_avModIdx;

void GetBaseParams(Script *scriptObj, TESObjectREFR *thisObj, const char *var)
{
	g_avIsPerm = (var[0] != '*');
	g_avModIdx = (var[!g_avIsPerm] == '_') ? 255 : GetScriptModIdx(scriptObj, thisObj);
}

void GetBaseParams(Script *scriptObj, TESObjectREFR *thisObj, UInt32 type)
{
	g_avIsPerm = !(type % 2);
	g_avModIdx = (type > 1) ? 255 : GetScriptModIdx(scriptObj, thisObj);
}

void AVSetChanged(UInt8 type)
{
	if (g_avIsPerm) g_dataChangedFlags |= type;
}

NVSEArrayVarInterface::Element *GetArrayData(NVSEArrayVarInterface::Array *arr, SInt32 &size)
{
	size = g_array->GetArraySize(arr);
	if (size > 0)
	{
		NVSEArrayVarInterface::Element *data = new NVSEArrayVarInterface::Element[size];
		g_array->GetElements(arr, data, NULL);
		return data;
	}
	return NULL;
}

UInt32 RGBHexToDec(UInt32 rgb)
{
	return ((rgb & 0xFF) * 1000000) + (((rgb >> 8) & 0xFF) * 1000) + (rgb >> 16);
}

UInt32 RGBDecToHex(UInt32 rgb)
{
	return (rgb / 1000000) + (((rgb / 1000) % 1000) << 8) + ((rgb % 1000) << 16);
}

bool FileExists(std::string &path)
{
	DWORD attr = GetFileAttributes(path.data());
	return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

ParamInfo kParams_JIP_OneInt_OneOptionalActorBase[2] =
{
	{"Integer", kParamType_Integer, 0},
	{"Actor Base", kParamType_ActorBase, 1},
};

ParamInfo kParams_JIP_OneObjectIDorList_OneInt_OneOptionalFloat[3] =
{
	{"ObjectID or List", kParamType_InvObjOrFormList, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 1},
};

ParamInfo kParams_JIP_OneString_OneFormatString[] =
{
	{"String", kParamType_String, 0},
	FORMAT_STRING_PARAMS,
};

ParamInfo kParams_JIP_OneForm_OneInt_OneFloat[3] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
};

ParamInfo kParams_JIP_OneForm_TwoInts[3] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_TwoForms[2] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Form", kParamType_AnyForm, 0},
};

ParamInfo kParams_JIP_OneForm_OneInt_OneOptionalForm[3] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_OneString_OneOptionalQuest[2] =
{
	{"String", kParamType_String, 0},
	{"Quest", kParamType_Quest, 1},
};

ParamInfo kParams_JIP_OneQuest_OneFloat[2] =
{
	{"Quest", kParamType_Quest, 0},
	{"Float", kParamType_Float, 0},
};

ParamInfo kParams_JIP_OneAIPackage_OneInt[2] =
{
	{"Package", kParamType_AIPackage, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneAIPackage_TwoInts[3] =
{
	{"Package", kParamType_AIPackage, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneGlobal[1] =
{
	{"Global", kParamType_Global, 0},
};

ParamInfo kParams_JIP_OneGlobal_OneFloat[2] =
{
	{"Global", kParamType_Global, 0},
	{"Float", kParamType_Float, 0},
};

ParamInfo kParams_JIP_OneFaction[1] =
{
	{"Faction", kParamType_Faction, 0},
};

ParamInfo kParams_JIP_OneFaction_OneOptionalReputation[2] =
{
	{"Faction", kParamType_Faction, 0},
	{"Reputation", kParamType_Reputation, 1},
};

ParamInfo kParams_JIP_OptionalForm[1] =
{
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_OneString_OneOptionalInt[2] =
{
	{ "String", kParamType_String, 0 },
	{ "Integer", kParamType_Integer, 1 },
};

ParamInfo kParams_JIP_OneAxis_OneOptionalForm[2] =
{
	{"Axis", kParamType_Axis, 0},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_TwoFloats_ThreeOptionalFloats[5] =
{
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1},
};

ParamInfo kParams_JIP_OneCombatStyle_OneInt[2] =
{
	{"Combat Style", kParamType_CombatStyle, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneCombatStyle_OneInt_OneFloat[3] =
{
	{"Combat Style", kParamType_CombatStyle, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
};

ParamInfo kParams_JIP_OneCombatStyle_TwoInts[3] =
{
	{"Combat Style", kParamType_CombatStyle, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneString_TwoOptionalInts[3] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1},
};

ParamInfo kParams_JIP_OneString_TwoInts[3] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneString_OneInt_OneFormatString[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	FORMAT_STRING_PARAMS,
};

ParamInfo kParams_JIP_OneClass_OneInt[2] =
{
	{"Class", kParamType_Class, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneClass_TwoInts[3] =
{
	{"Class", kParamType_Class, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneSound_OneInt[2] =
{
	{"Sound", kParamType_Sound, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneSound_OneInt_OneFloat[3] =
{
	{"Sound", kParamType_Sound, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
};

ParamInfo kParams_JIP_OneSound_TwoInts[3] =
{
	{"Sound", kParamType_Sound, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneForm_OneInt_OneOptionalSound[3] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Sound", kParamType_Sound, 1},
};

ParamInfo kParams_JIP_OneForm_OneInt_OneOptionalObjectID[3] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Object ID", kParamType_ObjectID, 1},
};

ParamInfo kParams_JIP_OneWeatherID_OneInt[2] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneWeatherID_OneInt_OneOptionalImageSpaceMod[3] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
	{"Image Space Mod", kParamType_ImageSpaceModifier, 1},
};

ParamInfo kParams_JIP_OneWeatherID_OneInt_OneString[3] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
	{"String", kParamType_String, 0},
};

ParamInfo kParams_JIP_OneWeatherID[1] =
{
	{"Weather ID", kParamType_WeatherID, 0},
};

ParamInfo kParams_JIP_OneWeatherID_OneString[2] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"String", kParamType_String, 0},
};

ParamInfo kParams_JIP_OneWeatherID_OneInt_OneFloat[3] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
};

ParamInfo kParams_JIP_OneWeatherID_TwoInts[3] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneWeatherID_ThreeInts[4] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneForm_OneInt_OneString[3] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"String", kParamType_String, 0},
};

ParamInfo kParams_JIP_OneForm_OneString[2] =
{
	{"Form", kParamType_AnyForm, 0},
	{"String", kParamType_String, 0},
};

ParamInfo kParams_JIP_OneForm_OneWeatherID_OneInt_OneOptionalGlobal[4] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
	{"Global", kParamType_Global, 1},
};

ParamInfo kParams_JIP_OneForm_OneWeatherID[2] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Weather ID", kParamType_WeatherID, 0},
};

ParamInfo kParams_JIP_OneAxis_OneFloat[2] =
{
	{"Axis", kParamType_Axis, 0},
	{"Float", kParamType_Float, 0},
};

ParamInfo kParams_JIP_OneForm_ThreeInts[4] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneItemOrList_OneObjectRef_OneOptionalInt[3] =
{
	{"Item/List", kParamType_InvObjOrFormList, 0},
	{"Object Ref", kParamType_ObjectRef, 0},
	{"Integer", kParamType_Integer, 1},
};

ParamInfo kParams_JIP_OneInt_OneFloat_OneOptionalActorBase[3] =
{
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
	{"Actor Base", kParamType_ActorBase, 1},
};

ParamInfo kParams_JIP_OneForm_OneInt_OneMagicEffect[3] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Magic Effect", kParamType_MagicEffect, 0},
};

ParamInfo kParams_JIP_OneOptionalQuest[1] =
{
	{"Quest", kParamType_Quest, 1},
};

ParamInfo kParams_JIP_OneString_OneOptionalForm[2] =
{
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_OneString_OneOptionalInt_OneOptionalForm[3] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 1},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_OneString_OneFloat_OneOptionalInt_OneOptionalForm[4] =
{
	{"String", kParamType_String, 0},
	{"Float", kParamType_Float, 0},
	{"Integer", kParamType_Integer, 1},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_OneString_OneForm_OneOptionalInt_OneOptionalForm[4] =
{
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 1},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_TwoStrings_OneOptionalInt_OneOptionalForm[4] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 1},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_OneInt_OneOptionalForm[2] =
{
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_OneString_OneInt_OneOptionalForm[3] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_OneString_OneFloat_OneOptionalForm[3] =
{
	{"String", kParamType_String, 0},
	{"Float", kParamType_Float, 0},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_OneString_OneForm_OneOptionalForm[3] =
{
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_TwoStrings_OneOptionalForm[3] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_FourInts[4] =
{
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneForm_TwoInts_OneFloat[4] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
};

ParamInfo kParams_JIP_OneForm_TwoInts_OneOptionalForm[4] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_OneForm_OneOptionalForm[2] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_OneSound[1] =
{
	{"Sound", kParamType_Sound, 0},
};

ParamInfo kParams_JIP_OneSound_OneString[2] =
{
	{"Sound", kParamType_Sound, 0},
	{"String", kParamType_String, 0},
};

ParamInfo kParams_JIP_OneObject_OneInt[2] =
{
	{"Object", kParamType_TESObject, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneObject_TwoInts[3] =
{
	{"Object", kParamType_TESObject, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneObject_ThreeInts[4] =
{
	{"Object", kParamType_TESObject, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneObject_OneInt_OneOptionalForm[3] =
{
	{"Object", kParamType_TESObject, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 1},
};

ParamInfo kParams_JIP_OneObject_OneInt_OneString[3] =
{
	{"Object", kParamType_TESObject, 0},
	{"Integer", kParamType_Integer, 0},
	{"String", kParamType_String, 0},
};

ParamInfo kParams_JIP_OneWorldspace[1] =
{
	{"Worldspace", kParamType_WorldSpace, 0},
};

ParamInfo kParams_JIP_OneWorldspace_OneForm[2] =
{
	{"Worldspace", kParamType_WorldSpace, 0},
	{"Form", kParamType_AnyForm, 0},
};

ParamInfo kParams_JIP_OneForm_OneOptionalActorBase[2] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Actor Base", kParamType_ActorBase, 1},
};

ParamInfo kParams_JIP_OneInt_OneFloat[2] =
{
	{ "Integer", kParamType_Integer, 0 },
	{ "Float", kParamType_Float, 0 },
};

ParamInfo kParams_JIP_OneInt_OneFormatString[] =
{
	{ "TraitID", kParamType_Integer, 0 },
	FORMAT_STRING_PARAMS,
};

ParamInfo kParams_JIP_TwoInts_OneOptionalFloat[3] =
{
	{ "Integer", kParamType_Integer, 0 },
	{ "Integer", kParamType_Integer, 0 },
	{ "Float", kParamType_Float, 1 },
};

ParamInfo kParams_JIP_OneObjectRef_OneOptionalInt[2] =
{
	{ "Object Ref", kParamType_ObjectRef, 0 },
	{ "Integer", kParamType_Integer, 1 },
};

ParamInfo kParams_JIP_OneList_TwoOptionalInts[3] =
{
	{ "List", kParamType_FormList, 0 },
	{ "Integer", kParamType_Integer, 1 },
	{ "Integer", kParamType_Integer, 1 },
};

ParamInfo kParams_JIP_OneFaction_OneOptionalInt[2] =
{
	{ "Faction", kParamType_Faction, 0 },
	{ "Integer", kParamType_Integer, 1 },
};
