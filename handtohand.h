//END RESULTS
#define MELEE_CONTINUE						0
#define MELEE_END_PLAYERBLUNTUNCONSCIOUS	1
#define MELEE_END_PLAYERBLOODLOSSDEAD		2
#define MELEE_END_PLAYERSHOTDEAD			3
#define MELEE_END_PLAYERSTRANGLEDDEAD		4
#define MELEE_END_PLAYERDRUGDEAD			5
#define MELEE_END_PLAYERDRUGUNCONSCIOUS		6
#define MELEE_END_ENEMYBLUNTUNCONSCIOUS		7
#define MELEE_END_ENEMYBLOODLOSSDEAD		8
#define MELEE_END_ENEMYSHOTDEAD				9
#define MELEE_END_ENEMYSTRANGLEDDEAD		10
#define MELEE_END_ENEMYDRUGDEAD				11
#define MELEE_END_ENEMYDRUGUNCONSCIOUS		12
#define MELEE_END_PLAYERCYANIDE	13

//GRAPHIC PLACEMENT
#define MELEE_HELPBOX_WIDTH			80
#define MELEE_HELPBOX_HEIGHT		2
#define MELEE_HELPBOX_TOPLEFT_X		0
#define MELEE_HELPBOX_TOPLEFT_Y		19

#define MELEE_TEXTBOX_WIDTH			53
#define MELEE_TEXTBOX_HEIGHT		2
#define MELEE_TEXTBOX_TOPLEFT_X		13
#define MELEE_TEXTBOX_TOPLEFT_Y		8

#define MELEE_TIMEBOX_WIDTH			9
#define MELEE_TIMEBOX_HEIGHT		1
#define MELEE_TIMEBOX_TOPLEFT_X		13
#define MELEE_TIMEBOX_TOPLEFT_Y		11

#define MELEE_PLAYERBOX_WIDTH		18
#define MELEE_PLAYERBOX_HEIGHT		4
#define MELEE_PLAYERBOX_TOPLEFT_X	59
#define MELEE_PLAYERBOX_TOPLEFT_Y	13

#define MELEE_ENEMYBOX_WIDTH		18
#define MELEE_ENEMYBOX_HEIGHT		4
#define MELEE_ENEMYBOX_TOPLEFT_X	5
#define MELEE_ENEMYBOX_TOPLEFT_Y	1

#define MELEE_WEAPONBOX_TOPLEFT_X	13
#define MELEE_WEAPONBOX_TOPLEFT_Y	16
#define MELEE_WEAPONBOX_WIDTH		19
#define MELEE_WEAPONBOX_HEIGHT		1

#define MELEE_MOVEBOX_TOPLEFT_X		37
#define MELEE_MOVEBOX_TOPLEFT_Y		16
#define MELEE_MOVEBOX_WIDTH			19
#define MELEE_MOVEBOX_HEIGHT		1

//TIME SYSTEM
#define MELEE_SPLITPERDISTANCEUNIT		6
#define MELEE_CONSTANTSPLITADDITIONAL	20
#define MELEE_ATTACKBONUSSPLIT			45

//POSTURE
#define MELEE_POSTURE_STANDING	0
#define MELEE_POSTURE_KNEELING	1
#define MELEE_POSTURE_PRONE		2

//DISTANCE
#define	MELEE_MAX_DISTANCE	20
#define	MELEE_MIN_DISTANCE	2

//TRAINING WEAPONS
#define MELEE_MAX_TRAINING_WEAPONS 7

//ACTIONS
#define MELEE_NO_ACTION		0
#define MELEE_VALID_ACTION	1

#define MELEE_MAX_CYCLE_ACTIONS 6

#define MELEE_ACTION_NONE				0
#define MELEE_ACTION_PUNCH				1	//THIS IS A CYCLE-ACTION
#define MELEE_ACTION_KICK				2	//THIS IS A CYCLE-ACTION
#define MELEE_ACTION_FIREWEAPON			3	//THIS IS A CYCLE-ACTION
#define MELEE_ACTION_CUT				4	//THIS IS A CYCLE-ACTION
#define MELEE_ACTION_HITWITHWEAPON		5	//THIS IS A CYCLE-ACTION
#define MELEE_ACTION_THROWWEAPON		6	//THIS IS A CYCLE-ACTION
#define MELEE_ACTION_STRANGLE			7
#define MELEE_ACTION_DRUGDEATH			8
#define MELEE_ACTION_DRUGUNCONSCIOUS	9
#define MELEE_ACTION_CYANIDE			10
#define MELEE_ACTION_CLOSEDISTANCE		11
#define MELEE_ACTION_GAINDISTANCE		12
#define MELEE_ACTION_DRAWWEAPON			13
#define MELEE_ACTION_CHANGEMOVE			14	//THIS CYCLES THE CYCLE-ACTIONS
#define MELEE_ACTION_ATTACK				15 //THIS IS FOR ATTACK ANALYSIS, ALWAYS CHANGES INTO SOMETHING ELSE.

