#include "common.h"
#include "spb.h"
#include "recorder.h"
#include "appplay.h"

#include "settings.h"
	
#include "audioplay.h"	

#include "ap3216c.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//APP通用 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/2/16
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//V1.1 20140216
//新增对各种分辨率LCD的支持.
//V1.2 20140727
//修改app_show_float函数的一个bug
////////////////////////////////////////////////////////////////////////////////// 	   

//模式选择列表的窗体名字
u8*const APP_MODESEL_CAPTION_TBL[GUI_LANGUAGE_NUM]=
{
"模式选择","模式選擇","Mode Select",
}; 
//提示信息的窗体名字
u8*const APP_REMIND_CAPTION_TBL[GUI_LANGUAGE_NUM]=
{
"提示信息","提示信息","Remind",	 
};
//提醒保存的窗体名字
u8*const APP_SAVE_CAPTION_TBL[GUI_LANGUAGE_NUM]=
{
"是否保存编辑后的文件?","是否保存編輯后的文件?","Do you want to save?",
}; 
//提醒删除的窗体名字
u8*const APP_DELETE_CAPTION_TBL[GUI_LANGUAGE_NUM]=
{
"确定删除这条短信?","確定刪除這條短信?","Are you sure to delete this SMS",
}; 
//创建文件失败,提示是否存在SD卡? ,200的宽度
u8*const APP_CREAT_ERR_MSG_TBL[GUI_LANGUAGE_NUM]=
{							 
"创建文件失败,请检查!","創建文件失敗,請檢查!","Creat File Failed,Please Check!",
}; 
//磁盘名字表
u8*const APP_DISK_NAME_TBL[3][GUI_LANGUAGE_NUM]=
{
	{"SD卡","SD卡","SD Card",},
 	{"FLASH磁盘","FLASH磁盤","FLASH Disk",},
 	{"U盘","U盤","U Disk",},
};  							 
//app主要功能界面标题
u8*const APP_MFUNS_CAPTION_TBL[APP_FUNS_NUM][GUI_LANGUAGE_NUM]=
{
	{"","","",},
	{"","","",},
	{"音乐播放器","音樂播放器","MUSIC PLAYER",},
 	{"","","",}, 
 	{"","","",},	//时钟 
	{"系统设置","系統設置","SYSTEM SET",},
	{"","","",},
	{"","","",},	//记事本
	{"","","",},
	{"","","",},	//手写画笔
	{"","","",},	//照相机
	{"","","",},
	{"","","",}, 
	{"","","",},
	{"","","",},
	{"","","",},
	{"","","",},	//电话
	{"","","",},
	{"","","",},
};

u8*const APP_OK_PIC="1:/SYSTEM/APP/COMMON/ok.bmp";				//确认图标
u8*const APP_CANCEL_PIC="1:/SYSTEM/APP/COMMON/cancel.bmp";		//取消图标
u8*const APP_UNSELECT_PIC="1:/SYSTEM/APP/COMMON/unselect.bmp";	//未选中图标
u8*const APP_SELECT_PIC="1:/SYSTEM/APP/COMMON/select.bmp";		//选中图标
u8*const APP_VOL_PIC="1:/SYSTEM/APP/COMMON/VOL.bmp";			//音量图片路径

//PC2LCD2002字体取模方法:逐列式,顺向(高位在前),阴码.C51格式.																		    
//特殊字体:
//数码管字体:ASCII集+℃(' '+95)(28~60字体)
//数码管字体:ASCII集+°(' '+95)(72~144字体,字体格式:BatangChe)
//普通字体:ASCII集

u8*const APP_ASCII_S14472="1:/SYSTEM/APP/COMMON/fonts144.fon";	//数码管字体144*72大数字字体路径 
u8*const APP_ASCII_S8844="1:/SYSTEM/APP/COMMON/fonts88.fon";	//数码管字体88*44大数字字体路径 
u8*const APP_ASCII_S7236="1:/SYSTEM/APP/COMMON/fonts72.fon";	//数码管字体72*36大数字字体路径 
u8*const APP_ASCII_S6030="1:/SYSTEM/APP/COMMON/fonts60.fon";	//数码管字体60*30大数字字体路径 
u8*const APP_ASCII_5427="1:/SYSTEM/APP/COMMON/font54.fon";		//普通字体54*27大数字字体路径 
u8*const APP_ASCII_3618="1:/SYSTEM/APP/COMMON/font36.fon";		//普通字体36*18大数字字体路径
u8*const APP_ASCII_2814="1:/SYSTEM/APP/COMMON/font28.fon";		//普通字体28*14大数字字体路径 

u8* asc2_14472=0;	//普通144*72大字体点阵集
u8* asc2_8844=0;	//普通字体88*44大字体点阵集
u8* asc2_7236=0;	//普通字体72*36大字体点阵集
u8* asc2_s6030=0;	//数码管字体60*30大字体点阵集
u8* asc2_5427=0;	//普通字体54*27大字体点阵集
u8* asc2_3618=0;	//普通字体36*18大字体点阵集
u8* asc2_2814=0;	//普通字体28*14大字体点阵集 






