# Microsoft Developer Studio Project File - Name="TextBased" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TextBased - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TextBased.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TextBased.mak" CFG="TextBased - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TextBased - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TextBased - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TextBased - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib fmodvc.lib /nologo /subsystem:console /machine:I386 /out:"Release/RazorOne.exe"

!ELSEIF  "$(CFG)" == "TextBased - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib fmodvc.lib /nologo /subsystem:console /profile /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"Debug/RazorOne.exe"

!ENDIF 

# Begin Target

# Name "TextBased - Win32 Release"
# Name "TextBased - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\bulletcalculate.cpp
# End Source File
# Begin Source File

SOURCE=.\cutscenes.cpp
# End Source File
# Begin Source File

SOURCE=.\dboxes.cpp
# End Source File
# Begin Source File

SOURCE=.\DSAIcon.rc
# End Source File
# Begin Source File

SOURCE=.\enemy.cpp
# End Source File
# Begin Source File

SOURCE=.\gizmos.cpp
# End Source File
# Begin Source File

SOURCE=.\graphics.cpp
# End Source File
# Begin Source File

SOURCE=.\guns.cpp
# End Source File
# Begin Source File

SOURCE=.\handtohand.cpp
# End Source File
# Begin Source File

SOURCE=.\inventory.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\math.cpp
# End Source File
# Begin Source File

SOURCE=.\mission.cpp
# End Source File
# Begin Source File

SOURCE=.\music.cpp
# End Source File
# Begin Source File

SOURCE=.\officeofrecords.cpp
# End Source File
# Begin Source File

SOURCE=.\opcon.cpp
# End Source File
# Begin Source File

SOURCE=.\playerselections.cpp
# End Source File
# Begin Source File

SOURCE=.\postmission.cpp
# End Source File
# Begin Source File

SOURCE=.\premission.cpp
# End Source File
# Begin Source File

SOURCE=.\radio.cpp
# End Source File
# Begin Source File

SOURCE=.\sartu.cpp
# End Source File
# Begin Source File

SOURCE=.\shooting.cpp
# End Source File
# Begin Source File

SOURCE=.\sniping.cpp
# End Source File
# Begin Source File

SOURCE=.\sounds.cpp
# End Source File
# Begin Source File

SOURCE=.\training.cpp
# End Source File
# Begin Source File

SOURCE=.\vehicle.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\agencydefines.h
# End Source File
# Begin Source File

SOURCE=.\agencyfunctions.h
# End Source File
# Begin Source File

SOURCE=.\agencyinclude.h
# End Source File
# Begin Source File

SOURCE=.\agencystatusgraph.h
# End Source File
# Begin Source File

SOURCE=.\agencystructs.h
# End Source File
# Begin Source File

SOURCE=.\game.h
# End Source File
# Begin Source File

SOURCE=.\guns.h
# End Source File
# Begin Source File

SOURCE=.\handtohand.h
# End Source File
# Begin Source File

SOURCE=.\lis.h
# End Source File
# Begin Source File

SOURCE=.\liststructs.h
# End Source File
# Begin Source File

SOURCE=.\mission.h
# End Source File
# Begin Source File

SOURCE=.\UiLib.h
# End Source File
# Begin Source File

SOURCE=.\weaponsammo.h
# End Source File
# Begin Source File

SOURCE=.\xa.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Group "Graphics"

# PROP Default_Filter ".gfx"
# Begin Source File

SOURCE=.\Data\Armory.gfx
# End Source File
# Begin Source File

SOURCE=.\Data\Atomic.gfx
# End Source File
# Begin Source File

SOURCE=.\Data\Cis.gfx
# End Source File
# Begin Source File

SOURCE=.\Data\Codescr.gfx
# End Source File
# Begin Source File

SOURCE=.\Data\Mainui.gfx
# End Source File
# Begin Source File

SOURCE=.\Data\Melee.gfx
# End Source File
# Begin Source File

SOURCE=.\Data\Prmtbox.gfx
# End Source File
# Begin Source File

SOURCE=.\Data\Prompt.gfx
# End Source File
# Begin Source File

SOURCE=.\Data\Radio.gfx
# End Source File
# Begin Source File

SOURCE=.\Data\Sartu.gfx
# End Source File
# Begin Source File

SOURCE=.\Data\Scope.gfx
# End Source File
# End Group
# Begin Group "Data"

# PROP Default_Filter ".dat"
# Begin Source File

SOURCE=.\Data\ammo.dat
# End Source File
# Begin Source File

SOURCE=.\Data\codebegin.dat
# End Source File
# Begin Source File

SOURCE=.\Data\codeend.dat
# End Source File
# Begin Source File

SOURCE=.\Data\dextra.dat
# End Source File
# Begin Source File

SOURCE=.\Data\dnamesa.dat
# End Source File
# Begin Source File

SOURCE=.\Data\dnamesb.dat
# End Source File
# Begin Source File

SOURCE=.\Data\dsacname.dat
# End Source File
# Begin Source File

SOURCE=.\Data\dsanamea.dat
# End Source File
# Begin Source File

SOURCE=.\Data\dsanameb.dat
# End Source File
# Begin Source File

SOURCE=.\Data\gg1.dat
# End Source File
# Begin Source File

SOURCE=.\Data\gg2.dat
# End Source File
# Begin Source File

SOURCE=.\Data\gge1.dat
# End Source File
# Begin Source File

SOURCE=.\Data\gge2.dat
# End Source File
# Begin Source File

SOURCE=.\Data\gizmos.dat
# End Source File
# Begin Source File

SOURCE=.\Data\songs.dat
# End Source File
# Begin Source File

SOURCE=.\Data\techs.dat
# End Source File
# Begin Source File

SOURCE=.\Data\weapons.dat
# End Source File
# End Group
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# End Group
# Begin Group "Dependencies"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\UiLib\Debug\UiLib.lib
# End Source File
# Begin Source File

SOURCE=.\WeaponsAmmoLib\Debug\WeaponsAmmoLib.lib
# End Source File
# Begin Source File

SOURCE=.\MapLibrary\Debug\MapLibrary.lib
# End Source File
# Begin Source File

SOURCE=.\XAudio\xaudio.lib
# End Source File
# End Group
# Begin Group "Other Crap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Logs\lissystem.txt
# End Source File
# Begin Source File

SOURCE=.\Logs\projectinfo.txt
# End Source File
# End Group
# End Target
# End Project
