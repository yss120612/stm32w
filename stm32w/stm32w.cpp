#include <stm32f1xx_hal.h>
#include <stm32_hal_legacy.h>

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
int a = 0;
int main(void)
{
	HAL_Init();

	__GPIOC_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();
	//__TIM2_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	GPIO_InitTypeDef encoder;
	encoder.Mode = GPIO_MODE_AF_INPUT;
	encoder.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	encoder.Pull = GPIO_PULLUP;
	encoder.Speed = GPIO_SPEED_HIGH;
	//encoder. = GPIO_AF2_TIM2;
	HAL_GPIO_Init(GPIOA, &encoder);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_13;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//TIM_Base_InitTypeDef timer;
	TIM_HandleTypeDef timer;
	timer.Instance = TIM2;// Prescaler = 0;
	timer.Init.Period = 0xFFFF;
	timer.Init.CounterMode = TIM_COUNTERMODE_UP | TIM_COUNTERMODE_DOWN;
	timer.Init.Prescaler = 0;
	timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
 
	//HAL_TIM_Base_Init(&timer);
	TIM_Encoder_InitTypeDef enc;
	
	//HAL_TIM_Encoder_Init(&timer, &enc);
	//HAL_TIM_Encoder_Start(&timer, TIM_CHANNEL_ALL);
	enc.EncoderMode = TIM_ENCODERMODE_TI12;
 
	enc.IC1Filter = 0x0F;
	enc.IC1Polarity = TIM_INPUTCHANNELPOLARITY_RISING;
	enc.IC1Prescaler = TIM_ICPSC_DIV4;
	enc.IC1Selection = TIM_ICSELECTION_DIRECTTI;
 
	enc.IC2Filter = 0x0F;
	enc.IC2Polarity = TIM_INPUTCHANNELPOLARITY_FALLING;
	enc.IC2Prescaler = TIM_ICPSC_DIV4;
	enc.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	
	HAL_TIM_Encoder_Init(&timer, &enc);
	HAL_TIM_Encoder_Start_IT(&timer, TIM_CHANNEL_1);
	
	
	for (;;)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		a = TIM2 ->CNT;
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(500);
	}
}
