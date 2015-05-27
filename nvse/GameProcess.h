#pragma once

class TESAmmo;
class TESObjectWEAP;

class BaseProcess
{
public:
	BaseProcess();
	~BaseProcess();

	struct AmmoInfo
	{
		void*	unk00;	// 00
		UInt32	count;	// 04
		TESAmmo* ammo;	// 08
		UInt32	unk0C;	// 0C
		UInt32	unk10;	// 10
		UInt32	unk14;	// 14
		UInt32	unk18;	// 18
		UInt32	unk1C;	// 1C
		UInt32	unk20;	// 20
		UInt32	unk24;	// 24
		UInt32	unk28;	// 28
		UInt32	unk2C;	// 2C
		UInt32	unk30;	// 30
		UInt32	unk34;	// 34
		UInt32	unk38;	// 38
		UInt32	unk3C;	// 3C
		UInt32	unk40;	// 40
		TESObjectWEAP* weapon;	// 44
	};

	struct Data004 {
		TESPackage		* package;		// 000
		TESPackageData	* packageData;	// 004
		TESObjectREFR	* targetRef;	// 008
		UInt32			unk00C;			// 00C	Initialized to 0FFFFFFFFh, set to 0 on start
		float			flt010;			// 010	Initialized to -1.0	. Set to GameHour on start so some time
		UInt32			flags;			// 014	Flags, bit0 would be not created and initialized
	};	// 018

	struct	Data02C {
		float	flt000;
		float	flt004;
		float	flt008;
		float	flt00C;
		float	flt010;
		float	flt014;
		float	flt018;
		float	flt01C;
		float	flt020;
		float	flt024;
		float	flt028;
		float	flt02C;
		UInt32	unk030;
		UInt32	unk034;
		float	flt038;
		float	flt03C;
		UInt8	byt040;
		UInt8	fil041[3];
		UInt32	unk044;			// flags
	};