//ALIENTEK logo 图标(18*24大小)
//PCtoLCD2002取模方式:阴码,逐行式,顺向
const u8 APP_ALIENTEK_ICO1824[]=
{
0x03,0xF0,0x00,0x0F,0xF8,0x00,0x1F,0xFC,0x00,0x3F,0xFE,0x00,0x7F,0xFF,0x00,0x7F,
0xFF,0x00,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,
0x80,0xFF,0xFF,0x80,0xCF,0xF9,0x80,0xC7,0xF1,0x80,0x43,0xE1,0x00,0x61,0xC3,0x00,
0x31,0xC6,0x00,0x39,0xCE,0x00,0x1F,0xFC,0x00,0x0F,0xF8,0x00,0x07,0xF0,0x00,0x03,
0xE0,0x00,0x01,0xC0,0x00,0x00,0x80,0x00
};
//ALIENTEK logo 图标(24*32大小)
//PCtoLCD2002取模方式:阴码,逐行式,顺向
const u8 APP_ALIENTEK_ICO2432[]=
{
0x00,0x7F,0x00,0x01,0xFF,0xC0,0x03,0xFF,0xE0,0x07,0xFF,0xF0,0x0F,0xFF,0xF8,0x1F,
0xFF,0xFC,0x1F,0xFF,0xFC,0x3F,0xFF,0xFE,0x3F,0xFF,0xFE,0x7F,0xFF,0xFF,0x7F,0xFF,
0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,
0x73,0xFF,0xE7,0x70,0xFF,0x87,0x30,0xFF,0x86,0x30,0x7F,0x06,0x38,0x3E,0x0E,0x1C,
0x3E,0x1C,0x1E,0x1C,0x3C,0x0F,0x1C,0x78,0x07,0xFF,0xF0,0x07,0xFF,0xF0,0x03,0xFF,
0xE0,0x01,0xFF,0xC0,0x00,0xFF,0x80,0x00,0x7F,0x00,0x00,0x3E,0x00,0x00,0x1C,0x00,
};
//ALIENTEK logo 图标(36*48大小)
//PCtoLCD2002取模方式:阴码,逐行式,顺向
const u8 APP_ALIENTEK_ICO3648[]=
{
0x00,0x01,0xFC,0x00,0x00,0x00,0x0F,0xFF,0x80,0x00,0x00,0x1F,0xFF,0xC0,0x00,0x00,
0x7F,0xFF,0xF0,0x00,0x00,0xFF,0xFF,0xF8,0x00,0x01,0xFF,0xFF,0xFC,0x00,0x03,0xFF,
0xFF,0xFE,0x00,0x07,0xFF,0xFF,0xFF,0x00,0x07,0xFF,0xFF,0xFF,0x80,0x0F,0xFF,0xFF,
0xFF,0x80,0x0F,0xFF,0xFF,0xFF,0xC0,0x1F,0xFF,0xFF,0xFF,0xC0,0x1F,0xFF,0xFF,0xFF,
0xC0,0x1F,0xFF,0xFF,0xFF,0xC0,0x1F,0xFF,0xFF,0xFF,0xE0,0x1F,0xFF,0xFF,0xFF,0xE0,
0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,
0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,
0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3C,0xFF,0xFF,0xFD,0xE0,0x38,0x3F,0xFF,
0xF0,0xE0,0x18,0x1F,0xFF,0xC0,0xE0,0x1C,0x0F,0xFF,0x80,0xC0,0x1C,0x07,0xFF,0x01,
0xC0,0x0C,0x03,0xFF,0x01,0xC0,0x0E,0x03,0xFE,0x03,0x80,0x07,0x01,0xFC,0x07,0x80,
0x07,0x81,0xFC,0x07,0x00,0x03,0xC0,0xFC,0x0F,0x00,0x03,0xE0,0xF8,0x3E,0x00,0x01,
0xF0,0xF8,0x7E,0x00,0x01,0xFE,0xFD,0xFC,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x7F,
0xFF,0xF8,0x00,0x00,0x3F,0xFF,0xF0,0x00,0x00,0x3F,0xFF,0xE0,0x00,0x00,0x1F,0xFF,
0xC0,0x00,0x00,0x0F,0xFF,0x80,0x00,0x00,0x07,0xFF,0x00,0x00,0x00,0x03,0xFE,0x00,
0x00,0x00,0x01,0xFC,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
////////////////////////////////伪随机数产生办法////////////////////////////////
u32 random_seed=1;
void app_srand(u32 seed)
{
	random_seed=seed;
}
//获取伪随机数
//可以产生0~RANDOM_MAX-1的随机数
//seed:种子
//max:最大值	  		  
//返回值:0~(max-1)中的一个值 		
u32 app_get_rand(u32 max)
{			    	    
	random_seed=random_seed*22695477+1;
	return (random_seed)%max; 
}  
void opt1_delay(vu8 i)
{
	while(i--);
}
//读取背景色
//x,y,width,height:背景色读取范围
//ctbl:背景色存放指针
void app_read_bkcolor(u16 x,u16 y,u16 width,u16 height,u16 *ctbl)
{
	u32	x0,y0,ccnt;
	ccnt=0;
	for(y0=y;y0<y+height;y0++)
	{
		for(x0=x;x0<x+width;x0++)
		{
			ctbl[ccnt]=gui_phy.read_point(x0,y0);//读取颜色
			ccnt++;
			opt1_delay(5);
		}
	}
}  
//恢复背景色
//x,y,width,height:背景色还原范围
//ctbl:背景色存放指针
void app_recover_bkcolor(u16 x,u16 y,u16 width,u16 height,u16 *ctbl)
{
	u32 x0,y0,ccnt;
	ccnt=0;
	for(y0=y;y0<y+height;y0++)
	{
		for(x0=x;x0<x+width;x0++)
		{
			gui_phy.draw_point(x0,y0,ctbl[ccnt]);//读取颜色
			ccnt++;
		}
	}
}
//2色条
//x,y,width,height:坐标及尺寸.
//mode:	设置分界线
//	    [3]:右边分界线
//		[2]:左边分界线
//		[1]:下边分界线
//		[0]:上边分界线
void app_gui_tcbar(u16 x,u16 y,u16 width,u16 height,u8 mode)
{
 	u16 halfheight=height/2;
 	gui_fill_rectangle(x,y,width,halfheight,LIGHTBLUE);  			//填充底部颜色(浅蓝色)	
 	gui_fill_rectangle(x,y+halfheight,width,halfheight,GRAYBLUE); 	//填充底部颜色(灰蓝色)
	if(mode&0x01)gui_draw_hline(x,y,width,DARKBLUE);
	if(mode&0x02)gui_draw_hline(x,y+height-1,width,DARKBLUE);
	if(mode&0x04)gui_draw_vline(x,y,height,DARKBLUE);
	if(mode&0x08)gui_draw_vline(x+width-1,y,width,DARKBLUE);
} 
//得到数字的位数
//num:数字
//dir:0,从高位到低位.1,从低位到高位.
//返回值:数字的位数.(最大为10位)
//注:0,的位数为0位.
u8 app_get_numlen(long long num,u8 dir)
{
#define MAX_NUM_LENTH		10		//最大的数字长度
	u8 i=0,j;
	u8 temp=0;  
	if(dir==0)//从高到底
	{
		i=MAX_NUM_LENTH-1;
		while(1)
		{
			temp=(num/gui_pow(10,i))%10;
			if(temp||i==0)break;
			i--;
		}
	}else	//从低到高
	{
		j=0;
 		while(1)
		{
			if(num%10)
			{
				i=app_get_numlen(num,0);    
				return i;
			}
			if(j==(MAX_NUM_LENTH-1))break;	 
			num/=10;
			j++;
		}
	}
	if(i)return i+1;
	else if(temp)return 1;
	else return 0;	
}


//显示单色图标
//x,y,width,height:坐标及尺寸.
//icosbase:点整位置
//color:画点的颜色.
//bkcolor:背景色
void app_show_mono_icos(u16 x,u16 y,u8 width,u8 height,u8 *icosbase,u16 color,u16 bkcolor)
{
	u16 rsize;
	u8 i,j;
	u8 temp;
	u8 t=0;
	u16 x0=x;//保留x的位置
	rsize=width/8+((width%8)?1:0);//每行的字节数
 	for(i=0;i<rsize*height;i++)
	{
		temp=icosbase[i];
		for(j=0;j<8;j++)
		{
	        if(temp&0x80)gui_phy.draw_point(x,y,color);  
			else gui_phy.draw_point(x,y,bkcolor);  	 
			temp<<=1;
			x++;
			t++;			//宽度计数器
			if(t==width)
			{
				t=0;
				x=x0;
				y++;  
				break;
			}
		}
	}				
} 
//显示一个浮点数(支持负数)
//注意:这里的坐标是从右到左显示的.
//x,y:开始显示的坐标
//num:数字	   
//flen:小数的位数.0,没有小数部分;1,有一位小数;2,有两位小数;其他以此类推.
//clen:要清除的位数(清除前一次的痕迹)
//font:字体
//color:字体颜色.
//bkcolor:背景色
void app_show_float(u16 x,u16 y,long long num,u8 flen,u8 clen,u8 font,u16 color,u16 bkcolor)
{			   
	u8 offpos=1;
	u8 ilen=0;						//整数部分长度和小数部分的长度
	u8 negative=0;					//负数标记
	u16 maxlen=(u16)clen*(font/2);	//清除的长度
	gui_fill_rectangle(x-maxlen,y,maxlen,font,bkcolor);//清除之前的痕迹
	if(num<0) 
	{
		num=-num;
  		negative=1;
	}
	ilen=app_get_numlen(num,0);	//得到总位数
	gui_phy.back_color=bkcolor;
	if(flen)
	{
 		gui_show_num(x-(font/2)*flen,y,flen,color,font,num,0X80);//显示小数部分
		gui_show_ptchar(x-(font/2)*(flen+1),y,gui_phy.lcdwidth,gui_phy.lcdheight,0,color,font,'.',0);//显示小数点
		offpos=2+flen;
	} 
	if(ilen<=flen)ilen=1;//至少要有一位整数部分
 	else 
	{
		if(flen)offpos=ilen+1;
		else offpos=ilen;
		ilen=ilen-flen;	//得到整数部分的长度.
	}
	num=num/gui_pow(10,flen);//得到整数部分
	gui_show_num(x-(font/2)*offpos,y,ilen,color,font,num,0X80);	//填充0显示 
	if(negative)gui_show_ptchar(x-(font/2)*(offpos+1),y,gui_phy.lcdwidth,gui_phy.lcdheight,0,color,font,'-',0);//显示符号		 
} 			  
//文件浏览横条显示
//topname:浏览的时候要显示的名字	 
//mode:
//[0]:0,不显示上方色条;1,显示上方色条
//[1]:0,不显示下方色条;1,显示下方色条
//[2]:0,不显示名字;1,显示名字
//[3~7]:保留
//返回值:无	 						  
void app_filebrower(u8 *topname,u8 mode)
{		
  	if(mode&0X01)app_gui_tcbar(0,0,lcddev.width,gui_phy.tbheight,0x02);								//下分界线
	if(mode&0X02)app_gui_tcbar(0,lcddev.height-gui_phy.tbheight,lcddev.width,gui_phy.tbheight,0x01);//上分界线
	if(mode&0X04)gui_show_strmid(0,0,lcddev.width,gui_phy.tbheight,WHITE,gui_phy.tbfsize,topname);	  
} 
//在一个区域中间显示数字
//x,y,width,height:区域
//num:要显示的数字
//len:位数
//size:字体尺寸
//ptcolor,bkcolor:画笔颜色以及背景色   
void app_show_nummid(u16 x,u16 y,u16 width,u16 height,u32 num,u8 len,u8 size,u16 ptcolor,u16 bkcolor)
{
	u16 numlen;
	u8 xoff,yoff;
	numlen=(size/2)*len;//数字长度
	if(numlen>width||size>height)return;
	xoff=(width-numlen)/2;
	yoff=(height-size)/2;
	POINT_COLOR=ptcolor;
	BACK_COLOR=bkcolor;
	LCD_ShowxNum(x+xoff,y+yoff,num,len,size,0X80);//显示这个数字
}
//画一条平滑过渡的彩色线(或矩形)
//以中间为间隔,两边展开
//x,y,width,height:线条的坐标尺寸
//sergb,mrgb:起止颜色和中间颜色
void app_draw_smooth_line(u16 x,u16 y,u16 width,u16 height,u32 sergb,u32 mrgb)
{	  
	gui_draw_smooth_rectangle(x,y,width/2,height,sergb,mrgb);	   		//前半段渐变
	gui_draw_smooth_rectangle(x+width/2,y,width/2,height,mrgb,sergb);   //后半段渐变
}      

//判断触摸屏当前值是不是在某个区域内
//tp:触摸屏
//x,y,width,height:坐标和尺寸 
//返回值:0,不在区域内;1,在区域内.
u8 app_tp_is_in_area(_m_tp_dev *tp,u16 x,u16 y,u16 width,u16 height)
{						 	 
	if(tp->x[0]<=(x+width)&&tp->x[0]>=x&&tp->y[0]<=(y+height)&&tp->y[0]>=y)return 1;
	else return 0;							 	
}
//显示条目
//x,y,itemwidth,itemheight:条目坐标及尺寸
//name:条目名字
//icopath:图标路径
void app_show_items(u16 x,u16 y,u16 itemwidth,u16 itemheight,u8*name,u8*icopath,u16 color,u16 bkcolor)
{
  	gui_fill_rectangle(x,y,itemwidth,itemheight,bkcolor);	//填充背景色
	gui_show_ptstr(x+5,y+(itemheight-16)/2,x+itemwidth-10-APP_ITEM_ICO_SIZE-5,y+itemheight,0,color,16,name,1);	//显示条目名字
	if(icopath)minibmp_decode(icopath,x+itemwidth-10-APP_ITEM_ICO_SIZE,y+(itemheight-APP_ITEM_ICO_SIZE)/2,APP_ITEM_ICO_SIZE,APP_ITEM_ICO_SIZE,0,0);			//解码APP_ITEM_ICO_SIZE*APP_ITEM_ICO_SIZE的bmp图片
}		 
//获得当前条目的图标路径
//mode:0,单选模式;1,多选模式
//selpath,unselpath:选中和非选中的图标路径
//selx:选中图标编号(单选模式)/有效图标掩码(多选模式)
//index:当前条目编号
u8 * app_get_icopath(u8 mode,u8 *selpath,u8 *unselpath,u8 selx,u8 index)
{
	u8 *icopath=0;
	if(mode)//多选模式
	{
		if(selx&(1<<index))icopath=selpath;	//是选中的条目
		else icopath=unselpath;			  	//是非选中的条目
	}else
	{
		if(selx==index)icopath=selpath;		//是选中的条目
		else icopath=unselpath;			  	//是非选中的条目
	}
	return icopath;
}
//条目选择
//x,y,width,height:坐标尺寸(width最小为150,height最小为72)
//items[]:条目名字集
//itemsize:总条目数(最大不超过8个)
//selx:结果.多选模式时,对应各项的选择情况.单选模式时,对应选择的条目.
//mode:
//[7]:0,无OK按钮;1,有OK按钮
//[6]:0,不读取背景色;1,读取背景色
//[5]:0,单选模式;1,多选模式
//[4]:0,不加载图表;1,加载图标
//[3:0]:保留
//caption:窗口名字	  
//返回值:0,ok;其他,取消或者错误.
u8 app_items_sel(u16 x,u16 y,u16 width,u16 height,u8 *items[],u8 itemsize,u8 *selx,u8 mode,u8*caption) 
{
	u8 rval=0,res;
	u8 selsta=0;	//选中状态为0,
					//[7]:标记是否已经记录第一次按下的条目;
					//[6:4]:保留
	                //[3:0]:第一次按下的条目
	u16 i;

	u8 temp;
	u16 itemheight=0;		//每个条目的高度
	u16 itemwidth=0;		//每个条目的宽度
	u8* unselpath=0;		//未选中的图标的路径
	u8* selpath=0;			//选中图标的路径
	u8* icopath=0;

 	_window_obj* twin=0;	//窗体
 	_btn_obj * okbtn=0;		//确定按钮
 	_btn_obj * cancelbtn=0; //取消按钮

	if(itemsize>8||itemsize<1)return 0xff;	//条目数错误
	if(width<150||height<72)return 0xff; 	//尺寸错误
	
	itemheight=(height-72)/itemsize-1;	//得到每个条目的高度
	itemwidth=width-10;					//每个条目的宽度

 	twin=window_creat(x,y,width,height,0,1|(1<<5)|((1<<6)&mode),16);//创建窗口
	if(twin==NULL)
	{
		spb_delete();//释放SPB占用的内存
		twin=window_creat(x,y,width,height,0,1|(1<<5)|((1<<6)&mode),16);//重新创建窗口
 	}
  	if(mode&(1<<7))
	{
   		temp=(width-APP_ITEM_BTN1_WIDTH*2)/3;
		okbtn=btn_creat(x+temp,y+height-APP_ITEM_BTN_HEIGHT-5,APP_ITEM_BTN1_WIDTH,APP_ITEM_BTN_HEIGHT,0,0x02);							//创建OK按钮
		cancelbtn=btn_creat(x+APP_ITEM_BTN1_WIDTH+temp*2,y+height-APP_ITEM_BTN_HEIGHT-5,APP_ITEM_BTN1_WIDTH,APP_ITEM_BTN_HEIGHT,0,0x02);//创建CANCEL按钮
		if(twin==NULL||okbtn==NULL||cancelbtn==NULL)rval=1;
		else
		{
	 		okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];//确认
			okbtn->bkctbl[0]=0X8452;//边框颜色
			okbtn->bkctbl[1]=0XAD97;//第一行的颜色				
			okbtn->bkctbl[2]=0XAD97;//上半部分颜色
			okbtn->bkctbl[3]=0X8452;//下半部分颜色
		}
	}else 
	{
   		temp=(width-APP_ITEM_BTN2_WIDTH)/2;
		cancelbtn=btn_creat(x+temp,y+height-APP_ITEM_BTN_HEIGHT-5,APP_ITEM_BTN2_WIDTH,APP_ITEM_BTN_HEIGHT,0,0x02);	//创建CANCEL按钮
		if(twin==NULL||cancelbtn==NULL)rval=1;
	}
 	if(rval==0)//之前的操作正常
	{
 		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	     
 		cancelbtn->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];//取消
 		cancelbtn->bkctbl[0]=0X8452;//边框颜色
		cancelbtn->bkctbl[1]=0XAD97;//第一行的颜色				
		cancelbtn->bkctbl[2]=0XAD97;//上半部分颜色
		cancelbtn->bkctbl[3]=0X8452;//下半部分颜色

		if(mode&(1<<4))//需要加载图标
		{
  			if(mode&(1<<5))//多选模式
			{
				unselpath=(u8*)APP_CANCEL_PIC;		//未选中的图标的路径
				selpath=(u8*)APP_OK_PIC;			//选中图标的路径
			}else		   //单选模式
			{
				unselpath=(u8*)APP_UNSELECT_PIC;	//未选中的图标的路径
				selpath=(u8*)APP_SELECT_PIC;		//选中图标的路径
			}
		}
		window_draw(twin);				//画出窗体
		btn_draw(cancelbtn);			//画按钮
	    if(mode&(1<<7))btn_draw(okbtn);	//画按钮
		for(i=0;i<itemsize;i++)
		{
			icopath=app_get_icopath(mode&(1<<5),selpath,unselpath,*selx,i); //得到图标路径
			app_show_items(x+5,y+32+i*(itemheight+1),itemwidth,itemheight,items[i],icopath,BLACK,twin->windowbkc);//显示所有的条目
			if((i+1)!=itemsize)app_draw_smooth_line(x+5,y+32+(i+1)*(itemheight+1)-1,itemwidth,1,0Xb1ffc4,0X1600b1);//画彩线
 		}
		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
			delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
			if(system_task_return){rval=1;break;};	//TPAD返回	
			if(mode&(1<<7))
			{
				res=btn_check(okbtn,&in_obj);		//确认按钮检测
				if(res)
				{
					if((okbtn->sta&0X80)==0)//有有效操作
					{
						rval=0XFF;
						break;//确认按钮
					}
				}
			}   
			res=btn_check(cancelbtn,&in_obj);		//取消按钮检测
			if(res)
			{
				if((cancelbtn->sta&0X80)==0)//有有效操作
				{
					rval=1;
					break;//取消按钮	 
				}
			}
			temp=0XFF;//标记量,如果为0XFF,在松开的时候,说明是不在有效区域内的.如果非0XFF,则表示TP松开的时候,是在有效区域内.
			for(i=0;i<itemsize;i++)
			{
				if(tp_dev.sta&TP_PRES_DOWN)//触摸屏被按下
				{
				 	if(app_tp_is_in_area(&tp_dev,x+5,y+32+i*(itemheight+1),itemwidth,itemheight))//判断某个时刻,触摸屏的值是不是在某个区域内
					{ 
						if((selsta&0X80)==0)//还没有按下过
						{
							icopath=app_get_icopath(mode&(1<<5),selpath,unselpath,*selx,i); //得到图标路径
							app_show_items(x+5,y+32+i*(itemheight+1),itemwidth,itemheight,items[i],icopath,BLACK,APP_ITEM_SEL_BKCOLOR);//反选条目
							selsta=i;		//记录第一次按下的条目
							selsta|=0X80;	//标记已经按下过了
						}
						break;		
					}
				}else //触摸屏被松开了
				{
				 	if(app_tp_is_in_area(&tp_dev,x+5,y+32+i*(itemheight+1),itemwidth,itemheight))//判断某个时刻,触摸屏的值是不是在某个区域内
					{ 
						temp=i;	   
						break;
					}
				}
			}
			if((selsta&0X80)&&(tp_dev.sta&TP_PRES_DOWN)==0)//有按下过,且按键松开了
			{
				if((selsta&0X0F)==temp)//松开之前的坐标也是在按下时的区域内.
				{
					if(mode&(1<<5))//多选模式,执行取反操作
					{
						if((*selx)&(1<<temp))*selx&=~(1<<temp);
						else *selx|=1<<temp;
					}else//单选模式
					{																					  
						app_show_items(x+5,y+32+(*selx)*(itemheight+1),itemwidth,itemheight,items[*selx],unselpath,BLACK,twin->windowbkc);//取消之前选择的条目
						*selx=temp;
					}
				}else temp=selsta&0X0F;//得到当时按下的条目号
 				icopath=app_get_icopath(mode&(1<<5),selpath,unselpath,*selx,temp); //得到图标路径
				app_show_items(x+5,y+32+temp*(itemheight+1),itemwidth,itemheight,items[temp],icopath,BLACK,twin->windowbkc);//反选条目
				selsta=0;//取消
			}
 		}
 	}
	window_delete(twin);
	btn_delete(okbtn);
	btn_delete(cancelbtn);
	system_task_return=0;
	if(rval==0XFF)return 0;
	return rval;
} 
  
