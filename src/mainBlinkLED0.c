/*----------------------------------------------------------------------------- 
						Inclusion des biblioth�ques 
-----------------------------------------------------------------------------*/
#include <asf.h> 
#include "FreeRTOS.h"  // Application utilisant le RTOS
#include "task.h"      // Application n�cessitant l'implantation de 2 t�ches
#include "semphr.h"    // Application n�cessitant l'utilisation d'un s�maphore
#include "timers.h"    // Application n�cessitant l'utilisation d'un timer

/* D�finition de prototypes g�n�raux */

void vSetupHardware(void);

/*----------------------------------------------------------------------------- 
	D�claration des priorit�s des t�ches 
-----------------------------------------------------------------------------*/ 
//exemple de d�finition de priorit� : #define MYTASK_PRIORITY (tskIDLE_PRIORITY +une valeur entre 0 et configMAX_PRIORITIES-1)

#define HORLOGE_TASK_PRIORITY ( tskIDLE_PRIORITY +3 )   // ici la t�che Horloge est la plus prioritaire
#define BlinkLED_TASK_PRIORITY ( tskIDLE_PRIORITY +1 )

/*----------------------------------------------------------------------------- 
	D�claration des s�maphores 
-----------------------------------------------------------------------------*/ 
//exemple pour cr�er un semaphore binaire : xSemaphoreHandle xMonSemaphore=NULL;

xSemaphoreHandle Sem=NULL; // D�claration d'un s�maphore pour r�veiller la t�che BlinkLED � partir de la t�che Horloge

/*-----------------------------------------------------------------------------
	D�claration de files de message 
 -----------------------------------------------------------------------------*/ 
//exemple pour cr�er un file de message : xQueueHandle MyMessageQueue; 
  // non utilis� dans cette application

/*----------------------------------------------------------------------------- 
	D�claration de timers 
-----------------------------------------------------------------------------*/ 
//exemple pour cr�er un file de message : xTimerHandle MyTimer; 
  // non utilis� dans cette application

/*----------------------------------------------------------------------------- 
	D�claration de constantes et de variables globales 
-----------------------------------------------------------------------------*/ 
//exemple pour un delay : #define delay (valeur * portTICK_PERIOD_MS) 

#define HORLOGE_TASK_DELAY 1000  // constante permettant de d�finir la vitesse de clignotement de la LED

/*----------------------------------------------------------------------------- 
	D�claration des t�ches 
-----------------------------------------------------------------------------*/
//exemple de d�claration des prototypes de fonction de t�che: void MaFonction( void * pvParameters ); 

void vCodeHorloge( void * pvParameters );  // pour la t�che Horloge
void vCodeBlinkLED( void * pvParameters ); // pour la t�che BlinkLED

/******************************************************** 
	 Fonctions primitives freeRTOS hook/callback 
*********************************************************/ 

/************************************************************************ 
		D�claration de la fonction d'initialisation mat�rielle 
************************************************************************/ 
void vSetupHardware(void) 
{ 
	system_init();// Initialisation mat�rielle 
	
} 

/******************************************************************************
						Programme principal 
******************************************************************************/ 

int main (void) 
{ 
	//Initialisation du mat�riel 
	vSetupHardware(); 
	
	//Cr�ation des s�maphores 
	//exemple : xMonSemaphore = xSemaphoreCreateBinary(); 
	
	Sem = xSemaphoreCreateBinary();
		
	//Cr�ation des t�ches 
	//exemple : xTaskCreate( MaFonction, ( const char * ) "Nom de la t�che", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY, NULL ); 
	
	xTaskCreate( vCodeHorloge, ( const char * ) "Horloge", configMINIMAL_STACK_SIZE, NULL, HORLOGE_TASK_PRIORITY, NULL ); 
	xTaskCreate( vCodeBlinkLED, ( const char * ) "BlinkLED", configMINIMAL_STACK_SIZE, NULL, BlinkLED_TASK_PRIORITY, NULL );
	
	//Lancement de l'ordonanceur 
	vTaskStartScheduler(); 
} 


/*************************************** 
	D�claration du code des t�ches 
****************************************/

/***************************************
	t�che : Horloge 
****************************************/
void vCodeHorloge(void * pvParameters) 
{ 
	for( ;; ) 
	{ 
		vTaskDelay(HORLOGE_TASK_DELAY); // Attente passive pour une t�che d'une dur�e d�finie en ticks
		xSemaphoreGive( Sem); 
	} 
}

/*************************************** 
		code t�che : BlinkLED 
****************************************/ 
void vCodeBlinkLED(void * pvParameters) 
{ 
	/* declaration de variables locales de la tache */ 
	bool Blink;
	
	/* initialisation des variables locales */ 
	Blink = false;
	
		for (;;) 
		{ 
			/* attente du semaphore d'eveil */ 
			xSemaphoreTake(Sem, portMAX_DELAY); 
			/* Cycle H*/ 
			if (Blink == false) {
				/* turn LED on. */
				port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
				Blink = true;
			} else {
				/* turn LED off. */
				port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
				Blink = false;
			}
		} 
} 