	virtual void	Destroy(bool noDealloc);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);
	virtual void	Unk_12(void);
	virtual void	Unk_13(void);
	virtual void	Unk_14(void);
	virtual void	Unk_15(void);
	virtual void	Unk_16(void);
	virtual void	Unk_17(void);
	virtual void	Unk_18(void);
	virtual void	Unk_19(void);
	virtual void	Unk_1A(void);
	virtual void	Unk_1B(void);
	virtual void	Unk_1C(void);
	virtual void	Unk_1D(void);
	virtual void	Unk_1E(void);
	virtual void	Unk_1F(void);
	virtual void	Unk_20(void);
	virtual void	Unk_21(void);
	virtual void	Unk_22(void);
	virtual void	Unk_23(void);
	virtual void	Unk_24(void);
	virtual void	Unk_25(void);
	virtual void	Unk_26(void);
	virtual void	Unk_27(void);
	virtual void	Unk_28(void);
	virtual void	Unk_29(void);
	virtual void	Unk_2A(void);
	virtual void	Unk_2B(void);
	virtual void	Unk_2C(void);
	virtual void	Unk_2D(void);
	virtual void	Unk_2E(void);
	virtual void	Unk_2F(void);
	virtual void	Unk_30(void);
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);
	virtual void	Unk_35(void);
	virtual void	Unk_36(void);
	virtual void	Unk_37(void);
	virtual void	Unk_38(void);
	virtual void	Unk_39(void);
	virtual void	Unk_3A(void);
	virtual void	Unk_3B(void);
	virtual void	Unk_3C(void);
	virtual void	Unk_3D(void);
	virtual void	Unk_3E(void);
	virtual void	Unk_3F(void);
	virtual void	Unk_40(void);
	virtual void	Unk_41(void);
	virtual void	Unk_42(void);
	virtual void	Unk_43(void);
	virtual void	Unk_44(void);
	virtual void	Unk_45(void);
	virtual void	Unk_46(void);
	virtual void	Unk_47(void);
	virtual void	Unk_48(void);
	virtual void	Unk_49(void);
	virtual void	Unk_4A(void);
	virtual void	Unk_4B(void);
	virtual void	Unk_4C(void);
	virtual void	Unk_4D(void);
	virtual void	Unk_4E(void);
	virtual void	Unk_4F(void);
	virtual void	Unk_50(void);
	virtual void	Unk_51(void);
	virtual void	Unk_52(void);		// GetEquippedWeapon it seems
	virtual AmmoInfo*	GetAmmoInfo();
	virtual void	Unk_54(void);
	virtual void	Unk_55(void);
	virtual void	Unk_56(void);
	virtual void	Unk_57(void);
	virtual void	Unk_58(void);
	virtual void	Unk_59(void);
	virtual void	Unk_5A(void);
	virtual void	Unk_5B(void);
	virtual void	Unk_5C(void);
	virtual void	Unk_5D(void);	// Called by 5E with count itemExtraList item
	virtual void	Unk_5E(void);	// EquipItem and UnEquipItem doEquip item count itemExtraList bytes = [equipArgC lockUnequip unk unEquipArcC lockEquip arg14 ] (arg as from Actor::(Un)EquipItem)
	virtual void	Unk_5F(void);
	virtual void	Unk_60(void);
	virtual void	Unk_61(void);
	virtual void	Unk_62(void);
	virtual void	Unk_63(void);

	virtual void	Unk_64(void);
	virtual void	Unk_65(void);
	virtual void	Unk_66(void);
	virtual void	Unk_67(void);
	virtual void	Unk_68(void);
	virtual void	Unk_69(void);
	virtual void	Unk_6A(void);
	virtual void	Unk_6B(void);
	virtual void	Unk_6C(void);
	virtual void	Unk_6D(void);
	virtual void	Unk_6E(void);
	virtual void	Unk_6F(void);
	virtual void	Unk_70(void);
	virtual void	Unk_71(void);
	virtual void	Unk_72(void);
	virtual void	Unk_73(void);
	virtual void	Unk_74(void);
	virtual void	Unk_75(void);
	virtual void	Unk_76(void);
	virtual void	Unk_77(void);
	virtual void	Unk_78(void);
	virtual void	Unk_79(void);
	virtual void	Unk_7A(void);
	virtual void	Unk_7B(void);
	virtual void	Unk_7C(void);
	virtual void	Unk_7D(void);
	virtual void	Unk_7E(void);
	virtual void	Unk_7F(void);
	virtual void	Unk_80(void);
	virtual void	Unk_81(void);
	virtual void	Unk_82(void);
	virtual void	Unk_83(void);
	virtual void	Unk_84(void);
	virtual void	Unk_85(void);
	virtual void	Unk_86(void);
	virtual void	Unk_87(void);
	virtual void	Unk_88(void);
	virtual void	Unk_89(void);
	virtual void	Unk_8A(void);
	virtual void	Unk_8B(void);
	virtual void	Unk_8C(void);
	virtual void	Unk_8D(void);
	virtual void	Unk_8E(void);
	virtual void	Unk_8F(void);
	virtual void	Unk_90(void);
	virtual void	Unk_91(void);
	virtual void	Unk_92(void);
	virtual void	Unk_93(void);
	virtual void	Unk_94(void);
	virtual void	Unk_95(void);
	virtual void	Unk_96(void);
	virtual void	Unk_97(void);
	virtual void	Unk_98(void);
	virtual void	Unk_99(void);
	virtual void	Unk_9A(void);
	virtual void	Unk_9B(void);
	virtual void	Unk_9C(void);
	virtual void	Unk_9D(void);
	virtual void	Unk_9E(void);
	virtual TESPackage*	GetCurrentPackage(void);

	// data

	Data004		data004;		// current package ?
	float		flt01C;			// not initialized, only by descendant!
	float		flt020;			// not initialized, only by descendant to -1.0! flt020 gets set to GameHour minus one on package evaluation
	UInt32		unk024;			// not initialized, only by descendant!
	UInt32		processLevel;	// not initialized, only by descendant to 3 for Low, 2 for MidlleLow, 1 MiddleHighProcess and 0 for HigProcess
	Data02C*	unk02C;
};

class LowProcess : public BaseProcess
{
public:
	LowProcess();
	~LowProcess();

	struct FloatPair {
		float	flt000;
		float	flt004;
	};

	struct	ActorValueModifier
	{
		UInt8	actorValue;	// 00 Might allow for other values
		UInt8	pad[3];		// 01
		float	damage;		// 04
	};

	struct ActorValueModifiers
	{
		tList<ActorValueModifier>	avModifierList;	// 00
		UInt8						unk008;			// 08
		UInt8						pad009[3];		// 09
		void*						** modifiedAV;	// 0C	array of damaged actorValue
	};	// 10