//新建选择框
//sel:当前选中的条目
//top:当前最顶部的条目
//caption:选择框名字
//items[]:条目名字集
//itemsize:总条目数
//返回值:[7]:0,按下的是返回按键,或者发生了错误;1,按下了确认按键,或者双击了选中的条目.
//     [6:0]:0,表示无错误;非零,错误代码.
u8 app_listbox_select(u8 *sel,u8 *top,u8 * caption,u8 *items[],u8 itemsize)
{
	u8  res;
	u8 rval=0;			//返回值	  
  	u16 i;	    						   
 
 	_btn_obj* rbtn=0;			//返回按钮控件
 	_btn_obj* okbtn=0;			//确认按钮控件
	_listbox_obj * tlistbox;	//listbox 
	
	if(*sel>=itemsize||*top>=itemsize)return 2;//参数错误/参数非法
 	app_filebrower(caption,0X07);				//显示标题  
   	tlistbox=listbox_creat(0,gui_phy.tbheight,lcddev.width,lcddev.height-gui_phy.tbheight*2,1,gui_phy.tbfsize);//创建一个filelistbox
	if(tlistbox==NULL)rval=1;					//申请内存失败.
	else	//添加条目
	{
		for(i=0;i<itemsize;i++)
		{
			res=listbox_addlist(tlistbox,items[i]);
			if(res)
			{
				rval=1;
				break;
			}
		}
	}
	if(rval==0)//成功添加了条目
	{
		tlistbox->scbv->topitem=*top;
		tlistbox->selindex=*sel;
		listbox_draw_listbox(tlistbox);
		rbtn=btn_creat(lcddev.width-2*gui_phy.tbfsize-8-1,lcddev.height-gui_phy.tbheight,2*gui_phy.tbfsize+8,gui_phy.tbheight-1,0,0x03);//创建文字按钮
 		okbtn=btn_creat(0,lcddev.height-gui_phy.tbheight,2*gui_phy.tbfsize+8,gui_phy.tbheight-1,0,0x03);//创建确认文字按钮
		if(rbtn==NULL||okbtn==NULL)rval=1;		//没有足够内存够分配
		else
		{
		 	rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];	//名字
			rbtn->font=gui_phy.tbfsize;//设置新的字体大小	 	 
			rbtn->bcfdcolor=WHITE;	//按下时的颜色
			rbtn->bcfucolor=WHITE;	//松开时的颜色
			btn_draw(rbtn);//画按钮
	
			
		 	okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];	//名字
		 	okbtn->font=gui_phy.tbfsize;//设置新的字体大小	 
			okbtn->bcfdcolor=WHITE;	//按下时的颜色
			okbtn->bcfucolor=WHITE;	//松开时的颜色
			btn_draw(okbtn);//画按钮
		}	   
 	}  
   	while(rval==0)
	{
		tp_dev.scan(0);    
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
		delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
 		if(system_task_return)break;			//TPAD返回
		res=btn_check(rbtn,&in_obj);		    //返回按钮检测
		if(res)if(((rbtn->sta&0X80)==0))break;	//退出 
		res=btn_check(okbtn,&in_obj);//确认按钮检测
		if(res)
		{
			if(((okbtn->sta&0X80)==0))//按钮状态改变了
			{ 
				*top=tlistbox->scbv->topitem;//记录退出时选择的条目
				*sel=tlistbox->selindex;
				rval|=1<<7;//标记双击按下了								 				   			   
  			}	 
		}   
		listbox_check(tlistbox,&in_obj);	//扫描 
		if(tlistbox->dbclick==0X80)//双击了
		{	  
			*top=tlistbox->scbv->topitem;//记录退出时选择的条目
			*sel=tlistbox->selindex;
			rval|=1<<7;//标记双击按下了								 				   			   
		}
	}	
	listbox_delete(tlistbox);		//删除listbox
	btn_delete(okbtn);				//删除按钮	  	 
	btn_delete(rbtn);				//删除按钮 
	return rval; 
}

