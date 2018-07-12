/*
 * driverValvSolenoide.h
 *
 *  Created on: Jul 11, 2018
 *      Author: Juan Carlos
 */

#ifndef PROYECTOB_PWM_MOTOR_V3_INC_DRIVERVALVSOLENOIDE_H_
#define PROYECTOB_PWM_MOTOR_V3_INC_DRIVERVALVSOLENOIDE_H_

/*==================[inclusiones]============================================*/
#include "sapi.h"
/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
#define valvula1    LEDB    /** Cambiar por GPIO0 */
#define valvula2    LED2	/** Cambiar por GPIO2 */
#define Valvula3    LED3    /** Cambiar por GPIO4 */

/*==================[typedef]================================================*/
typedef enum{VALVULA1 = valvula1, VALVULA2 = valvula2, VALVULA3 = Valvula3}tipoValvula_t;
/*==================[tipos de datos declarados por el usuario]===============*/

/*==================[declaraciones de datos externos]========================*/

/*==================[declaraciones de funciones externas]====================*/

/*
 * @brief:   Configura los gpio de las válvulas solenoide.
 * @param:   Ninguno
 * @return:  Nada
 */
void configurarGpioValvulas(void);

/*
 * @brief:   Inicia la apertura de la válvula solenoide.
 * @param:   valvula:  Válvula a seleccionar.
 * @return:  Nada
 */
void abrirValvula (tipoValvula_t valvula);

/*
 * @brief:   Inicia la apertura de la válvula solenoide.
 * @param:   valvula:  Válvula a seleccionar.
 * @return:  Nada
 */

/*
 * @brief:   Inicia el cierre de la válvula solenoide.
 * @param:   valvula:  Válvula a seleccionar.
 * @return:  Nada
 */
void cerrarValvula(tipoValvula_t valvula);
/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

#endif /* PROYECTOB_PWM_MOTOR_V3_INC_DRIVERVALVSOLENOIDE_H_ */
