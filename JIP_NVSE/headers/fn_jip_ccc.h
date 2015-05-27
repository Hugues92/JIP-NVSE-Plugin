#pragma once

DEFINE_COMMAND_PLUGIN(CCCOnRestart, "None", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(CCCOnLoad, "None", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(CCCSetFloat, "None", 0, 2, kParams_JIP_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(CCCSetString, "None", 0, 12, kParams_JIP_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(CCCSetTrait, "None", 0, 3, kParams_JIP_TwoInts_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(CCCGetDistance, "None", 1, 2, kParams_JIP_OneObjectRef_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(CCCGetCombatStyle, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(CCCInfAmmoWeapon, "None", 0, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(CCCInFaction, "None", 1, 2, kParams_JIP_OneFaction_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(CCCSetScript, "None", 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(CCCIsModified, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(CCCLoadNCCS, "None", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(CCCSavedForm, "None", 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(CCCLocationName, "None", 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(CCCGetReputation, "None", 1, 0, NULL);

UInt32 g_CCCModIdx, g_savedForms[11] = {0};
std::unordered_map<std::string, std::string> g_avatarsPath, g_avatarsCommon;

struct MapMarkers
{
	UInt32				wspcID;
	std::vector<UInt32>	markerIDs;

	MapMarkers(UInt32 pWspcID) : wspcID(pWspcID) {}
};
std::vector<MapMarkers> g_mapMarkers;

bool Cmd_CCCOnRestart_Execute(COMMAND_ARGS)
{
	g_CCCModIdx = DataHandler::Get()->GetModIndex("JIP Companions Command & Control.esp") << 24;

	g_avatarsPath.insert({{"A L P H A", "alphadroid"}, {"Achilles of Atlanta", "achilles"}, {"Amanda", "amanda"}, {"Amata", "amata"}, 
						{"Ambassador", "deathclaw"}, {"Andrea ARES 2-14", "andrea"}, {"Angel", "angel"}, {"Arcade Gannon", "arcade"}, 
						{"Assemblotron", "assemblotron"}, {"B-6-RK", "b6rk"}, {"Beanbot", "beanbot"}, {"Bella", "bella"}, {"Ben Kurtz", "benkurtz"}, 
						{"Benny", "benny"}, {"Betsy", "brahmin"}, {"Bittercup", "bittercup"}, {"Blaze", "blaze"}, {"Boone", "boone"}, 
						{"Bradley", "bradley"}, {"Brianna", "brianna"}, {"Brisa", "brisa"}, {"Brisa Almodovar", "brisa"}, {"Bryan Wilks", "bryan"}, 
						{"Bullets", "bullets"}, {"Butch", "butch"}, {"Cass", "cass"}, {"Charon", "charon"}, {"Cheyenne", "cheyenne"}, 
						{"Christine", "christine"}, {"Clover", "clover"}, {"Clover Ace", "cloverace"}, {"Coffeebot", "coffeebot"}, 
						{"Crystal", "crystal"}, {"Cyan", "cyan"}, {"Cyber-Claw", "cyberclaw"}, {"Cyberus", "cyberus"}, {"Deadpan", "deadpan"}, 
						{"Dean Domino", "dean"}, {"Delilah", "delilah"}, {"Denver", "denver"}, {"Desmond", "desmond"}, {"Digger", "molerat"}, 
						{"Doc Friday", "docfriday"}, {"Dog", "doggod"}, {"Dogmeat", "dogmeat"}, {"ED-E", "eyebot"}, {"Electrobot", "electrobot"}, 
						{"Emaciated Cat", "emaciatedcat"}, {"Eric Campbell", "ericcampbell"}, {"Farsight", "mongrel"}, {"Fawkes", "fawkes"}, 
						{"Fichebot", "fichebot"}, {"Floating Eye", "floatingeye"}, {"Follows-Chalk", "followschalk"}, {"God", "doggod"}, 
						{"Great Jack", "greatjack"}, {"Guts", "guts"}, {"Hans", "hans"}, {"Holobot", "holobot"}, {"Hoverbot", "hoverbot"}, 
						{"Hoverbrain", "hoverbrain"}, {"Hydrobot", "hydrobot"}, {"J.T.", "dogmeat"}, {"Jamie Campbell", "jamiecampbell"}, 
						{"Jasmine", "jasmine"}, {"Jenifer Hale", "jeniferhale"}, {"Jericho", "jericho"}, {"Jessi", "jessi"}, {"Jocelyn", "jocelyn"}, 
						{"Johnny Matheson", "johnnymatheson"}, {"Joshua Graham", "joshua"}, {"Jukebot", "jukebot"}, {"Kat", "kat"}, 
						{"Kate", "bbdkate"}, {"Katie", "katie"}, {"Kelsey", "kelsey"}, {"Kira Mann", "kiramann"}, {"Lily", "lily"}, 
						{"Lucy West", "lucywest"}, {"Malibu", "malibu"}, {"Malika Kiowa", "malika"}, {"Marcus", "marcus"}, {"Melissa", "melissa"}, 
						{"Mezcal", "mezcal"}, {"Mika", "mika"}, {"Milton", "milton"}, {"Miria", "miria"}, {"Niner", "niner"}, {"Outcast Nitro", "nitro"}, 
						{"Ovenbot", "ovenbot"}, {"Private Gilbert", "wendy"}, {"Projectorbot", "projectorbot"}, {"Proxima", "cyberdog"}, {"Raul", "raul"}, 
						{"Recursobot", "recursobot"}, {"Rex", "rex"}, {"Rogue", "sentrybot"}, {"Russell", "russell"}, {"Rusty", "rusty"}, 
						{"Safebot", "safebot"}, {"Sarah Conner", "sarahconner"}, {"Sarah Lyons", "sarahlyons"}, {"Scuttlebrain", "scuttlebrain"}, 
						{"Sergeant RL-3", "mrgutsy"}, {"Serverbot", "serverbot"}, {"Shieldbot", "shieldbot"}, {"Shrapnel", "shrapnel"}, {"Sonia", "sonia"}, 
						{"Star Destroyer", "brahmin"}, {"Star Paladin Cross", "cross"}, {"Sunny Smiles", "sunny"}, {"Sydney", "sydney"}, 
						{"Teddy", "yaoguai"}, {"Teslatron", "protectron"}, {"Tess", "tess"}, {"The Highwayman", "thehighwayman"}, 
						{"Thinktank", "thinktank"}, {"Transformer", "transformer"}, {"Turretbot", "turretbot"}, {"TVtron", "tvtron"}, 
						{"Ulysses", "ulysses"}, {"Veronica", "veronica"}, {"Vicious Dog", "dogvicious"}, {"Waking Cloud", "wakingcloud"}, 
						{"Wendy", "wendy"}, {"Willow", "willow"}, {"Zen", "zen"}, {"Zippy", "gecko"}, {"&PCName}, 's Best Friend", "deathclaw"}});

	g_avatarsCommon.insert({{"Alien", "alien"}, {"Ant", "giantant"}, {"Bark Scorpion", "barkscorpion"}, {"Bighorner", "bighorner"}, 
						{"Bloatfly", "bloatfly"}, {"Brahmin", "brahmin"}, {"Cazador", "cazador"}, {"Centaur", "centaur"}, {"Coyote", "coyote"}, 
						{"Cyber Dog", "cyberdog"}, {"Cyberdog", "cyberdog"}, {"Cyberroach", "cyberroach"}, {"Deathclaw", "deathclaw"}, 
						{"Eyebot", "eyebot"}, {"Feral Ghoul", "ghoulferal"}, {"Gecko", "gecko"}, {"Ghoul Reaver", "ghoulreaver"}, {"Giant Rat", "giantrat"}, 
						{"Glowing One", "ghoulglowing"}, {"Lakelurk", "mirelurk"}, {"Large Dog", "dogmeat"}, {"Lobotomite", "mechalobotomite"}, 
						{"Mantis", "giantmantis"}, {"Mirelurk", "mirelurk"}, {"Mister Gutsy", "mrgutsy"}, {"Mister Handy", "mrgutsy"}, 
						{"Mister Orderly", "mrgutsy"}, {"Mole Rat", "molerat"}, {"Mongrel", "mongrel"}, {"Multibot", "multibot"}, 
						{"Night Stalker", "nightstalker"}, {"Nightstalker", "nightstalker"}, {"Nightkin", "nightkin"}, {"Protectron", "protectron"}, 
						{"Radroach", "radroach"}, {"Radscorpion", "radscorpion"}, {"Robo-Scorpion", "roboscorpion"}, {"Robobrain", "robobrain"}, 
						{"Securitron", "securitron"}, {"Sentry Bot", "sentrybot"}, {"Spore Carrier", "sporecarrier"}, {"Super Mutant", "supermutant"}, 
						{"Tunneler", "tunneler"}, {"Turretbot", "miniturretbot"}, {"Wild Dog", "dogmeat"}, {"Yao Guai", "yaoguai"}});

	UInt32 modIdx;
	TESForm *form;

	if ((modIdx = DataHandler::Get()->GetModIndex("Companion Core.esm")) < 255)
	{
		modIdx <<= 24;
		if (form = LookupFormByID(modIdx + 0x812)) g_savedForms[0] = modIdx + 0x812;
		if (form = LookupFormByID(modIdx + 0x810)) g_savedForms[1] = modIdx + 0x810;
		if (form = LookupFormByID(modIdx + 0x950D)) g_savedForms[2] = modIdx + 0x950D;
		if (form = LookupFormByID(modIdx + 0x5B3B4)) g_savedForms[3] = modIdx + 0x5B3B4;
		TESPackage *package = NULL;
		if ((form = LookupFormByID(modIdx + 0x80E)) && (package = DYNAMIC_CAST(form, TESForm, TESPackage)) && package->location && package->GetLocationData())
			package->GetLocationData()->radius = 0;
	}

	if ((modIdx = DataHandler::Get()->GetModIndex("NosCo Companion System.esm")) < 255)
	{
		modIdx <<= 24;
		if (form = LookupFormByID(modIdx + 0xAE0)) g_savedForms[4] = modIdx + 0xAE0;
		if (form = LookupFormByID(modIdx + 0xAE2)) g_savedForms[5] = modIdx + 0xAE2;
		if (form = LookupFormByID(modIdx + 0x86A7)) g_savedForms[6] = modIdx + 0x86A7;
		if (form = LookupFormByID(modIdx + 0xAE5)) g_savedForms[7] = modIdx + 0xAE5;
		if (form = LookupFormByID(modIdx + 0xADD)) g_savedForms[8] = modIdx + 0xADD;
	}

	if (((modIdx = DataHandler::Get()->GetModIndex("EZ_CompanionNVSE.esm")) < 255) && (form = LookupFormByID((modIdx << 24) + 0xDC3)))
		g_savedForms[9] = (modIdx << 24) + 0xDC3;

	if (((modIdx = DataHandler::Get()->GetModIndex("DeadMoney.esm")) < 255) && (form = LookupFormByID((modIdx << 24) + 0x11468)))
		g_savedForms[10] = (modIdx << 24) + 0x11468;

	if ((modIdx = DataHandler::Get()->GetModIndex("Niner.esm")) < 255)
	{
		modIdx <<= 24;
		BGSMessage *message;
		if ((form = LookupFormByID(modIdx + 0x23AE1)) && (message = DYNAMIC_CAST(form, TESForm, BGSMessage)))
		{
			message->description.formDiskOffset = 0;
			message->unk02C[3] = 0;
		}
		if ((form = LookupFormByID(modIdx + 0x23AE2)) && (message = DYNAMIC_CAST(form, TESForm, BGSMessage)))
		{
			message->description.formDiskOffset = 0;
			message->unk02C[3] = 0;
		}
	}

	DataHandler *data;
	TESWorldSpace *wspc;
	std::vector<UInt32> *markerList;
	TESObjectCELL::RefList::Iterator cIter;
	TESObjectREFR *refr;
	ExtraMapMarker *mapMkr;
	for (auto wIter = data->Get()->worldSpaceList.Begin(); !wIter.End(); ++wIter)
	{
		if ((wspc = wIter.Get()) && wspc->cell && (wspc->texture.ddsPath.m_data || wspc->parent))
		{
			markerList = &g_mapMarkers.insert(g_mapMarkers.end(), MapMarkers(wspc->refID))->markerIDs;
			for (cIter = wspc->cell->objectList.Begin(); !cIter.End(); ++cIter)
			{
				if ((refr = cIter.Get()) && (refr->baseForm->refID == 0x10))
				{
					mapMkr = (ExtraMapMarker*)GetByTypeCast(refr->extraDataList, MapMarker);
					if (mapMkr && mapMkr->data && mapMkr->data->fullName.name.m_data) markerList->push_back(refr->refID);
				}
			}
			if (markerList->empty()) g_mapMarkers.pop_back();
		}
	}

	return true;
}

BGSListForm *g_infAmmoList;
Script *g_forcedAltScript, *g_NCCSAltScript;
TESFaction *g_hiredFaction;
Tile::Value *g_UIelements[129] = {0};

bool Cmd_CCCOnLoad_Execute(COMMAND_ARGS)
{
	g_UIelements[0] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\_AltStyle");
	g_UIelements[1] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\_SelectMode");
	g_UIelements[2] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\_Adjust");
	g_UIelements[3] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\_TotalAvatars");
	g_UIelements[4] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\_NumHired");
	g_UIelements[5] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\_Layout");
	g_UIelements[6] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\_Size");
	g_UIelements[7] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\_Anchor");
	g_UIelements[8] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\_Selected");
	g_UIelements[9] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\visible");
	g_UIelements[10] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox0\\_HealthFill");
	g_UIelements[11] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox1\\_HealthFill");
	g_UIelements[12] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox2\\_HealthFill");
	g_UIelements[13] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox3\\_HealthFill");
	g_UIelements[14] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox4\\_HealthFill");
	g_UIelements[15] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox5\\_HealthFill");
	g_UIelements[16] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox6\\_HealthFill");
	g_UIelements[17] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox7\\_HealthFill");
	g_UIelements[18] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox8\\_HealthFill");
	g_UIelements[19] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox9\\_HealthFill");
	g_UIelements[20] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox0\\_Distance");
	g_UIelements[21] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox1\\_Distance");
	g_UIelements[22] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox2\\_Distance");
	g_UIelements[23] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox3\\_Distance");
	g_UIelements[24] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox4\\_Distance");
	g_UIelements[25] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox5\\_Distance");
	g_UIelements[26] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox6\\_Distance");
	g_UIelements[27] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox7\\_Distance");
	g_UIelements[28] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox8\\_Distance");
	g_UIelements[29] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox9\\_Distance");
	g_UIelements[30] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox0\\_Action");
	g_UIelements[31] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox1\\_Action");
	g_UIelements[32] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox2\\_Action");
	g_UIelements[33] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox3\\_Action");
	g_UIelements[34] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox4\\_Action");
	g_UIelements[35] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox5\\_Action");
	g_UIelements[36] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox6\\_Action");
	g_UIelements[37] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox7\\_Action");
	g_UIelements[38] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox8\\_Action");
	g_UIelements[39] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox9\\_Action");
	g_UIelements[40] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox0\\CompAvatar\\filename");
	g_UIelements[41] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox1\\CompAvatar\\filename");
	g_UIelements[42] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox2\\CompAvatar\\filename");
	g_UIelements[43] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox3\\CompAvatar\\filename");
	g_UIelements[44] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox4\\CompAvatar\\filename");
	g_UIelements[45] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox5\\CompAvatar\\filename");
	g_UIelements[46] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox6\\CompAvatar\\filename");
	g_UIelements[47] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox7\\CompAvatar\\filename");
	g_UIelements[48] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox8\\CompAvatar\\filename");
	g_UIelements[49] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox9\\CompAvatar\\filename");
	g_UIelements[50] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox0\\CompName\\string");
	g_UIelements[51] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox1\\CompName\\string");
	g_UIelements[52] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox2\\CompName\\string");
	g_UIelements[53] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox3\\CompName\\string");
	g_UIelements[54] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox4\\CompName\\string");
	g_UIelements[55] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox5\\CompName\\string");
	g_UIelements[56] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox6\\CompName\\string");
	g_UIelements[57] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox7\\CompName\\string");
	g_UIelements[58] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox8\\CompName\\string");
	g_UIelements[59] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\AvatarsBar\\AvatarBox9\\CompName\\string");
	g_UIelements[60] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\_Size");
	g_UIelements[61] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\_Selected");
	g_UIelements[62] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\_Anchor");
	g_UIelements[63] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\_IsWaiting");
	g_UIelements[64] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\_IsFollowingLong");
	g_UIelements[65] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\_IsUsingMelee");
	g_UIelements[66] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\_IsAggressive");
	g_UIelements[67] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\_LabelsON");
	g_UIelements[68] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\x");
	g_UIelements[69] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\Command0\\_CmdType");
	g_UIelements[70] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\Command1\\_CmdType");
	g_UIelements[71] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\Command2\\_CmdType");
	g_UIelements[72] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\Command3\\_CmdType");
	g_UIelements[73] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\Command4\\_CmdType");
	g_UIelements[74] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\CommandsBar\\CommandLabel\\string");
	g_UIelements[75] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\_Menu");
	g_UIelements[76] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\_Selected");
	g_UIelements[77] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsMain\\_HasHome");
	g_UIelements[78] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsMain\\Setting0\\_Darken");
	g_UIelements[79] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsMain\\Setting0\\_Ticked");
	g_UIelements[80] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsMain\\Setting1\\_Darken");
	g_UIelements[81] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsMain\\Setting1\\_Ticked");
	g_UIelements[82] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsMain\\Setting2\\_Darken");
	g_UIelements[83] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsMain\\Setting2\\_Ticked");
	g_UIelements[84] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsMain\\SetHome\\_Darken");
	g_UIelements[85] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsMain\\Remove\\string");
	g_UIelements[86] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsHome\\Setting0\\_Ticked");
	g_UIelements[87] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsHome\\Setting1\\_Ticked");
	g_UIelements[88] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsHome\\Setting2\\_Ticked");
	g_UIelements[89] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsHome\\Setting3\\_Ticked");
	g_UIelements[90] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsHome\\Setting4\\_Ticked");
	g_UIelements[91] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsHome\\Setting5\\_Ticked");
	g_UIelements[92] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\CompLevel\\string");
	g_UIelements[93] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\CompDTREnc\\string");
	g_UIelements[94] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\STRBar\\_Value");
	g_UIelements[95] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\PERBar\\_Value");
	g_UIelements[96] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\ENDBar\\_Value");
	g_UIelements[97] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\CHABar\\_Value");
	g_UIelements[98] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\INTBar\\_Value");
	g_UIelements[99] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\AGIBar\\_Value");
	g_UIelements[100] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\LUCBar\\_Value");
	g_UIelements[101] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\BarterBar\\_Value");
	g_UIelements[102] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\EnergyBar\\_Value");
	g_UIelements[103] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\ExplosivesBar\\_Value");
	g_UIelements[104] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\GunsBar\\_Value");
	g_UIelements[105] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\LockpickBar\\_Value");
	g_UIelements[106] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\MedicineBar\\_Value");
	g_UIelements[107] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\MeleeBar\\_Value");
	g_UIelements[108] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\RepairBar\\_Value");
	g_UIelements[109] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\ScienceBar\\_Value");
	g_UIelements[110] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\SneakBar\\_Value");
	g_UIelements[111] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\SpeechBar\\_Value");
	g_UIelements[112] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\SurvivalBar\\_Value");
	g_UIelements[113] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\UnarmedBar\\_Value");
	g_UIelements[114] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\HeadLimb\\_Value");
	g_UIelements[115] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\TorsoLimb\\_Value");
	g_UIelements[116] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\LHandLimb\\_Value");
	g_UIelements[117] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\RHandLimb\\_Value");
	g_UIelements[118] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\LLegLimb\\_Value");
	g_UIelements[119] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\SettingsStats\\RLegLimb\\_Value");
	g_UIelements[120] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\SettingsMenu\\CompLocation\\LocText\\string");
	g_UIelements[121] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\Crosshair\\_Targeting");
	g_UIelements[122] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\Crosshair\\string");
	g_UIelements[123] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\Help\\HelpText\\string");
	g_UIelements[124] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\SneakMeter\\y");
	if (InterfaceManager::GetMenuComponentValue("HUDMainMenu\\JIPCCCHUDDisplay\\_DUILoaded")->num)
	{
		g_UIelements[125] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\QuestWrap\\QuestReminder\\x");
		g_UIelements[126] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\XPWrap\\x");
		g_UIelements[127] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\InfoWrap\\Info\\y");
		g_UIelements[128] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\EH_Wrap\\EnemyHealth\\y");
	}
	else
	{
		g_UIelements[125] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\QuestReminder\\x");
		g_UIelements[126] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\XPMeter\\x");
		g_UIelements[127] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\Info\\y");
		g_UIelements[128] = InterfaceManager::GetMenuComponentValue("HUDMainMenu\\EnemyHealth\\y");
	}

	g_infAmmoList = DYNAMIC_CAST(LookupFormByID(g_CCCModIdx + 0x6D1E9), TESForm, BGSListForm);
	g_forcedAltScript = DYNAMIC_CAST(LookupFormByID(g_CCCModIdx + 0x1E80E), TESForm, Script);
	g_NCCSAltScript = DYNAMIC_CAST(LookupFormByID(g_CCCModIdx + 0x1E80F), TESForm, Script);
	g_hiredFaction = DYNAMIC_CAST(LookupFormByID(g_CCCModIdx + 0x540EA), TESForm, TESFaction);

	return true;
}

bool Cmd_CCCSetFloat_Execute(COMMAND_ARGS)
{
	UInt32 trait = 0;
	float value = 0;

	if (ExtractArgs(EXTRACT_ARGS, &trait, &value))
		if (g_UIelements[trait]) CALL_MEMBER_FN(g_UIelements[trait]->parent, SetFloatValue)(g_UIelements[trait]->id, value, true);

	return true;
}

bool Cmd_CCCSetString_Execute(COMMAND_ARGS)
{
	UInt32 trait = 0;
	char str[kMaxMessageLength] = {0};

	if (ExtractFormatStringArgs(1, str, paramInfo, scriptData, opcodeOffsetPtr, scriptObj, eventList, kCommandInfo_CCCSetString.numParams, &trait))
		if (g_UIelements[trait]) CALL_MEMBER_FN(g_UIelements[trait]->parent, SetStringValue)(g_UIelements[trait]->id, str, true);

	return true;
}

std::string g_CCCPath = g_gamePath + "data\\textures\\jazzisparis\\ccc\\avatar_";

bool Cmd_CCCSetTrait_Execute(COMMAND_ARGS)
{
	UInt32 trait = 0, child = 0;
	float value = 0;

	if (!ExtractArgs(EXTRACT_ARGS, &trait, &child, &value)) return true;

	if (trait == 40)
	{
		std::string refName, match;
		if (thisObj)
		{
			TESForm *form = thisObj->baseForm;
			if (form)
			{
				refName = GetFullName(form);
				if (g_avatarsPath.count(refName)) match = g_avatarsPath[refName];
				else if (FileExists(g_CCCPath + refName + ".dds")) g_avatarsPath[refName] = match = refName;
				else if (form->typeID == 43)
				{
					for (auto entry = g_avatarsCommon.begin(); entry != g_avatarsCommon.end(); ++entry)
					{
						if (refName.find(entry->first) != -1)
						{
							g_avatarsPath[refName] = match = entry->second;
							break;
						}
					}
				}
				else
				{
					TESActorBaseData *baseData = DYNAMIC_CAST(form, TESForm, TESActorBaseData);
					if (baseData && baseData->IsFemale()) g_avatarsPath[refName] = match = "female";
				}
				if (match.empty()) g_avatarsPath[refName] = match = "male";

				if (g_UIelements[50 + child])
					CALL_MEMBER_FN(g_UIelements[50 + child]->parent, SetStringValue)(g_UIelements[50 + child]->id, refName.data(), true);
			}
		}
		else match = (child % 2) ? "female" : "male";

		match = "jazzisparis\\ccc\\avatar_" + match + ".dds";

		if (g_UIelements[40 + child])
			CALL_MEMBER_FN(g_UIelements[40 + child]->parent, SetStringValue)(g_UIelements[40 + child]->id, match.data(), true);
	}
	else
	{
		if (trait == 30)
		{
			if ((value == 37) || (value == 35) || (value == 17) || (value == 14) || (value == 13)) value = 1;
			else if (value == 1) value = 2;
			else if (value == 5) value = 3;
			else if (value == 57) value = 4;
			else if (value == 58) value = 5;
			else if ((value == 59) || (value == 61)) value = 6;
			else if (value == 60) value = 7;
			else if (value == 62) value = 8;
			else if ((value == 36) || (value == 63)) value = 9;
			else return true;
		}
		if (g_UIelements[trait + child])
			CALL_MEMBER_FN(g_UIelements[trait + child]->parent, SetFloatValue)(g_UIelements[trait + child]->id, value, true);
	}
	return true;
}

bool Cmd_CCCGetDistance_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectREFR *objRef = NULL;
	UInt32 axis = 0;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &objRef, &axis))
	{
		if (!axis)
			*result = sqrt(pow(thisObj->posX - objRef->posX, 2) + pow(thisObj->posY - objRef->posY, 2) + pow(thisObj->posZ - objRef->posZ, 2));
		else if (axis == 1)
			*result = sqrt(pow(thisObj->posY - objRef->posY, 2) + pow(thisObj->posZ - objRef->posZ, 2));
		else if (axis == 2)
			*result = sqrt(pow(thisObj->posX - objRef->posX, 2) + pow(thisObj->posZ - objRef->posZ, 2));
		else if (axis == 3)
			*result = abs(thisObj->posZ - objRef->posZ);
		else if (axis == 4)
			*result = sqrt(pow(thisObj->posX - objRef->posX, 2) + pow(thisObj->posY - objRef->posY, 2));
		else if (axis == 5)
			*result = abs(thisObj->posY - objRef->posY);
		else if (axis == 6)
			*result = abs(thisObj->posX - objRef->posX);
	}
	return true;
}

bool Cmd_CCCGetCombatStyle_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;

	if (thisObj)
	{
		ExtraCombatStyle *xStyle = (ExtraCombatStyle*)GetByTypeCast(thisObj->extraDataList, CombatStyle);
		if (xStyle && xStyle->combatStyle) *refResult = xStyle->combatStyle->refID;
		else
		{
			TESForm *form = thisObj->baseForm;
			if (form->typeID == 42)
			{
				TESNPC *npc = DYNAMIC_CAST(form, TESForm, TESNPC);
				if (npc && npc->unk1D4) *refResult = npc->unk1D4->refID;
			}
			else if (form->typeID == 43)
			{
				TESCreature *cre = DYNAMIC_CAST(form, TESForm, TESCreature);
				if (cre && cre->combatStyle) *refResult = cre->combatStyle->refID;
			}
		}
	}
	return true;
}

std::unordered_map<UInt32, UInt32> g_clonedWeapons;

bool Cmd_CCCInfAmmoWeapon_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	TESForm *form = NULL, *clone = NULL;

	if (!ExtractArgs(EXTRACT_ARGS, &form) || !g_infAmmoList) return true;

	auto findForm = g_clonedWeapons.find(form->refID);
	if (findForm != g_clonedWeapons.end())
	{
		*refResult = findForm->second;
		return true;
	}

	TESObjectWEAP *weapon = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
	if (!weapon || weapon->weaponFlags1.IsSet(128) || (weapon->eWeaponType < 3) || (weapon->eWeaponType > 9) || !weapon->ammoUse) return true;
	BGSAmmoForm *ammo = DYNAMIC_CAST(weapon, TESForm, BGSAmmoForm);
	if (!ammo || !ammo->ammo) return true;
	TESAttackDamageForm *damageForm = DYNAMIC_CAST(form, TESForm, TESAttackDamageForm);
	if (!damageForm) return true;
	UInt16 damage = damageForm->damage + 1;
	if (!(clone = form->CloneForm(0)) || !(weapon = DYNAMIC_CAST(clone, TESForm, TESObjectWEAP)) ||
		!(damageForm = DYNAMIC_CAST(clone, TESForm, TESAttackDamageForm))) return true;
	clone->SetQuestItem(1);
	weapon->weaponFlags1.Write(8, 1);
	weapon->weaponFlags1.Write(128, 1);
	damageForm->damage = damage;
	TESWeightForm *weightForm = DYNAMIC_CAST(clone, TESForm, TESWeightForm);
	if (weightForm) weightForm->weight = 0;
	g_clonedWeapons[form->refID] = clone->refID;
	g_infAmmoList->AddAt(clone, eListEnd);
	*refResult = clone->refID;
	return true;
}

bool Cmd_CCCInFaction_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESFaction *faction = NULL;
	UInt32 inBase = 0;

	if (thisObj && ExtractArgs(EXTRACT_ARGS, &faction, &inBase))
	{
		if (inBase)
		{
			TESActorBaseData *actorBase = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESActorBaseData);
			if (actorBase && (actorBase->GetFactionRank(faction) >= 0))
			{
				*result = 1;
				return true;
			}
		}
		if (GetExtraFactionRank(thisObj->extraDataList, faction) >= 0) *result = 1;
	}
	return true;
}

std::unordered_set<UInt32> g_modifiedForms;
std::unordered_map<UInt32, UInt32> g_NCCSScripts;

void SetAltScript(TESObjectREFR *actor, UInt32 type)
{
	TESScriptableForm *scriptable = DYNAMIC_CAST(actor->baseForm, TESForm, TESScriptableForm);
	Script *pScript = NULL;

	if (type == 6)
	{
		pScript = g_forcedAltScript;
		g_modifiedForms.insert(actor->baseForm->refID);
	}
	else if (type == 4)
	{
		pScript = g_NCCSAltScript;
		if (!g_NCCSScripts.count(actor->refID)) g_NCCSScripts[actor->refID] = scriptable->script->refID;
	}
	else if (g_NCCSScripts.count(actor->refID)) pScript = DYNAMIC_CAST(LookupFormByID(g_NCCSScripts[actor->refID]), TESForm, Script);

	bool bHasXScript = actor->extraDataList.HasType(kExtraData_Script);

	if (scriptable->script != pScript)
	{
		scriptable->script = pScript;
		if (bHasXScript)
		{
			actor->extraDataList.RemoveByType(kExtraData_Script);
			bHasXScript = false;
		}
	}

	if (pScript && !bHasXScript)
	{
		actor->extraDataList.Add(ExtraScript::Create(actor->baseForm, true));
		if (!type)
		{
			VariableInfo *info = pScript->GetVariableByName("Me");
			if (info)
			{
				ScriptEventList *eventList = actor->GetEventList();
				if (eventList)
				{
					ScriptEventList::Var *var = eventList->GetVariable(info->idx);
					if (var) *(UInt64*)&var->data = actor->refID;
				}
			}
		}
	}
}

bool Cmd_CCCSetScript_Execute(COMMAND_ARGS)
{
	UInt32 type = 0;
	if (thisObj && ExtractArgs(EXTRACT_ARGS, &type)) SetAltScript(thisObj, type);
	return true;
}

bool Cmd_CCCIsModified_Execute(COMMAND_ARGS)
{
	*result = (thisObj && g_modifiedForms.count(thisObj->baseForm->refID)) ? 1 : 0;
	return true;
}

bool Cmd_CCCLoadNCCS_Execute(COMMAND_ARGS)
{
	TESForm *form;
	TESObjectREFR *ref;
	for (auto refIter = g_NCCSScripts.begin(); refIter != g_NCCSScripts.end(); ++refIter)
	{
		if ((form = LookupFormByID(refIter->first)) && (ref = DYNAMIC_CAST(form, TESForm, TESObjectREFR)) && 
			(GetExtraFactionRank(ref->extraDataList, g_hiredFaction) < 0)) SetAltScript(ref, 0);
	}
	return true;
}

bool Cmd_CCCSavedForm_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;
	UInt32 idx = 0;
	if (ExtractArgs(EXTRACT_ARGS, &idx) && g_savedForms[idx]) *refResult = g_savedForms[idx];
	return true;
}

bool Cmd_CCCLocationName_Execute(COMMAND_ARGS)
{
	if (thisObj)
	{
		TESObjectCELL *cell = thisObj->parentCell;
		TESWorldSpace *wspc = NULL;
		std::string locName;

		if (!cell)
		{
			ExtraPersistentCell *xPersistentCell = (ExtraPersistentCell*)GetByTypeCast(thisObj->extraDataList, PersistentCell);
			if (xPersistentCell && xPersistentCell->persistentCell) wspc = xPersistentCell->persistentCell->worldSpace;
		}
		else if (!(wspc = cell->worldSpace))
		{
			locName = "in ";
			locName += cell->fullName.name.CStr();
			g_string->Assign(PASS_COMMAND_ARGS, locName.data());
			return true;
		}
		if (wspc)
		{
			std::vector<UInt32> *markerList = NULL;
			ExtraMapMarker *mapMkr = NULL;
			for (auto wspcIter = g_mapMarkers.begin(); wspcIter != g_mapMarkers.end(); ++wspcIter)
			{
				if (wspcIter->wspcID == wspc->refID)
				{
					markerList = &wspcIter->markerIDs;
					break;
				}
			}
			if (markerList)
			{
				TESObjectREFR *refr, *mkrRef = NULL;
				double tX = thisObj->posX, tY = thisObj->posY, minX = tX - 40000, maxX = tX + 40000, minY = tY - 40000, maxY = tY + 40000, rX, rY, curDist, minDist = 60000.0;

				for (auto mkIter = markerList->begin(); mkIter != markerList->end(); ++mkIter)
				{
					if ((refr = DYNAMIC_CAST(LookupFormByID(*mkIter), TESForm, TESObjectREFR)) && ((rX = refr->posX) > minX) && (rX < maxX) && 
						((rY = refr->posY) > minY) && (rY < maxY) && ((curDist = sqrt(pow(tX - rX, 2) + pow(tY - rY, 2))) < minDist) && !(refr->flags & 0x800))
					{
						minDist = curDist;
						mkrRef = refr;
					}
				}
				if (mkrRef) mapMkr = (ExtraMapMarker*)GetByTypeCast(mkrRef->extraDataList, MapMarker);
				if (mapMkr)
				{
					if (minDist > 4096)
					{
						rX = (tX - mkrRef->posX) / minDist;
						rY = tY - mkrRef->posY;

						if ((rX >= 0.97) && (rX <= 1.0)) locName = "east of ";
						else if ((rX >= -1.0) && (rX <= -0.97)) locName = "west of ";
						else if ((rX >= -0.26) && (rX <= 0.26))
						{
							if (rY > 0) locName = "north of ";
							else locName = "south of ";
						}
						else if (rX > 0)
						{
							if (rY > 0) locName = "northeast of ";
							else locName = "southeast of ";
						}
						else if (rY > 0) locName = "northwest of ";
						else locName = "southwest of ";
					}
					else locName = "at ";
					locName += mapMkr->data->fullName.name.CStr();
				}
			}
			if (!mapMkr)
			{
				locName = "at ";
				locName += wspc->fullName.name.CStr();
			}
			g_string->Assign(PASS_COMMAND_ARGS, locName.data());
		}
		else g_string->Assign(PASS_COMMAND_ARGS, "at an unknown location");
	}
	else g_string->Assign(PASS_COMMAND_ARGS, "");

	return true;
}

bool Cmd_CCCGetReputation_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result;
	*refResult = 0;

	if (thisObj)
	{
		TESActorBaseData *actorBase = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESActorBaseData);
		if (actorBase)
		{
			for (auto baseFacIt = actorBase->factionList.Begin(); !baseFacIt.End(); ++baseFacIt)
			{
				if (baseFacIt->faction->reputation)
				{
					*refResult = baseFacIt->faction->reputation->refID;
					return true;
				}
			}
		}
		ExtraFactionChanges *xChanges = GetByTypeCast(thisObj->extraDataList, FactionChanges);
		if (xChanges && xChanges->data)
		{
			for (auto refFacIt = xChanges->data->Begin(); !refFacIt.End(); ++refFacIt)
			{
				if (refFacIt->faction->reputation)
				{
					*refResult = refFacIt->faction->reputation->refID;
					return true;
				}
			}
		}
	}
	return true;
}