//LWIP Web Server支持所需文件
//u8*const lwip_webserver_path_tbl[11]=
//{
//"1:/SYSTEM/LWIP/WebServer/404.html",
//"1:/SYSTEM/LWIP/WebServer/index.html",	 
//"1:/SYSTEM/LWIP/WebServer/STM32_ADC_TEMPERATE.shtml",	
//"1:/SYSTEM/LWIP/WebServer/STM32_LED_OFF_BEEP_OFF.shtml",	
//"1:/SYSTEM/LWIP/WebServer/STM32_LED_OFF_BEEP_ON.shtml",	
//"1:/SYSTEM/LWIP/WebServer/STM32_LED_ON_BEEP_OFF.shtml",	
//"1:/SYSTEM/LWIP/WebServer/STM32_LED_ON_BEEP_ON.shtml",	
//"1:/SYSTEM/LWIP/WebServer/STM32_RTC.shtml",	
//"1:/SYSTEM/LWIP/WebServer/image/head.jpg",
//"1:/SYSTEM/LWIP/WebServer/image/stm32.jpg",	
//"1:/SYSTEM/LWIP/WebServer/image/tab.jpg",	 	
//};
//将磁盘路径按要求修改
//des:目标缓存区
//diskx:新的磁盘编号
//src:原路径
void app_system_file_chgdisk(u8 *des,u8 *diskx,u8 *src)
{ 
	//static u8 t=0;
	strcpy((char*)des,(const char*)src);
	des[0]=diskx[0]; 
	//printf("path[%d]:%s\r\n",t++,des);	//打印文件名
}
//检查磁盘下面的某个文件
//diskx:磁盘编号
//fname:文件名(带路径)
//返回值:0,正常
//      1,内存错误
u8 app_file_check(u8* diskx,u8 *fname)
{
	FIL *f_check;
	u8 *path;
	u8 res=0;
	f_check=(FIL *)gui_memin_malloc(sizeof(FIL));	//开辟FIL字节的内存区域 
	path=gui_memin_malloc(200);						//为path申请内存 
	if(!f_check||!path)//申请失败
	{
		gui_memin_free(f_check);
		gui_memin_free(path);
		return 1;
	}
	app_system_file_chgdisk(path,diskx,fname);		//修改路径
	res=f_open(f_check,(const TCHAR*)path,FA_READ);	//只读方式打开文件 
	f_close(f_check);								//关闭文件   
	gui_memin_free(f_check);
	gui_memin_free(path);
	return res;
}
//系统总共需要的图标/图片/系统文件有193个  
//总大小:10,313,328 字节(9.83MB)
//检测所有的系统文件是否正常
//返回值:0,正常
//    其他,错误代码
u8 app_system_file_check(u8* diskx)
{
	u8 i,j;
	u8 rval=0;
	u8 res=0; 
	FIL *f_check;
	u8 *path;
	f_check=(FIL *)gui_memin_malloc(sizeof(FIL));	//开辟FIL字节的内存区域 
	path=gui_memin_malloc(200);						//为path申请内存 
	if(!f_check||!path)rval=0XFF;//申请失败
	while(1)
	{	
//		//检测5个SPB背景图片
//		for(j=0;j<6;j++)
//		{ 
//			res=app_file_check(diskx,spb_bkpic_path_tbl[j]); 
//			rval++;  
//			if(res)break;		//检查错误
//		}
//		if(rval!=6)break;
//		//检测3*SPB_ICOS_NUM个SPB界面图标
//		for(i=0;i<3;i++)
//		{
//			for(j=0;j<SPB_ICOS_NUM;j++)
//			{
//				res=app_file_check(diskx,spb_icos_path_tbl[i][j]); 
//				if(res)break;	//检查错误
//				rval++;
//			} 
//		}
//		if(rval!=6+3*SPB_ICOS_NUM)break;
//		//检测9个SPB界面图标
//		for(j=0;j<3;j++)
//		{
//			for(i=0;i<3;i++)
//			{
//				res=app_file_check(diskx,spb_micos_path_tbl[j][i]); 
//				if(res)break;	//检查错误 
//				rval++;
//			}
//			if(res)break;
//		}
		if(j!=3&&i!=3)break;		
		//APP/COMMON文件夹内12个文件检测
		res=app_file_check(diskx,APP_OK_PIC); 
		if(res)break;			//检查错误    
		rval++;		
		res=app_file_check(diskx,APP_CANCEL_PIC); 
		if(res)break;			//检查错误    
		rval++;		
		res=app_file_check(diskx,APP_UNSELECT_PIC); 
		if(res)break;			//检查错误    
		rval++;		
		res=app_file_check(diskx,APP_SELECT_PIC); 
		if(res)break;			//检查错误    
		rval++;		
		res=app_file_check(diskx,APP_VOL_PIC); 
		if(res)break;			//检查错误    
		rval++;		
		res=app_file_check(diskx,APP_ASCII_S14472); 
		if(res)break;			//检查错误    
		rval++;		
		res=app_file_check(diskx,APP_ASCII_S8844); 
		if(res)break;			//检查错误    
		rval++;		
		res=app_file_check(diskx,APP_ASCII_S7236); 
		if(res)break;			//检查错误    
		rval++;		
		res=app_file_check(diskx,APP_ASCII_S6030); 
		if(res)break;			//检查错误    
		rval++;		
		res=app_file_check(diskx,APP_ASCII_5427); 
		if(res)break;			//检查错误    
		rval++;		
		res=app_file_check(diskx,APP_ASCII_3618); 
		if(res)break;			//检查错误    
		rval++;		
		res=app_file_check(diskx,APP_ASCII_2814); 
		if(res)break;			//检查错误    
		rval++;		
//		//APP/AUDIO 文件夹内15个文件检测
//		for(j=0;j<2;j++)
//		{
//			for(i=0;i<5;i++)
//			{
//				res=app_file_check(diskx,AUDIO_BTN_PIC_TBL[j][i]); 
//				if(res)break;	//检查错误      
//				rval++;
//			}
//			if(res)break;
//		}
//		if(j!=2&&i!=5)break;
//		for(j=0;j<6;j++)
//		{ 
//			res=app_file_check(diskx,AUDIO_BACK_PIC[j]); 
//			if(res)break;		//检查错误    
//			rval++; 
//		}
//		if(j!=6)break;	
//		res=app_file_check(diskx,AUDIO_PLAYR_PIC); 
//		if(res)break;			//检查错误    
//		rval++;		
//		res=app_file_check(diskx,AUDIO_PLAYP_PIC); 
//		if(res)break;			//检查错误    
//		rval++;		
//		res=app_file_check(diskx,AUDIO_PAUSER_PIC); 
//		if(res)break;			//检查错误    
//		rval++;		
//		res=app_file_check(diskx,AUDIO_PAUSEP_PIC); 
//		if(res)break;			//检查错误    
//		rval++;	 
		//APP/PHONE 文件夹内9个文件检测
// 		for(j=0;j<3;j++)
//		{ 
//			res=app_file_check(diskx,PHONE_CALL_PIC[j]); 
//			if(res)break;		//检查错误   
//			rval++; 
//		}
//		if(j!=3)break;
// 		for(j=0;j<3;j++)
//		{ 
//			res=app_file_check(diskx,PHONE_HANGUP_PIC[j]); 
//			if(res)break;		//检查错误   
//			rval++; 
//		}
//		if(j!=3)break;		
// 		for(j=0;j<3;j++)
//		{ 
//			res=app_file_check(diskx,PHONE_DEL_PIC[j]); 
//			if(res)break;		//检查错误    
//			rval++; 
//		}
//		if(j!=3)break;	 
//		//检测录音机7个图标
//		
//		res=app_file_check(diskx,RECORDER_DEMO_PIC); 
//		if(res)break;			//检查错误    
//		rval++;		
//		res=app_file_check(diskx,RECORDER_RECR_PIC); 
//		if(res)break;			//检查错误    
//		rval++;		
//		res=app_file_check(diskx,RECORDER_RECP_PIC); 
//		if(res)break;			//检查错误    
//		rval++;		
//		res=app_file_check(diskx,RECORDER_PAUSER_PIC); 
//		if(res)break;			//检查错误    
//		rval++;		
//		res=app_file_check(diskx,RECORDER_PAUSEP_PIC); 
//		if(res)break;			//检查错误    
//		rval++;		
//		res=app_file_check(diskx,RECORDER_STOPR_PIC); 
//		if(res)break;			//检查错误    
//		rval++;		
//		res=app_file_check(diskx,RECORDER_STOPP_PIC); 
//		if(res)break;			//检查错误    
//		rval++;		 
		//检测应用程序48个图标
//		for(j=0;j<3;j++)
//		{
//			for(i=0;i<16;i++)
//			{
//				res=app_file_check(diskx,appplay_icospath_tbl[j][i]); 
//				if(res)break;	//检查错误     
//				rval++;
//			}
//			if(res)break;
//		}
//		if(j!=3&&i!=16)break;    
// 		//检测画图1个图标
//		res=app_file_check(diskx,PAINT_COLOR_TBL_PIC); 
//		if(res)break;			//检查错误    
//		rval++;		  
//		//LWIP/WebServer文件夹内文件统计
// 		for(j=0;j<11;j++)
//		{ 
//			res=app_file_check(diskx,lwip_webserver_path_tbl[j]); 
//			if(res)break;		//检查错误     
//			rval++; 
//		}
		printf("\r\ntotal system files:%d\r\n",rval);
		rval=0;
		break;    
	}
	if(res)printf("lost file:%s\r\n",path);//打印丢失的文件
	gui_memin_free(f_check);//释放内存
	gui_memin_free(path);	//释放内存
	return rval;  
}

