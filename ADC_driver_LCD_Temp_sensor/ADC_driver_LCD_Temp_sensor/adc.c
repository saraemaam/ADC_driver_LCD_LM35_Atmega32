/******************************************************************
 * Module      : ADC
 * File name   : adc.c
 * Description : Analog to digital converter configurable driver
 * Author      : Sarah
 ****************************************************************/

#include"adc.h"
#include"common_macros.h"


#if (ADC_uint8IMPLEMENTATION_BASED == ADC_uint8INTERRUPT_BASED)
/*******************************************************************************
 *                          Global Variables                                   *
 *******************************************************************************/

volatile uint16 g_adcResult = 0;

/*******************************************************************************
 *                          ISR's Definitions                                  *
 *******************************************************************************/

ISR(ADC_vect)
{
	/* Read ADC Data after conversion complete */
#if ADC_u8RESULT_ADJUST == ADC_u8RIGHT_ADJUST

	g_adcResult=ADCL.Byte;
	g_adcResult|=((ADCH.Byte)<<8);

#elif ADC_u8RESULT_ADJUST == ADC_u8LEFT_ADJUST
	g_adcResult = ADCH.Byte ;
#endif


}

#endif

/*
 * Description :
 * Function responsible for initialize the ADC driver.
 */
void ADC_init(void)
{
#if ADC_REF_VOLT == ADC_AREF
	/* Select AREF pin as reference voltage */

	CLEAR_BIT(ADMUX.Byte,7);
	CLEAR_BIT(ADMUX.Byte,6);

	/*Or by using Bit fields
	 * ADMUX.Bits.REFS = 0b00;
	 */

#elif  ADC_REF_VOLT== ADC_AVCC

	/*
	CLEAR_BIT(ADMUX,7);
	SET_BIT(ADMUX,6);
	 */

	/*ADMUX.Bits.REFS0 = 1;*/
	/*Or make 2 bits of REFS */
	ADMUX.Bits.REFS = 0b01;

#elif ADC_REF_VOLT== ADC_INTERNAL_2_56V
	/* Select internal 2.56V as reference voltage */
	/*SET_BIT(ADMUX,7);
	SET_BIT(ADMUX,6);*/
	ADMUX.Bits.REFS = 0b11;
#endif

#if ADC_u8RESULT_ADJUST==ADC_u8RIGHT_ADJUST
	/* Select Right adjustment */
	//CLEAR_BIT(ADMUX,5);
	ADMUX.Bits.ADLAR=0;

#elif ADC_u8RESULT_ADJUST == ADC_u8LEFT_ADJUST
	/* Select Left adjustment */
	//SET_BIT(ADMUX,5);
	ADMUX.Bits.ADLAR=1;

#endif

#if ADC_u8AUTO_TRIGGER == ADC_u8TRIGGER_SOURCE_DISABLED
	/* Disable AUTO Trigger */
	ADCSRA.Bits.ADATE=0;

#elif ADC_u8AUTO_TRIGGER== ADC_u8TRIGGER_SOURCE_FREE_RUN

	/* Enable AUTO Trigger */
	//SET_BIT(ADCSRA , 5);

	ADCSRA.Bits.ADATE=1;

	/* Select Free Run as AUTO Trigger */
	/*CLEAR_BIT(SFIOR , 7);
	  CLEAR_BIT(SFIOR , 6);
	  CLEAR_BIT(SFIOR , 5); */
	ADCSRA.Bits.ADTS=0;

#elif ADC_u8AUTO_TRIGGER== ADC_u8TRIGGER_SOURCE_ANALOG_COMP
	/* Enable AUTO Trigger */
	//SET_BIT(ADCSRA , 5);
	ADCSRA.Bits.ADATE=1;
	/* Select Free Run as ANALOG_COMPARATOR Trigger */
	/*CLEAR_BIT(SFIOR , 7);
	CLEAR_BIT(SFIOR , 6);
	SET_BIT(SFIOR , 5);*/
	ADCSRA.Bits.ADTS=1;

#elif ADC_u8AUTO_TRIGGER==ADC_u8TRIGGER_SOURCE_EXT_INT_REQ0
	/* Enable AUTO Trigger */
	//SET_BIT(ADCSRA , 5);
	ADCSRA.Bits.ADATE=1;

	/* Select Free Run as EXTERNAL INTERPUT Trigger */
	/*CLEAR_BIT(SFIOR , 7);
	SET_BIT(SFIOR , 6);
	CLEAR_BIT(SFIOR , 5);*/
	ADCSRA.Bits.ADTS=2;

#elif ADC_u8AUTO_TRIGGER==ADC_u8TRIGGER_SOURCE_TIMER_COMP_MATCH
	/* Enable AUTO Trigger */
	ADCSRA.Bits.ADATE=1;

	/* Select Free Run as COMPARE_MATCH Trigger */
	/*CLEAR_BIT(SFIOR , 7);
	SET_BIT(SFIOR , 6);
	SET_BIT(SFIOR , 5);*/
	ADCSRA.Bits.ADTS=3;

#elif ADC_u8AUTO_TRIGGER== ADC_u8TRIGGER_SOURCE_TIMER_0_OVF
	/* Enable AUTO Trigger */
	//SET_BIT(ADCSRA , 5);
	ADCSRA.Bits.ADATE=1;

	/* Select Free Run as TIMER0_OVERFLOW Trigger */
	/*SET_BIT(SFIOR , 7);
	CLEAR_BIT(SFIOR , 6);
	CLEAR_BIT(SFIOR , 5);*/
	ADCSRA.Bits.ADTS=4;

#elif ADC_u8AUTO_TRIGGER== ADC_u8TRIGGER_SOURCE_TIMER_COMP_MATCH_B
	/* Enable AUTO Trigger */
	ADCSRA.Bits.ADATE=1;

	/* Select Free Run as TIMER0_OVERFLOW Trigger */
	/*SET_BIT(SFIOR , 7);
	CLEAR_BIT(SFIOR , 6);
	SET_BIT(SFIOR , 5);*/
	ADCSRA.Bits.ADTS=5;

#elif ADC_u8AUTO_TRIGGER== ADC_u8TRIGGER_SOURCE_TIMER_1_OVF
	/* Enable AUTO Trigger */
	ADCSRA.Bits.ADATE=1;

	/* Select Free Run as TIMER1_OVERFLOW Trigger */


#elif ADC_u8AUTO_TRIGGER== ADC_u8TRIGGER_SOURCE_TIMER_1_CAPT
	/* Enable AUTO Trigger */
	//SET_BIT(ADCSRA , 5);
	ADCSRA.Bits.ADATE=1;
	/* Select Free Run as TIMER1_CAPTURE_EVENT Trigger */
	/*	SET_BIT(SFIOR , 7);
	SET_BIT(SFIOR , 6);
	SET_BIT(SFIOR , 5);  */
	ADCSRA.Bits.ADTS=6;
#endif

	/* Enable ADC module */
	SET_BIT(ADCSRA.Byte,7);

	/* Disable start conversion it will be enabled in adc_read Fun*/
	CLEAR_BIT(ADCSRA.Byte,6);

	/* Clear ADIF Flag */
	SET_BIT(ADCSRA.Byte,4);


#if ADC_u8IMPLEMENTATION_BASED == ADC_u8POOLLING_BASED
	/* Disable interrupt*/
	CLEAR_BIT(ADCSRA.Byte,3);

#elif ADC_u8IMPLEMENTATION_BASED == ADC_uint8INTERRUPT_BASED
	SET_BIT(ADCSRA.Byte,3);
	/* ACSRA.Bits.ADIE = 1
	 */
#endif

#if ADC_u8PRESCALLER_VALUE == ADC_u8FCPU_8
	/*CLEAR_BIT(ADCSRA,2);
	SET_BIT(ADCSRA,1);
	SET_BIT(ADCSRA,0);*/
	ADCSRA.Bits.ADPS=3;

#elif ADC_u8PRESCALLER_VALUE == ADC_u8FCPU_2
	/*CLEAR_BIT(ADCSRA,2);
	CLEAR_BIT(ADCSRA,1);
	CLEAR_BIT(ADCSRA,0);*/
	ADCSRA.Bits.ADPS=0;

#elif ADC_u8PRESCALLER_VALUE == ADC_u8FCPU_4
	/*CLEAR_BIT(ADCSRA,2);
	SET_BIT(ADCSRA,1);
	CLEAR_BIT(ADCSRA,0);*/
	ADCSRA.Bits.ADPS=2;

#elif ADC_u8PRESCALLER_VALUE == ADC_u8FCPU_16
	/*SET_BIT(ADCSRA,2);
	CLEAR_BIT(ADCSRA,1);
	CLEAR_BIT(ADCSRA,0);*/
	ADCSRA.Bits.ADPS=4;

#elif ADC_u8PRESCALLER_VALUE == ADC_u8FCPU_32
	/*SET_BIT(ADCSRA,2);
	CLEAR_BIT(ADCSRA,1);
	SET_BIT(ADCSRA,0);*/
	ADCSRA.Bits.ADPS=5;

#elif ADC_u8PRESCALLER_VALUE == ADC_u8FCPU_128
	/*SET_BIT(ADCSRA,2);
	SET_BIT(ADCSRA,1);
	SET_BIT(ADCSRA,0);*/
	ADCSRA.Bits.ADPS=7;

#endif

}

