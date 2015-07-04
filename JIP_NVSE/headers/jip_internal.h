#pragma once

PluginHandle g_pluginHandle = kPluginHandle_Invalid;
NVSESerializationInterface *g_serialization;
NVSEScriptInterface *g_script;
NVSEStringVarInterface *g_string;
NVSEArrayVarInterface *g_array;

#define ExtractArgsEx(...) g_script->ExtractArgsEx(__VA_ARGS__)
#define ExtractFormatStringArgs(...) g_script->ExtractFormatStringArgs(__VA_ARGS__)

// 10
class ExtraFollower : public BSExtraData
{
public:
	ExtraFollower();
	virtual ~ExtraFollower();

	tList<Character>	*followers;	// 0C
};
STATIC_ASSERT(sizeof(ExtraFollower) == 0x10);

// 14
class ExtraEnableStateChildren : public BSExtraData
{
public:
	ExtraEnableStateChildren();
	virtual ~ExtraEnableStateChildren();

	tList<TESObjectREFR>	children;	// 0C
};
STATIC_ASSERT(sizeof(ExtraEnableStateChildren) == 0x14);

// 10
class ExtraLinkedRef : public BSExtraData
{
public:
	ExtraLinkedRef();
	virtual ~ExtraLinkedRef();

    TESObjectREFR	*linkedRef;	// 0C
};
STATIC_ASSERT(sizeof(ExtraLinkedRef) == 0x10);

// 14
class ExtraLinkedRefChildren : public BSExtraData
{
public:
	ExtraLinkedRefChildren();
	virtual ~ExtraLinkedRefChildren();

	tList<TESObjectREFR>	children;	// 0C
};
STATIC_ASSERT(sizeof(ExtraLinkedRefChildren) == 0x14);

// 10
class ExtraPersistentCell : public BSExtraData
{
public:
	ExtraPersistentCell();
	virtual ~ExtraPersistentCell();

	TESObjectCELL	*persistentCell;	// 0C
};
STATIC_ASSERT(sizeof(ExtraPersistentCell) == 0x10);

// 10
class ExtraMerchantContainer : public BSExtraData
{
public:
	ExtraMerchantContainer();
	virtual ~ExtraMerchantContainer();

	TESObjectREFR	*containerRef;	// 0C
};
STATIC_ASSERT(sizeof(ExtraMerchantContainer) == 0x10);

// 34/4C
class BGSPrimitive
{
public:
	BGSPrimitive();
	~BGSPrimitive();

	UInt32		vtbl;		// 00
	UInt32		type;		// 04
	float		unk08[4];	// 08
	float		bounds[3];	// 18
	UInt32		unk24[4];	// 24
};

// 10
class ExtraPrimitive : public BSExtraData
{
public:
	ExtraPrimitive();
	virtual ~ExtraPrimitive();

	BGSPrimitive	*primitive;		// 0C
};
STATIC_ASSERT(sizeof(ExtraPrimitive) == 0x10);

// 08
class NiObject : public NiRefObject
{
public:
	NiObject();
	~NiObject();
};
STATIC_ASSERT(sizeof(NiObject) == 0x8);

// 18
class BSTempEffect : public NiObject
{
public:
	BSTempEffect();
	~BSTempEffect();

	float			duration;	// 08
	TESObjectCELL	*cell;		// 0C
	UInt32			unk10[2];	// 10
};
STATIC_ASSERT(sizeof(BSTempEffect) == 0x18);

// 28
class MagicHitEffect : public BSTempEffect
{
public:
	MagicHitEffect();
	~MagicHitEffect();

	UInt32			unk18;		// 18
	TESObjectREFR	*target;	// 1C
	UInt32			unk20[2];	// 20
};
STATIC_ASSERT(sizeof(MagicHitEffect) == 0x28);

// 4C
class MagicShaderHitEffect : public MagicHitEffect
{
public:
	MagicShaderHitEffect();
	~MagicShaderHitEffect();

	UInt32				unk28[2];			// 28
	TESEffectShader		*effectShader;		// 30
	UInt32				unk34[6];			// 34
};
STATIC_ASSERT(sizeof(MagicShaderHitEffect) == 0x4C);

