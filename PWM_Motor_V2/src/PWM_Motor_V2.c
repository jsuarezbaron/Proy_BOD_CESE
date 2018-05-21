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

/*==================[inlcusiones]============================================*/

//#include "program.h"   // <= su propio archivo de cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI
/*==================[definiciones y macros]==================================*/

// Esto es pensando en que el driver del motor tiene un pin donde si se
// pone en ON se gira a la derecha y si se pone en OFF gira a la izquierda.
// Ver si va asi o invertido segun el driver

/* Se modifica el código para el driver VNH2SP30 de ST:
 /* Ref: SparkFun Monster Moto Shield
 * https://www.sparkfun.com/products/10182
 *
// Monster Motor Shield VNH2SP30 Pinout:
A0 : Enable pin for motor 1 (INA)
A1 : Enable pin for motor 2 (INB)
A2 : Current sensor for motor 1  (CS)
A3 : Current sensor for motor 2  (CS)
D7 : Clockwise (CW) for motor 1
D8 : Counterclockwise (CCW) for motor 1
D4 : Clockwise (CW) for motor 2
D9 : Counterclockwise (CCW) for motor 2
D5 : PWM for motor 1
D6 : PWM for motor 2      */

#define BRAKE 				  0
#define CW                    1
#define CCW                   2
#define CS_THRESHOLD 		  150   // Definition of safety current (Check: "1.3 Monster Shield Example").

#define MOTOR_1				  0
#define MOTOR_2 			  1
#define MOTOR_3 			  2

#define GIRO_DERECHA      	  ON
#define GIRO_IZQUIERDA     	  OFF

// Pines a utilizar
#define PIN_PWM_MOTOR         PWM7    // Es (CAMBIAR) Pin de salida PWM donde conecto el motor (PWM7 = LED1)
#define PIN_GPIO_SENTIDO_IN1  LEDB    // (CAMBIAR) Pin que le da al driver el sentido de giro
#define PIN_GPIO_MOT_DER   	  LED2    // Pin de led que indica el sentido de giro a la derecha
#define PIN_GPIO_MOT_IZQ      LED3    // Pin de led que indica el sentido de giro a la izquierda
                                      // Si LED2 y LED3 apagados, entonces motor detenido

// Pines a utilizar //
#define ENA   PWM7   // (CAMBIAR) Pin de salida PWM donde conecto el motor (PWM7 = LED1)
#define ENB   PWM6   //  Salida de PWM de la Edu CIAA: PWM6
#define IN1   GPIO5   // Entrada 1 del L298n
#define IN3   GPIO6   // Entrada 3 del L298n*/



#define LED_INDICADOR_SENTIDO LEDR

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
						"1. Detener Motor.\n\r"
						"2. Girar Adelante.\n\r"
						"3. Girar Atrás.\n\r"
						"+. Incrementar Velocidad.\n\r"
						"-. Disminuir Velocidad.\n\r"
						""
						;
/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/


bool_t sentidoGiro = GIRO_DERECHA;      // Sentido de giro actual.  Inicialmente gira a la derecha
uint8_t velocidad  = 0;                 // Velocidad del motor, de 0 a 100
uint8_t secuencia = OFF;    	        // Iniciar secuencia
delay_t tiempoEncendido;			    // Variable de Retardo no bloqueante

void driverInicializarMotor( void ){

	gpioConfig( PIN_GPIO_SENTIDO_IN1,      GPIO_OUTPUT );		// Configurar los pines GPIO utilizados
	gpioConfig( PIN_GPIO_MOT_DER,      GPIO_OUTPUT );
	gpioConfig( PIN_GPIO_MOT_IZQ,      GPIO_OUTPUT );

	gpioConfig( LED_INDICADOR_SENTIDO, GPIO_OUTPUT );

	gpioWrite( PIN_GPIO_SENTIDO_IN1,       sentidoGiro );       	// Inicialmente gira a la derecha
	gpioWrite( LED_INDICADOR_SENTIDO,  sentidoGiro );

	pwmConfig( 0, PWM_ENABLE );						   // Configurar timers para modo PWM

	pwmConfig( PIN_PWM_MOTOR, PWM_ENABLE_OUTPUT );    // Enciendo el pin de salida que quiero en modo PWM
	// 0=0% a 255=100%
	pwmWrite( PIN_PWM_MOTOR, 0 ); // Inicio el PWM detenido (duty cycle = 0%)

	// Inicializar Retardo no bloqueante con tiempo en milisegundos
	// (1000ms = 1s)
	delayConfig( &tiempoEncendido, 0 );
}