//TARGETS
#define MELEE_TARGET_HEAD		7
#define MELEE_TARGET_TORSO		4
#define MELEE_TARGET_ABDOMEN	1
#define MELEE_TARGET_LSHOULDER	8
#define MELEE_TARGET_RSHOULDER	6
#define MELEE_TARGET_LLEG		2
#define MELEE_TARGET_RLEG		0
#define MELEE_TARGET_LSIDE		5
#define MELEE_TARGET_RSIDE		3


//ATTACK CALCULATION TABLES
//PUNCHES------------------------------------------------
#define MELEE_PUNCH_HITMISS_BASE_SUCCESS			80.0
#define MELEE_PUNCH_HITMISS_DISTANCE_MODIFIER		2.0
#define MELEE_PUNCH_HITMISS_SKILL_MODIFIER			3.0

#define MELEE_PUNCH_BLOCK_MODIFIER	20.0

#define MELEE_PUNCH_HITDAMAGE_BASE_MINIMUM			3.0
#define MELEE_PUNCH_HITDAMAGE_BASE_MAXIMUM			7.0
#define MELEE_PUNCH_HITDAMAGE_DISTANCE_MODIFIER		0.15
#define MELEE_PUNCH_HITDAMAGE_AGGRESSION_MODIFIER	0.4
#define MELEE_PUNCH_HITDAMAGE_SKILL_MODIFIER		0.7

#define MELEE_PUNCH_SPECIALHIT_BASE_SUCCESS			5.0
#define MELEE_PUNCH_SPECIALHIT_DISTANCE_MODIFIER	0.5
#define MELEE_PUNCH_SPECIALHIT_SKILL_MODIFIER		2.0
#define	MELEE_PUNCH_SPECIALHIT_DAMAGE				25

#define MELEE_PUNCH_OPENWOUND_BONUS					8

#define MELEE_PUNCH_TURNAROUND_BASE_SUCCESS			20
#define MELEE_PUNCH_TURNAROUND_DISTANCE_MODIFIER	0.5
#define MELEE_PUNCH_TURNAROUND_REFLEX_MODIFIER		4.0

#define MELEE_PUNCH_KNOCKDOWN_BASE_SUCCESS			25.0
#define MELEE_PUNCH_KNOCKDOWN_AGGRESSION_MODIFIER	2.5
//KICKS------------------------------------------------
#define MELEE_KICK_HITMISS_BASE_SUCCESS				70.0
#define MELEE_KICK_HITMISS_DISTANCE_MODIFIER		0.75
#define MELEE_KICK_HITMISS_SKILL_MODIFIER			3.0

#define MELEE_KICK_HITDAMAGE_BASE_MINIMUM			6.0
#define MELEE_KICK_HITDAMAGE_BASE_MAXIMUM			10.0
#define MELEE_KICK_HITDAMAGE_DISTANCE_MODIFIER		0.15
#define MELEE_KICK_HITDAMAGE_AGGRESSION_MODIFIER	0.4
#define MELEE_KICK_HITDAMAGE_SKILL_MODIFIER			1.0

#define MELEE_KICK_SPECIALHIT_BASE_SUCCESS			15.0
#define MELEE_KICK_SPECIALHIT_DISTANCE_MODIFIER		0.5
#define MELEE_KICK_SPECIALHIT_SKILL_MODIFIER		1.0
#define	MELEE_KICK_SPECIALHIT_DAMAGE				35

#define MELEE_KICK_OPENWOUND_BONUS					12

#define MELEE_KICK_TURNAROUND_BASE_SUCCESS			30
#define MELEE_KICK_TURNAROUND_DISTANCE_MODIFIER		1
#define MELEE_KICK_TURNAROUND_REFLEX_MODIFIER		4.0

