/*
 * driverValvSolenoide.c
 *
 *  Created on: Jul 11, 2018
 *      Author: juan
 */

/*============================================================================

 *
 *===========================================================================*/

/*==================[inlcusiones]============================================*/
#include "driverValvSolenoide.h"
/*==================[definiciones y macros]==================================*/

/*=================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

void abrirValvula (tipoValvula_t valvula){
	gpioWrite( valvula, TRUE );

}

void cerrarValvula(tipoValvula_t valvula){
	gpioWrite( valvula, !TRUE );
}

void configurarGpioValvulas(void){
	gpioConfig( VALVULA1 ,   GPIO_OUTPUT );
	gpioConfig( VALVULA2 ,   GPIO_OUTPUT );
	gpioConfig( VALVULA3 ,   GPIO_OUTPUT );
}
/*==================[funcion principal]======================================*/


/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
