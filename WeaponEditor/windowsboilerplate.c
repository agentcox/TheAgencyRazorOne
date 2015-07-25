#include <windows.h>
#include <winuser.h>
#include <windowsx.h>
#include <mmsystem.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wparam,
        LPARAM lparam);


int main (HWND hwnd);

int WINAPI WinMain (HINSTANCE hinstance, HINSTANCE hprev, PSTR cmdline,
        int ishow)
{
    static char app[] = "The Sempai";

    HWND hwnd;

    MSG msg;
    WNDCLASS wndclass;

    memset(&wndclass, 0, sizeof(WNDCLASS));

    wndclass.style = CS_HREDRAW | CS_VREDRAW;

    wndclass.lpfnWndProc = WinProc;

    wndclass.hInstance = hinstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = GetStockObject(BLACK_BRUSH);




  
    wndclass.lpszClassName = app;



    RegisterClass(&wndclass);



    hwnd = CreateWindow(app, 
            "Charles Cox's Window",
          WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 
            CW_USEDEFAULT, 
            CW_USEDEFAULT, 
            CW_USEDEFAULT, 
            NULL,
            NULL,
            hinstance,
            NULL);




    ShowWindow(hwnd, ishow);

    UpdateWindow(hwnd);
srand((int)time(NULL));


    while (GetMessage(&msg, NULL, 0, 0))
        DispatchMessage(&msg);

    return(msg.wParam);
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wparam,
        LPARAM lparam)
{
    static HDC hdc;
    static PAINTSTRUCT ps;
    static RECT rect;





    switch (message)
    {

	case WM_CREATE:

            return(0);


        case WM_PAINT:         
			hdc = BeginPaint(hwnd, &ps);
			
            EndPaint(hwnd, &ps);
            return(0);

		case WM_SIZE:
			GetClientRect(hwnd, &rect);
	
		return(0);

        case WM_DESTROY:
		
            PostQuitMessage(0);
            return(0);


	


	

    }


	return(DefWindowProc(hwnd, message, wparam, lparam));
}