void driverActualizarLedsIndicadores( void ){
	if( velocidad == 0 ){
		// Apagar los leds indicadores de sentido de giro
		gpioWrite( PIN_GPIO_MOT_DER, OFF );
		gpioWrite( PIN_GPIO_MOT_IZQ, OFF );

	} else{
		if( sentidoGiro == GIRO_DERECHA ){
			// Setear debidamente los leds indicadores de sentido de giro
			gpioWrite( PIN_GPIO_MOT_DER, ON );
			gpioWrite( PIN_GPIO_MOT_IZQ, OFF );
			//debugPrintlnString( "Bomba girando a la der.\n\r\0" );
		} else{
			// Setear debidamente los leds indicadores de sentido de giro
			gpioWrite( PIN_GPIO_MOT_DER, OFF );
			gpioWrite( PIN_GPIO_MOT_IZQ, ON );
			//debugPrintlnString( "Bomba girando a la izq.\n\r\0" );
		}
	}
}

void driverSetearVelocidad( uint8_t vel ){
	velocidad = vel;
	// Seteo el ciclo de trabajo del PWM al dutyCycle % = velociad
	// Seteo el ciclo de trabajo del PWM va de 0 a 255
	pwmWrite( PIN_PWM_MOTOR, velocidad*255/100 );
	// Actualizar leds indicadores
	driverActualizarLedsIndicadores();
}

void driverEncenderMotor( void ){
	secuencia = OFF;
	debugPrintlnString( "Motor Encendido.\n\r\0" )
	driverSetearVelocidad( 10 );
}

void driverDetenerMotor( void ){
	secuencia = OFF;
	debugPrintlnString( "Motor Apagado.\n\r\0" )
	driverSetearVelocidad( 0 );
}

void driverCambiarSentidoDeGiro( bool_t sentido ){
	sentidoGiro = sentido;
	// Cambiar sentido de giro (pin)
	gpioWrite( PIN_GPIO_SENTIDO, sentidoGiro );
	// Actualizar leds indicadores
	driverActualizarLedsIndicadores();
}

void driverInvertirSentidoDeGiro( void ){
	if( sentidoGiro == GIRO_DERECHA ){
		driverCambiarSentidoDeGiro( GIRO_IZQUIERDA );
		debugPrintlnString( "Girando a la izquierda.\n\r\0" )
	} else{
		driverCambiarSentidoDeGiro( GIRO_DERECHA );
		debugPrintlnString( "Girando a la derecha.\n\r\0" )
	}
}

void driverIncrementarVelocidad( void ){
	velocidad  = velocidad + 10;
	if( velocidad > 255 ){
		velocidad = 255;
	}
	debugPrintlnString( "Velocidad +=:\r\0" )
	debugPrintIntFormat( velocidad, DEC_FORMAT );
	debugPrintlnString( "\r" );


	//printf("Velocidad +=: %d", velocidad);
	driverSetearVelocidad( velocidad );
}

void driverDecrementarVelocidad ( void ){
	velocidad = velocidad - 10 ;
	if( velocidad < 0){
		velocidad = 0;
	}
	debugPrintlnString( "Velocidad -=:\r\0 " )
	debugPrintIntFormat( velocidad, DEC_FORMAT  );
	debugPrintlnString( "\r" );
	driverSetearVelocidad( velocidad );
}

void driverElegirSentido_y_Velocidad ( void ){
	char entradaUsuario;

	//uint8_t entradaUsuario;

	if (uartReadByte( UART_USB, &entradaUsuario ) != FALSE) {
		if(entradaUsuario != 0)
			switch ( entradaUsuario ) {
			case '1':{
				driverDetenerMotor();
			}break;

			case '2':{
				driverInvertirSentidoDeGiro();
			}break;

			case '3':{
				driverInvertirSentidoDeGiro();
			}break;

			case '+':{
				driverIncrementarVelocidad();
			}break;

			case '-':{
				driverDecrementarVelocidad();
			}break;

			default:
				uartWriteString(UART_USB, menu);
				uartWriteString(UART_USB, "Caracter recibido fuera de menu\n\r");
				break;
			}

	}
}

