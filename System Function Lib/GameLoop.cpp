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
DWORD fpstimer = timeGetTime();

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
	/*if (ZT7660_OpenDevice(m_cardNO) != 0)
	{
		MessageBox(window, "Error initializing device USB7660", APPTITLE.c_str(), 0);
		return false;
	}*/

	if (!Bar.Load_texture("blue_boxTick.png"))
	{
		MessageBox(window, "Error loading blue_boxTick.png", APPTITLE.c_str(), 0);
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
	
	//fps counter
	fps_c++;
	if (timeGetTime() > fpstimer + 500)
	{
		fpstimer = timeGetTime();
		fps = fps_c * 2;
		fps_c = 0;
	}
	
	//read data
	read_data = 1;
	read_data = ZT7660_AIonce(m_cardNO, m_chMode, 21, m_AIrange, m_AIAmp, 0, 0, 0, 0, 0, 0);

	//bar movement
	if (Bar.Mouse_over() && Mouse_Button(0))
	{
		Bar.Set_anchor_to_mouse();
		Bar.coord_x = Bar.original_x;
		if (Bar.coord_y >= Bar.original_y + 200)
			Bar.coord_y = Bar.original_y + 200;
		if (Bar.coord_y <= Bar.original_y - 200)
			Bar.coord_y = Bar.original_y - 200;
	}
	
	//controls
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

			//input value
			std::ostringstream text_1;
			text_1 << read_data;
			FontPrint(font1, 0, 0, 300, 250, text_1.str(), D3DCOLOR_XRGB(255, 255, 255));

			//fps
			std::ostringstream fps_out;
			fps_out << "fps: " << fps;
			FontPrint(font1, SCREENW - 150, 0, 300, 250, fps_out.str(), D3DCOLOR_XRGB(255, 255, 255));

			//input data 
			std::ostringstream inputdata;
			inputdata << "input: " << input_data;
			FontPrint(font1, 20, SCREENH / 2 + 250, 300, 250, inputdata.str(), D3DCOLOR_XRGB(255, 255, 255));

			//draw bar
			Bar.Sprite_Draw();

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