class ActorProcessManager
{
public:
	ActorProcessManager();
	~ActorProcessManager();

	UInt32					unk00[24];		// 00
	tList<BSTempEffect>		tempEffects;	// 60
};

// 150
class Projectile : public MobileObject
{
public:
	Projectile();
	~Projectile();

	UInt32			unk088[2];		// 088
	float			unk090[14];		// 090
	UInt32			unk0C8;			// 0C8
	float			speedMult1;		// 0CC
	float			speedMult2;		// 0D0
	float			unk0D4;			// 0D4
	float			elapsedTime;	// 0D8
	float			hitDamage;		// 0DC
	float			unk0E0[6];		// 0E0
	TESObjectWEAP	*sourceWeap;	// 0F8
	TESObjectREFR	*sourceRef;		// 0FC
	float			unk100[4];		// 100
	float			distTravelled;	// 110
	UInt32			unk114[15];		// 114
};
STATIC_ASSERT(sizeof(Projectile) == 0x150);

// 154
class BeamProjectile : public Projectile
{
public:
	BeamProjectile();
	~BeamProjectile();

	UInt32		unk150;		// 150
};

// 158
class ContinuousBeamProjectile : public Projectile
{
public:
	ContinuousBeamProjectile();
	~ContinuousBeamProjectile();

	UInt32		unk150[2];	// 150
};

// 158
class FlameProjectile : public Projectile
{
public:
	FlameProjectile();
	~FlameProjectile();

	UInt32		unk150[2];	// 150
};

// 154
class GrenadeProjectile : public Projectile
{
public:
	GrenadeProjectile();
	~GrenadeProjectile();

	UInt32		unk150;		// 150
};

// 160
class MissileProjectile : public Projectile
{
public:
	MissileProjectile();
	~MissileProjectile();

	UInt32		unk150[4];	// 150
};

struct MenuItemEntry
{
	Tile								*unkTile;
	ExtraContainerChanges::EntryData	*entry;
};
typedef BSSimpleList<MenuItemEntry> MenuItemEntryList;

// 124
class InventoryMenu : public Menu	// 1002
{
public:
	InventoryMenu();
	~InventoryMenu();

	TileRect			*unk028;		// 028
	TileRect			*unk02C;		// 02C
	TileRect			*unk030;		// 030
	TileRect			*unk034;		// 034
	TileImage			*unk038;		// 038
	TileRect			*unk03C;		// 03C
	TileImage			*unk040;		// 040
	TileImage			*unk044;		// 044
	TileImage			*unk048;		// 048
	TileImage			*unk04C;		// 04C
	TileImage			*unk050;		// 050
	TileImage			*unk054;		// 054
	TileRect			*unk058;		// 058
	TileRect			*unk05C;		// 05C
	TileRect			*unk060;		// 060
	TileRect			*unk064;		// 064
	TileRect			*unk068;		// 068
	TileRect			*unk06C;		// 06C
	TileRect			*unk070;		// 070
	TileImage			*unk074;		// 074
	TileImage			*unk078;		// 078
	TileRect			*unk07C;		// 07C
	TileText			*unk080;		// 080
	UInt32				filter;			// 084
	UInt32				unk088[12];		// 088
	MenuItemEntryList	items;			// 0B8
	TileImage			*unk0C4;		// 0C4
	TileRect			*unk0C8;		// 0C8
	TileImage			*unk0CC;		// 0CC
	UInt32				unk0D0[6];		// 0D0
	TileRect			*unk0E8;		// 0E8
	TileRect			*unk0EC;		// 0EC
	TileRect			*unk0F0;		// 0F0
	TileRect			*unk0F4;		// 0F4
	TileRect			*unk0F8;		// 0F8
	TileRect			*unk0FC;		// 0FC
	TileRect			*unk100;		// 100
	TileRect			*unk104;		// 104
	TileRect			*unk108;		// 108
	UInt32				unk10C[6];		// 10C
};
STATIC_ASSERT(sizeof(InventoryMenu) == 0x124);

// 10C
class ContainerMenu : public Menu	// 1008
{
public:
	ContainerMenu();
	~ContainerMenu();

