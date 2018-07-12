/*============================================================================
 * Autor: Juan Carlos Suárez Barón
 * Licencia:
 * Fecha:09/05/2018
 *
 * * Programa para controlar el sentido y velocidad de giro de un motor Dc de 12v
 * con el driver basado en el L298n
 * Pines de configuración del L298n:
 *ENA ------> GPIO8  Salida de PWM de la Edu CIAA: PWM6
 *ENB ------> GPIO2  Salida de PWM de la Edu CIAA: PWM10
 *IN1 ------> GPIO5
 *IN2 ------> GND
 *IN3 ------> GPIO6
 *IN4 ------> GND
 *
 * Pines para otras conexiones:
 * Solenoide1:  GPIO7
 *
 *
 *
 *
 *===========================================================================*/

/*==================[inclusiones]============================================*/

//#include "program.h"   // <= su propio archivo de cabecera (opcional)
#include "sapi.h"       		 /** <= Biblioteca sAPI */
#include "pwm_motor.h"  		 /** Constantes para pwm */
#include "driverValvSolenoide.h" /** Conjunto de funciomes para usar las válvulas solenoide */
/*==================[definiciones y macros]==================================*/

typedef enum {BOMBA1 = BOMBA1_PIN_PWM, BOMBA2 = BOMBA2_PIN_PWM, BOMBA3 = BOMBA3_PIN_PWM}bombas_t;
typedef enum {ENCENDIDA, APAGADA}estadosBombas_t;
typedef enum {INCREMENTAR_VEL, DISMINUIR_VEL}operacionVelocidad_t;

/* Estructura de datos necesarios para la MEF antirebote */
/*typedef struct{

};*/
/*==================[definiciones de datos internos]=========================*/
DEBUG_PRINT_ENABLE
//CONSOLE_PRINT_ENABLE
/*==================[definiciones de datos externos]=========================*/
static uint8_t menu[] =
						"\n\r"
						"********************* MENU DE SELECCION********************\n\r"
						"TEC1: Encender Motor.\n\r"
						"TEC2: Detener la secuencia.\n\r"
						"TEC3: Invertir Sentido De Giro.\n\r"
						"TEC4: Iniciar secuencia no bloqueante.\n\r"
						"1. Encender Bombas.\n\r"
						"2. Detener Bomba.\n\r"
						"3. Abrir Valvula.\n\r"
						"4. Cerrar Valvula.\n\r"
						"+. Incrementar Velocidad.\n\r"
						"-. Disminuir Velocidad.\n\r"
						""
						;
/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/


bool_t sentidoGiro = GIRO_DERECHA;      // Sentido de giro actual.  Inicialmente gira a la derecha
int16_t velocidad  = 0;                 // Velocidad del motor, de 0 a 100
uint8_t secuencia = OFF;    	        // Iniciar secuencia
delay_t tiempoEncendido;			    // Variable de Retardo no bloqueante

/*
 * @brief:   Inicializa el módulo de PWM y los pines de IN1, giroDer y giroDer para los tres motores
 * @param:	 bomba: Bomba peristática a seleccionar
 * @return:  nada
 */

void inicializarBombas(bombas_t bomba){

	pwmConfig ( 0, PWM_ENABLE );	/** Configurar timers para modo PWM */

	switch( bomba){


	case BOMBA1:{

		gpioConfig( BOMBA1_PIN_PWM ,               GPIO_OUTPUT );
		gpioConfig( BOMBA1_PIN_GPIO_MOT_DER ,      GPIO_OUTPUT );
		gpioConfig( BOMBA1_PIN_GPIO_MOT_IZQ,       GPIO_OUTPUT );
		gpioConfig( LED_INDICADOR_SENTIDO_BOMBA1,  GPIO_OUTPUT );
		gpioWrite( BOMBA1_PIN_GPIO_SENTIDO_IN1,    sentidoGiro );  /** Inicialmente gira a la derecha */
		pwmConfig( BOMBA1_PIN_PWM,           PWM_ENABLE_OUTPUT );  /** Enciendo el pin de salida que quiero en modo PWM */
		pwmWrite ( BOMBA1_PIN_PWM,                           0 );  /** Inicio el PWM detenido (duty cycle = 0%, 255 = 100 %) */
	}break;

	case BOMBA2:{
		gpioConfig( BOMBA2_PIN_PWM ,               GPIO_OUTPUT );
		gpioConfig( BOMBA2_PIN_GPIO_MOT_DER ,      GPIO_OUTPUT );
		gpioConfig( BOMBA2_PIN_GPIO_MOT_IZQ,       GPIO_OUTPUT );
		gpioConfig( LED_INDICADOR_SENTIDO_BOMBA2,  GPIO_OUTPUT );
		gpioWrite( BOMBA2_PIN_GPIO_SENTIDO_IN1,    sentidoGiro );  /** Inicialmente gira a la derecha */
		pwmConfig( BOMBA2_PIN_PWM,           PWM_ENABLE_OUTPUT );  /** Enciendo el pin de salida que quiero en modo PWM */
		pwmWrite ( BOMBA2_PIN_PWM,                           0 );  /** Inicio el PWM detenido (duty cycle = 0%, 255 = 100 %) */

	}break;

	case BOMBA3:{
		gpioConfig( BOMBA3_PIN_PWM ,               GPIO_OUTPUT );
		gpioConfig( BOMBA3_PIN_GPIO_MOT_DER ,      GPIO_OUTPUT );
		gpioConfig( BOMBA3_PIN_GPIO_MOT_IZQ,       GPIO_OUTPUT );
		gpioConfig( LED_INDICADOR_SENTIDO_BOMBA3,  GPIO_OUTPUT );
		gpioWrite( BOMBA3_PIN_GPIO_SENTIDO_IN1,    sentidoGiro );  /** Inicialmente gira a la derecha */
		pwmConfig( BOMBA3_PIN_PWM,           PWM_ENABLE_OUTPUT );  /** Enciendo el pin de salida que quiero en modo PWM */
		pwmWrite ( BOMBA3_PIN_PWM,                           0 );  /** Inicio el PWM detenido (duty cycle = 0%, 255 = 100 %) */
	}break;

	}
}

