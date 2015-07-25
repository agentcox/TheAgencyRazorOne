#include "agencyinclude.h"


void playerselectdrugs(PPLAYER ps)
{
	int i;
	int choice;
	
	cls();
	LGREEN;
	bufferprint(20,"Level IV - Medical Systems - OPERATIONAL PHARMACEUTICALS\n");
	bufferprint(20,"DISTRIBUTION OF DRUGS TO SYRINGES \"ALPHA\", \"BETA\", \"GAMMA\".\n");
	LRED;
	bufferprint(20, "Press a Key To Begin");
	waitforkey();
	
	for (i=0; i < MAX_SYRINGES; i++)
	{
		cls();
		LBLUE;
		printf("\nPlease select drug for syringe ");
		if (i == SYRINGE_ALPHA){
			printf("\"ALPHA\":");
		}
		else if (i == SYRINGE_BETA){
			printf("\"BETA\":");
		}
		else{
			printf("\"GAMMA\":");
		}
		LYELLOW;
		printf("\n");
		printf("(1) - Oxytrichloride Methyl - Stimulant for Alertness\n");
		printf("(2) - Neurophsyical Corticadrenalin - Adrenalin for Reaction\n");
		printf("(3) - ReMed - K Restructurizer - Restructurizer for Wounds\n");
		printf("(4) - Atropine/Amyl Nitrate Mix - GP Antidotes for Toxic Exposure\n");
		
		for(;;)
		{
			LBLUE;
			printf("->");
			choice = getche();
			printf("\n");
			if (choice == '1'){
				LRED;
				ps->drugs.drugtype[i] = DRUG_STIMULANT;
				bufferprint(20,"Syringe Loaded With Oxytrichloride Methyl.\n");
				Sleep(1500);
				//	cls();
				break;
			}
			else if (choice == '2'){
				LRED;
				ps->drugs.drugtype[i] = DRUG_ADRENALIN;
				bufferprint(20,"Syringe Loaded With Neurophsyical Corticadrenalin.\n");
				Sleep(1500);
				//		cls();
				break;
			}
			else  if (choice == '3'){
				LRED;
				ps->drugs.drugtype[i] = DRUG_REMEDK;
				bufferprint(20,"Syringe Loaded With ReMed - K Restructurizer.\n");
				Sleep(1500);
				//	cls();
				break;
			}
			else if (choice == '4'){
				LRED;
				ps->drugs.drugtype[i] = DRUG_ANTIDOTE;
				bufferprint(20,"Syringe Loaded With Atropine/Amyl Nitrate Mix.\n");
				Sleep(1500);
				//	cls();
				break;
			}	
		}
	}
}

