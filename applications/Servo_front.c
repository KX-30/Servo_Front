#include "Servo_front.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;


/**
  * @brief          ��ʼ��ͷ�������ж������λ
  * @param[]      	none
  * @retval         ��ʼ��״̬	Servo_Front_OK��Servo_Front_ERROR
  */
Servo_Front_State Servo_Front_Init(void)
{	
	HAL_StatusTypeDef state1 = HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3 | TIM_CHANNEL_4);
	HAL_StatusTypeDef state2 = HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	
	//��צ���
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 0);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, 0);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);	
	//360���
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 0);
			
	HAL_Delay(delay);
	
	//�����λ
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
  * @brief          ���Ƕ�ת��ΪPWMֵ
  * @param[]      	�Ƕ�ֵ	Angle
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
  * @brief          ��ͷ������ץȡ�������ж����̧��
  * @param[]      	��ͷ�ļ��ĺ궨�����޸ĽǶ�ֵ
  * @retval         ���״̬
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
  * @brief          ��ͷ������ץȡ�������ж���۷���
  * @param[]      	��ͷ�ļ��ĺ궨�����޸ĽǶ�ֵ
  * @retval         ���״̬
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
  * @brief          ��ͷ������ץȡ���ĵ��������̧������
  * @param[0]      	������		Servo_Front_Num
  * @param[1]		�Ƕ�			Angle
  * @retval         ���״̬
  */
Servo_Front_State Servo_Front_Single(Servo_Front_Num Servo_Num, uint16_t angle)
{
	uint32_t TIM_Channel = 0;
    TIM_HandleTypeDef* htim = NULL;

    // ���ݶ���������ͨ���Ͷ�ʱ��
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
            return Servo_Front_ERROR;  // �����������Ч�����ش���
    }

    // ���ö���Ƕ�
    __HAL_TIM_SetCompare(htim, TIM_Channel, __Angle_to_PWM(angle));
	
	HAL_Delay(delay);

    // ���������ɣ����سɹ�״̬
    return Servo_Front_OK;
}

/**
  * @brief          ��ת360�����������ת�������ת�ٶ�
  * @param[0]      	��ת����		Servo_360_Direction
  * @param[1]		��ת�ٶ�		Servo_360_Speed
  * @retval         ���״̬
  */
Servo_Front_State Servo_360(Servo_360_Direction Direction, Servo_360_Speed Speed)
{
	uint16_t pwm;
	
	switch (Direction)
	{
		case Speed_Clockwise:
			 switch(Speed)
			 {
				 case Speed_Fast:		//˳ʱ�����
					 pwm = 50;
				 case Speed_Slow:		//˳ʱ�����
					 pwm = 100;
				 default:
					 pwm = 0;
					 return Servo_Front_ERROR;
			 }
		case Speed_Anticlockwise:
			 switch(Speed)
			 {
				 case Speed_Fast:		//��ʱ�����
					 pwm = 150;
				 case Speed_Slow:		//��ʱ�����
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
  * @brief          ����ʼ��ͷ�������ж��
  * @param[]      	none
  * @retval         ����ʼ��״̬	Servo_Front_OK��Servo_Front_ERROR
  */
Servo_Front_State Servo_Front_Deinit(void)
{
	//�����λ
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, Angle_Start);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, Angle_Start);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, Angle_Start);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, Angle_Start);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, Angle_Start);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, Angle_Start);
	
	//ʧ�ܶ��
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