/*
 * @brief:   Asigna la velocidad al motor a través del PWM
 * @param:	 velBomba: Velocidad a asignar, expresada de 0 a 100 %
 * @param:   bomba: Número de la bomba peristáltica.
 * @return:  nada
 */
void actualizarVelocidad(bombas_t bomba, int velBomba){
	////velocidad = velBomba;
	pwmWrite( bomba, velBomba);
}

/*
 * @brief: Detiene la bomba seleccionada asignandole velocidad 0
 * @param: bomba: Número de la bomba peristáltica.
 * @return:nada
 */
void detenerBomba(bombas_t bomba){
	actualizarVelocidad(bomba, DETENIDA);
	debugPrintlnString( "Bomba Apagada.\n\r\0" );
}

/*
 * @brief: Enciende la bomba seleccionada
 * @param: bomba: Número de la bomba peristáltica.
 * @return:  nada
 */
void encenderBomba(bombas_t bomba,int velBomba){
	actualizarVelocidad(bomba, velBomba);
	/*debugPrintlnString( "Bomba Encendida.\n\r\0" );
	debugPrintIntFormat( (int)bomba, DEC_FORMAT );
	debugPrintlnString( "\r\n" );*/
	printf(" Bomba %s encendida \r\n", (char)bomba);
}

/*
 * @brief: Funcion que me permita incrementar o decrementar la velocidad de la bomba seleccionada.
 * @param: bomba: Número de la bomba peristáltica.
 * @param: operacionVelocidad_t: Incrementar o decrementar velocidad
 * @return:  nada
 */

void operacionVelocidad(bombas_t bomba, operacionVelocidad_t tipoOpVel){

	int16_t velocidadBomba=0;
	velocidadBomba=pwmRead(bomba);
	if ( tipoOpVel == INCREMENTAR_VEL){
		velocidadBomba  +=  INCREMENTO;
		if( velocidadBomba > MAXVELOCIDAD ){
			velocidadBomba = MAXVELOCIDAD;
		}
	}else{
		velocidadBomba  -=  INCREMENTO;
		if( velocidadBomba < 0 ){
			velocidadBomba = 0;
		}
	}

	debugPrintlnString( "Velocidad +=:\r\0" )
	debugPrintIntFormat( velocidadBomba, DEC_FORMAT );
	debugPrintlnString( "\r\n" );
	actualizarVelocidad(bomba, velocidadBomba);
}

void driverElegirSentido_y_Velocidad ( void ){
	char entradaUsuario;

	//uint8_t entradaUsuario;

	if (uartReadByte( UART_USB, &entradaUsuario ) != FALSE) {
		if(entradaUsuario != 0)
			switch ( entradaUsuario ) {
			case '1':{
				encenderBomba(BOMBA1, 5);
			}break;

			case '2':{
				detenerBomba(BOMBA1);
			}break;

			case '3':{
				abrirValvula(VALVULA1);
			}break;

			case '4':{
				cerrarValvula(VALVULA1);
			}break;

			case '+':{
				operacionVelocidad(BOMBA1, INCREMENTAR_VEL);
			}break;

			case '-':{
				operacionVelocidad(BOMBA1, DISMINUIR_VEL);
			}break;

			default:
				uartWriteString(UART_USB, menu);
				uartWriteString(UART_USB, "Caracter recibido fuera de menu\n\r");
				break;
			}

	}
}

/*==================[funcion principal]======================================*/

/** FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET. */
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------

   boardConfig();

   /** Inicializar UART_USB como salida Serial de debug */
   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "DEBUG: UART_USB configurada.\n\r\0" );
   
   inicializarBombas(BOMBA1);
   inicializarBombas(BOMBA2);
   inicializarBombas(BOMBA3);

   configurarGpioValvulas();

   debugPrintlnString(menu);

    // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
	   driverElegirSentido_y_Velocidad();

   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