BOOL CheckAccessCode(PPLAYER ps)
{
	cls();
	int key;
	char* entercode = "ENTER YOUR FOUR-DIGIT ACCESS CODE";
	char* entercode2 = "ONE DIGIT AT A TIME";
	char* entercode3 = "CODE VALIDATED. AUTHORIZATION RECIEVED.";
	char* entercode4 = "PLEASE PRESS A KEY TO CONTINUE.";
	char* entercode5 = "CODE INCORRECT - CODE INCORRECT";
	char* entercode6 = "PLEASE RE-ENTER";
	
	int index = 0;
	int codekey[4];
	int finalcode = 0; 
	int i;
	char buf[1];
	int tries = 3;
	COORD graphicsize;
	COORD consize;
	COORD drawcoord;
	
	RED;
	consize = getconsize();
	graphicsize = getgraphicdimensions(globals.graphicslist, CODESCREEN_GRAPHIC_ID);
	
	drawcoord.Y = ((consize.Y / 2) - (graphicsize.Y / 2)) - 1;
	drawcoord.X = ((consize.X / 2) - (graphicsize.X / 2)) -1;
	
	printgraphic(globals.graphicslist, drawcoord, CODESCREEN_GRAPHIC_ID);
	LRED;
	setcursor(AC_TEXTBOXBOTTOMRIGHT_X / 2 - (strlen(entercode) / 3) , AC_TEXTBOXTOPLEFT_Y);
	printf("%s", entercode);
	setcursor(AC_TEXTBOXBOTTOMRIGHT_X / 2 -  (strlen(entercode2) / 2)/ 2, AC_TEXTBOXBOTTOMRIGHT_Y);
	printf("%s", entercode2);
	
	while (tries > 0)
	{
		index = 0;
		
		drawcoord.X = AC_INITIALNUMBERBLOCK_X;
		drawcoord.Y = AC_INITIALNUMBERBLOCK_Y;
		
		while (index < MAX_ACCESSCODE_DIGITS)
		{
			LGREEN;
			key = waitforkey();		buf[0] = key;
			buf[1] = '\0';
			
			if (key == '0' || key =='1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9'){
				codekey[index] = atoi(buf);
				
				LBLUE;
				
				DrawHugeNumber(codekey[index], drawcoord);
				twobeepsmod();
				index++;
				drawcoord.X += 16;
			}
			else{
				onebeepa();
			}
			
		}
		finalcode = codekey[3] + 10*codekey[2]  + 100*codekey[1] + 1000*codekey[0];
		if (finalcode == ps->accesscode){
			LGREEN;
			setcursor(AC_TEXTBOXTOPLEFT_X, AC_TEXTBOXTOPLEFT_Y);
			for(i = AC_TEXTBOXTOPLEFT_X; i < AC_TEXTBOXBOTTOMRIGHT_X; i++){
				printf(" ");
			}
			setcursor(AC_TEXTBOXTOPLEFT_X, AC_TEXTBOXTOPLEFT_Y + 1);
			for(i = AC_TEXTBOXTOPLEFT_X; i < AC_TEXTBOXBOTTOMRIGHT_X; i++){
				printf(" ");
			}
			setcursor(AC_TEXTBOXBOTTOMRIGHT_X / 2 - (strlen(entercode3) / 3) , AC_TEXTBOXTOPLEFT_Y);
			printf("%s", entercode3);
			setcursor(AC_TEXTBOXBOTTOMRIGHT_X / 2 -  (strlen(entercode4) / 2)/ 2, AC_TEXTBOXBOTTOMRIGHT_Y);
			printf("%s", entercode4);
			waitforkey();
			return SUCCESS;
			
		}
		else{
			
			tries --;
			
			LGREEN;
			setcursor(AC_TEXTBOXTOPLEFT_X, AC_TEXTBOXTOPLEFT_Y);
			for(i = AC_TEXTBOXTOPLEFT_X; i < AC_TEXTBOXBOTTOMRIGHT_X; i++){
				printf(" ");
			}
			setcursor(AC_TEXTBOXTOPLEFT_X, AC_TEXTBOXTOPLEFT_Y + 1);
			for(i = AC_TEXTBOXTOPLEFT_X; i < AC_TEXTBOXBOTTOMRIGHT_X; i++){
				printf(" ");
			}
			setcursor(AC_TEXTBOXBOTTOMRIGHT_X / 2 - (strlen(entercode5) / 3) , AC_TEXTBOXTOPLEFT_Y);
			printf("%s", entercode5);
			setcursor(AC_TEXTBOXBOTTOMRIGHT_X / 2 -  (strlen(entercode6) / 2)/ 2, AC_TEXTBOXBOTTOMRIGHT_Y);
			printf("%s", entercode6);
			
			drawcoord.X = AC_INITIALNUMBERBLOCK_X;
			drawcoord.Y = AC_INITIALNUMBERBLOCK_Y;
			setcursor(drawcoord.X, drawcoord.Y);
			
			for (index = 0; index < MAX_ACCESSCODE_DIGITS; index++){
				DrawHugeNumber(-1, drawcoord);
				drawcoord.X += 16;
			}
			
			
		}
	}
	return FAILURE;
}