#define MELEE_KICK_KNOCKDOWN_BASE_SUCCESS			35.0
#define MELEE_KICK_KNOCKDOWN_AGGRESSION_MODIFIER	2.5

//HIT WITH WEAPONS--------------------------
#define MELEE_HITWITHWEAPON_HITMISS_BASE_SUCCESS				55.0
#define MELEE_HITWITHWEAPON_HITMISS_DISTANCE_MODIFIER		0.75
#define MELEE_HITWITHWEAPON_HITMISS_SKILL_MODIFIER			3.0

#define MELEE_HITWITHWEAPON_HITDAMAGE_BASE_MINIMUM			6.0
#define MELEE_HITWITHWEAPON_HITDAMAGE_BASE_MAXIMUM			18.0
#define MELEE_HITWITHWEAPON_HITDAMAGE_DISTANCE_MODIFIER		0.15
#define MELEE_HITWITHWEAPON_HITDAMAGE_AGGRESSION_MODIFIER	0.4
#define MELEE_HITWITHWEAPON_HITDAMAGE_SKILL_MODIFIER			0.5
#define MELEE_HITWITHWEAPON_HITDAMAGE_WEIGHT_MODIFIER			0.5

#define MELEE_HITWITHWEAPON_SPECIALHIT_BASE_SUCCESS			10.0
#define MELEE_HITWITHWEAPON_SPECIALHIT_DISTANCE_MODIFIER		0.5
#define MELEE_HITWITHWEAPON_SPECIALHIT_SKILL_MODIFIER		1.0
#define	MELEE_HITWITHWEAPON_SPECIALHIT_DAMAGE				45

#define MELEE_HITWITHWEAPON_OPENWOUND_BONUS					15

#define MELEE_HITWITHWEAPON_TURNAROUND_BASE_SUCCESS			45
#define MELEE_HITWITHWEAPON_TURNAROUND_DISTANCE_MODIFIER		1
#define MELEE_HITWITHWEAPON_TURNAROUND_REFLEX_MODIFIER		4.0

#define MELEE_HITWITHWEAPON_KNOCKDOWN_BASE_SUCCESS			45.0
#define MELEE_HITWITHWEAPON_KNOCKDOWN_AGGRESSION_MODIFIER	2.5
//STAB WITH WEAPON------------------------------------------------------------
#define MELEE_CUT_HITMISS_BASE_SUCCESS				48.0
#define MELEE_CUT_HITMISS_DISTANCE_MODIFIER		0.75
#define MELEE_CUT_HITMISS_SKILL_MODIFIER			3.0

#define MELEE_CUT_HITDAMAGE_BASE_MINIMUM			12.0
#define MELEE_CUT_HITDAMAGE_BASE_MAXIMUM			30.0
#define MELEE_CUT_HITDAMAGE_DISTANCE_MODIFIER		0.15
#define MELEE_CUT_HITDAMAGE_AGGRESSION_MODIFIER	0.4
#define MELEE_CUT_HITDAMAGE_SKILL_MODIFIER			0.5
#define MELEE_CUT_HITDAMAGE_WEIGHT_MODIFIER			0.5

#define MELEE_CUT_SPECIALHIT_BASE_SUCCESS			10.0
#define MELEE_CUT_SPECIALHIT_DISTANCE_MODIFIER		0.5
#define MELEE_CUT_SPECIALHIT_SKILL_MODIFIER		1.0
#define	MELEE_CUT_SPECIALHIT_DAMAGE				45

#define MELEE_CUT_OPENWOUND_BONUS					15

#define MELEE_CUT_TURNAROUND_BASE_SUCCESS			48.0
#define MELEE_CUT_TURNAROUND_DISTANCE_MODIFIER		1.0
#define MELEE_CUT_TURNAROUND_REFLEX_MODIFIER		4.0

#define MELEE_CUT_KNOCKDOWN_BASE_SUCCESS			45.0
#define MELEE_CUT_KNOCKDOWN_AGGRESSION_MODIFIER	2.5

