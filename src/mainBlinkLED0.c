/*----------------------------------------------------------------------------- 
						Inclusion des bibliothèques 
-----------------------------------------------------------------------------*/
#include <asf.h> 
#include "FreeRTOS.h"  // Application utilisant le RTOS
#include "task.h"      // Application nécessitant l'implantation de 2 tâches
#include "semphr.h"    // Application nécessitant l'utilisation d'un sémaphore
#include "timers.h"    // Application nécessitant l'utilisation d'un timer

/* Définition de prototypes généraux */

void vSetupHardware(void);

/*----------------------------------------------------------------------------- 
	Déclaration des priorités des tâches 
-----------------------------------------------------------------------------*/ 
//exemple de définition de priorité : #define MYTASK_PRIORITY (tskIDLE_PRIORITY +une valeur entre 0 et configMAX_PRIORITIES-1)

#define HORLOGE_TASK_PRIORITY ( tskIDLE_PRIORITY +3 )   // ici la tâche Horloge est la plus prioritaire
#define BlinkLED_TASK_PRIORITY ( tskIDLE_PRIORITY +1 )

/*----------------------------------------------------------------------------- 
	Déclaration des sémaphores 
-----------------------------------------------------------------------------*/ 
//exemple pour créer un semaphore binaire : xSemaphoreHandle xMonSemaphore=NULL;

xSemaphoreHandle Sem=NULL; // Déclaration d'un sémaphore pour réveiller la tâche BlinkLED à partir de la tâche Horloge

/*-----------------------------------------------------------------------------
	Déclaration de files de message 
 -----------------------------------------------------------------------------*/ 
//exemple pour créer un file de message : xQueueHandle MyMessageQueue; 
  // non utilisé dans cette application

/*----------------------------------------------------------------------------- 
	Déclaration de timers 
-----------------------------------------------------------------------------*/ 
//exemple pour créer un file de message : xTimerHandle MyTimer; 
  // non utilisé dans cette application

/*----------------------------------------------------------------------------- 
	Déclaration de constantes et de variables globales 
-----------------------------------------------------------------------------*/ 
//exemple pour un delay : #define delay (valeur * portTICK_PERIOD_MS) 

#define HORLOGE_TASK_DELAY 1000  // constante permettant de définir la vitesse de clignotement de la LED

/*----------------------------------------------------------------------------- 
	Déclaration des tâches 
-----------------------------------------------------------------------------*/
//exemple de déclaration des prototypes de fonction de tâche: void MaFonction( void * pvParameters ); 

void vCodeHorloge( void * pvParameters );  // pour la tâche Horloge
void vCodeBlinkLED( void * pvParameters ); // pour la tâche BlinkLED

/******************************************************** 
	 Fonctions primitives freeRTOS hook/callback 
*********************************************************/ 

/************************************************************************ 
		Déclaration de la fonction d'initialisation matérielle 
************************************************************************/ 
void vSetupHardware(void) 
{ 
	system_init();// Initialisation matérielle 
	
} 

/******************************************************************************
						Programme principal 
******************************************************************************/ 

int main (void) 
{ 
	//Initialisation du matériel 
	vSetupHardware(); 
	
	//Création des sémaphores 
	//exemple : xMonSemaphore = xSemaphoreCreateBinary(); 
	
	Sem = xSemaphoreCreateBinary();
		
	//Création des tâches 
	//exemple : xTaskCreate( MaFonction, ( const char * ) "Nom de la tâche", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY, NULL ); 
	
	xTaskCreate( vCodeHorloge, ( const char * ) "Horloge", configMINIMAL_STACK_SIZE, NULL, HORLOGE_TASK_PRIORITY, NULL ); 
	xTaskCreate( vCodeBlinkLED, ( const char * ) "BlinkLED", configMINIMAL_STACK_SIZE, NULL, BlinkLED_TASK_PRIORITY, NULL );
	
	//Lancement de l'ordonanceur 
	vTaskStartScheduler(); 
} 


/*************************************** 
	Déclaration du code des tâches 
****************************************/

/***************************************
	tâche : Horloge 
****************************************/
void vCodeHorloge(void * pvParameters) 
{ 
	for( ;; ) 
	{ 
		vTaskDelay(HORLOGE_TASK_DELAY); // Attente passive pour une tâche d'une durée définie en ticks
		xSemaphoreGive( Sem); 
	} 
}

/*************************************** 
		code tâche : BlinkLED 
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