	TileImage			*unk028;		// 028
	TileText			*unk02C;		// 02C
	TileImage			*unk030;		// 030
	TileText			*unk034;		// 034
	TileImage			*unk038;		// 038
	TileImage			*unk03C;		// 03C
	TileText			*unk040;		// 040
	TileImage			*unk044;		// 044
	TileImage			*unk048;		// 048
	TileImage			*unk04C;		// 04C
	TileImage			*unk050;		// 050
	TileImage			*unk054;		// 054
	TileImage			*unk058;		// 058
	TileRect			*unk05C;		// 05C
	TileRect			*unk060;		// 060
	TileRect			*unk064;		// 064
	TileRect			*unk068;		// 068
	TileRect			*unk06C;		// 06C
	TileRect			*unk070;		// 070
	TESObjectREFR		*containerRef;	// 074
	UInt32				unk078[5];		// 078
	UInt32				leftFilter;		// 08C
	UInt32				rightFilter;	// 090
	UInt32				unk094;			// 094
	MenuItemEntryList	leftItems;		// 098
	TileImage			*unk0A4;		// 0A4
	TileImage			*leftActive;	// 0A8
	TileImage			*unk0AC;		// 0AC
	UInt32				unk0B0;			// 0B0
	UInt16				leftCount;		// 0B4
	UInt16				unk0B6;			// 0B6
	UInt32				unk0B8[4];		// 0B8
	MenuItemEntryList	rightItems;		// 0C8
	TileImage			*unk0D4;		// 0D4
	TileImage			*rightActive;	// 0D8
	TileImage			*unk0DC;		// 0DC
	UInt32				unk0E0;			// 0E0
	UInt16				rightCount;		// 0E4
	UInt16				unk0E6;			// 0E6
	UInt32				unk0E8[4];		// 0E8
	MenuItemEntryList	*currentItems;	// 0F8
	UInt32				unk0FC[4];		// 0FC
};
STATIC_ASSERT(sizeof(ContainerMenu) == 0x10C);

// 13C
class DialogMenu : public Menu	// 1009
{
public:
	DialogMenu();
	~DialogMenu();

	UInt32				unk028[22];		// 028
	TESObjectREFR		*partnerRef;	// 080
	UInt32				unk084[46];		// 084
};
STATIC_ASSERT(sizeof(DialogMenu) == 0x13C);

// 0E4
class LockPickMenu : public Menu	// 1014
{
public:
	LockPickMenu();
	~LockPickMenu();

	UInt32				unk028[17];		// 028
	TESObjectREFR		*targetRef;		// 06C
	UInt32				unk070[29];		// 070
};
STATIC_ASSERT(sizeof(LockPickMenu) == 0xE4);

typedef tList<ExtraContainerChanges::EntryData> BarterItemList;

// 120
class BarterMenu : public Menu	// 1053
{
public:
	BarterMenu();
	~BarterMenu();