void SetAccessCode(PPLAYER ps)
{
	cls();
	int key;
	char* entercode = "ENTER YOUR NEW FOUR-DIGIT ACCESS CODE";
	char* entercode2 = "ONE DIGIT AT A TIME";
	char* entercode3 = "CODE FINALIZED AND STORED. THANK YOU.";
	char* entercode4 = "PLEASE PRESS A KEY TO CONTINUE.";
	
	int index = 0;
	int codekey[4];
	int finalcode = 0; 
	int i;
	char buf[1];
	COORD drawcoord;
	COORD graphicsize;
	COORD consize;
	
	RED;
	consize = getconsize();
	graphicsize = getgraphicdimensions(globals.graphicslist, CODESCREEN_GRAPHIC_ID);
	
	drawcoord.Y = ((consize.Y / 2) - (graphicsize.Y / 2)) - 1;
	drawcoord.X = ((consize.X / 2) - (graphicsize.X / 2)) - 1;
	
	printgraphic(globals.graphicslist, drawcoord, CODESCREEN_GRAPHIC_ID);
	
	LRED;
	setcursor(AC_TEXTBOXBOTTOMRIGHT_X / 2 - (strlen(entercode) / 3) , AC_TEXTBOXTOPLEFT_Y);
	printf("%s", entercode);
	setcursor(AC_TEXTBOXBOTTOMRIGHT_X / 2 -  (strlen(entercode2) / 2)/ 2, AC_TEXTBOXBOTTOMRIGHT_Y);
	printf("%s", entercode2);
	
	drawcoord.X = AC_INITIALNUMBERBLOCK_X;
	drawcoord.Y = AC_INITIALNUMBERBLOCK_Y;
	
	while (index < MAX_ACCESSCODE_DIGITS)
	{
		LGREEN;
		key = waitforkey();		buf[0] = key;
		buf[1] = '\0';
		
		if (key == '0' || key =='1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9'){
			codekey[index] = atoi(buf);
			
			LGREEN;
			
			DrawHugeNumber(codekey[index], drawcoord);
			twobeepsmod();
			index++;
			drawcoord.X += 16;
		}
		else{
			onebeepa();
		}
	}
	finalcode = codekey[3] + 10*codekey[2]  + 100*codekey[1] + 1000*codekey[0];
	ps->accesscode = finalcode;
	
	
	//bufferprints(40, "\nCODE FINALIZED AND STORED. THANK YOU.", BEEPC);
	LGREEN;
	setcursor(AC_TEXTBOXTOPLEFT_X, AC_TEXTBOXTOPLEFT_Y);
	for(i = AC_TEXTBOXTOPLEFT_X; i < AC_TEXTBOXBOTTOMRIGHT_X; i++){
		printf(" ");
	}
	setcursor(AC_TEXTBOXTOPLEFT_X, AC_TEXTBOXTOPLEFT_Y + 1);
	for(i = AC_TEXTBOXTOPLEFT_X; i < AC_TEXTBOXBOTTOMRIGHT_X; i++){
		printf(" ");
	}
	setcursor(AC_TEXTBOXBOTTOMRIGHT_X / 2 - (strlen(entercode3) / 3) , AC_TEXTBOXTOPLEFT_Y);
	printf("%s", entercode3);
	setcursor(AC_TEXTBOXBOTTOMRIGHT_X / 2 -  (strlen(entercode4) / 2)/ 2, AC_TEXTBOXBOTTOMRIGHT_Y);
	printf("%s", entercode4);
	waitforkey();
}

void bufferprints(int speed, char* buffer, int snd)
{
	unsigned int i;
	
	for (i=0; i < (strlen(buffer)); i++)
	{
		printf("%c", buffer[i]);
		if (speed > 0)
			switch(snd)
		{
			case TYPEWRITER:
				typeclick();
				break;
			case BEEPA:
				onebeepa();
				break;
			case BEEPB:
				onebeepb();
				break;
				
			case BEEPC:
				onebeepc();
				break;
			default:
				break;
		}
		Sleep(speed);
	}
	return;
}

