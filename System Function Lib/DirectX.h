//organized and commented by Yuelin Deng
#pragma once

//header files
#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <xinput.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <sstream>
#include <cmath>
using namespace std;

//libraries
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")


//program values
extern const string APPTITLE;
extern const int SCREENW;
extern const int SCREENH;
extern bool gameover;
//macro to detect key presses
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

//Direct3D objects
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;
extern LPD3DXSPRITE spriteobj;

//DirectInput objects, devices, and states
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;
extern XINPUT_GAMEPAD controllers[4];

//DirectInput functions
bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
bool Key_Down(int);
bool Key_Up(int);
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();
void XInput_Vibrate(int contNum = 0, int amount = 65535);
bool XInput_Controller_Found();
bool Left_Click_Once();
bool Right_Click_Once();

//Direct3D functions
//the way to use them can be found in word file "functions" by Enlightenment 
bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen);
void Direct3D_Shutdown();
LPDIRECT3DSURFACE9 LoadSurface(string filename);
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));
void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty,
int framenum, int framew, int frameh, int columns);
void Sprite_Animate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay);
void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height,
int frame = 0, int columns = 1, float rotation = 0.0f, float scaling = 1.0f,
D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));
void DrawLine(float x1, float y1, float x2, float y2, DWORD color);
void DrawPoint(float x, float y, DWORD color);
//super slow to draw circle
void DrawCircle(float x, float y, float radius, DWORD color);

//game functions
bool Game_Init(HWND window);
void Game_Run(HWND window);
void Game_End();

//font functions
LPD3DXFONT MakeFont(string name, int size);
void FontPrint(LPD3DXFONT font, int x, int y, int w, int h, string text, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

//sound funcions
#include "DirectSound.h"

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxerr.lib")

//primary DirectSound object
extern CSoundManager *dsound;

//function prototypes
bool DirectSound_Init(HWND hwnd);
void DirectSound_Shutdown();
CSound *LoadSound(string filename);
void PlaySound(CSound *sound);		//play sound once
void LoopSound(CSound *sound);		//play sound unless stopsound
void StopSound(CSound *sound);		//stop loop sound

									//sprite structure
class BASIC_SPRITE
{
public:
	/*Functions*/
	BASIC_SPRITE(const int blocknumber);
	BASIC_SPRITE(const bool input_effective = false, const bool input_selected = false, const bool input_anchortype = 1, const float input_original_x = 0, const float input_original_y = 0, const float input_coord_x = 0, const float input_coord_y = 0, const int input_frame = 0, const int input_columns = 1, const int input_width = 1, const int input_height = 1, const float input_scaling = 1.0f, const float input_rotation = 0.0f, const int input_startframe = 0, const int input_endframe = 0, const int input_starttime = 0, const int input_delay = 0, const int input_direction = 1, const D3DCOLOR input_color = D3DCOLOR_XRGB(255, 255, 254), const LPDIRECT3DTEXTURE9 input_texture = NULL, const int input_Block_number = 0);
	~BASIC_SPRITE();
	void Sprite_Draw();
	void Sprite_Draw_Copy_At_Mouse(const int frame, const D3DCOLOR color);
	bool Collision(const BASIC_SPRITE sprite2);
	bool Mouse_over();
	int Adjust_Frame();				//���ظ��ĺ��֡����״̬��
	int Adjust_Frame_Button();
	bool CollisionD(const BASIC_SPRITE sprite2);
	void Set_anchor_to_mouse();
	void Set_anchor_to_original();
	bool Load_texture(const std::string filename, const D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));
	void Displayinfo();
	void Free();
	/*Pic*/
	bool effective;				//to draw this sprite or not
	bool selected;
	bool anchortype;			//mouse(0) or coordinate(1)
	float original_x, original_y;		//coordinates of the original place
	float coord_x, coord_y;		//coordinates to draw the sprite
	int frame, columns;			//the frame number for a animate sprite and the columns in the source file
	int width, height;			//the width and heights for each frame in the source file
	float scaling, rotation;	//the scaling rate and rotation angle(radian)
	int startframe, endframe;
	int starttime, delay;		//fps
	int direction;				//forwards or backwards(animation)
	D3DCOLOR color;				//color change
	LPDIRECT3DTEXTURE9 texture; //image
	int previous_state;
	/*Data*/
	int Block_number;
};


inline BASIC_SPRITE::BASIC_SPRITE(const int blocknumber) :
	effective(false),
	selected(false),
	anchortype(1),
	original_x(0),
	original_y(0),
	coord_x(0),
	coord_y(0),
	frame(0),
	columns(1),
	width(1),
	height(1),
	scaling(1.0f),
	rotation(0.0f),
	startframe(0),
	endframe(0),
	starttime(0),
	delay(0),
	direction(1),
	color(D3DCOLOR_XRGB(255, 255, 254)),
	texture(NULL),
	previous_state(0)
{
	this->Block_number = blocknumber;
}