//THROWING WEAPONS
#define MELEE_THROW_HITMISS_BASE_SUCCESS	15.0
#define MELEE_THROW_BLUNTWEAPON_SPECIALHITDAMAGE	40
#define MELEE_THROW_CUTTINGWEAPON_SPECIALHITDAMAGE 60
#define MELEE_THROW_OPENWOUND_BONUS							15
#define	MELEE_THROW_HITMISS_DISTANCE_MODIFIER	10.0
#define	MELEE_THROW_HITMISS_STEALTHCOMBAT_MODIFIER	20.0
#define	MELEE_THROW_HITMISS_ARMEDCOMBAT_MODIFIER	20.0
#define	MELEE_THROW_HITMISS_SPECIALIZATION_MODIFIER	5.0
#define MELEE_THROW_BLOCK_MODIFIER	20.0
#define MELEE_THROW_SPECIALHIT_BASE_SUCCESS	5.0
#define MELEE_THROW_SPECIALHIT_DISTANCE_MODIFIER	20.0
#define MELEE_THROW_SPECIALHIT_STEALTHCOMBAT_MODIFIER	30.0
#define MELEE_THROW_KNOCKDOWN_CUTTINGWEAPON_CHANCE	10
#define MELEE_THROW_KNOCKDOWN_BLUNTWEAPON_CHANCE	30
#define MELEE_THROW_CUTTINGWEAPON_MINDAMAGE	10
#define MELEE_THROW_CUTTINGWEAPON_MAXDAMAGE	30
#define MELEE_THROW_BLUNTWEAPON_MINDAMAGE	8
#define MELEE_THROW_BLUNTWEAPON_MAXDAMAGE	20

//FIRING WEAPONS--------------------------
#define MELEE_SHOOT_HITMISS_BASE_SUCCESS	30.0


//SPECIAL DAMAGE SCALARS AND CHANCES
#define MELEE_HEAD_DAMAGE_SCALAR	1.25
#define MELEE_LEG_DAMAGE_SCALAR			0.75
#define MELEE_SHOULDER_DAMAGE_SCALAR	0.65
#define MELEE_KICK_LEG_SPECIAL_CHANCE		40
#define MELEE_KICK_SHOULDER_SPECIAL_CHANCE	50
#define MELEE_PUNCH_LEG_SPECIAL_CHANCE		25
#define MELEE_PUNCH_SHOULDER_SPECIAL_CHANCE	35


//TURNAROUNDS
#define MELEE_TURNAROUND_DAMAGE				15

//DAMAGE CONVERSION
#define MELEE_SHOT_CONVERSION_PERCENTAGE	7
#define MELEE_CUT_CONVERSION_PERCENTAGE		3

//SOUNDS!
#define MELEE_SOUND_SPECIALHIT	"sounds\\hits\\specialhit.wav"
#define MELEE_SOUND_HIT1		"sounds\\hits\\meleehit1.wav"
#define MELEE_SOUND_HIT2		"sounds\\hits\\meleehit2.wav"
#define MELEE_SOUND_HIT3		"sounds\\hits\\meleehit3.wav"
#define MELEE_SOUND_HIT4		"sounds\\hits\\meleehit4.wav"
#define MELEE_SOUND_HIT5		"sounds\\hits\\meleehit5.wav"
#define MELEE_SOUND_MISS1	"sounds\\common\\miss1.wav"
#define MELEE_SOUND_MISS2	"sounds\\common\\miss2.wav"
#define MELEE_SOUND_MISS3	"sounds\\common\\miss3.wav"
#define MELEE_SOUND_FALLDOWN	"sounds\\hits\\splat2.wav"
#define MELEE_SOUND_STRANGLEDEATH	"sounds\\hits\\strangletry.wav"
#define MELEE_SOUND_STRANGLEATTEMPT	"sounds\\hits\\strangle.wav"
#define MELEE_SOUND_HITWITHWEAPON	"sounds\\hits\\hitwithweapon.wav"
#define MELEE_SOUND_CATCHPUNCH	"sounds\\hits\\catchpunch.wav"
#define MELEE_SOUND_WATCH	"sounds\\gizmos\\watch.wav"
#define MELEE_SOUND_RING	"sounds\\gizmos\\ring.wav"
#define MELEE_SOUND_SCREAM "sounds\\hits\\baddeath.wav"
#define MELEE_SOUND_STAB	"sounds\\hits\\stab.wav"
#define MELEE_SOUND_KNIFETHROW	"sounds\\weapons\\knifethrow.wav"
#define MELEE_SOUND_SPECIALSTAB	"sounds\\hits\\specialstab.wav"
#define MELEE_SOUND_REMOVEKNIFE	"sounds\\hits\\pulloutknife.wav"
#define MELEE_SOUND_DRUGATTEMPT "sounds\\gizmos\\syringe.wav"
#define MELEE_SOUND_DRUGDEATH	"sounds\\hits\\drugdeath.wav"