	TileImage			*unk028;		// 028
	TileImage			*unk02C;		// 02C
	TileImage			*unk030;		// 030
	TileText			*unk034;		// 034
	TileImage			*unk038;		// 038
	TileText			*unk03C;		// 03C
	TileImage			*unk040;		// 040
	TileImage			*unk044;		// 044
	TileText			*unk048;		// 048
	TileImage			*unk04C;		// 04C
	TileText			*unk050;		// 050
	TileImage			*unk054;		// 054
	TileRect			*unk058;		// 058
	TileImage			*unk05C;		// 05C
	TileRect			*unk060;		// 060
	TileRect			*unk064;		// 064
	TileRect			*unk068;		// 068
	TileRect			*unk06C;		// 06C
	TileRect			*unk070;		// 070
	TileImage			*unk074;		// 074
	TileImage			*unk078;		// 078
	TileRect			*unk07C;		// 07C
	TESObjectREFR		*merchantRef;	// 080
	float				barterTotalSum;	// 084
	UInt32				unk088;			// 084
	UInt32				playerGold;		// 08C
	UInt32				merchantGold;	// 090
	UInt32				unk094[2];		// 094
	UInt32				leftFilter;		// 09C
	UInt32				rightFilter;	// 0A0
	UInt32				unk0A4;			// 0A4
	MenuItemEntryList	leftItems;		// 0A8
	TileImage			*unk0B4;		// 0B4
	TileImage			*leftActive;	// 0B8
	TileImage			*unk0BC;		// 0BC
	UInt32				unk0C0;			// 0C0
	UInt16				leftCount;		// 0C4
	UInt16				unk0C6;			// 0C6
	UInt32				unk0C8[4];		// 0C8
	MenuItemEntryList	rightItems;		// 0D8
	TileImage			*unk0E4;		// 0E4
	TileImage			*rightActive;	// 0E8
	TileImage			*unk0EC;		// 0EC
	UInt32				unk0F0;			// 0F0
	UInt16				rightCount;		// 0F4
	UInt16				unk0F6;			// 0F6
	UInt32				unk0F8[4];		// 0F8
	MenuItemEntryList	*currentItems;	// 108
	BarterItemList		leftBarter;		// 10C
	BarterItemList		rightBarter;	// 114
	UInt32				unk11C;			// 11C
};
STATIC_ASSERT(sizeof(BarterMenu) == 0x120);

// 1DC
class HackingMenu : public Menu	// 1055
{
public:
	HackingMenu();
	~HackingMenu();

	UInt32				unk028[92];		// 028
	TESObjectREFR		*targetRef;		// 198
	UInt32				unk19C[16];		// 028

};
STATIC_ASSERT(sizeof(HackingMenu) == 0x1DC);

// 144
class VATSMenu : public Menu	// 1056
{
public:
	VATSMenu();
	~VATSMenu();

	struct VATSAction;

	UInt32						unk028;			// 028
	TileImage					*unk02C;		// 02C
	TileImage					*unk030;		// 030
	TileImage					*unk034;		// 034
	TileImage					*unk038;		// 038
	TileImage					*unk03C;		// 03C
	TileImage					*unk040;		// 040
	TileImage					*unk044;		// 044
	TileImage					*unk048;		// 048
	TileImage					*unk04C;		// 04C
	TileImage					*unk050;		// 050
	TileText					*unk054;		// 054
	TileText					*unk058;		// 058
	TileText					*unk05C;		// 05C
	TileText					*unk060;		// 060
	TileImage					*unk064;		// 064
	TileImage					*unk068;		// 068
	TileImage					*unk06C;		// 06C
	TileImage					*unk070;		// 070
	TileText					*unk074;		// 074
	TileRect					*unk078;		// 078
	TileRect					*unk07C;		// 07C
	TileRect					*unk080;		// 080
	TileImage					*unk084;		// 084
	TileRect					*unk088;		// 088
	TileImage					*unk08C;		// 08C
	TileImage					*unk090;		// 090
	TileImage					*unk094;		// 094
	TileImage					*unk098;		// 098
	TileText					*unk09C;		// 09C
	TileImage					*unk0A0;		// 0A0
	TileImage					*unk0A4;		// 0A4
	UInt32						unk0A8[2];		// 0A8
	BSSimpleList<VATSAction>	queuedActions;	// 0B0
	TileImage					*unk0BC;		// 0BC
	TileImage					*unk0C0;		// 0C0
	TileImage					*unk0C4;		// 0C4
	UInt32						unk0C8[24];		// 0C8
	TESObjectREFR				*targetRef;		// 128
	UInt32						unk12C;			// 12C
	void						*unk130;		// 130
	UInt32						unk134[4];		// 134
};
STATIC_ASSERT(sizeof(VATSMenu) == 0x144);

// 0FC
class ComputersMenu : public Menu	// 1057
{
public:
	ComputersMenu();
	~ComputersMenu();

	UInt32				unk028[33];		// 028
	TESObjectREFR		*targetRef;		// 0AC
	UInt32				unk0B0[19];		// 0B0
};
STATIC_ASSERT(sizeof(ComputersMenu) == 0xFC);

// 088
class CompanionWheelMenu : public Menu	// 1075
{
public:
	CompanionWheelMenu();
	~CompanionWheelMenu();

