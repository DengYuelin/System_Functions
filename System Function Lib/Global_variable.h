#pragma once
#include "original_PID.h"
#include "DirectX.h"
#include"LList.h"
#include"arrayList.h"
#include"Constants.h"
//used for mouse click detection
bool mouse_left_up = true;
bool mouse_right_up = true; 
POINT point;
//card variables
unsigned long m_cardNO = 1;
unsigned long m_chMode = 0;
unsigned long m_chNO = 21;
unsigned long m_AIrange = 0;
unsigned long m_AIAmp = 0;
unsigned long m_ADstartMode;
unsigned long m_ADfreq;
unsigned long m_ABflag;
unsigned long m_ch1Flag;
unsigned long m_ADctrlWord;
unsigned long m_ADoverTime;

//scanner
//read the data from the device ; generate sin wave
DWORD scantimer = timeGetTime();
unsigned long read_data;
LList <int> read_list;
int scanner_cycle = 4;
double scanner_frequency = (1000.0 / (scanner_cycle + 1));//Hz
int sincounter = 0;
double sin_frequency = 0.25; // Hz
int sin_amplitude = 1000;
bool running = false;

//target data
int target_mode = 1;
int target_data = 0;
LList <int> target_list;

//output data
PID pid(5, 2, 8);//250ms
//PID step_pid(1.5,0.1,1);
LPF lpf(0.9);
int output_data = 0;
LList <int> output_list;
DWORD outputtimer = timeGetTime();
int output_cycle = 9;
double output_frequency = (1000.0 / (output_cycle + 1));//Hz


//display
int lenth = 620; //bar hight
int origonx = 522;
int origony = 786;
int timeaxis = 1000;


//fps display
DWORD fpstimer = timeGetTime();
unsigned int fps;
unsigned int fps_c;

//Font
LPD3DXFONT TNR45;
LPD3DXFONT TNR32;

//Background
int background = 1;
BASIC_SPRITE Background1(true, false, 1, 0, 0, 0, 0, 0, 1, 1920, 1080, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Background2(false, false, 1, 0, 0, 0, 0, 0, 1, 1920, 1080, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Background3(false, false, 1, 0, 0, 0, 0, 0, 1, 1920, 1080, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Background1_ClickBox(true, false, 1, 40, 166, 40, 166, 0, 1, 356, 92, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Background2_ClickBox(true, false, 1, 40, 258, 40, 258, 0, 1, 356, 92, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Background3_ClickBox(true, false, 1, 40, 350, 40, 350, 0, 1, 356, 92, 1, 0, 0, 0, 0, 0, 1);


//Background 1
BASIC_SPRITE Sin1_Button(true, false, 1, 542, 300, 542, 300, 0, 2, 100, 100, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Sin2_Button(true, false, 1, 642, 300, 642, 300, 0, 2, 100, 100, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Sin3_Button(true, false, 1, 742, 300, 742, 300, 0, 2, 100, 100, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Sin4_Button(true, false, 1, 842, 300, 842, 300, 0, 2, 100, 100, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Sin5_Button(true, false, 1, 942, 300, 942, 300, 0, 2, 100, 100, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Position_Button(true, false, 1, 1162, 300, 1162, 300, 0, 2, 270, 100, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Valve_Button(true, false, 1, 1432, 300, 1432, 300, 0, 2, 270, 100, 1, 0, 0, 0, 0, 0, 1);
//Background 2

//Background 3
BASIC_SPRITE Start_Button(false, false, 1, 1616, 900, 1616, 900, 0, 2, 100, 100, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Stop_Button(false, false, 1, 1216, 900, 1716, 900, 0, 2, 100, 100, 1, 0, 0, 0, 0, 0, 1);
BASIC_SPRITE Bar(false, false, 1, 1780, 460, 1780, 460, 0, 1, 124, 124, 0.5, 0, 0, 0, 0, 1);