//AI DEFINES
#define MELEEAI_SHOOTPLAYER_CRITICAL	0
#define MELEEAI_SHOOTPLAYER_ANYWHERE	1
#define MELEEAI_SHOOTPLAYER_DISABLE			2
#define MELEEAI_WEAPON_THROW_CRITICAL		3
#define MELEEAI_WEAPON_THROW_ANYWHERE	4
#define MELEEAI_WEAPON_THROW_DISABLE		5
#define MELEEAI_WEAPON_ATTACK_CRITICAL		6
#define MELEEAI_WEAPON_ATTACK_ANYWHERE		7
#define MELEEAI_WEAPON_ATTACK_DISABLE		8
#define MELEEAI_DRAWKNIFE									9
#define MELEEAI_DRAWGUN										10
#define MELEEAI_DRAWSTANDARD					11
#define MELEEAI_STRANGLE_PLAYER					12
#define MELEEAI_PUNCHPLAYER_CRITICAL	13
#define MELEEAI_PUNCHPLAYER_ANYWHERE	14
#define MELEEAI_PUNCHPLAYER_DISABLE			15
#define MELEEAI_KICKPLAYER_CRITICAL	16
#define MELEEAI_KICKPLAYER_ANYWHERE	17
#define MELEEAI_KICKPLAYER_DISABLE			18
#define MELEEAI_CLOSEDISTANCE						19
#define MELEEAI_GAINDISTANCE						20
#define MELEEAI_DRAWANY									21
#define MELEEAI_PUNCHPLAYER_WOUND		22
#define MELEEAI_KICKPLAYER_WOUND		23

//PREDEFINED STRINGS
//#define MELEE_ATTACK_HELP_STRING "[KEYPAD KEYS - ATTACK BODY PART] [(ENTER) - CHANGE ATTACK MODE] [(+,-) GAIN/CLOSE DISTANCE] [(0) DRAW WEAPON] [(/) DRUG] [(*) STRANGLE OPPONENT] [(C) CYANIDE SELF]"
#define MELEE_ATTACK_HELP_STRING "[KEYPAD KEYS - ATTACK BODY PART]  [(+,-) GAIN/CLOSE DISTANCE] [(ENTER) - CHANGE ATTACK MODE]  [(INS) - DRAW WEAPON]  [(*) - STRANGLE]"
#define MELEE_BLOCK_HELP_STRING		"[KEYPAD KEYS - BLOCK ATTACK ON BODY PART]"

