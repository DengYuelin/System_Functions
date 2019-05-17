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




	if (timeGetTime() > screentimer + 14)		//slow rendering to approximately 60 fps
	{
		screentimer = timeGetTime();

		//start rendering
		if (d3ddev->BeginScene())
		{
			//*** insert sprite code here ***

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