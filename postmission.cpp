#include "agencyinclude.h"

void debrief(PPLAYER ps, PMISSION ms)
{
	char buf[60];

	cls();
	LRED;
	wsprintf(buf, "This concludes mission #%s.", ms->missioncode);
	bufferprints(90, buf, TYPEWRITER);
	twobeepsmod();
	LCYAN;
	printf("\nPress A Key To Continue");
	waitforkey();
}




void debugchangecharacterstatus(PPLAYER player)
{
	char* typesofactions[8] = {"Activate", "Cyanide", "NOTOCHORD", "Retire", "Escape DSA", "Dead After Torture", "Terminate Employment", "EXIT WITHOUT MODIFICATION"};
/*
	PLIST typelist = createList();
	POTYPE types[6];

	for(int i = 0; i < 6; i++){
		types[i].string = (char*) malloc(sizeof(char) * (strlen(typesofactions[i]) + 1));
		strcpy(types[i].string, typesofactions[i]);
		addtoList(typelist, types[i], StringCompare);
	}
	*/
	PLIST typelist;
	CreateDBStringList(&typelist, typesofactions, 8);
	switch(SimpleListDialogBox("DEBUG", "MODE", typelist, &globals.dboxparams, CreateStringDBListItem)){
	case 0:
		player->status = OPERATIVE_STATUS_ACTIVE;
		player->sealedflag = 0;
		break;
	case 1:
		player->status = OPERATIVE_STATUS_CYANIDED;
		player->sealedflag = 0;
		break;
	case 2:
		player->status = OPERATIVE_STATUS_NOTOCHORDED;
		player->sealedflag = 0;
		break;
	case 3:
		player->status = OPERATIVE_STATUS_RETIRED;
		player->sealedflag = 0;
		break;
	case 4:
		player->status = OPERATIVE_STATUS_WHEREABOUTS_UNKNOWN;
		player->sealedflag = 0;
		break;
	case 5:
		player->status = OPERATIVE_STATUS_DEADAFTERTORTURE;
		player->sealedflag = 1;
	case 6:
		player->sealedflag = 1;
		break;
	case 7:
		break;
	}
	SavePlayer(player);
	FreeDBStringList(typelist);
}

void debugchangecharacterrank(PPLAYER player)
{
	char* ranks[7] = {"Field Operative Recruit", "Field Operative", "Covert Operative", "Covert Operations Specialist", "Elite Covert Operations Specialist", "Elite Covert Ops Specialist First Class", "EXIT WITHOUT MODIFICATION"};
	PLIST typelist;
	CreateDBStringList(&typelist, ranks, 7);
	int result;
	result = SimpleListDialogBox("DEBUG", "MODE", typelist, &globals.dboxparams, CreateStringDBListItem);
	if(result >= 0 && result < 6){
		player->rank = result;
		SavePlayer(player);
	}
	else{
		FreeDBStringList(typelist);
		return;
	}
}

void debugchangepsych(PPLAYER player)
{
	char* psych[6] = {"Exit", "Aggression", "Reflex", "Calmness", "Intelligence", "Willpower"};
	char* psychstr[6];
	char buf[60];
	char psychbuf[60];
	int result;
	int psychatt;
	int psychresult;
	PLIST list;

	while(1)
	{
		psychstr[0] = (char*)malloc(sizeof(char) * 60);
		strcpy(psychstr[0], "Exit To Main Debug");
		for(int i = 1; i < 6; i++){
		psychstr[i] = (char*)malloc(sizeof(char) * 60);
			strcpy(psychstr[i], psych[i]);
		strcat(psychstr[i], " - ");
		}
		sprintf(buf, "%d", player->psychskill.psych.agression);
		strcat(psychstr[1], buf);
		sprintf(buf, "%d",player->psychskill.psych.reflex);
		strcat(psychstr[2], buf);
		sprintf(buf, "%d",player->psychskill.psych.calmness);
		strcat(psychstr[3], buf);
		sprintf(buf, "%d",player->psychskill.psych.intelligence);
		strcat(psychstr[4], buf);
		sprintf(buf, "%d",player->psychskill.psych.willpower);
		strcat(psychstr[5], buf);
	CreateDBStringList(&list, (char**)psychstr, 6);
	
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, TRUE);
	result = SimpleListDialogBox("DEBUG", "MODE", list, &globals.dboxparams, CreateStringDBListItem);
	
	switch(result)
	{
	case 0:
		FreeDBStringList(list);
		for(i = 0; i < 6; i++){
		free(psychstr[i]);
		}
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, FALSE);
	SavePlayer(player);

	return;
	case 1:
		psychatt = player->psychskill.psych.agression;
		break;
	case 2:
		psychatt = player->psychskill.psych.reflex;
		break;
	case 3:
		psychatt = player->psychskill.psych.calmness;
		break;
	case 4:
		psychatt = player->psychskill.psych.intelligence;
		break;
	case 5:
		psychatt = player->psychskill.psych.willpower;
		break;
	}
	sprintf(psychbuf, "Agent %s's %s level is currently set at %d. Please enter a number from 0 to %d to change.", player->lname, psych[result], psychatt, MAX_PSYCH);
	psychresult = NumberDialogBox("PSYCHOLOGICAL", psychbuf, "ADJUSTMENT", 1, &globals.dboxparams);
	psychresult = CapInclusive(psychresult, 0, MAX_PSYCH);
	switch(result)
	{
	case 1:
	player->psychskill.psych.agression = psychresult;
		break;
	case 2:
		player->psychskill.psych.reflex = psychresult;
		break;
	case 3:
		player->psychskill.psych.calmness = psychresult;
		break;
	case 4:
		player->psychskill.psych.intelligence = psychresult;
		break;
	case 5:
		player->psychskill.psych.willpower = psychresult;
		break;
	}
SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, TRUE);
	StaticDialogBox("PSYCHOLOGY", "Thank you. Your adjustment has been made.", "ADJUSTED", &globals.dboxparams);
	freeList(list);
		for(i = 0; i < 6; i++){
		free(psychstr[i]);
		}
	}
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, FALSE);
	SavePlayer(player);

	return;
}

void debugchangeskills(PPLAYER player)
{
		char* psych[8] = {"Exit", "Firearms", "Electronics", "Mechanical", "Sniping", "Stealth Combat", "Unarmed Melee Combat", "Armed Melee Combat"};
	char* psychstr[8];
	char buf[60];
	char psychbuf[60];
	int result;
	int psychatt;
	int psychresult;
	PLIST list;

	while(1)
	{
		psychstr[0] = (char*)malloc(sizeof(char) * 60);
		strcpy(psychstr[0], "Exit To Main Debug");
		for(int i = 1; i < 8; i++){
		psychstr[i] = (char*)malloc(sizeof(char) * 60);
			strcpy(psychstr[i], psych[i]);
		strcat(psychstr[i], " - ");
		}
		sprintf(buf, "%d", player->psychskill.skills.firearms);
		strcat(psychstr[1], buf);
		sprintf(buf, "%d",player->psychskill.skills.electronics);
		strcat(psychstr[2], buf);
		sprintf(buf, "%d",player->psychskill.skills.mechanical);
		strcat(psychstr[3], buf);
		sprintf(buf, "%d",player->psychskill.skills.snipingcombat);
		strcat(psychstr[4], buf);
		sprintf(buf, "%d",player->psychskill.skills.stealthcombat);
		strcat(psychstr[5], buf);
		sprintf(buf, "%d",player->psychskill.skills.meleecombatNOweapon);
		strcat(psychstr[6], buf);
		sprintf(buf, "%d",player->psychskill.skills.meleecombatWITHweapon);
		strcat(psychstr[7], buf);
	CreateDBStringList(&list, (char**)psychstr, 8);
	
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, TRUE);
	result = SimpleListDialogBox("DEBUG", "MODE", list, &globals.dboxparams, CreateStringDBListItem);
	
	switch(result)
	{
	case 0:
		FreeDBStringList(list);
		for(i = 0; i < 8; i++){
		free(psychstr[i]);
		}
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, FALSE);
	SavePlayer(player);

	return;
	case 1:
		psychatt = player->psychskill.skills.firearms;
		break;
	case 2:
		psychatt = player->psychskill.skills.electronics;
		break;
	case 3:
		psychatt =player->psychskill.skills.mechanical;
		break;
	case 4:
		psychatt = player->psychskill.skills.snipingcombat;
		break;
	case 5:
		psychatt = player->psychskill.skills.stealthcombat;
		break;
	case 6:
		psychatt = player->psychskill.skills.meleecombatNOweapon;
		break;
	case 7:
		psychatt = player->psychskill.skills.meleecombatWITHweapon;
		break;
	}
	sprintf(psychbuf, "Agent %s's %s skill level is currently set at %d. Please enter a number from 0 to %d to change.", player->lname, psych[result], psychatt, MAX_SKILL);
	psychresult = NumberDialogBox("SKILLSET", psychbuf, "ADJUSTMENT", 2, &globals.dboxparams);
	psychresult = CapInclusive(psychresult, 0, MAX_SKILL);
	switch(result)
	{
case 1:
		player->psychskill.skills.firearms = psychresult;
		break;
	case 2:
	player->psychskill.skills.electronics = psychresult;
		break;
	case 3:
		player->psychskill.skills.mechanical = psychresult;
		break;
	case 4:
		player->psychskill.skills.snipingcombat = psychresult;
		break;
	case 5:
		 player->psychskill.skills.stealthcombat = psychresult;
		break;
	case 6:
		 player->psychskill.skills.meleecombatNOweapon = psychresult;
		break;
	case 7:
		 player->psychskill.skills.meleecombatWITHweapon = psychresult;
		break;
	}
SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, TRUE);
	StaticDialogBox("SKILLSET", "Thank you. Your adjustment has been made.", "ADJUSTED", &globals.dboxparams);
	freeList(list);
		for(i = 0; i < 8; i++){
		free(psychstr[i]);
		}
	}
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, FALSE);
	SavePlayer(player);

	return;
}

void debugchangespecializations(PPLAYER player)
{
		char* psych[10] = {"Exit", "Hand Weapons", "Handguns", "Sub-Machine Guns", "Shotguns", "Rifles", "Special Weapons", "Explosives", "Electronics","Mechanical"};
	char* psychstr[10];
	char buf[60];
	char psychbuf[60];
	int result;
	int psychatt;
	int psychresult;
	PLIST list;

	while(1)
	{
		psychstr[0] = (char*)malloc(sizeof(char) * 60);
		strcpy(psychstr[0], "Exit To Main Debug");
		for(int i = 1; i < 10; i++){
		psychstr[i] = (char*)malloc(sizeof(char) * 60);
			strcpy(psychstr[i], psych[i]);
		strcat(psychstr[i], " - ");
		}
		sprintf(buf, "%d", player->psychskill.specializations.handweapons);
		strcat(psychstr[1], buf);
		sprintf(buf, "%d",player->psychskill.specializations.handguns);
		strcat(psychstr[2], buf);
		sprintf(buf, "%d",player->psychskill.specializations.submachineguns);
		strcat(psychstr[3], buf);
		sprintf(buf, "%d",player->psychskill.specializations.shotguns);
		strcat(psychstr[4], buf);
		sprintf(buf, "%d",player->psychskill.specializations.rifles);
		strcat(psychstr[5], buf);
		sprintf(buf, "%d",player->psychskill.specializations.specialweapons);
		strcat(psychstr[6], buf);
		sprintf(buf, "%d",player->psychskill.specializations.explosives);
		strcat(psychstr[7], buf);
		sprintf(buf, "%d",player->psychskill.specializations.electronics);
		strcat(psychstr[8], buf);
		sprintf(buf, "%d",player->psychskill.specializations.mechanical);
		strcat(psychstr[9], buf);
	CreateDBStringList(&list, (char**)psychstr, 10);
	
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, TRUE);
	result = SimpleListDialogBox("DEBUG", "MODE", list, &globals.dboxparams, CreateStringDBListItem);
	
	switch(result)
	{
	case 0:
		FreeDBStringList(list);
		for(i = 0; i < 10; i++){
		free(psychstr[i]);
		}
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, FALSE);
	SavePlayer(player);

	return;
	case 1:
		psychatt = player->psychskill.specializations.handweapons;
		break;
	case 2:
				psychatt = player->psychskill.specializations.handguns;
		break;
	case 3:
				psychatt = player->psychskill.specializations.submachineguns;
		break;
	case 4:
				psychatt = player->psychskill.specializations.shotguns;
		break;
	case 5:
				psychatt = player->psychskill.specializations.rifles;
		break;
	case 6:
			psychatt = player->psychskill.specializations.specialweapons;
		break;
	case 7:
			psychatt = player->psychskill.specializations.explosives;
		break;
	case 8:
				psychatt = player->psychskill.specializations.electronics;
		break;
	case 9:
				psychatt = player->psychskill.specializations.mechanical;
		break;
	}
	sprintf(psychbuf, "Agent %s's %s Specialization level is currently set at %d. Please enter a number from 0 to %d to change.", player->lname, psych[result], psychatt, MAX_SPECIALIZATION);
	psychresult = NumberDialogBox("SPECIALIZATION", psychbuf, "ADJUSTMENT", 2, &globals.dboxparams);
	psychresult = CapInclusive(psychresult, 0, MAX_SPECIALIZATION);
	switch(result)
	{
	case 1:
	 player->psychskill.specializations.handweapons = psychresult;
		break;
	case 2:
			player->psychskill.specializations.handguns = psychresult;
		break;
	case 3:
				 player->psychskill.specializations.submachineguns = psychresult;
		break;
	case 4:
			 player->psychskill.specializations.shotguns = psychresult;
		break;
	case 5:
			player->psychskill.specializations.rifles = psychresult;
		break;
	case 6:
		player->psychskill.specializations.specialweapons = psychresult;
		break;
	case 7:
		player->psychskill.specializations.explosives = psychresult;
		break;
	case 8:
			player->psychskill.specializations.electronics = psychresult;
		break;
	case 9:
				player->psychskill.specializations.mechanical = psychresult;
		break;
	}
SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, TRUE);
	StaticDialogBox("SPECIALIZATION", "Thank you. Your adjustment has been made.", "ADJUSTED", &globals.dboxparams);
	freeList(list);
		for(i = 0; i < 10; i++){
		free(psychstr[i]);
		}
	}
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, FALSE);
	SavePlayer(player);

	return;
}

void WrapUpMonth(PPLAYER player)
{
		player->monthscompleted++;
		player->numberofdaysincycle = 30;

		//CHECK FOR INSUFFICIENT MISSIONS....
		player->sealedflag = 0; //IF IT'S NO GOOD, SEAL HIM, HE'S DONE FOR.
}

void Surgery(PPLAYER player)
{
	int Chance = player->currentmissioninfo.missionstats.damagetaken - 80;
	Chance = CapInclusive(Chance, 0, 100);
	if(RandomPercentage(Chance))
	{
		//SURGERY IS NEEDED.
		LoopSong(globals.musiclist.songs[CAPTIVITY_SONG]);
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("MEDICAL EMERGENCY", "Entering the van, you are seized with pain as the adrenaline clears from your system. You realize, as your body tenses from chills, that you're in trouble. You are barely able to communicate your problem to the navigator before you collapse, unconscious. The driver radios in for the nearest DSA Medical Center. Your vision fades as you pass out from the pain.", "MEDICAL EMERGENCY", &globals.dboxparams);
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 1, TRUE);
		cls(0);
		switch(rand()%MAX_SURGERY_TYPES)
		{
		case SURGERY_TYPE_BROKENARM:
			ComputerPrintOutString("\"What the hell is THIS,\" you exclaim the next morning, as you realize your arm is immobile. \"That\", exclaims a tall, prudish doctor as he enters the room, \"Is what's left of your arm.\" As you gasp, he smiles. \"Just kidding. I love that one. Your arm is broken. We've run a new procedure on it called Catalyzed Ossification to attempt to speed up the recovery rate. You should be out of here tomorrow. But know that your arm isn't as strong as it was, and won't be for a while. Now get some rest; and by the way...you're welcome.\" The doctor strides out of the room, leaving you to your arm.", 20, "Press ENTER.");
			StaticDialogBox("SKILL LOSS", "RESULT:\nLOSS OF MELEE COMBAT SKILL", "PRESS ENTER", &globals.dboxparams);
			if(player->psychskill.skills.meleecombatNOweapon > 1){
				player->psychskill.skills.meleecombatNOweapon -= 1;
			}
			if(player->psychskill.skills.meleecombatWITHweapon < 1){
			player->psychskill.skills.meleecombatWITHweapon -= 1;
			}
			break;
		case SURGERY_TYPE_VERTEBRAEFRAG:
			ComputerPrintOutString("You open your eyes to find yourself in a medical bed. As you sit up, your back emits a creak and you gasp for air. You pound on the metal side of the bed, and a doctor rushes in to straighten you out, raising your hands above your head. You can breathe again. \"Careful,\" he warns. \"You've got an artificial vertebra now, and it takes some getting used to. Your old one was fragmented by a severe impact. It's all right now, you're just going to have to get used to it. Our materials can't quite mimic the real thing, so you might feel a little sluggish in your reflexes, but that'll soon set right with time.\" You attempt to get used to the idea of artificial vertebrae.", 20, "Press ENTER.");
			StaticDialogBox("PSYCH LOSS", "RESULT:\nLOSS OF REFLEX", "PRESS ENTER", &globals.dboxparams);
			if(player->psychskill.psych.reflex > 1){
				player->psychskill.psych.reflex -= 1;
			}
			break;
		case SURGERY_TYPE_ORGANDAMAGE:
			ComputerPrintOutString("You're in a hospital cot. You sit up and feel a burning inside your chest. You collapse back down and groan. \"Ah, you're up.\" You groan your confirmation. \"Yes, we figured that'd be your response. You've had some internal organ damage, I'm afraid. We've used laser sealing to close you up, but we've had to put you on anti-inflammatory medication, and that's got some fairly bad side effects. Most notably, you're going to feel a bit out of it for a while; a bit weak. You're going to have to deal with it, I'm afraid. Do get some rest now. You'll be back in action soon.\"", 20, "Press ENTER.");
			StaticDialogBox("PSYCH LOSS", "RESULT:\nLOSS OF AGGRESSION", "PRESS ENTER", &globals.dboxparams);
			if(player->psychskill.psych.agression > 1){
				player->psychskill.psych.agression -= 1;
			}
			break;
		case SURGERY_TYPE_NERVEDAMAGE:
			ComputerPrintOutString("You wake up. Everything seems fine for once. Suddenly, your eyelid twitches. It happens again. \"Hey...what the...oh, wonderful.\" It's HIM again. The doctor looks at you with the same expression he always does. \"Yes,\" he begins, \"Afraid you've suffered some nerve damage on your last spree. Thanks to my team and myself, we've been able to fix that with some neuron rerouting, but we overcompensated a tad. I expect you'll be twitchy for a while. It's part of the whole process, I'm afraid. Now, get some rest and try to keep yourself alive next time.\"", 20, "Press ENTER.");
			StaticDialogBox("PSYCH LOSS", "RESULT:\nLOSS OF CALMNESS", "PRESS ENTER", &globals.dboxparams);
			if(player->psychskill.psych.calmness > 1){
				player->psychskill.psych.calmness -= 1;
			}
			break;
		case SURGERY_TYPE_NERVESHREDDING:
			ComputerPrintOutString("Your body is tingling. It's no dream, you feel pinpricks here and there, picking and stabbing at you. \"What the hell is happening to me?\" The doctor walks in as if he expected the question. \"You're not going to like this.\" Your eyes rise to meet him. \"Nerve shredding. Whatever you did out there ripped some of your nerves apart. We did our best to fix that; and our neurologist was able to work some axon pathing to stave off most of the permanent damage, but you're getting some of the side effects. Look on the bright side...there's a reason they call it nerve shredding; it could have been much worse.\"", 20, "Press ENTER.");
			StaticDialogBox("SKILL LOSS", "RESULT:\nLOSS OF STEALTH COMBAT SKILL", "PRESS ENTER", &globals.dboxparams);
			if(player->psychskill.skills.stealthcombat > 1){
				player->psychskill.skills.stealthcombat -= 1;
			}
			break;
		case SURGERY_TYPE_OCULARDAMAGE:
			ComputerPrintOutString("You see the light! And then you see the doctor. \"Not you again...\" The doctor sniffs derisively. \"Don't blame me. I assume you're not seeing as clearly as you once did. You've suffered some ocular damage on your last run, and we've had to be quick on the laser to reconstruct your eyes. But of course...\" You can finish the rest. \"There's side effects.\" The doctor nods. \"You've been here too many times, you know the routine already. The upshot is that your night vision is hampered, we've reconstructed the cones and rods in your eyes, but they're unbalanced. With a little work, you'll be back to where you were. Now rest up and then get out of my ward.\"", 20, "Press ENTER.");
			StaticDialogBox("SKILL LOSS", "RESULT:\nLOSS OF SNIPING SKILL", "PRESS ENTER", &globals.dboxparams);
			if(player->psychskill.skills.snipingcombat > 1){
				player->psychskill.skills.snipingcombat -= 1;
			}
			break;
		case SURGERY_TYPE_TORNHANDTENDONS:
			ComputerPrintOutString("You wake up suddenly, realizing that your hand HURTS. You look around for that damn doctor...and there he is, as always. \"So, you got your hand into a bit of trouble. Tore the tendons, you did. Normally that'd take months to heal. We were able to apply alkaline catalysis to speed the process. Such a process overrides the body's natural ability to heal, so there's a drawback. Expect your hand to be a bit tight, I doubt if you'll be able to grip too strongly. But I assumed that you'd want your hand rather than be without it. You're welcome.\" He turns and leaves.", 20, "Press ENTER.");
			StaticDialogBox("SKILL LOSS", "RESULT:\nLOSS OF FIREARMS SKILL", "PRESS ENTER", &globals.dboxparams);
			if(player->psychskill.skills.firearms > 1){
				player->psychskill.skills.firearms -= 1;
			}
			break;
		case SURGERY_TYPE_TORNHANDLIGAMENTS:
			ComputerPrintOutString("\"Good Morning,\" the doctor says rather abruptly. You wake from your sleep and groan. You're back in the clinic. \"Yes,\" he begins as always, \"Your hand nearly popped open. Lost all of the synovial fluid and most of the ligaments are torn. We reseated the ligaments and replaced the synovial fluid with what synthetics we had on hand. Best not to ask, it's a bit like WD-40. The problem is that your hands' going to be a bit unresponsive. Don't expect to be making any sort of incredibly delicate maneuvers. Better you than me...that'd put me straight out of a job.\"", 20, "Press ENTER.");
			StaticDialogBox("SKILL LOSS", "RESULT:\nLOSS OF MECHANICAL SKILL", "PRESS ENTER", &globals.dboxparams);
			if(player->psychskill.skills.mechanical > 1){
				player->psychskill.skills.mechanical -= 1;
			}
			break;
		}
	}
}