PROMPTRESPONSE DSAPlayerPrompt(char* question, int answertype)
{
	//#define PROMPTANSWER_YESNO 0
	//#define PROMPTANSWER_NUMBER 1
	//#define PROMPTANSWER_STRING 2
	//#define PROMPTANSWER_ANYKEY 3

#define PROMPTINPUTSTARTXCOORDINATEFROMLEFT	20
#define PROMPTINPUTSTARTYCOORDINATEFROMTOP	16
#define PROMPTTEXTWIDTH 47
#define PROMPTTEXTHEIGHT 15
	
	PROMPTRESPONSE response;
	COORD promptgraphicdrawcoords = {0,0};
	COORD randomnumberonecoord = {0, 2};
	COORD randomnumbertwocoord = {63, 21};
	COORD textdrawcoord = {17,5};
	COORD textdimensions = {PROMPTTEXTWIDTH - 1, PROMPTTEXTHEIGHT - 1};
	COORD promptresponsegraphicdrawcoords = {PROMPTINPUTSTARTXCOORDINATEFROMLEFT -1 , PROMPTINPUTSTARTYCOORDINATEFROMTOP -1};
	COORD yesnocoord;
	int numberofcharstyped = 0;
	int key;
	char numberresponsestring[MAX_RESPONSE_CHARS];
	
	memset(&response, 0, sizeof(PROMPTRESPONSE));
	cls();
	zerocursor();
	GREEN;
	printgraphic(globals.graphicslist, promptgraphicdrawcoords, PROMPT_GRAPHIC_ID);
	GREEN;
	printgraphic(globals.graphicslist, promptresponsegraphicdrawcoords, PROMPTRESPONSE_GRAPHIC_ID);
	Sleep(80);
	LGREEN;
	printgraphic(globals.graphicslist, promptresponsegraphicdrawcoords, PROMPTRESPONSE_GRAPHIC_ID);
	setcursor(textdrawcoord.X, textdrawcoord.Y);
	GREEN;
	printwordwrapcoordinate(question, textdimensions, textdrawcoord);
	Sleep(80);
	LGREEN;
	printwordwrapcoordinate(question, textdimensions, textdrawcoord);
	
	
	/************************************************************
	; YES/NO BLOCK
	************************************************************/
	
	if (answertype == PROMPTANSWER_YESNO){
		response.typeofresponse = PROMPTANSWER_YESNO;
		LRED;
		yesnocoord.X = retrieveTextCenterH("Y/N");
		yesnocoord.Y = PROMPTINPUTSTARTYCOORDINATEFROMTOP;
		setcursor(yesnocoord.X, yesnocoord.Y);
		printf("Y/N");
		
		for(;;)
		{
			key = waitforkey();
		

					
					if (key == 'y' || key == 'Y' || key == 'n' || key == 'N'){
						if (key == 'y' || key == 'Y'){
							response.yesnoresponse = TRUE;
							FLUSH;
							return response;
						}
						else if (key == 'n' || key == 'N'){
							response.yesnoresponse = FALSE;
							FLUSH;
							return response;
						}
				
			}
		
			
			
			//DO THE RANDOM NUMBER THING.
		}
	}
	
	/************************************************************
	; END OF YES/NO BLOCK
	************************************************************/
	

	/************************************************************
	; NUMERICAL BLOCK
	************************************************************/
		else if (answertype == PROMPTANSWER_NUMBER){
		response.typeofresponse = PROMPTANSWER_NUMBER;
				
		for(;;)
		{
			key = waitforkey();

					if (key == VK_BACK){
						numberofcharstyped--;
						setcursor(PROMPTINPUTSTARTXCOORDINATEFROMLEFT + numberofcharstyped, PROMPTINPUTSTARTYCOORDINATEFROMTOP);
						RED;
						printf("%c", numberresponsestring[numberofcharstyped]);
						setcursor(PROMPTINPUTSTARTXCOORDINATEFROMLEFT + numberofcharstyped, PROMPTINPUTSTARTYCOORDINATEFROMTOP);
						Sleep(30);
						printf(" ");
						setcursor(PROMPTINPUTSTARTXCOORDINATEFROMLEFT + numberofcharstyped, PROMPTINPUTSTARTYCOORDINATEFROMTOP);
					}
					else if (isdigit(key) && numberofcharstyped < MAX_RESPONSE_CHARS - 1){
						LRED;
						setcursor(PROMPTINPUTSTARTXCOORDINATEFROMLEFT + numberofcharstyped, PROMPTINPUTSTARTYCOORDINATEFROMTOP);
						RED;
						printf("%c", key);
						setcursor(PROMPTINPUTSTARTXCOORDINATEFROMLEFT + numberofcharstyped, PROMPTINPUTSTARTYCOORDINATEFROMTOP);
						Sleep(30);
						LRED;
						printf("%c", key);
						numberresponsestring[numberofcharstyped] = key;
						numberofcharstyped++;
					}
					else if (key == VK_RETURN && numberofcharstyped > 0){
						numberresponsestring[numberofcharstyped] = '\0';
						response.numberresponse = atoi(numberresponsestring);
						FLUSH;
						return response;
					}
					FLUSH;
			
			
			//DO THE RANDOM NUMBER THING.
		}
	}


		/************************************************************
		; END NUMBER BLOCK
		************************************************************/


		/************************************************************
		; BEGIN STRING BLOCK
		************************************************************/
		else if (answertype == PROMPTANSWER_STRING){
		response.typeofresponse = PROMPTANSWER_STRING;
				
		for(;;)
		{
			key = waitforkey();
					
					if (key == VK_BACK){
						numberofcharstyped--;
						setcursor(PROMPTINPUTSTARTXCOORDINATEFROMLEFT + numberofcharstyped, PROMPTINPUTSTARTYCOORDINATEFROMTOP);
						RED;
						printf("%c", numberresponsestring[numberofcharstyped]);
						setcursor(PROMPTINPUTSTARTXCOORDINATEFROMLEFT + numberofcharstyped, PROMPTINPUTSTARTYCOORDINATEFROMTOP);
						Sleep(30);
						printf(" ");
						setcursor(PROMPTINPUTSTARTXCOORDINATEFROMLEFT + numberofcharstyped, PROMPTINPUTSTARTYCOORDINATEFROMTOP);
					}
					else if (isprint(key) && numberofcharstyped < MAX_RESPONSE_CHARS - 1){
						LRED;
						setcursor(PROMPTINPUTSTARTXCOORDINATEFROMLEFT + numberofcharstyped, PROMPTINPUTSTARTYCOORDINATEFROMTOP);
						RED;
						printf("%c", key);
						setcursor(PROMPTINPUTSTARTXCOORDINATEFROMLEFT + numberofcharstyped, PROMPTINPUTSTARTYCOORDINATEFROMTOP);
						Sleep(30);
						LRED;
						printf("%c", key);
						numberresponsestring[numberofcharstyped] = key;
						numberofcharstyped++;
					}
					else if (key == VK_RETURN && numberofcharstyped > 0){
						numberresponsestring[numberofcharstyped] = '\0';
						strcpy(response.textresponse, numberresponsestring);
						FLUSH;
						return response;
					}
					FLUSH;
	
		}
		}
		/************************************************************
		; END STRING BLOCK
		************************************************************/

			/************************************************************
	; YES/NO BLOCK
	************************************************************/
	
	if (answertype == PROMPTANSWER_ANYKEY){
		response.typeofresponse = PROMPTANSWER_ANYKEY;
		LRED;
		yesnocoord.X = retrieveTextCenterH("PRESS A KEY TO CONTINUE");
		yesnocoord.Y = PROMPTINPUTSTARTYCOORDINATEFROMTOP;
		setcursor(yesnocoord.X, yesnocoord.Y);
		printf("PRESS A KEY TO CONTINUE");
		
		for(;;)
		{
			key = waitforkey();
		
							return response;
		}
	}
	
	/************************************************************
	; END OF YES/NO BLOCK
	************************************************************/


	FLUSH;
		return response;
	
}