/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel
 * and convert it to digital using the ADC driver.
 */
#if ADC_u8RESULT_ADJUST == ADC_u8RIGHT_ADJUST

uint16 ADC_readChannel(uint8 Chnl_Id)

#elif ADC_u8RESULT_ADJUST == ADC_u8LEFT_ADJUST

uint8 ADC_readChannel(uint8 Chnl_Id)

#endif
{
	/* Input channel number must be from 0 --> 7 */
	Chnl_Id &= 0x07;

	/* Clear first 5 bits in the ADMUX (channel number  :0 bits) before set the required channel */
	ADMUX.Byte &= 0xE0;
	/*
	 * ADMUX.Bits.MUX = 0;
	 */
	/* Choose the correct channel by setting the channel number in MUX4:0 bits */
	ADMUX.Byte = ADMUX.Byte | Chnl_Id;

	/* Start conversion write '1' to ADSC */
	//SET_BIT(ADCSRA,ADSC);
	ADCSRA.Bits.ADSC = 1;

	while(ADCSRA.Bits.ADIF == 0); /* Wait for conversion to complete, ADIF becomes '1' */

	/* Clear ADIF by write '1' to it */
	ADCSRA.Bits.ADIF = 1;

	/* Read the digital value from the data register */

#if ADC_u8RESULT_ADJUST == ADC_u8RIGHT_ADJUST
	uint16 Result=0;
	Result=ADCL.Byte;
	Result|=((ADCH.Byte)<<8);

#elif ADC_u8RESULT_ADJUST == ADC_u8LEFT_ADJUST
	Result = ADCH.Byte ;
#endif

	return Result;

}