inline BASIC_SPRITE::BASIC_SPRITE(
	const bool input_effective,
	const bool input_selected,
	const bool input_anchortype,
	const float input_original_x,
	const float input_original_y,
	const float input_coord_x,
	const float input_coord_y,
	const int input_frame,
	const int input_columns,
	const int input_width,
	const int input_height,
	const float input_scaling,
	const float input_rotation,
	const int input_startframe,
	const int input_endframe,
	const int input_starttime,
	const int input_delay,
	const int input_direction,
	const D3DCOLOR input_color,
	const LPDIRECT3DTEXTURE9 input_texture,
	const int input_Block_number)
{
	this->effective = input_effective;
	this->selected = input_selected;
	this->anchortype = input_anchortype;
	this->original_x = input_original_x;
	this->original_y = input_original_y;
	this->coord_x = input_coord_x;
	this->coord_y = input_coord_y;
	this->frame = input_frame;
	this->columns = input_columns;
	this->width = input_width;
	this->height = input_height;
	this->scaling = input_scaling;
	this->rotation = input_rotation;
	this->startframe = input_startframe;
	this->endframe = input_endframe;
	this->starttime = input_starttime;
	this->delay = input_delay;
	this->direction = input_direction;
	this->color = input_color;
	this->texture = input_texture;
	this->Block_number = input_Block_number;
}

inline BASIC_SPRITE::~BASIC_SPRITE()
{
}

inline void BASIC_SPRITE::Sprite_Draw()
{
	if (this->effective)
	{
		//create a scale vector
		D3DXVECTOR2 scale(this->scaling, this->scaling);

		//create a translate vector
		D3DXVECTOR2 trans((float)this->coord_x, (float)this->coord_y);

		//set center by dividing width and height by two
		D3DXVECTOR2 center((float)(this->width * this->scaling) / 2, (float)(this->height * this->scaling) / 2);

		//create 2D transformation matrix
		D3DXMATRIX mat;
		D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, this->rotation, &trans);

		//tell sprite object to use the transform
		spriteobj->SetTransform(&mat);

		//calculate frame location in source image
		int fx = (this->frame % this->columns) * this->width;
		int fy = (this->frame / this->columns) * this->height;
		RECT srcRect = { fx, fy, fx + this->width, fy + this->height };

		//draw the sprite frame
		spriteobj->Draw(this->texture, &srcRect, NULL, NULL, this->color);
	}
}

inline void BASIC_SPRITE::Sprite_Draw_Copy_At_Mouse(const int frame, const D3DCOLOR color)
{
	if (this->effective)
	{

		extern POINT point;
		GetCursorPos(&point);

		//create a scale vector
		D3DXVECTOR2 scale(this->scaling, this->scaling);

		//create a translate vector
		D3DXVECTOR2 trans((point.x - (this->width * this->scaling) / 2), (point.y - (this->height * this->scaling) / 2));

		//set center by dividing width and height by two
		D3DXVECTOR2 center((float)(this->width * this->scaling) / 2, (float)(this->height * this->scaling) / 2);

		//create 2D transformation matrix
		D3DXMATRIX mat;
		D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, this->rotation, &trans);

		//tell sprite object to use the transform
		spriteobj->SetTransform(&mat);

		//calculate frame location in source image
		int fx = (frame % this->columns) * this->width;
		int fy = (frame / this->columns) * this->height;
		RECT srcRect = { fx, fy, fx + this->width, fy + this->height };

		//draw the sprite frame
		spriteobj->Draw(this->texture, &srcRect, NULL, NULL, color);
	}
}

inline bool BASIC_SPRITE::Collision(const BASIC_SPRITE sprite2)
{
	if (this->effective && sprite2.effective)
	{
		RECT rect1;
		rect1.left = (long)this->coord_x;
		rect1.top = (long)this->coord_y;
		rect1.right = (long)(this->coord_x + this->width * this->scaling);
		rect1.bottom = (long)(this->coord_y + this->height * this->scaling);

		RECT rect2;
		rect2.left = (long)sprite2.coord_x;
		rect2.top = (long)sprite2.coord_y;
		rect2.right = (long)(sprite2.coord_x + sprite2.width * sprite2.scaling);
		rect2.bottom = (long)(sprite2.coord_y + sprite2.height * sprite2.scaling);

		RECT dest; //ignored
		return IntersectRect(&dest, &rect1, &rect2);
	}
	else
		return false;
}

