//organized and commented by Yuelin Deng
//from 2.8-3.9
//most of the code in this file is unchanged from the sourse code
//except some variables in WNDCLASSEX wc and HWND window.
#include "DirectX.h"
using namespace std;
bool gameover = false;


LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		gameover = true;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//initialize window settings
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APPTITLE.c_str();
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);

	//create a new window
	HWND window = CreateWindow(
		APPTITLE.c_str(),				//window class
		APPTITLE.c_str(),				//title bar
		WS_POPUP,						//window style
		CW_USEDEFAULT, CW_USEDEFAULT,	//position of window
		SCREENW, SCREENH,				//dimensions of the window
		NULL,							//parent window (not used)
		NULL,	                        //menu (not used)
		hInstance,						//application instance
		NULL);							//window parameters (not used)
	if (window == 0) return 0;

	//display the window
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	//initialize the game
	if (!Game_Init(window)) return 0;

	// main message loop
	MSG message;
	while (!gameover)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		//process game loop 
		Game_Run(window);
	}

	//shutdown
	Game_End();
	return message.wParam;
}

