//frame organized and commented by Yuelin Deng
//Control Experiment
#include "DirectX.h"
#include "Global_variable.h"
#include "usb7660.h"
#include <tchar.h>
#include "original_PID.h"
using namespace std;

const string APPTITLE = "DirectX Project <Rename Me>";
const int SCREENW = 1920;
const int SCREENH = 1080;
const bool FULLSCREEN = true;
double sine_steady_state_error;
double last_reference;
double now_reference;
double amplitude;
double maximum = 0;
double overshoot;

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

	//initialize Background1
	if (!Background1.Load_texture("background1.png"))
	{
		MessageBox(window, "Error loading background1.png", APPTITLE.c_str(), 0);
		return false;
	}

	//initialize Background2
	if (!Background2.Load_texture("background2.png"))
	{
		MessageBox(window, "Error loading background2.png", APPTITLE.c_str(), 0);
		return false;
	}

	//initialize Background3
	if (!Background3.Load_texture("background3.png"))
	{
		MessageBox(window, "Error loading background3.png", APPTITLE.c_str(), 0);
		return false;
	}

	//initialize frequency button
	if (!Sin1_Button.Load_texture("frebutton.png"))
	{
		MessageBox(window, "Error loading frebutton.png", APPTITLE.c_str(), 0);
		return false;
	}
	if (!Sin2_Button.Load_texture("frebutton.png"))
	{
		MessageBox(window, "Error loading frebutton.png", APPTITLE.c_str(), 0);
		return false;
	}
	if (!Sin3_Button.Load_texture("frebutton.png"))
	{
		MessageBox(window, "Error loading frebutton.png", APPTITLE.c_str(), 0);
		return false;
	}
	if (!Sin4_Button.Load_texture("frebutton.png"))
	{
		MessageBox(window, "Error loading frebutton.png", APPTITLE.c_str(), 0);
		return false;
	}
	if (!Sin5_Button.Load_texture("frebutton.png"))
	{
		MessageBox(window, "Error loading frebutton.png", APPTITLE.c_str(), 0);
		return false;
	}

	//initialize position button
	if (!Position_Button.Load_texture("posbutton.png"))
	{
		MessageBox(window, "Error loading posbutton.png", APPTITLE.c_str(), 0);
		return false;
	}

	//initialize valve button
	if (!Valve_Button.Load_texture("valvebutton.png"))
	{
		MessageBox(window, "Error loading valvebutton.png", APPTITLE.c_str(), 0);
		return false;
	}

	//initialize start button
	if (!Start_Button.Load_texture("start.png"))
	{
		MessageBox(window, "Error loading start.png", APPTITLE.c_str(), 0);
		return false;
	}

	//initialize stop button
	if (!Stop_Button.Load_texture("stop.png"))
	{
		MessageBox(window, "Error loading stop.png", APPTITLE.c_str(), 0);
		return false;
	}

	//initialize bar
	if (!Bar.Load_texture("bar.png"))
	{
		MessageBox(window, "Error loading bar.png", APPTITLE.c_str(), 0);
		return false;
	}

	TNR45 = MakeFont("Times New Roman", 45);
	TNR32 = MakeFont("Times New Roman", 35);

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
	//get cusor point
	//GetCursorPos(&point);


	//*** insert game code here ***
	
	//fps counter
	fps_c++;
	if (timeGetTime() > fpstimer + 499)
	{
		fpstimer = timeGetTime();
		fps = fps_c * 2;
		fps_c = 0;
	}

	//scaner cycle
	if (timeGetTime() > scantimer + scanner_cycle)
	{
		scantimer = timeGetTime();
		//sine wave
		if (running)
		{
			//read data
			read_data = ZT7660_AIonce(m_cardNO, m_chMode, 21, m_AIrange, m_AIAmp, 0, 0, 0, 0, 0, 0);
			read_data = lpf.lpf(read_data);

			if (target_mode == 1)
			{
				sincounter++;
				target_data = (int)(sin(sincounter * 2 * M_PI / (1000 / (scanner_cycle + 1) / sin_frequency)) * sin_amplitude + 2047);
				//calcuate output data
				output_data = pid.PID_control(target_data, read_data);
			}
			else if (target_mode == 2)
			{
				target_data = (int)((780 - Bar.coord_y) * 4096 / lenth);
				output_data = pid.PID_control(target_data, read_data);
			}
			else if (target_mode == 3)
			{
				output_data = (int)((780 - Bar.coord_y) * 4096 / lenth);
			}


			//record data
			output_list.append(output_data);
			read_list.append(read_data);
			target_list.append(target_data);
			double instant_error = target_data - read_data;
			sine_steady_state_error = abs(instant_error)*275.0/4095.0;
			if (sine_steady_state_error < 10 && caltr)
			{
				Tr = timeGetTime() - tr;
				caltr = false;
			}


		}
		else
			output_data = 1800;

		


	}

	//Graphics

	//background switch
	if (Background1_ClickBox.Mouse_over() && Left_Click_Once())
	{
		background = 1;
		Background1.effective = true;
		Background2.effective = false;
		Background3.effective = false;
		//group 1
		Sin1_Button.effective = true;
		Sin2_Button.effective = true;
		Sin3_Button.effective = true;
		Sin4_Button.effective = true;
		Sin5_Button.effective = true;
		Position_Button.effective = true;
		Valve_Button.effective = true;
		//group 3
		Start_Button.effective = false;
		Stop_Button.effective = false;
		Bar.effective = false;
	}

	if (Background2_ClickBox.Mouse_over() && Left_Click_Once())
	{
		background = 2;
		Background1.effective = false;
		Background2.effective = true;
		Background3.effective = false;
		//group 1
		Sin1_Button.effective = false;
		Sin2_Button.effective = false;
		Sin3_Button.effective = false;
		Sin4_Button.effective = false;
		Sin5_Button.effective = false;
		Position_Button.effective = false;
		Valve_Button.effective = false;
		//group 3
		Start_Button.effective = false;
		Stop_Button.effective = false;
		Bar.effective = false;
	}

	if (Background3_ClickBox.Mouse_over() && Left_Click_Once())
	{
		background = 3;
		Background1.effective = false;
		Background2.effective = false;
		Background3.effective = true;
		//group 1
		Sin1_Button.effective = false;
		Sin2_Button.effective = false;
		Sin3_Button.effective = false;
		Sin4_Button.effective = false;
		Sin5_Button.effective = false;
		Position_Button.effective = false;
		Valve_Button.effective = false;
		//group 3
		Start_Button.effective = true;
		Stop_Button.effective = true;
		Bar.effective = true;
	}
	switch (background)
	{
	case 1:
		if (Sin1_Button.Mouse_over() && Left_Click_Once())
		{
			Sin1_Button.frame = 1;
			Sin2_Button.frame = 0;
			Sin3_Button.frame = 0;
			Sin4_Button.frame = 0;
			Sin5_Button.frame = 0;
			Position_Button.frame = 0;
			Valve_Button.frame = 0;
			Start_Button.frame = 0;
			Stop_Button.frame = 0;
			target_mode = 1;
			sin_frequency = 0.25;
			sincounter = 0;
			pid.reset(5,2,10);
			running = false;
		}
		else if (Sin2_Button.Mouse_over() && Left_Click_Once())
		{
			Sin1_Button.frame = 0;
			Sin2_Button.frame = 1;
			Sin3_Button.frame = 0;
			Sin4_Button.frame = 0;
			Sin5_Button.frame = 0;
			Position_Button.frame = 0;
			Valve_Button.frame = 0;
			Start_Button.frame = 0;
			Stop_Button.frame = 0;
			target_mode = 1;
			sin_frequency = 0.21;
			sincounter = 0;
			pid.reset(5,2,10);
			running = false;
		}
		else if (Sin3_Button.Mouse_over() && Left_Click_Once())
		{
			Sin1_Button.frame = 0;
			Sin2_Button.frame = 0;
			Sin3_Button.frame = 1;
			Sin4_Button.frame = 0;
			Sin5_Button.frame = 0;
			Position_Button.frame = 0;
			Valve_Button.frame = 0;
			Start_Button.frame = 0;
			Stop_Button.frame = 0;
			target_mode = 1;
			sin_frequency = 0.20;
			sincounter = 0;
			pid.reset(5,2,10);
			running = false;
		}
		else if (Sin4_Button.Mouse_over() && Left_Click_Once())
		{
			Sin1_Button.frame = 0;
			Sin2_Button.frame = 0;
			Sin3_Button.frame = 0;
			Sin4_Button.frame = 1;
			Sin5_Button.frame = 0;
			Position_Button.frame = 0;
			Valve_Button.frame = 0;
			Start_Button.frame = 0;
			Stop_Button.frame = 0;
			target_mode = 1;
			sin_frequency = 0.15;
			sincounter = 0;
			pid.reset(5,2,9);
			running = false;
		}
		else if (Sin5_Button.Mouse_over() && Left_Click_Once())
		{
			Sin1_Button.frame = 0;
			Sin2_Button.frame = 0;
			Sin3_Button.frame = 0;
			Sin4_Button.frame = 0;
			Sin5_Button.frame = 1;
			Position_Button.frame = 0;
			Valve_Button.frame = 0;
			Start_Button.frame = 0;
			Stop_Button.frame = 0;
			target_mode = 1;
			sin_frequency = 0.10;
			sincounter = 0;
			pid.reset(5,2,10);
			running = false;
		}
		else if (Position_Button.Mouse_over() && Left_Click_Once())
		{
			Sin1_Button.frame = 0;
			Sin2_Button.frame = 0;
			Sin3_Button.frame = 0;
			Sin4_Button.frame = 0;
			Sin5_Button.frame = 0;
			Position_Button.frame = 1;
			Valve_Button.frame = 0;
			Start_Button.frame = 0;
			Stop_Button.frame = 0;
			target_mode = 2;
			pid.reset(0.7,5,1.5);
			running = false;
		}
		else if (Valve_Button.Mouse_over() && Left_Click_Once())
		{
			Sin1_Button.frame = 0;
			Sin2_Button.frame = 0;
			Sin3_Button.frame = 0;
			Sin4_Button.frame = 0;
			Sin5_Button.frame = 0;
			Position_Button.frame = 0;
			Valve_Button.frame = 1;
			Start_Button.frame = 0;
			Stop_Button.frame = 0;
			target_mode = 3;
			running = false;
		}
		break;
	case 3:
		if (Start_Button.Mouse_over() && Mouse_Button(0))
		{
			Start_Button.frame = 1;
			running = true;
		}
		else if (Stop_Button.Mouse_over() && Mouse_Button(0))
		{
			Stop_Button.frame = 1;
			running = false;
			sincounter = 0;
			output_list.clear();
			read_list.clear();
			target_list.clear();
			pid.clearpid();
		}
		else
		{
			Start_Button.frame = 0;
			Stop_Button.frame = 0;
		}
		if (target_mode == 1)
		{
			Bar.coord_y = 780 - (read_data * lenth / 4096);
		}
		else
		{
			GetCursorPos(&point);
			if (Mouse_Button(0) && point.x <= 1800 && point.x >= 1720 && running)
			{
				overshoot = 0.1 + 0.1 * (Bar.coord_y - point.y) / 1080.0;
				tr = timeGetTime();
				caltr = true;
				Bar.coord_y = point.y - 32;
				if (Bar.coord_y <= 180)
					Bar.coord_y = 180;
				if (Bar.coord_y >= 780)
					Bar.coord_y = 780;

			}
		}
		break;
	}

	//output cycle
	if (timeGetTime() > outputtimer + output_cycle)
	{
		ZT7660_AOonce(1, 1, 0, output_data);
	}
	//mouse state
	if (Mouse_Button(0))
		mouse_left_up = false;
	else
		mouse_left_up = true;
	if (Mouse_Button(1))
		mouse_right_up = false;
	else
		mouse_right_up = true;

	if (timeGetTime() > screentimer + 40)		//slow rendering to approximately 30 fps
	{
		screentimer = timeGetTime();

		//start rendering
		if (d3ddev->BeginScene())
		{

			//start drawing
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
			//*** insert sprite code here ***
			//draw background
			Background1.Sprite_Draw();
			Background2.Sprite_Draw();
			Background3.Sprite_Draw();


			//draw interface
			Sin1_Button.Sprite_Draw();
			Sin2_Button.Sprite_Draw();
			Sin3_Button.Sprite_Draw();
			Sin4_Button.Sprite_Draw();
			Sin5_Button.Sprite_Draw();
			Position_Button.Sprite_Draw();
			Valve_Button.Sprite_Draw();
			Start_Button.Sprite_Draw();
			Stop_Button.Sprite_Draw();
			Bar.Sprite_Draw();

			//display text
			//group 2
			std::ostringstream p1_sin;
			std::ostringstream p2_kp;
			std::ostringstream p2_ki;
			std::ostringstream p2_kd;
			std::ostringstream p2_lpf;
			std::ostringstream p2_read;
			std::ostringstream p2_out;
			std::ostringstream p2_sf;
			std::ostringstream p2_of;
			std::ostringstream p3_target;
			std::ostringstream p3_time;
			std::ostringstream p4_time;
			switch (background)
			{
			case 1:
				p1_sin << "0.25   0.21   0.20   0.15   0.10";
				FontPrint(TNR45, 560, 331, 590, 250, p1_sin.str(), D3DCOLOR_XRGB(255, 255, 255));
				break;
			case 2:
				p2_kp << pid.kp;
				FontPrint(TNR45, 1139, 244, 300, 250, p2_kp.str(), D3DCOLOR_XRGB(139, 0, 0));
				p2_ki << pid.ki;
				FontPrint(TNR45, 1338, 244, 300, 250, p2_ki.str(), D3DCOLOR_XRGB(139, 0, 0));
				p2_kd << pid.kp;
				FontPrint(TNR45, 1538, 244, 300, 250, p2_kd.str(), D3DCOLOR_XRGB(139, 0, 0));
				p2_lpf << lpf.a;
				FontPrint(TNR45, 1139, 294, 300, 250, p2_lpf.str(), D3DCOLOR_XRGB(139, 0, 0));
				p2_read << read_data;
				FontPrint(TNR45, 1100, 344, 300, 250, p2_read.str(), D3DCOLOR_XRGB(139, 0, 0));
				p2_out << output_data;
				FontPrint(TNR45, 1100, 399, 300, 250, p2_out.str(), D3DCOLOR_XRGB(139, 0, 0));
				p2_sf << scanner_frequency;
				FontPrint(TNR45, 1100, 449, 300, 250, p2_sf.str(), D3DCOLOR_XRGB(139, 0, 0));
				p2_of << output_frequency;
				FontPrint(TNR45, 1100, 504, 300, 250, p2_of.str(), D3DCOLOR_XRGB(139, 0, 0));
				break;
			case 3:
				p2_read << ( 275 * read_data / 4095.0);
				FontPrint(TNR32, 1530, 150, 300, 250, p2_read.str(), D3DCOLOR_XRGB(0, 0, 0));
				p3_target << (275 * target_data / 4095.0);
				FontPrint(TNR32, 1530, 178, 300, 250, p3_target.str(), D3DCOLOR_XRGB(0, 68, 139));
				p2_out << output_data;
				FontPrint(TNR32, 1530, 208, 300, 250, p2_out.str(), D3DCOLOR_XRGB(139, 0, 0));
				p3_time << "Step responce specifications: overshoot-"<< "NA" << "\n"<<"Tr = " << "NA" << "ms" << " Ts = " << "NA"  << "ms" ;

				FontPrint(TNR32, 590, 908, 600, 500, p3_time.str(), D3DCOLOR_XRGB(0, 0, 0));
				p4_time << "Sine responce specifications: steady_state_error = "<<sine_steady_state_error<<"mm";
				FontPrint(TNR32, 590, 998, 600, 500, p4_time.str(), D3DCOLOR_XRGB(0, 0, 0));
				break;
			}


			//fps
			std::ostringstream fps_out;
			fps_out << "fps: " << fps;
			FontPrint(TNR45, SCREENW - 150, 0, 300, 250, fps_out.str(), D3DCOLOR_XRGB(0, 0, 0));

			//mouse pos
			/*std::ostringstream mouse_pos;
			mouse_pos << "X：" << point.x << " Y:" << point.y;
			FontPrint(TNR45, SCREENW - 300, 50, 300, 250, mouse_pos.str(), D3DCOLOR_XRGB(0, 0, 0));*/

			//stop drawing
			spriteobj->End();

			//draw wave
			if (background == 3 && running)
			{
				if (output_data < 0)
					output_data = 0;
				else if (output_data > 4095)
					output_data = 4095;
				if (target_list.length() < timeaxis)
				{
					target_list.moveToStart();
					read_list.moveToStart();
					output_list.moveToStart();
					for (int i = 0; i < target_list.length(); i++)
					{
						DrawPoint(origonx + i, origony - (lenth * target_list.getValue() / 4095), D3DCOLOR_XRGB(0, 0, 0));
						//DrawPoint(origonx + i, 1 + origony - (lenth * target_list.getValue() / 4095), D3DCOLOR_XRGB(0, 0, 0));
						target_list.next();
						DrawPoint(origonx + i, origony - (lenth * read_list.getValue() / 4095), D3DCOLOR_XRGB(0, 0, 255));
						//DrawPoint(origonx + i, 1 + origony - (lenth * read_list.getValue() / 4095), D3DCOLOR_XRGB(0, 0, 255));
						read_list.next();
						//DrawPoint(origonx + i, origony - (lenth * output_list.getValue() / 4095), D3DCOLOR_XRGB(255, 0, 0));
						//DrawPoint(origonx + i, 1 + origony - (lenth * output_list.getValue() / 4095), D3DCOLOR_XRGB(255, 0, 0));
						//output_list.next();
					}
				}
				else
				{
					target_list.moveToEnd();
					target_list.prev();
					read_list.moveToEnd();
					read_list.prev();
					output_list.moveToEnd();
					output_list.prev();
					for (int i = timeaxis; i >0 ; i--)
					{
						DrawPoint(origonx + i, origony - (lenth * target_list.getValue() / 4095), D3DCOLOR_XRGB(0, 0, 0));
						//DrawPoint(origonx + i, 1 + origony - (lenth * target_list.getValue() / 4095), D3DCOLOR_XRGB(0, 0, 0));
						target_list.prev();
						DrawPoint(origonx + i, origony - (lenth * read_list.getValue() / 4095), D3DCOLOR_XRGB(0, 0, 255));
						//DrawPoint(origonx + i, 1 + origony - (lenth * read_list.getValue() / 4095), D3DCOLOR_XRGB(0, 0, 255));
						read_list.prev();
						//DrawPoint(origonx + i, origony - (lenth * output_list.getValue() / 4095), D3DCOLOR_XRGB(255, 0, 0));
						//DrawPoint(origonx + i, 1 + origony - (lenth * output_list.getValue() / 4095), D3DCOLOR_XRGB(255, 0, 0));
						//output_list.prev();
					}
				}
			}

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

	output_list.clear();
	read_list.clear();
	target_list.clear();

	DirectSound_Shutdown();
	DirectInput_Shutdown();
	Direct3D_Shutdown();

}