	UInt32				unk028[16];		// 028
	Actor				*companionRef;	// 068
	UInt32				unk06C[7];		// 06C
};
STATIC_ASSERT(sizeof(CompanionWheelMenu) == 0x88);

// 138
class Sky
{
public:
	Sky();
	~Sky();

	virtual void	Destructor(void);

	NiNode			*niNode04;			// 00
	NiNode			*niNode08;			// 04
	TESClimate		*firstClimate;		// 08
	TESWeather		*firstWeather;		// 0C
	TESWeather		*weather14;			// 10
	TESWeather		*weather18;			// 14
	TESWeather		*weatherOverride;	// 18
	UInt32			atmosphere;			// 1C	* class Atmosphere
	UInt32			stars;				// 20	* class Stars
	UInt32			sun;				// 24	* class Sun
	UInt32			clouds;				// 28	* class Clouds
	UInt32			masserMoon;			// 2C	* class Moon
	UInt32			secundaMoon;		// 30	* class Moon
	UInt32			precipitation;		// 34	* class Precipitation
	UInt32			unk3C[63];			// 38

	static Sky *GetSingleton();
	void RefreshClimate(TESClimate *climate);
};
STATIC_ASSERT(sizeof(Sky) == 0x138);

// C4
class TES
{
public:
	TES();
	~TES();

	UInt32		unk00[49];
};
STATIC_ASSERT(sizeof(TES) == 0xC4);

typedef bool (*_IsMenuMode)(void);
typedef Sky *(*_Sky_GetSingleton)(void);
#if RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525
IDebugLog gLog("jip_nvse.log");
static const UInt32 s_InjectXML = 0xA01B00;
static const UInt32 s_Sky_RefreshClimate = 0x63C8F0;
static const UInt32 s_TES_GetTerrainHeight = 0x4572E0;
static const UInt32 s_GetInventoryItem = 0x5754A0;
static const UInt32 s_TESModelTextureSwapVtbl = 0x101D124;
static const UInt32 s_ExtraLinkedRefVtbl = 0x1015CC0;
static const UInt32 s_ExtraPrimitiveVtbl = 0x10151B4;
static const UInt32 s_BGSPrimitiveVtbl[3] = {0x101E8C4, 0x101EA64, 0x101E75C};
static const Cmd_Execute Cmd_SetStage_Execute = (Cmd_Execute)0x5C7140;
static const Cmd_Execute Cmd_DisableNavMesh_Execute = (Cmd_Execute)0x5BD240;
static const Cmd_Execute Cmd_EnableNavMesh_Execute = (Cmd_Execute)0x5BD300;
static const _IsMenuMode IsMenuMode = (_IsMenuMode)0x702360;
static const _Sky_GetSingleton Sky_GetSingleton = (_Sky_GetSingleton)0x46DD00;
#else
IDebugLog gLog("jip_nvse_ng.log");
static const UInt32 s_InjectXML = 0xA019D0;
static const UInt32 s_Sky_RefreshClimate = 0x63C6C0;
static const UInt32 s_TES_GetTerrainHeight = 0x457FE0;
static const UInt32 s_GetInventoryItem = 0x575740;
static const UInt32 s_TESModelTextureSwapVtbl = 0x101D114;
static const UInt32 s_ExtraLinkedRefVtbl = 0x1015CB0;
static const UInt32 s_ExtraPrimitiveVtbl = 0x10151A4;
static const UInt32 s_BGSPrimitiveVtbl[3] = {0x101E8B4, 0x101EA54, 0x101E74C};
static const Cmd_Execute Cmd_SetStage_Execute = (Cmd_Execute)0x5C7190;
static const Cmd_Execute Cmd_DisableNavMesh_Execute = (Cmd_Execute)0x5BD2B0;
static const Cmd_Execute Cmd_EnableNavMesh_Execute = (Cmd_Execute)0x5BD370;
static const _IsMenuMode IsMenuMode = (_IsMenuMode)0x7022C0;
static const _Sky_GetSingleton Sky_GetSingleton = (_Sky_GetSingleton)0x639B60;
#endif
static PlayerCharacter **g_thePlayer = (PlayerCharacter**)0x11DEA3C;
static InterfaceManager **g_interfaceManager = (InterfaceManager**)0x11D8A80;
static ActorProcessManager *g_actorProcessManager = (ActorProcessManager*)0x11E0E80;
static TES **g_TES = (TES**)0x11DEA10;
static ExtraContainerChanges::EntryData **g_inventoryMenuSelection = (ExtraContainerChanges::EntryData**)0x11D9EA8;
static ExtraContainerChanges::EntryData **g_containerMenuSelection = (ExtraContainerChanges::EntryData**)0x11D93FC;
static ExtraContainerChanges::EntryData **g_barterMenuSelection = (ExtraContainerChanges::EntryData**)0x11D8FA8;