void FinalCable(PPLAYER player)
{
	char buf[5000];
	char tempbuf[5000];
	sprintf(buf, "-DSA OPEN WIRE-\nOFFICIAL DEFENSE SECURITY AGENCY CABLE\nUNAUTHORIZED REPRODUCTION CARRIES MINIMUM $250000 FINE\n-BEGIN CABLE-\nSUBJECT: AGENT %d\nRE: TERMINATION\nOfficial DSA Report - Alexandria, VA TRUNK V\n", player->id);
	switch(player->status)
	{
	case OPERATIVE_STATUS_ACTIVE:
		sprintf(tempbuf, "Regretfully informing Agent %s TERMINATED after unsatisfactory performance. Due to violations DSA AEGIS 15 PARAGRAPH 6, et cetera, Agent removed from active duty, disavowed, and released. Withhold monetary bonus and pension; redistribute to Channel Islands Liquid ACCT %d upon recieve.\n\nSuggest latent tracing of Agent for a period of no less than 6 MONTHS following final closing procedures. Avoid contact unless absolutely necessary. DO NOT ALLOW SUBJECT TO REVEAL PAST EMPLOYMENT.", player->lname, rand()%90000 + 10000);
		break;
	case OPERATIVE_STATUS_CYANIDED:
		sprintf(tempbuf, "Informing Agent %s SELF-TERMINATION during field operation. Details of mission CLOSED at this time, OPEN upon final investigation. Next of kin to be paid in the amount of $25,000 US pending final investigation, route through Trinidad Financiers ACCT %d, SPECIAL MONETARY. Recommend MEDAL OF TRUTH, authorized for DSA SERVICE. Arrangements for SILENT WALL made within 72 HOURS from date of enclosure. Recommend file remain OPEN, research group notified for possible AGENDA 6. Priority ONE.", player->lname, rand()%90000 + 10000);
		break;
	case OPERATIVE_STATUS_NOTOCHORDED:
		sprintf(tempbuf, "Informing Agent %s FORCIBLY TERMINATED due to possible leakage of information during torture sessions in field operations. Agent #%d was dispatched for NOTOCHORD operation, reports MISSION SUCCESS within 9%d.%d%% probability. While a possible case for immediate SEALING, recommend DO NOT SEAL for purposes of study.\nRequired: NOTOCHORD Agent for NK Assay - report ANY PSYCHOLOGICAL INSTABILITY following operation. Next of kin to be awarded DSA Summation Policy D-3N9 ON ORDER OF $15,000 US. FINAL.", player->lname, rand()%90000 + 10000, rand()%10, rand()%10);
		break;
	case OPERATIVE_STATUS_WHEREABOUTS_UNKNOWN:
		sprintf(tempbuf, "Regretfully Reporting Agent %s MISSING IN ACTION following CLASSIFIED field operation. Operative ASSUMED DECEASED, recommend contingency SEARCH Level III - US EXTERNAL, Agent may be in hiding. Orders to RECOVER if found. Recommend INVESTIGATE Level V Extraction Team - Polygraph indication may show negligence in recovery efforts. Withhold pension and summation until official investigation can confirm operative death. FINAL.", player->lname);
		break;
	case OPERATIVE_STATUS_RETIRED:
		sprintf(tempbuf, "Reporting Standard Operative Retirement Procedure. Pension on the order of US $50,000 to be paid to Ex-Operative %s. Operative has requested Bank De Geneve ACCT %d to recieve. Operations bonus RETROGRADE PER 6 MONTHS into indicated account.\n REQUIRED: INVESTIGATION into Agents activities, notify research group POSSIBLE AGENDA 6 - study mission recordings, logs, post-reports. Take all necessary precautions and duplications. OPERATIVE CALLBACK MAY BE NECESSARY - DO NOT MISPLACE.\nAgent %s OFFICIAL HONORABLE RETIREMENT FROM DSA SERVICE - GRANTED.", player->lname, rand()%90000 + 10000, player->lname );
		break;
	case OPERATIVE_STATUS_DEADAFTERTORTURE:
		sprintf(tempbuf, "BURN THIS TRANSMISSION UPON RECIEPT\nEMERGENCY - Operative %s SECURITY BREACH\nREQUIRED URGENTLY: Closing procedures. Burn all records, transcripts, interviews, test results, recordings, dossiers, voiceprints, DNA records, serological samples RECURSIVE UP TO THREE LEVELS DEEP - IMMEDIATELY\n\nOPERATIVE %d DOES NOT EXIST.\nPension and bonuses to IMMEDIATELY BE RE-DISTRIBUTED TO NON-PROFIT-ORGANIZATIONS AS CASH.\nBURN THIS TRANSMISSION UPON RECIEPT.", player->lname, player->id);
		break;
	}
	strcat(buf, tempbuf);
	strcat(buf, "\n-Level V\n-THIS ENDS TRANSMISSION-\n-DSA CLOSE WIRE-");
	ComputerPrintOutString(buf, 20, "Press ENTER To View Final Analysis.");
	cls(0);
}

