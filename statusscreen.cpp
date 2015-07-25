#include "agencyinclude.h"
#include "agencystatusgraph.h"

/* TheDraw C Ascii Screen Image.  Date: 02/04/00 */


char choice;
char itemid;
//  COORD cursor;
COORD lastarrow = {0,0};
COORD currentarrow = {0,0};


void statusscreen()
{
	int retval;

	LoopSong(globals.musiclist.songs[INVENTORY_SONG]);
    LRED;
    writestring((char*)STATUSSCREEN);
    zerocursor();

	LYELLOW;
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
    
    for(;;)
    {
      retval = chooseslot();
	  if (retval == RET_EXIT){
		  return;
	  }
    }
}

void swaparrows()
{
    lastarrow.X = currentarrow.X;
    lastarrow.Y = currentarrow.Y;
    
    if (lastarrow.X && lastarrow.Y)
    {
        setcursor(lastarrow.X, lastarrow.Y);
        printf(" ");
    }
}

void setarrow(int xcoord, int ycoord)
{
    
    currentarrow.X = xcoord;
    currentarrow.Y = ycoord;
    setcursor(currentarrow.X, currentarrow.Y);
    LGREEN;
    printf("%c", ARROWCHAR);
}

int chooseslot()
{
	  FLUSH;
        choice = waitforkey();
        
        switch (choice)
        {
        case '1':
            swaparrows();
            setarrow(ST_WEAPXCOORD, ST_WEAPYCOORD1);
            itemid = ST_IDWEAPON1;
            break;
        case '2':
            swaparrows();
            setarrow(ST_WEAPXCOORD, ST_WEAPYCOORD2);
            itemid = ST_IDWEAPON2;
            break;
        case '3':
            swaparrows();
            setarrow(ST_WEAPXCOORD, ST_WEAPYCOORD3);
            itemid = ST_IDWEAPON3;
            break;
        case '4':
            swaparrows();
            setarrow(ST_WEAPXCOORD, ST_WEAPYCOORD4);
            itemid = ST_IDWEAPON4;
            break;
        case '5':
            swaparrows();
            setarrow(ST_WEAPXCOORD, ST_WEAPYCOORD5);
            itemid = ST_IDWEAPON5;
            break;
        case '6':
            swaparrows();
            setarrow(ST_WEAPXCOORD, ST_WEAPYCOORD6);
            itemid = ST_IDWEAPON6;
            break;
        case '7':
            swaparrows();
            setarrow(ST_WEAPXCOORD, ST_WEAPYCOORD7);
            itemid = ST_IDGIZMO1;
            break;
        case '8':
            swaparrows();
            setarrow(ST_WEAPXCOORD, ST_WEAPYCOORD8);
            itemid = ST_IDGIZMO2;
            break;
        case '9':
            swaparrows();
            setarrow(ST_WEAPXCOORD, ST_WEAPYCOORD9);
            itemid = ST_IDGIZMO3;
            break;
        case 'a':
        case 'A':
            swaparrows();
            setarrow(ST_SYRINGEXCOORD, ST_SYRINGEYCOORD1);
            itemid = ST_IDSYRINGE1;
            break;
        case 'b':
        case 'B':
            swaparrows();
            setarrow(ST_SYRINGEXCOORD, ST_SYRINGEYCOORD2);
            itemid = ST_IDSYRINGE2;
            break;
		case 'g':
		case 'G':
			swaparrows();
			setarrow(ST_SYRINGEXCOORD, ST_SYRINGEYCOORD3);
			itemid = ST_IDSYRINGE3;
			break;
		case 'x':
		case 'X':
			return RET_EXIT;
			break;
			
		default:
			break;
			
        }
		return RET_OK;
}