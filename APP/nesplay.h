#ifndef __NESPLAY_H
#define __NESPLAY_H 	
#include "common.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//APP-NES模拟器 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/7/20
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved							  
//*******************************************************************************
//V1.1 20160627
//新增对SMS游戏的支持	
////////////////////////////////////////////////////////////////////////////////// 	   
  
extern u8 nesruning ;	//退出NES的标志
extern u8 frame_cnt;	//统计帧数
 
void nes_clock_set(u8 PLL);	 
void load_nes(u8* path);   
u8 nes_play(void);
#endif