//FUNCTIONS
void MeleeCalculateTimeToTurn(int distance, PMISSIONCHRONO timetoturn);
void MeleeMakeStatusString(char* string, PMELEESTRUCT ms, PWEAPON weapon, BOOL player);
void MeleeTimeToMeleeTurn(int distance, PMISSIONCHRONO timetoturn);
void MeleePrintActionString(char* string);
void MeleePrintActionStringHold(char* string);
void MeleePrintTimeString();
void MeleeStartTimer();
void MeleeCycleTurn();
int MeleeResolveDeath(PPLAYER player, PENEMY enemy);
int MeleeGetPlayerMove(PPLAYER player, int key);
void MeleeResolvePlayerOffensiveAction(PPLAYER player, PENEMY enemy);
BOOL MeleeResolvePlayerPostures(PPLAYER player);
void MeleePrintPlayerandEnemyStats(PPLAYER player, PENEMY enemy);
void MeleePlayerChangeMove(PPLAYER player);
void MeleePrintStringInHelpBox(char* string);
BOOL MeleeCanPlayerPerformMove(PPLAYER player, int move);
void MeleePrintCycleMove(PPLAYER player, int move);
void MeleePrintDistance();
void MeleeModifyPlayerOffensiveAction(PPLAYER player);
void MeleeCalculatePlayerAttack(int attacktype, int location, PPLAYER player, PENEMY enemy);
int MeleeCalculatePunch(int location, bool gizmo, bool blocked, PPLAYER player, PENEMY enemy, bool playerattack); //WORKS FOR ENEMIES AND PLAYER
int MeleeCalculateKick(int location, bool correctlyblocked, PPLAYER player, PENEMY enemy, bool playerattack); //WORKS FOR ENEMIES AND PLAYER
void MeleeCalculateStrangle(PPLAYER player, PENEMY enemy, bool playerattack); //WORKS FOR ENEMIES AND PLAYER
void MeleeCalculateFirearmAttack(int location, PPLAYER player, PENEMY enemy, BOOL playerattack); //WORKS FOR ENEMIES AND PLAYER
int MeleeCalculateHitWithWeapon(int location, bool correctlyblocked, PPLAYER player, PENEMY enemy, bool playerattack); //WORKS FOR ENEMIES AND PLAYER
int MeleeCalculateCutWithWeapon(int location, bool correctlyblocked, PPLAYER player, PENEMY enemy, bool playerattack); //WORKS FOR ENEMIES AND PLAYER
int MeleeCalculateThrowWeapon(int location, bool correctlyblocked, PPLAYER player, PENEMY enemy, bool playerattack); //WORKS FOR ENEMIES AND PLAYER
void MeleeCalculateDrug(int location,  bool correctlyblocked, PPLAYER player, PENEMY enemy, bool playerattack); //WORKS FOR ENEMIES AND PLAYER
void MeleePlayRandomHitSoundSync();
void MeleePlayRandomHitSoundAsync();
void MeleePlayRandomMissSoundAsync();
void MeleePlayRandomMissSoundSync();
void MeleeSpecialHitDamageCorrection(PPLAYER player, PENEMY enemy, int damage, BOOL playerattack);
void MeleeConvertPlayerDamageAtEndOfCombat(PPLAYER player);
void MeleeCombatRedraw(PPLAYER player, PENEMY enemy);
bool MeleePlayerDrawWeapon(PPLAYER player);
void MeleePlayerDropWeapon(PENEMY enemy);
void MeleeEnemyDropWeapon(PENEMY enemy);
int MeleeGetGunShotHitChance(PWEAPON weapon, PPSYCHSKILLSTRUCT firerpsych, PPSYCHSKILLSTRUCT targetpsych);
bool MeleeGetGunShotKnockdown(PROUND round, PPSYCHSKILLSTRUCT firerpsych, PPSYCHSKILLSTRUCT targetpsych);
int MeleeGetGunShotDamage(PROUND round, PPSYCHSKILLSTRUCT firerpsych, PPSYCHSKILLSTRUCT targetpsych);
int MeleeGetGunShotHitChance(PWEAPON weapon, PPSYCHSKILLSTRUCT firerpsych, PPSYCHSKILLSTRUCT targetpsych, int targetstance);
void MeleeTrainingMessage(int result);
void MeleePrintDebugNumber(char* name, int number);
bool MeleeCustomScenario(PPLAYER player, PENEMY enemy);
int MeleeTrainingSelectSubAmmoTypeForWeapon(PWEAPON weapon);
int MeleeTrainingGetNumAvailWeapons(PPLAYER player);
bool MeleeTrainingSelectWeapon(PPLAYER player, PLIST weaponlist, PPLAYERAMMO ammostruct);

//AI FUNCTIONS
int MeleeAIGetCurrentPriority(PPLAYER player, PENEMY enemy);
void MeleeCalculateEnemyAttack(int attacktype, int location, bool correctlyblocked, PPLAYER player, PENEMY enemy);
int MeleeResolveEnemyPostures(PENEMY enemy);
void MeleeGetEnemyMoveOffensive(PPLAYER player, PENEMY enemy);
void MeleeGetEnemyMoveDefensive(PENEMY enemy);
void MeleeResolveEnemyOffensiveAction(PENEMY enemy, PPLAYER player);
void MeleeAIRaiseHonor();
void MeleeAIDropHonor();
void MeleeAIRaiseAnger();
void MeleeAIDropAnger();
bool HasMeleeAmmunition(PWEAPON weapon);
bool MeleeHasAnyAcceptableWeapons(PLIST weaponlist, int attacktype);
bool MeleeCurrentWeaponHasAction(PWEAPON weapon, int attacktype);
void MeleeAIActionReferenceResolution(PENEMY enemy, int actionref);
int MeleeGetCriticalBodyPart();
int MeleeGetDisableBodyPart();
void MeleeAIPrepareToDrawSuitableWeapon(PENEMY enemy, int attacktype);
void MeleeCreatePoorOpponent(PENEMY enemy);
void MeleeCreateAverageOpponent(PENEMY enemy);
void MeleeCreateSkilledOpponent(PENEMY enemy);
void MeleeCreateEliteOpponent(PENEMY enemy);
