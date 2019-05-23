#pragma once
//used for mouse click detection
bool mouse_left_up = true;
bool mouse_right_up = true; 
POINT point;
//card variables
unsigned long m_cardNO = 1;
unsigned long m_chMode = 0;
unsigned long m_chNO = 21;
unsigned long m_AIrange = 2;
unsigned long m_AIAmp = 0;
unsigned long m_ADstartMode;
unsigned long m_ADfreq;
unsigned long m_ABflag;
unsigned long m_ch1Flag;
unsigned long m_ADctrlWord;
unsigned long m_ADoverTime;
//入口参数：
//          cardNO：模块号，默认从 1 开始。有关这个参数的详细说明，请参考 ZT7660_OpenDevice 函数
//          chMode：通道方式
//                    0 = 表示单端，固定单通道方式
//                    2 = 表示双端，固定单通道方式
//            chNO：通道号
//                    当chMode = 0 时，为通道号(1--48)
//                    当chMode = 2 时，为通道号(1--24)
//         AIrange：选择对采集到的AD原码值做何种变换，应该与AD采集量程(一般需跳线)相匹配：
//                    AIrange = 0，表示返回原码值，所有卡都有这种方式。
//                                 对于12位的卡返回 0~4095 之间的值
//                                 对于16位的卡返回 0~65535 之间的值
//                    AIrange = 1，表示把原码值转化为 0 -- 5000mV 之间的值，如果此卡没有这种量程，不应选这种方式
//                    AIrange = 2，表示把原码值转化为 0 -- 10000mV 之间的值，如果此卡没有这种量程，不应选这种方式
//                    AIrange = 5，表示把原码值转化为 -5000 -- 5000mV 之间的值，如果此卡没有这种量程，不应选这种方式
//           AIAmp：程控增益系数(0～2)。
//                    0 = 1倍增益(无增益)；1 = 10倍增益；2 = 100倍增益
//     ADstartMode：保留。保持此参数为0
//          ADfreq：保留。保持此参数为0
//          ABflag：设置模块为A型(12bit)还是B型(16bit)
//                    0 = A型；1 = B型
//         ch1Flag：保留。保持此参数为0
//      ADctrlWord：一般保持此参数为0。
//                    若此参数不为0，函数将直接用这个参数的值初始化AD，而对其他影响AD方式的参数，
//                    例如chMode、AIrange、AIAmp、ABflag、ch1Flag的值不予考虑。
//                    此参数的说明请参考说明书中对 usb7660AI 函数的lCode参数的说明。
//      ADoverTime：保留。保持这个参数为0
//返回值： 不等于 -1，表采集到的AD结果
//             -1   ，当 ZT7660_GetLastErr 返回值等于 0 时，表AD结果
//                    当 ZT7660_GetLastErr 返回值小于 0 时，表失败

//read variables
unsigned long read_data;

//input data
double input_data = 0;

//fps display
unsigned int fps;
unsigned int fps_c;

//Font
LPD3DXFONT font1;

//sprites
BASIC_SPRITE Bar(true, false, 1, 50, SCREENH / 2, 50, SCREENH / 2, 0, 1, 36, 36, 1, 0, 0, 0, 0, 0, 1, D3DCOLOR_ARGB(255, 255, 255, 255), NULL, 0);//slide bar