// Secuencia Bloqueante:
// Encender motor con ciclio de trabajo (duty cycle) al:
//  - 25%, por 4 segundos
//  - 50%, por 3 segundos
//  - 75%, por 2 segundos
//  - 100%, por 1 segundos
// Luego detener el motor.
// Al ser bloqueante hay que esperar a que complete la secuencia
// para que permita responder a otros botones.
void driverSecuenciaBloqueante( void ){
	// Seteo la velocidad al 25%
	driverSetearVelocidad( 25 );
	delay(4*1000);
	// Seteo la velocidad al 50%
	driverSetearVelocidad( 50 );
	delay(3*1000);
	// Seteo la velocidad al 75%
	driverSetearVelocidad( 75 );
	delay(2*1000);
	// Seteo la velocidad al 100%
	driverSetearVelocidad( 100 );
	delay(3*1000);
	// Detener motor
	driverDetenerMotor();
}

// Secuencia No bloqueante:
// Encender motor con ciclio de trabajo (duty cycle) al:
//  - 25%, por 4 segundos
//  - 50%, por 3 segundos
//  - 75%, por 2 segundos
//  - 100%, por 1 segundos
// Luego detener el motor.
// Al ser no bloqueante se puede detener la secuencia con TEC2 en cualquier momento.
void driverSecuenciaNoBloqueante( void ){

	static uint8_t fase = 0;

	if( secuencia == ON ){
		// Si se cumple el tiempo, cambiar de estado
		if( delayRead( &tiempoEncendido ) ){
			switch( fase ){
			case 0:
				// Seteo la velocidad al 25%
				driverSetearVelocidad( 25 );
				fase++;
				delayWrite( &tiempoEncendido, 4*1000 );
				debugPrintlnString( "Bomba al 25% \n\r\0" );
				break;
			case 1:
				// Seteo la velocidad al 50%
				driverSetearVelocidad( 50 );
				fase++;
				delayWrite( &tiempoEncendido, 3*1000 );
				debugPrintlnString( "Motor al 50% \n\r\0" );
				break;
			case 2:
				// Seteo la velocidad al 75%
				driverSetearVelocidad( 75 );
				fase++;
				delayWrite( &tiempoEncendido, 2*1000 );
				debugPrintlnString( "Motor al 75% \n\r\0" );
				break;
			case 3:
				// Seteo la velocidad al 100%
				driverSetearVelocidad( 100 );
				fase++;
				delayWrite( &tiempoEncendido, 1*1000 );
				debugPrintlnString( "Motor al 100% \n\r\0 " );
				break;

			default:
				fase = 0;
				secuencia = OFF;
				driverDetenerMotor();
				delayWrite( &tiempoEncendido, 0 );
				break;
			}
		}
	} else{
		fase = 0;
	}
}

void driverSecuencaTemporizada(void){

	if(  delayRead( &tiempoEncendido )  ){
		driverSetearVelocidad( 100 );
		delayWrite( &tiempoEncendido, 1*10000 );
	}
}

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardConfig();

   // Inicializar UART_USB como salida Serial de debug
   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "DEBUG: UART_USB configurada." );
   
   // Inicializar driver de motor
   driverInicializarMotor();
   debugPrintlnString(menu);

    // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
	   driverElegirSentido_y_Velocidad();

	   // Iniciar Motor con TEC1
	   if( !gpioRead( TEC1 ) ){
		   driverEncenderMotor(); // Enciende el motor con un duty cyle al 100%
	   }

	   // Detener Motor con TEC2
	   if( !gpioRead( TEC2 ) ){
		   driverDetenerMotor();
	   }

	   // Invertir sentido de giro con TEC3
	   if( !gpioRead( TEC3 ) ){
		   driverInvertirSentidoDeGiro();
		   delay(500); // Para evitar rebotes
	   }

	   /* ---------- Opcion 1, bloqueante ---------------- */

	   // Iniciar secuencia bloqueante con TEC4:
	   /*if( !gpioRead( TEC4 ) ){
	           driverSecuenciaBloqueante();
	        }*/

	   /* ---------- Opcion 2, no bloqueante ------------- */

	   // Iniciar secuencia no bloqueante con TEC4:
	   if( !gpioRead( TEC4 ) ){
		   secuencia = ON;
	   }
	   driverSecuenciaNoBloqueante();

   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
