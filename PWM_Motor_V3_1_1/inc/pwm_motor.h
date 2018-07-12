/*
 * pwm_motor.h
 *
 *  Created on: Jul 9, 2018
 *      Author: juan
 */

#ifndef PROYECTOB_PWM_MOTOR_V3_INC_PWM_MOTOR_H_
#define PROYECTOB_PWM_MOTOR_V3_INC_PWM_MOTOR_H_

/*==================[inclusiones]============================================*/

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#include "sapi.h"


// Esto es pensando en que el driver del motor tiene un pin donde si se
// pone en ON se gira a la derecha y si se pone en OFF gira a la izquierda.
// Ver si va asi o invertido segun el driver

/* Se modifica el código para el driver VNH2SP30 de ST:
 /* Ref: SparkFun Monster Moto Shield
 * https://www.sparkfun.com/products/10182
 *
// Monster Motor Shield VNH2SP30 Pinout:
A0 : Enable pin for motor 1 			(INA)
A1 : Enable pin for motor 2 			(INB)
A2 : Current sensor for motor 1  		(CS)
A3 : Current sensor for motor 2  		(CS)
D7 : Clockwise (CW) for motor 1  		(INA1)
D8 : Counterclockwise (CCW) for motor 1 (INA2)
D4 : Clockwise (CW) for motor 2  		(INB1)
D9 : Counterclockwise (CCW) for motor 2 (INB2)
D5 : PWM for motor 1 					(PWM1)
D6 : PWM for motor 2 					(PWM2)     */

#define BRAKE1				  0
#define CW1                   1
#define CCW1                  2
#define CS_THRESHOLD1 		  150   // Definition of safety current (Check: "1.3 Monster Shield Example").

#define MOTOR_1				  0
#define MOTOR_2 			  1
#define MOTOR_3 			  2

#define GIRO_DERECHA      	  ON
#define GIRO_IZQUIERDA     	  OFF

#define DETENIDA              OFF

#define MAXVELOCIDAD          255
#define INCREMENTO            15


// Pines a utilizar //
//#define ENA   PWM7   // (CAMBIAR) Pin de salida PWM donde conecto el motor (PWM7 = LED1)
//#define ENB   PWM6   //  Salida de PWM de la Edu CIAA: PWM6
//#define IN1   GPIO5   // Entrada 1 del L298n
//#define IN3   GPIO6   // Entrada 3 del L298n*/

//Para el Monster Motor Shield cambiar LED1 (PWM7) por PWM6(GPIO8) y LEDB por GPIO5 para el Pin de sentido de giro

// Pines a utilizar
#define BOMBA1_PIN_PWM        			PWM7    /** Es (CAMBIAR PWM7 (led1) por PWM6 = GPIO8) Pin de salida PWM donde conecto el motor (PWM7 = LED1) */
#define BOMBA1_PIN_GPIO_SENTIDO_IN1  	GPIO7    // (CAMBIAR) Pin que le da al driver el sentido de giro
#define BOMBA1_PIN_GPIO_MOT_DER 		LED2    // Pin de led que indica el sentido de giro a la derecha
#define BOMBA1_PIN_GPIO_MOT_IZQ      	LED3    // Pin de led que indica el sentido de giro a la izquierda
                                      // Si LED2 y LED3 apagados, entonces motor detenido
#define LED_INDICADOR_SENTIDO_BOMBA1	LEDR


#define BOMBA2_PIN_PWM         			PWM8    /** Es (CAMBIAR PWM8 (led2) por PWM10 = GPIO2 ) Pin de salida PWM donde conecto el motor (PWM8 = LED2) */
#define BOMBA2_PIN_GPIO_SENTIDO_IN1     GPIO5    // (CAMBIAR) Pin que le da al driver el sentido de giro
#define BOMBA2_PIN_GPIO_MOT_DER         LED2    // Pin de led que indica el sentido de giro a la derecha
#define BOMBA2_PIN_GPIO_MOT_IZQ         LED3    // Pin de led que indica el sentido de giro a la izquierda
                                                // Si LED2 y LED3 apagados, entonces motor detenido
#define LED_INDICADOR_SENTIDO_BOMBA2 	LEDG


#define BOMBA3_PIN_PWM      			 PWM9    /** Es (CAMBIAR  PWM9 (led3) por PWM0 = T_FIL1 ) Pin de salida PWM donde conecto el motor (PWM9 = LED3) */
#define BOMBA3_PIN_GPIO_SENTIDO_IN1      GPIO3    // (CAMBIAR) Pin que le da al driver el sentido de giro
#define BOMBA3_PIN_GPIO_MOT_DER     	 LED2    // Pin de led que indica el sentido de giro a la derecha
#define BOMBA3_PIN_GPIO_MOT_IZQ		     LED3    // Pin de led que indica el sentido de giro a la izquierda
#define LED_INDICADOR_SENTIDO_BOMBA3 	 LEDB

/*==================[tipos de datos declarados por el usuario]===============*/

/*==================[declaraciones de datos externos]========================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

#endif /* PROYECTOB_PWM_MOTOR_V3_INC_PWM_MOTOR_H_ */
