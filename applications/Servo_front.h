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
	Speed_Clockwise,		//˳ʱ��
	Speed_Anticlockwise,	//��ʱ��
	
}Servo_360_Direction;



/*---------------------------------------------------------*/



/**
  * @brief          ��ʼ��ͷ�������ж������λ
  * @param[]      	none
  * @retval         ���״̬
  */
Servo_Front_State Servo_Front_Init(void);

/**
  * @brief          ��ͷ������ץȡ�������ж����̧��
  * @param[]      	��ͷ�ļ��ĺ궨�����޸ĽǶ�ֵ
  * @retval         ���״̬
  */
Servo_Front_State Servo_Front_All_Up(void);

/**
  * @brief          ��ͷ������ץȡ�������ж���۷���
  * @param[]      	��ͷ�ļ��ĺ궨�����޸ĽǶ�ֵ
  * @retval         ���״̬
  */
Servo_Front_State Servo_Front_All_Down(void);

/**
  * @brief          ��ͷ������ץȡ���ĵ��������̧������
  * @param[0]      	������		Servo_Front_Num
  * @param[1]		�Ƕ�			angle
  * @retval         ���״̬
  */
Servo_Front_State Servo_Front_Single(Servo_Front_Num Servo_Num, uint16_t angle);

/**
  * @brief          ��ת360�����������ת�������ת�ٶ�
  * @param[0]      	��ת����		Servo_360_Direction
  * @param[1]		��ת�ٶ�		Servo_360_Speed
  * @retval         ���״̬
  */
Servo_Front_State Servo_360(Servo_360_Direction Direction, Servo_360_Speed Speed);

/**
  * @brief          ����ʼ��ͷ�������ж��
  * @param[]      	none
  * @retval         ���״̬
  */
Servo_Front_State Servo_Front_Deinit(void);


/*---------------------------------------------------------*/

#endif