inline bool BASIC_SPRITE::Mouse_over()
{
	if (this->effective)
	{
		RECT rect1;
		rect1.left = (long)this->coord_x;
		rect1.top = (long)this->coord_y;
		rect1.right = (long)(this->coord_x + this->width * this->scaling);
		rect1.bottom = (long)(this->coord_y + this->height * this->scaling);

		extern POINT point;
		GetCursorPos(&point);

		RECT rect2;
		rect2.left = (long)point.x;
		rect2.top = (long)point.y;
		rect2.right = (long)(point.x + 1);
		rect2.bottom = (long)(point.y + 1);

		RECT dest; //ignored
		return IntersectRect(&dest, &rect1, &rect2);
	}
	else
		return false;
}

inline int BASIC_SPRITE::Adjust_Frame()
{
	if (this->effective)
	{
		if (!this->Mouse_over() && !this->selected)
		{
			this->previous_state = this->frame;
			this->frame = 0;
			return 0;
		}
		else if (this->Mouse_over() && !this->selected)
		{
			this->previous_state = this->frame;
			this->frame = 1;
			if (Left_Click_Once())
			{
				this->selected = true;
			}
			return 1;
		}
		else if (this->selected)
		{
			this->previous_state = this->frame;
			this->frame = 2;
			if (Right_Click_Once())
				this->selected = false;
			if (Left_Click_Once() && !this->Mouse_over())
				this->selected = false;
			return 2;
		}
	}
}

inline int BASIC_SPRITE::Adjust_Frame_Button()
{
	if (this->effective)
	{
		if (!this->Mouse_over() && !this->selected)
		{
			this->previous_state = this->frame;
			this->frame = 0;
			return 0;
		}
		else if (this->Mouse_over() && !this->selected)
		{
			this->previous_state = this->frame;
			this->frame = 1;
			if (Mouse_Button(0))
			{
				this->selected = true;
			}
			return 1;
		}
		else if (this->selected)
		{
			this->previous_state = this->frame;
			this->frame = 2;
			if (!Mouse_Button(0))
				this->selected = false;
			return 2;
		}

		return 0;
	}
}

inline bool BASIC_SPRITE::CollisionD(const BASIC_SPRITE sprite2)
{
	if (this->effective && sprite2.effective)
	{
		double radius1, radius2;

		//calculate radius 1
		if (this->width > this->height)
			radius1 = (this->width * this->scaling) / 2.0;
		else
			radius1 = (this->height * this->scaling) / 2.0;

		//center point 1
		double x1 = this->coord_x + radius1;
		double y1 = this->coord_y + radius1;
		D3DXVECTOR2 vector1((float)x1, (float)y1);

		//calculate radius 2
		if (sprite2.width > sprite2.height)
			radius2 = (sprite2.width * sprite2.scaling) / 2.0;
		else
			radius2 = (sprite2.height * sprite2.scaling) / 2.0;

		//center point 2
		double x2 = sprite2.coord_x + radius2;
		double y2 = sprite2.coord_y + radius2;
		D3DXVECTOR2 vector2((float)x2, (float)y2);

		//calculate distance
		double deltax = vector1.x - vector2.x;
		double deltay = vector2.y - vector1.y;
		double dist = sqrt((deltax * deltax) + (deltay * deltay));

		//return distance comparison
		return (dist < radius1 + radius2);
	}
	else
		return false;
}

inline void BASIC_SPRITE::Set_anchor_to_mouse()
{
	extern POINT point;
	GetCursorPos(&point);
	this->anchortype = 0;
	this->coord_x = (point.x - (this->width * this->scaling) / 2);
	this->coord_y = (point.y - (this->height * this->scaling) / 2);
}

inline void BASIC_SPRITE::Set_anchor_to_original()
{
	this->anchortype = 1;
	this->coord_x = this->original_x;
	this->coord_y = this->original_y;
}

inline bool BASIC_SPRITE::Load_texture(const std::string filename, const D3DCOLOR transcolor)
{
	string str;
	this->texture = LoadTexture(filename, transcolor);
	if (this->texture == NULL) return false;
	else return true;
}


inline void BASIC_SPRITE::Displayinfo()
{
	if (this->selected && this->effective)
	{
		extern LPD3DXFONT font1;
		std::ostringstream text;
		text << this->Block_number;
		Sprite_Transform_Draw(this->texture, 0, 0, 0, 0, 0, 1);
		//FontPrint(font1, 400, 300, text.str(), D3DCOLOR_XRGB(0, 0, 0));
	}
}

inline void BASIC_SPRITE::Free()
{
	if (this->texture) this->texture->Release();
}




#pragma once