void FinalAnalysis(PPLAYER player)
{
	char buf[500];
	char tempbuf[500];
	
	sprintf(buf, "Agent %s #%d", player->lname, player->id);
	PrintFinalData("Termination Analysis:", buf, false, 60, 80, 500, 2500);
	
	switch(player->status)
	{
	case OPERATIVE_STATUS_ACTIVE:
		strcpy(buf, "DSA Employment Terminated");
		break;
	case OPERATIVE_STATUS_CYANIDED:
		strcpy(buf, "Self-Terminated In Field");
		break;
	case OPERATIVE_STATUS_NOTOCHORDED:
		strcpy(buf, "Forcibly Terminated In Field");
		break;
	case OPERATIVE_STATUS_WHEREABOUTS_UNKNOWN:
		strcpy(buf, "Whereabouts Unknown");
		break;
	case OPERATIVE_STATUS_RETIRED:
		strcpy(buf, "Honorably Retired From DSA");
		break;
	case OPERATIVE_STATUS_DEADAFTERTORTURE:
		strcpy(buf, "Deceased Following Torture");
		break;
	}
	PrintFinalData("Operative Status:", buf, false, 60, 80, 500, 2000);
	
	if(player->sealedflag){
		PrintFinalData("Operative Rank Upon Termination:", "SEALED", true, 60, 80, 500, 2000);
		PrintFinalData("Length Of Service:", "SEALED", true, 60, 80, 500, 2000);
		PrintFinalData("Missions Completed:", "SEALED", true, 60, 80, 500, 2000);
		PrintFinalData("Dollar Damage:", "SEALED", true, 60, 80, 500, 2000);
		PrintFinalData("Confirmed Kills:", "SEALED", true, 60, 80, 500, 2000);
		PrintFinalData("Cumulative Mission Rating:", "SEALED", true, 60, 80, 500, 2000);
		PrintFinalData("Final Analysis:", "DISCARDED", true, 60, 80, 500, 2000);
		PrintFinalData("Recommendation:", "CLASSIFIED", true, 60, 80, 500, 2000);
		return;
	}

	switch(player->rank)
	{
	case AGENT_RANK_FOR:
		strcpy(buf, "Field Operative Recruit");
		break;
	case AGENT_RANK_FO:
		strcpy(buf, "Field Operative");
		break;
	case AGENT_RANK_CO:
		strcpy(buf, "Covert Operative");
		break;
	case AGENT_RANK_COS:
		strcpy(buf, "Covert Operations Specialist");
		break;
	case AGENT_RANK_ECOS:
		strcpy(buf, "Elite Covert Operations Specialist");
		break;
	case AGENT_RANK_ECOSSPECONE:
		strcpy(buf, "Elite Covert Operations Specialist First Class");
		break;
	default:
		strcpy(buf, "Operative");
		break;
	}
	PrintFinalData("Operative Rank Upon Termination:", buf, false, 60, 80, 500, 2000);

	sprintf(buf, "%d MONTHS, %d DAYS", player->monthscompleted, 30 - player->numberofdaysincycle);
	PrintFinalData("Length Of Service:", buf, false, 60, 80, 500, 2000);

	sprintf(buf, "%d", player->history.missionsaccomplished);
	PrintFinalData("Missions Completed:", buf, false, 60, 80, 500, 2000);

	sprintf(buf, "$%2.2f Million US", player->history.damageinmillions);
	PrintFinalData("Dollar Damage:", buf, false, 60, 80, 500, 2000);

	sprintf(buf, "%d Dead", player->history.kills);
	PrintFinalData("Confirmed Kills:", buf, false, 60, 80, 500, 2000);

	sprintf(buf, "%2.2f%%", player->history.cumulativemissionrating);
	PrintFinalData("Cumulative Mission Rating:", buf, false, 80, 100, 500, 2000);

	if(player->history.cumulativemissionrating > 90){
		strcpy(buf, "Ideal Agent");
		strcpy(tempbuf, "Contact VII Concerning Agent Recovery");
	}
	else if(player->history.cumulativemissionrating > 80){
		strcpy(buf, "Profitable Venture");
		strcpy(tempbuf, "Contact Next Of Kin For Employment");
	}
	else if (player->history.cumulativemissionrating > 70){
		strcpy(buf, "Useful Venture");
		strcpy(tempbuf, "Contact Previous Employer Regarding Employment");
	}
	else if(player->history.cumulativemissionrating > 60){
		strcpy(buf, "Fair Agent");
		strcpy(tempbuf, "No Recommendation At This Time");
	}
	else if(player->history.cumulativemissionrating > 50){
		strcpy(buf, "Poor Performer");
		strcpy(tempbuf, "Study Replay Tapes For Negative Trends");
	}
	else if(player->history.cumulativemissionrating > 40){
		strcpy(buf, "Failed Venture");
		strcpy(tempbuf, "No Recommendation At This Time");
	}
	else{
		strcpy(buf, "Abysmal Performance");
		strcpy(tempbuf, "Begin Sealing Procedures");
	}
	PrintFinalData("Final Analysis:", buf, false, 80, 100, 700, 2500);
	PrintFinalData("Recommendation:", tempbuf, true, 80, 100, 700, 2500);

}


int Captivity(PPLAYER player)
{
	//THIS CAN CREATE MAPS AND RUN THE GAME, BUT WILL ALWAYS END UP RETURNING THE OPERATIVE'S STATUS.
	return OPERATIVE_STATUS_DEADAFTERTORTURE;
}

void AnalyzeMissionEndCode(PPLAYER player, int RetVal)
{
	//IF THE PLAYER IS ALIVE AND WELL AND NOT TORTURED.....
	player->numberofdaysincycle--;
	if(player->numberofdaysincycle < 1){
		WrapUpMonth(player);
	}
	int numbermissions;
	double percent = 0.0;
	double percentincrement = 100;
	ShutDownSartuForTheDay(player, &globals.sartudata);
	numbermissions = GetNumberOfMissionsPerDay(player);
	SartuDailySession(player, numbermissions);
	InitializeRealisticWeaponAvailability(globals.globalweaponsammo.weaponlist, globals.globalweaponsammo.ammolist, globals.armorygizmolist);
	LoopOpConMusic(player);
	PrintDayAndMonth(player);

	//IF HE'S ALIVE BUT HAS BEEN TORTURED, GIVE HIM TIME OFF.

}