	UInt8		byt030;		// used before being initialized . Ok, the initial value is zeroed out by a "and 0" but ???
	UInt8		pad031[3];
	UInt32		unk034;
	FloatPair	unk038;
	UInt32		unk03C;		// not initialized!
	UInt32		unk040;		// not initialized!	refr, expected actor
	UInt32		unk044;
	UInt32		unk048;
	UInt32		unk04C;
	UInt32		unk050;		// not initialized!
	UInt32		unk054;
	UInt32		unk058;
	tList<UInt32>	unk05C;		// List
	UInt32		unk064;
	UInt32		unk068;
	tList<TESForm*>	unk06C;
	tList<UInt32>	unk074;
	tList<UInt32>	unk07C;
	UInt32		unk084;
	UInt32		unk088;
	UInt32		unk08C;
	UInt32		unk090;
	ActorValueModifiers	damageModifiers;
	UInt32		unk098;		// not initialized!
	UInt32		unk0A0;		// not initialized!
	UInt32		unk0A4;		// not initialized!
	float		flt0A8;
	float		flt0AC;
	UInt8		byt0B0;
	UInt8		pad0B1[3];	// Filler
};
// LowProcess has 207 virtual func


class MiddleLowProcess : public LowProcess
{
public:
	MiddleLowProcess();
	~MiddleLowProcess();

	UInt32				unk0B4;			// 0B4
	ActorValueModifiers	tempModifiers;	// 0B8
};	// 0C8

// MiddleLowProcess has 208 virtual func, 208 would be SetAnimation

class MiddleHighProcess : public MiddleLowProcess
{
public:
	MiddleHighProcess();
	~MiddleHighProcess();

	struct	Unk11C {
		UInt32	unk000;
		UInt32	unk004;
		UInt32	unk008;		// Semaphore
			// ... ?
	};

	struct	Unk138 {
		UInt32	unk000;
		UInt32	unk004;		// Semaphore
			// ... ?
	};

	struct	Unk148 {
		UInt32	unk000;
		UInt32	unk004;
		UInt32	unk008;
		UInt16	unk00C;
		UInt16	unk00E;		// Might be byte
	};

	tList<TESForm*>			unk0C8;			// 0C8
	tList<UInt32>			unk0D0;			// 0D0
	UInt32					unk0D8[(0xE4-0xD8) >> 2];
	BaseProcess::Data004	unk0E4;			// 0E4	I suspect interrupt package

	UInt8					unk0FC[0x0C];	// 0FC	Saved as one, might be Pos/Rot given size

	UInt32					unk108;			// 108

	UInt32					unk114;			// 114	EntryData: would point to equipped weapon in unk008
	UInt32					unk118;			// 118  EntryData, also handled as part of weapon code.
	Unk11C*					unk11C;			// 11C
	Unk11C*					unk120;			// 120
	UInt8					byt124;			// 124	OneHandGrenade equipped
	UInt8					byt125;			// 125	OneHandMine equipped
	UInt8					byt126;			// 126	OneHandThrown equipped
	UInt8					byt127;			// 127
	UInt8					byt128;			// 128
	UInt8					byt129;			// 129
	UInt8					byt12A;			// 12A
	UInt8					fil12B;			// 12B
	UInt32					unk12C;			// 12C
	UInt32					unk130;			// 130 Gets copied over during TESNPC.CopyFromBase
	UInt8					byt134;			// 134
	UInt8					byt135;			// 135
	UInt8					byt136;			// 136
	UInt8					byt137;			// 137

	Unk138*					unk138;			// 138

	UInt8					byt13C;			// 13C
	UInt8					byt13D;			// 13D

	UInt8					unk13E[(0x148-0x13E)];

	Unk148					unk148;			// 148

	//UInt32				unk158;			
	//UInt32				unk15C;
	//UInt32				unk160;		// get/set.

	//UInt8					byt168;

	//UInt32				unk170;
	//UInt32				unk174;

	//UInt8					byt18C;

	//tList<UInt32>			unk1B0;
	//tList<UInt32>			* unk1B8;		// get.

	//UInt32				animation;	// 1C0
	//UInt8					unk1C4[12];	// Cleared at the same time as the animation

	//UInt32				unk1DA;

	//Unk138*				unk220;
	
};
// MiddleHighProcess has 21B virtual func

// HighProcess: 	// something in wrd2EC and unk2F0
