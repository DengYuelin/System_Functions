//frame organized and commented by Yuelin Deng
//Control Experiment
#include "DirectX.h"
#include "Global_variable.h"
#include "usb7660.h"
#include <tchar.h>
using namespace std;

const string APPTITLE = "DirectX Project <Rename Me>";
const int SCREENW = 1024;
const int SCREENH = 768;
const bool FULLSCREEN = false;


DWORD screentimer = timeGetTime();

bool Game_Init(HWND window)
{
	srand(time(NULL));

	//initialize Direct3D
	if (!Direct3D_Init(window, SCREENW, SCREENH, FULLSCREEN))
	{
		MessageBox(window, "Error initializing Direct3D", APPTITLE.c_str(), 0);
		return false;
	}

	//initialize DirectInput
	if (!DirectInput_Init(window))
	{
		MessageBox(window, "Error initializing DirectInput", APPTITLE.c_str(), 0);
		return false;
	}

	//initialize DirectSound
	if (!DirectSound_Init(window))
	{
		MessageBox(window, "Error initializing DirectSound", APPTITLE.c_str(), 0);
		return false;
	}

	//initialize USB device
	if (ZT7660_OpenDevice(m_cardNO) != 0)
	{
		MessageBox(window, "Error initializing device USB7660", APPTITLE.c_str(), 0);
		return false;
	}
	font1 = MakeFont("Times New Roman", 45);

	return true;
}

void Game_Run(HWND window)
{
	//check for d3ddev
	if (!d3ddev) return;
	//update inputs
	DirectInput_Update();
	//clear screen
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 100), 1.0f, 0);

	//*** insert game code here ***
	read_data = 1;
	read_data = ZT7660_AIonce(m_cardNO,m_chMode,21,m_AIrange,m_AIAmp,0,0,0,0,0,0);
	ZT7660_AOonce(1, 1, 0, 4096);
	
	
	


	if (timeGetTime() > screentimer + 14)		//slow rendering to approximately 60 fps
	{
		screentimer = timeGetTime();

		//start rendering
		if (d3ddev->BeginScene())
		{

			//start drawing
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
			//*** insert sprite code here ***


			std::ostringstream text_1;
			text_1 << read_data;
			FontPrint(font1, 0, 0, 300, 250, text_1.str(), D3DCOLOR_XRGB(255, 255, 255));


			//stop drawing
			spriteobj->End();
			//stop rendering
			d3ddev->EndScene();
			d3ddev->Present(NULL, NULL, NULL, NULL);
		}
	}

	//exit with escape key
	if (KEY_DOWN(VK_ESCAPE)) gameover = true;
}

void Game_End()
{
	//shut down device USB7660
	ZT7660_CloseDevice(m_cardNO);

	//free memory and shut down
	DirectSound_Shutdown();
	DirectInput_Shutdown();
	Direct3D_Shutdown();
}