////////////////////////////////////////////////////////////////////////////////////////////
//以下代码仅用于实现SYSTEM文件夹更新
u8*const APP_SYSTEM_APPPATH="/SYSTEM/APP";			//APP文件夹路径 径
u8*const APP_SYSTEM_SPBPATH="/SYSTEM/SPB";			//SPB文件夹路径
u8*const APP_SYSTEM_DSTPATH="1:/SYSTEM";			//系统文件目标路径

u8*const APP_TESTFILE_SRCPATH="/测试用文件";			//测试文件源路径
u8*const APP_TESTFILE_DSTPATH="1:";					//测试文件目标路径

u8*const APP_NAND_TESTFILE_SRCPATH="/NAND测试文件";	//NADN 测试文件源路径
u8*const APP_NAND_TESTFILE_DSTPATH="2:";			//NAND 测试文件目标路径
 
//文件复制信息提示坐标和字体
static u16 cpdmsg_x;
static u16 cpdmsg_y;
static u8  cpdmsg_fsize;	


//系统启动的时候,用于显示更新进度
//*pname:更新文件名字
//pct:百分比
//mode:模式
//[0]:更新文件名
//[1]:更新百分比pct
//[2]:更新文件夹
//[3~7]:保留
//返回值:0,正常;
//       1,结束复制
u8 app_boot_cpdmsg(u8*pname,u8 pct,u8 mode)
{													 
	if(mode&0X01)
	{
		LCD_Fill(cpdmsg_x+9*(cpdmsg_fsize/2),cpdmsg_y,cpdmsg_x+(9+16)*(cpdmsg_fsize/2),cpdmsg_y+cpdmsg_fsize,BLACK);	//填充底色
		LCD_ShowString(cpdmsg_x+9*(cpdmsg_fsize/2),cpdmsg_y,16*(cpdmsg_fsize/2),cpdmsg_fsize,cpdmsg_fsize,pname);	//显示文件名,最长16个字符宽度
		printf("\r\nCopy File:%s\r\n",pname);  
	}
	if(mode&0X04)printf("Copy Folder:%s\r\n",pname);
	if(mode&0X02)//更新百分比
	{
		LCD_ShowString(cpdmsg_x+(9+16+1+3)*(cpdmsg_fsize/2),cpdmsg_y,lcddev.width,lcddev.height,cpdmsg_fsize,"%");		
 		LCD_ShowNum(cpdmsg_x+(9+16+1)*(cpdmsg_fsize/2),cpdmsg_y,pct,3,cpdmsg_fsize);//显示数值
		printf("File Copyed:%d\r\n",pct);
	}
	return 0;	
}
//设置app_boot_cpdmsg的显示坐标
//x,y:坐标.
//fisze:字体大小
void app_boot_cpdmsg_set(u16 x,u16 y,u8 fsize)
{
	cpdmsg_x=x;
	cpdmsg_y=y;
	cpdmsg_fsize=fsize;
}
//系统更新
//fcpymsg:百分比显示函数
//src:文件来源,"0:",SD卡;"3:",U盘.
//返回值:0,正常
//    其他,错误,0XFF强制退出了
u8 app_system_update(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode),u8* src)
{
	u32 totsize=0;
	u32 cpdsize=0;
	u8 res=0;
	u8 *p_app;
	u8 *p_spb; 
	u8 *p_lwip;  
	u8 *p_test;
	u8 *p_nand_test;
	u8 rval;
	p_app=gui_memin_malloc(100);
	p_spb=gui_memin_malloc(100);
//	p_lwip=gui_memin_malloc(100); 
	p_test=gui_memin_malloc(100);
	p_nand_test=gui_memin_malloc(100);
	if(p_nand_test==NULL)
	{
		gui_memin_free(p_app);
		gui_memin_free(p_spb);
		gui_memin_free(p_lwip);
		gui_memin_free(p_test);
		return 1;//内存申请失败
	}
	sprintf((char*)p_app,"%s%s",src,APP_SYSTEM_APPPATH);
	sprintf((char*)p_spb,"%s%s",src,APP_SYSTEM_SPBPATH);
	sprintf((char*)p_test,"%s%s",src,APP_TESTFILE_SRCPATH);
	sprintf((char*)p_nand_test,"%s%s",src,APP_NAND_TESTFILE_SRCPATH);
	totsize+=exf_fdsize(p_app);//得到4个文件夹的总大小
	totsize+=exf_fdsize(p_spb);
//	totsize+=exf_fdsize(p_lwip); 
	totsize+=exf_fdsize(p_test);
	totsize+=exf_fdsize(p_nand_test);
	printf("totsize:%d\r\n",totsize);
	f_mkdir((const TCHAR *)APP_SYSTEM_DSTPATH);//强制创建目标文件夹
 	res=exf_fdcopy(fcpymsg,p_app,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
	if(res)
	{
		if(res!=0xff)rval=1;				//第一阶段复制出错
		else rval=0XFF;						//强制退出
	}else									//第一阶段OK,启动第二阶段
	{
		res=exf_fdcopy(fcpymsg,p_spb,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
		if(res)
		{
			if(res!=0xff)rval=2;			//第二阶段复制出错
			else rval=0XFF;					//强制退出
		}else								//第二阶段OK,启动第三阶段
		{		
			res=exf_fdcopy(fcpymsg,p_lwip,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
			if(res)
			{
				if(res!=0xff)rval=3;		//第三阶段复制出错
				else rval=0XFF;				//强制退出
			}else							//第三阶段OK,启动第四阶段
			{	 
				res=exf_fdcopy(fcpymsg,p_test,(u8*)APP_TESTFILE_DSTPATH,&totsize,&cpdsize,1);
				if(res)
				{
					if(res!=0xff)rval=4;	//第四阶段复制出错
					else rval=0XFF;			//强制退出
				}else
				{
					res=exf_fdcopy(fcpymsg,p_nand_test,(u8*)APP_NAND_TESTFILE_DSTPATH,&totsize,&cpdsize,1);
					if(res)
					{
						if(res!=0xff)rval=5;//第五阶段复制出错
						else rval=0XFF;		//强制退出
					}  
					rval=0;					//第五阶段非强制性，整个过程无错误,无强制退出
				}
			}			 
		}
	}
	gui_memin_free(p_app);
	gui_memin_free(p_spb);
//	gui_memin_free(p_lwip); 
	gui_memin_free(p_test);
	gui_memin_free(p_nand_test);
	return rval;
}
//得到STM32的序列号
//sn0,sn1,sn2:3个固有序列号
void app_getstm32_sn(u32 *sn0,u32 *sn1,u32 *sn2)
{
	*sn0=*(vu32*)(0x1FFF7A10);
	*sn1=*(vu32*)(0x1FFF7A14);
	*sn2=*(vu32*)(0x1FFF7A18);    
}
//打印SN
void app_usmart_getsn(void)
{
	u32 sn0,sn1,sn2;
	app_getstm32_sn(&sn0,&sn1,&sn2);
	printf("\r\nSerial Number:%X%X%X\r\n",sn0,sn1,sn2);
}
//得到版本信息
//buf:版本缓存区(最少需要6个字节,如保存:V1.00)
//ver:版本宏定义
//len:版本位数.1,代表只有1个位的版本,V1;2,代表2个位版本,V1.0;3,代表3个位版本,V1.00;
void app_get_version(u8*buf,u32 ver,u8 len)
{
	u8 i=0;
	buf[i++]='V';
	if(len==1)
	{
		buf[i++]=ver%10+'0';
	}else if(len==2)
	{ 	
		buf[i++]=(ver/10)%10+'0';
		buf[i++]='.';
		buf[i++]=ver%10+'0';
	}else
	{
		buf[i++]=(ver/100)%10+'0';
		buf[i++]='.';
		buf[i++]=(ver/10)%10+'0';
		buf[i++]=ver%10+'0';	 
	}
	buf[i]=0;//加入结束符	 			   
}

//初始化各个系统参数
//包括系统设置,闹钟数据,WM8978数据等
//u8 app_system_parameter_init(void)
//{ 
//	u8 i;
////	sysset_read_para(&systemset);		//读取系统设置信息 
//	wm8978_read_para(&wm8978set);		//读取WM8978设置数据
////  	calendar_read_para(&alarm);			//读取闹钟信息  
//	if(systemset.saveflag!=0X0A||systemset.syslanguage>2)//之前没有保存过/语言信息错误
//	{
//		systemset.syslanguage=0;		//默认为简体中文
// 		systemset.lcdbklight=100;		//背光默认最亮
//		systemset.picmode=0;			//图片顺序播放
//		systemset.audiomode=0;			//音乐播放默认是按顺序播放
//		systemset.videomode=0;			//视频播放默认是按顺序播放 
//		systemset.saveflag=0X0A;  		//设置保存标记
////		sysset_save_para(&systemset);	//保存系统设置信息 
// 	} 
//	if(wm8978set.saveflag!=0X0A)  		//之前没有保存过
//	{
//		wm8978set.mvol=50;				//音量:50
//		for(i=0;i<5;i++)wm8978set.cfreq[i]=0;//中心频率都设置为代号0 
//		wm8978set.d3=0;					//3d效果关闭
//		for(i=0;i<5;i++)wm8978set.freqval[i]=12;//全部设置位0dB增益 
// 		wm8978set.speakersw=1;  		//设置喇叭开启
// 		wm8978set.saveflag=0X0A;  		//设置保存标记
//		wm8978_save_para(&wm8978set);	//保存WM8978的设置信息 
// 	}

////  	gui_phy.language=systemset.syslanguage;	//设置语言
//	app_wm8978_setall();				//设置WM8978参数
//	app_wm8978_volset(0);				//暂时关闭输出
////	TIM3_CH2_PWM_Init(100,9600-1);		//初始化TIM3_CH2 PWM输出,作为LCD的背光控制
//// 	if(systemset.lcdbklight==0)app_lcd_auto_bklight();	//自动背光
////	else if(lcddev.id==0X1963)LCD_SSD_BackLightSet(systemset.lcdbklight);
////	else LCD_BLPWM_VAL=systemset.lcdbklight;			//手动背光
//	return 0;	
//} 
////LCD背光自动控制
//void app_lcd_auto_bklight(void)
//{
//	u16 ir,ps,als; 
//	AP3216C_ReadData(&ir,&ps,&als);
//	if(ps>700)//接近传感器,关闭屏幕背光
//	{
//		if(lcddev.id==0X1963)LCD_SSD_BackLightSet(0);
//		else LCD_BLPWM_VAL=0;		
//	}else
//	{
//		als/=20;//缩小20倍
//		if(als>70)als=70;
//		if(lcddev.id==0X1963)LCD_SSD_BackLightSet(30+als);
//		else LCD_BLPWM_VAL=30+als;
//	} 
//}
//WM8978音量设置
//vol:音量值
void app_wm8978_volset(u8 vol)
{
	if(vol>63)vol=63;
	if(wm8978set.speakersw)WM8978_SPKvol_Set(vol);//开启喇叭
	else WM8978_SPKvol_Set(0);	//关闭喇叭
	WM8978_HPvol_Set(vol,vol);
}
//WM8978 EQ设置
//eqx:频段编号,0~4,对应EQ1~5
//EQ:均衡器，wm8978有5路均衡器
void app_wm8978_eqset(_wm8978_obj *wmset,u8 eqx)
{
	switch(eqx)
	{
		case 0://EQ1设置
			WM8978_EQ1_Set(wmset->cfreq[eqx],wmset->freqval[eqx]);
			break;
		case 1:
			WM8978_EQ2_Set(wmset->cfreq[eqx],wmset->freqval[eqx]);
			break;
		case 2:
			WM8978_EQ3_Set(wmset->cfreq[eqx],wmset->freqval[eqx]);
			break;
		case 3:
			WM8978_EQ4_Set(wmset->cfreq[eqx],wmset->freqval[eqx]);
			break;
		case 4:
			WM8978_EQ5_Set(wmset->cfreq[eqx],wmset->freqval[eqx]);
			break;
	}
}
//设置所有WM8978参数(音频输出部分)
void app_wm8978_setall(void)
{
	u8 i=0;
	app_wm8978_volset(wm8978set.mvol);
	for(i=0;i<5;i++)
	{ 
		app_wm8978_eqset(&wm8978set,i);
	}
	WM8978_3D_Set(wm8978set.d3);
}
//恢复默认设置
//void app_set_default(void)
//{
//	u8 i;
//	//系统设置参数,恢复默认设置
//	systemset.syslanguage=0;		//默认为简体中文
//	systemset.lcdbklight=100;		//背光默认为最亮
//	systemset.picmode=0;			//图片顺序播放
//	systemset.audiomode=0;			//音乐播放默认是按顺序播放
//	systemset.videomode=0;			//视频播放默认是按顺序播放 
//	systemset.saveflag=0X0A;  		//设置保存标记
////	sysset_save_para(&systemset);	//保存系统设置信息  
//	
//	//WM8978设置参数,恢复默认设置
//	wm8978set.mvol=50;				//音量:50
//	for(i=0;i<5;i++)wm8978set.cfreq[i]=0;//中心频率都设置为代号0 
//	wm8978set.d3=0;					//3d效果关闭
//	for(i=0;i<5;i++)wm8978set.freqval[i]=12;//全部设置位0dB增益 
//	wm8978set.speakersw=1;  		//喇叭开启
//	wm8978set.saveflag=0X0A;  		//设置保存标记
//	wm8978_save_para(&wm8978set);	//保存WM8978的设置信息 

//	

//	
//	gui_phy.language=systemset.syslanguage;//设置语言
//	app_wm8978_setall();			//音量全部重新设置
//	if(lcddev.id==0X1963)LCD_SSD_BackLightSet(systemset.lcdbklight);
//	else LCD_BLPWM_VAL=systemset.lcdbklight;//设置背光亮度  

//}