Sky *Sky::GetSingleton()
{
	return Sky_GetSingleton();
}

void Sky::RefreshClimate(TESClimate *climate)
{
	ThisStdCall(s_Sky_RefreshClimate, this, climate, 1);
}

bool GetTerrainHeight(float *posVec, float *outHeight)
{
	return ThisStdCall(s_TES_GetTerrainHeight, *g_TES, posVec, outHeight) ? true : false;
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

TESObjectREFR *GetMerchantContainer(TESObjectREFR *vendor)
{
	if (!vendor) return NULL;
	ExtraMerchantContainer *xMerchCont = (ExtraMerchantContainer*)GetByTypeCast(vendor->extraDataList, MerchantContainer);
	return xMerchCont ? xMerchCont->containerRef : NULL;
}

TESForm *GetInventoryItem(TESObjectREFR *ref, UInt32 idx, bool getWares)
{
	ExtraContainerChanges::EntryData *data = (ExtraContainerChanges::EntryData*)ThisStdCall(s_GetInventoryItem, ref, idx, getWares);
	return data ? data->type : NULL;
}

class InventoryRef
{
public:
	struct Data
	{
		TESForm								*type;
		ExtraContainerChanges::EntryData	*entry;
		ExtraDataList						*xData;

		Data(TESForm *_type, ExtraContainerChanges::EntryData *_entry, ExtraDataList *_xData) : type(_type), entry(_entry), xData(_xData) {}
	};

	Data			data;
	TESObjectREFR	*containerRef;
	TESObjectREFR	*tempRef;
	UInt32			deferredActions[6];
	bool			doValidation;
	bool			removed;
	UInt16			jipMark;

	TESObjectREFR	*GetRef();
	bool			CreateExtraData(BSExtraData *xBSData);
};
STATIC_ASSERT(sizeof(InventoryRef) == 0x30);

typedef InventoryRef *(*_InventoryRefCreate)(TESObjectREFR *container, const InventoryRef::Data &data, bool bValidate);
static _InventoryRefCreate InventoryRefCreate;

typedef InventoryRef *(*_InventoryRefGetForID)(UInt32 refID);
static _InventoryRefGetForID InventoryRefGetForID;

static UInt32 s_nvseInvRefGetRef;
TESObjectREFR *InventoryRef::GetRef()
{
	return (TESObjectREFR*)ThisStdCall(s_nvseInvRefGetRef, this);
}

bool InventoryRef::CreateExtraData(BSExtraData *xBSData)
{
	ExtraContainerChanges *xChanges = ExtraContainerChanges::GetForRef(containerRef);
	if (!xChanges) return false;
	if (!xChanges->data)
	{
		xChanges->data = ExtraContainerChanges::Data::Create(containerRef);
		if (!xChanges->data) return false;
	}
	if (!xChanges->data->objList)
	{
		xChanges->data->objList = ExtraContainerChangesEntryDataListCreate();
		if (!xChanges->data->objList) return false;
	}
	ExtraContainerChanges::EntryData *xEntry = xChanges->data->objList->Find(ItemInEntryDataListMatcher(data.type));
	if (!xEntry)
	{
		xEntry = ExtraContainerChanges::EntryData::Create(data.type->refID, data.entry->countDelta);
		if (!xEntry) return false;
	}
	data.xData = ExtraDataList::Create(xBSData);
	if (!xEntry->extendData)
	{
		xEntry->extendData = xEntry->Add(data.xData);
		if (!xEntry->extendData) return false;
	}
	else xEntry->extendData->AddAt(data.xData, eListEnd);
	if (data.entry->extendData && (jipMark == '#$')) data.entry->extendData->AddAt(data.xData, eListEnd);
	return true;
}

TESObjectREFR *CreateRefForStack(TESObjectREFR *thisObj, ExtraContainerChanges::EntryData *entry)
{
	if (!entry) return NULL;
	ExtraDataList *xData = NULL;
	if (entry->extendData)
	{
		xData = entry->extendData->GetNthItem(0);
		if (xData)
		{
			ExtraCount *xCount = (ExtraCount*)xData->GetByType(kExtraData_Count);
			if (xCount) xCount->count = min(entry->countDelta, 32767);
			else if (entry->countDelta > 1)
			{
				xCount = ExtraCount::Create(min(entry->countDelta, 32767));
				if (!xData->Add(xCount)) FormHeap_Free(xCount);
			}
		}
		else if (entry->countDelta > 1)
		{
			xData = ExtraDataList::Create(ExtraCount::Create(min(entry->countDelta, 32767)));
			entry->extendData->AddAt(xData, eListEnd);
		}
	}
	InventoryRef *iref = InventoryRefCreate(thisObj, InventoryRef::Data(entry->type, entry, xData), false);
	if (iref)
	{
		iref->jipMark = '#$';
		return iref->GetRef();
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

void DumpMemImg(void *data, UInt32 dwords, UInt8 perLine = 8)
{
	UInt32 *ptr = (UInt32*)data;
	char val[9] = {0};
	std::string buffer;

	Console_Print("Output");
	_MESSAGE("\n");

	for (UInt32 it = 1; it <= dwords; it++)
	{
		sprintf_s(val, "%08X", *ptr);
		buffer += val;
		if (it % perLine) buffer += "  ";
		else
		{
			_MESSAGE("%s", buffer.data());
			buffer.clear();
		}
		ptr++;
	}
	if (!buffer.empty()) _MESSAGE("%s", buffer.data());
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
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
};

ParamInfo kParams_JIP_OneInt_OneFormatString[] =
{
	{"TraitID", kParamType_Integer, 0},
	FORMAT_STRING_PARAMS,
};

ParamInfo kParams_JIP_TwoInts_OneOptionalFloat[3] =
{
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 1},
};

ParamInfo kParams_JIP_OneObjectRef_OneOptionalInt[2] =
{
	{"Object Ref", kParamType_ObjectRef, 0},
	{"Integer", kParamType_Integer, 1},
};

ParamInfo kParams_JIP_OneList_TwoOptionalInts[3] =
{
	{"List", kParamType_FormList, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1},
};

ParamInfo kParams_JIP_OneFaction_OneOptionalInt[2] =
{
	{"Faction", kParamType_Faction, 0},
	{"Integer", kParamType_Integer, 1},
};

ParamInfo kParams_JIP_OneQuest_OneInt[2] =
{
	{"Quest", kParamType_Quest, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneEffectShader[1] =
{
	{"Effect Shader", kParamType_EffectShader, 0},
};

ParamInfo kParams_JIP_OneWorldspace_OneInt[2] =
{
	{"Worldspace", kParamType_WorldSpace, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneWorldspace_TwoInts[3] =
{
	{"Worldspace", kParamType_WorldSpace, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_JIP_OneString_OneInt_OneOptionalActorBase[3] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	{"Actor Base", kParamType_ActorBase, 1},
};

ParamInfo kParams_JIP_OneString_OneOptionalActorBase[2] =
{
	{"String", kParamType_String, 0},
	{"Actor Base", kParamType_ActorBase, 1},
};

ParamInfo kParams_JIP_OneObjectRef[1] =
{
	{"Object Ref", kParamType_ObjectRef, 0},
};

ParamInfo kParams_JIP_OneInt_ThreeFloats[4] =
{
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
};

ParamInfo kParams_JIP_OneForm_OneEffect_TwoInts_TwoOptionalInts[6] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Magic Effect", kParamType_MagicEffect, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1},
};
