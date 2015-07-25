#define ST_WEAPONSTITLEX 21
#define ST_WEAPONSTITLEY 1

#define ST_PLAYERSTATTITLEX 57
#define ST_PLAYERSTATTITLEY 1

#define ST_GIZMOSTITLEX 21
#define ST_GIZMOSTITLEY 9

#define ST_SYRINGETITLEX 56
#define ST_SYRINGETITLEY 14

#define ST_USEX 12
#define ST_USEY 21

#define ST_DROPX 29
#define ST_DROPY 21

#define ST_CHECKX 49
#define ST_CHECKY 21

#define ST_SELECTWEAPX 8
#define ST_SELECTWEAPY 22

#define ST_SELECTSTATUSX 43
#define ST_SELECTSTATUSY 22

SetConsoleTextAttribute(hOutput, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			setcursor(ST_WEAPONSTITLEX, ST_WEAPONSTITLEY);
			printf("WEAPONS");
			setcursor(ST_PLAYERSTATTITLEX, ST_PLAYERSTATTITLEY);
			printf("PLAYER STATUS");
			setcursor(ST_GIZMOSTITLEX, ST_GIZMOSTITLEY);
			printf("GIZMOS");
			setcursor(ST_SYRINGETITLEX, ST_SYRINGETITLEY);
			printf("SYRINGE STATUS");
			setcursor(ST_USEX, ST_USEY);
			printf("(U) - Use");
			setcursor(ST_DROPX, ST_DROPY);
			printf("(D) - Drop");
			setcursor(ST_CHECKX, ST_CHECKY);
			printf("(C) - Check Nearby");
			setcursor(ST_SELECTWEAPX, ST_SELECTWEAPY);
			printf("(W) - Select As New Weapon");
			setcursor(ST_SELECTSTATUSX, ST_SELECTSTATUSY);
			printf("(S) - Selected Weapon Status");