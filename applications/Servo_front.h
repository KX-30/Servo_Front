#include "stm32f4xx_hal.h"

#ifndef __SERVO_FRONT_H
#define __SERVO_FRONT_H

#define Angle_Start			0
#define Angle_End			90
#define delay				500


typedef enum
{
	Servo_Front_OK,
	Servo_Front_ERROR,
	
}Servo_Front_State;


typedef enum
{
	Servo_Front_1,
	Servo_Front_2,
	Servo_Front_3,
	Servo_Front_4,
	Servo_Front_5,
	
}Servo_Front_Num;

typedef enum
{
	Speed_Fast,
	Speed_Slow,
	
}Servo_360_Speed;

typedef enum
{
	Speed_Clockwise,		//顺时针
	Speed_Anticlockwise,	//逆时针
	
}Servo_360_Direction;



/*---------------------------------------------------------*/



/**
  * @brief          初始化头部的所有舵机并复位
  * @param[]      	none
  * @retval         舵机状态
  */
Servo_Front_State Servo_Front_Init(void);

/**
  * @brief          将头部负责抓取物块的所有舵机臂抬起
  * @param[]      	在头文件的宏定义中修改角度值
  * @retval         舵机状态
  */
Servo_Front_State Servo_Front_All_Up(void);

/**
  * @brief          将头部负责抓取物块的所有舵机臂放下
  * @param[]      	在头文件的宏定义中修改角度值
  * @retval         舵机状态
  */
Servo_Front_State Servo_Front_All_Down(void);

/**
  * @brief          将头部负责抓取物块的单个舵机臂抬起或放下
  * @param[0]      	舵机编号		Servo_Front_Num
  * @param[1]		角度			angle
  * @retval         舵机状态
  */
Servo_Front_State Servo_Front_Single(Servo_Front_Num Servo_Num, uint16_t angle);

/**
  * @brief          旋转360舵机，控制旋转方向和旋转速度
  * @param[0]      	旋转方向		Servo_360_Direction
  * @param[1]		旋转速度		Servo_360_Speed
  * @retval         舵机状态
  */
Servo_Front_State Servo_360(Servo_360_Direction Direction, Servo_360_Speed Speed);

/**
  * @brief          反初始化头部的所有舵机
  * @param[]      	none
  * @retval         舵机状态
  */
Servo_Front_State Servo_Front_Deinit(void);


/*---------------------------------------------------------*/

#endif
