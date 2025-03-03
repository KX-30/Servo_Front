#include "Servo_front.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;


/**
  * @brief          初始化头部的所有舵机并复位
  * @param[]      	none
  * @retval         初始化状态	Servo_Front_OK，Servo_Front_ERROR
  */
Servo_Front_State Servo_Front_Init(void)
{	
	HAL_StatusTypeDef state1 = HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3 | TIM_CHANNEL_4);
	HAL_StatusTypeDef state2 = HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	
	//夹爪舵机
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 0);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, 0);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);	
	//360舵机
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 0);
			
	HAL_Delay(delay);
	
	//舵机复位
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, Angle_Start);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, Angle_Start);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, Angle_Start);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, Angle_Start);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, Angle_Start);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, Angle_Start);
	
	
	if (state1 == HAL_OK && state2 == HAL_OK)
		return Servo_Front_OK;
	else
		return Servo_Front_ERROR;
}

/**
  * @brief          将角度转换为PWM值
  * @param[]      	角度值	Angle
  * @retval         PWM
  */
uint16_t __Angle_to_PWM(uint16_t Angle)
{
	if (Angle > 180)		Angle = 180;
//	if (Angle < 0)			Angle = 0;
	
	float PWM;	
	PWM = Angle/180.0f * 200.0f + 50.0f;
	
	return (uint16_t)PWM;
}

/**
  * @brief          将头部负责抓取物块的所有舵机臂抬起
  * @param[]      	在头文件的宏定义中修改角度值
  * @retval         舵机状态
  */
Servo_Front_State Servo_Front_All_Up(void)
{
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, __Angle_to_PWM(Angle_Start));
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, __Angle_to_PWM(Angle_Start));
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, __Angle_to_PWM(Angle_Start));	
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, __Angle_to_PWM(Angle_Start));
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, __Angle_to_PWM(Angle_Start));
	
	HAL_Delay(delay);
	
	return Servo_Front_OK;
}

/**
  * @brief          将头部负责抓取物块的所有舵机臂放下
  * @param[]      	在头文件的宏定义中修改角度值
  * @retval         舵机状态
  */
Servo_Front_State Servo_Front_All_Down(void)
{
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, __Angle_to_PWM(Angle_End));
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, __Angle_to_PWM(Angle_End));
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, __Angle_to_PWM(Angle_End));	
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, __Angle_to_PWM(Angle_End));
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, __Angle_to_PWM(Angle_End));
	
	HAL_Delay(delay);
	
	return Servo_Front_OK;
}

/**
  * @brief          将头部负责抓取物块的单个舵机臂抬起或放下
  * @param[0]      	舵机编号		Servo_Front_Num
  * @param[1]		角度			Angle
  * @retval         舵机状态
  */
Servo_Front_State Servo_Front_Single(Servo_Front_Num Servo_Num, uint16_t angle)
{
	uint32_t TIM_Channel = 0;
    TIM_HandleTypeDef* htim = NULL;

    // 根据舵机编号设置通道和定时器
    switch (Servo_Num)
    {
        case Servo_Front_1:
            htim = &htim3;
            TIM_Channel = TIM_CHANNEL_1;
            break;
        case Servo_Front_2:
            htim = &htim3;
            TIM_Channel = TIM_CHANNEL_2;
            break;
        case Servo_Front_3:
            htim = &htim3;
            TIM_Channel = TIM_CHANNEL_3;
            break;
        case Servo_Front_4:
            htim = &htim3;
            TIM_Channel = TIM_CHANNEL_4;
            break;
        case Servo_Front_5:
            htim = &htim4;
            TIM_Channel = TIM_CHANNEL_1;
            break;
        default:
            return Servo_Front_ERROR;  // 如果舵机编号无效，返回错误
    }

    // 设置舵机角度
    __HAL_TIM_SetCompare(htim, TIM_Channel, __Angle_to_PWM(angle));
	
	HAL_Delay(delay);

    // 舵机动作完成，返回成功状态
    return Servo_Front_OK;
}

/**
  * @brief          旋转360舵机，控制旋转方向和旋转速度
  * @param[0]      	旋转方向		Servo_360_Direction
  * @param[1]		旋转速度		Servo_360_Speed
  * @retval         舵机状态
  */
Servo_Front_State Servo_360(Servo_360_Direction Direction, Servo_360_Speed Speed)
{
	uint16_t pwm;
	
	switch (Direction)
	{
		case Speed_Clockwise:
			 switch(Speed)
			 {
				 case Speed_Fast:		//顺时针高速
					 pwm = 50;
				 case Speed_Slow:		//顺时针低速
					 pwm = 100;
				 default:
					 pwm = 0;
					 return Servo_Front_ERROR;
			 }
		case Speed_Anticlockwise:
			 switch(Speed)
			 {
				 case Speed_Fast:		//逆时针高速
					 pwm = 150;
				 case Speed_Slow:		//逆时针低速
					 pwm = 200;
				 default:
					 pwm = 0;
					 return Servo_Front_ERROR;
			 }
	}
	
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, pwm);
	
	HAL_Delay(delay);
	
	return Servo_Front_OK;	
}

/**
  * @brief          反初始化头部的所有舵机
  * @param[]      	none
  * @retval         反初始化状态	Servo_Front_OK，Servo_Front_ERROR
  */
Servo_Front_State Servo_Front_Deinit(void)
{
	//舵机复位
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, Angle_Start);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, Angle_Start);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, Angle_Start);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, Angle_Start);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, Angle_Start);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, Angle_Start);
	
	//失能舵机
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 0);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, 0);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);	
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 0);
			
	HAL_Delay(delay);
	
	HAL_StatusTypeDef s1 = HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	HAL_StatusTypeDef s2 = HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
	HAL_StatusTypeDef s3 = HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
	HAL_StatusTypeDef s4 = HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
	HAL_StatusTypeDef s5 = HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);	
	HAL_StatusTypeDef s6 = HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
	
	if (s1^s2^s3^s4^s5^s6)
		return Servo_Front_ERROR;
	else
		return Servo_Front_OK;
}
