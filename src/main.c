/*----------------------------------------------------------------------------- 
						Inclusion des biblioth�ques 
-----------------------------------------------------------------------------*/
#include <asf.h>
#include <stdio.h> 
#include <string.h>
#include "FreeRTOS.h" 
#include "task.h" 
#include "semphr.h" 
#include "timers.h" 
#include "MesTemps.h"
#include "oled1.h"

/* D�finition de prototypes g�n�raux */

void vSetupHardware(void);
int8_t get_Entree(void);
void set_Sortie(int8_t);

/*-----------------------------------------------------------*/
//! Extension header for the OLED1 Xplained Pro
#define OLED1_EXT_HEADER  EXT1 // d�finition de la connection de la carte OLED1 sur l'extension 3 de la carte SAMD21

static OLED1_CREATE_INSTANCE(oled1, OLED1_EXT_HEADER);

/*-----------------------------------------------------------*/

/*----------------------------------------------------------------------------- 
	D�claration des priorit�s des t�ches 
-----------------------------------------------------------------------------*/ 

#define CLOCK_TASK_PRIORITY ( tskIDLE_PRIORITY +5 ) 
#define OBSERVATION_TASK_PRIORITY ( tskIDLE_PRIORITY +2 )
#define SUPERVISION_TASK_PRIORITY ( tskIDLE_PRIORITY +3 )
#define AFFICHAGE_TASK_PRIORITY ( tskIDLE_PRIORITY +1 )

// D�finition de la structure R�sultat
typedef struct {
	uint16_t periodeObservation;
	uint16_t MesMin;
	uint16_t Moyenne;
	uint16_t MesMax;
} Resultat_t;

/*----------------------------------------------------------------------------- 
	D�claration des s�maphores 
-----------------------------------------------------------------------------*/ 

xSemaphoreHandle xSem_H1=NULL; // Pour r�veiller LEDControl
xSemaphoreHandle xSem_H2=NULL; // Pour r�veiller LEDControl

/*-----------------------------------------------------------------------------
	D�claration de files de message 
 -----------------------------------------------------------------------------*/ 

xQueueHandle ResultQueue;

/*----------------------------------------------------------------------------- 
	D�claration de constantes et de variables globales 
-----------------------------------------------------------------------------*/ 

#define HORLOGE_TASK_DELAY 1

enum mode_t {OBSERVATION_MODE, NORMAL_MODE};
enum mode_t g_mode = NORMAL_MODE;

/*----------------------------------------------------------------------------- 
	D�claration des t�ches 
-----------------------------------------------------------------------------*/
//exemple de d�claration de fonction de t�che: void MaFonction( void * pvParameters ); 

void vCodeClock( void * pvParameters ); 
void vCodeObservation( void * pvParameters );
void vCodeSupervison( void * pvParameters );
void vCodeAffichage( void * pvParameters );

/******************************************************** 
	 Fonctions primitives freeRTOS hook/callback 
*********************************************************/ 

/************************************************************************ 
		D�claration de la fonction d'initialisation mat�rielle 
************************************************************************/ 
void vSetupHardware(void) 
{ 
	system_init();      // Initialisation mat�rielle 
	gfx_mono_init();    // Initialisation �cran
	oled1_init(&oled1); // initialisation de la carte OLED1
	
	// reset ecran
	gfx_mono_draw_filled_rect(0, 0, GFX_MONO_LCD_WIDTH, GFX_MONO_LCD_HEIGHT, GFX_PIXEL_CLR);
	
	gfx_mono_draw_string("Temps mini:", 0, 0, &sysfont);
	gfx_mono_draw_string("Temps moyen:", 0, 7, &sysfont);
	gfx_mono_draw_string("Temps maxi:", 0, 14, &sysfont);
	gfx_mono_draw_string("Periode obs:", 0, 21, &sysfont);
		   	
}

/************************************************************************ 
		D�claration des fonctions pour les entr�es/sorties
************************************************************************/ 

// THE PROBLEM IS HERE
int8_t get_Entree(void) {
	
	bool BP1, BP2, BP3;
	BP1 = oled1_get_button_state(&oled1, OLED1_BUTTON1_ID);
	BP2 = oled1_get_button_state(&oled1, OLED1_BUTTON2_ID);
	BP3 = oled1_get_button_state(&oled1, OLED1_BUTTON3_ID);
	
	return (BP1 | (BP2 << 1) | (BP3 << 2));
}

void set_Sortie(int8_t Sortie) {
	
	oled1_set_led_state(&oled1, OLED1_LED1_ID, Sortie & 0x1);
	oled1_set_led_state(&oled1, OLED1_LED2_ID, Sortie & (1 << 1));
	oled1_set_led_state(&oled1, OLED1_LED3_ID, Sortie & (1 << 2));
	
}

/******************************************************************************
	Programme principal 
******************************************************************************/ 

int main (void) 
{ 
	//Initialisation du mat�riel 
	vSetupHardware(); 
	ConfigureMeasure();
	
	ResultQueue = xQueueCreate( 3, sizeof( Resultat_t ) );

	 if( ResultQueue == NULL )
	 {
		 /* Queue was not created and must not be used. */
		 while(1);
	 }
	
	//Cr�ation des s�maphores 

	xSem_H1 = xSemaphoreCreateBinary();
	xSem_H2 = xSemaphoreCreateBinary();		
	
	//Cr�ation des t�ches 

		xTaskCreate( vCodeClock, ( const char * ) "Clock", 
	configMINIMAL_STACK_SIZE, NULL, CLOCK_TASK_PRIORITY, NULL ); 
		xTaskCreate( vCodeObservation, ( const char * ) "Observation",
	configMINIMAL_STACK_SIZE, NULL, OBSERVATION_TASK_PRIORITY, NULL );
	    xTaskCreate( vCodeSupervison, ( const char * ) "Supervision",
	configMINIMAL_STACK_SIZE, NULL, SUPERVISION_TASK_PRIORITY, NULL );
		xTaskCreate( vCodeAffichage, ( const char * ) "Affichage",
	configMINIMAL_STACK_SIZE, NULL, AFFICHAGE_TASK_PRIORITY, NULL );
	
	//Lancement de l'ordonanceur 
		vTaskStartScheduler(); 
} 


/*************************************** 
	D�claration du code des t�ches 
****************************************/

/***************************************
	t�che : Clock 
****************************************/
void vCodeClock(void * pvParameters) 
{ 
	for( ;; ) 
	{ 
		vTaskDelay(HORLOGE_TASK_DELAY);
		xSemaphoreGive( xSem_H1);
		xSemaphoreGive( xSem_H2); 
	} 
}

/*************************************** 
		code t�che : Supervision 
****************************************/ 

void vCodeSupervison (void * pvParameters){
	bool start_bp_state;
	static bool prev_start_bp_state;
	
	for(;;){
		
		xSemaphoreTake(xSem_H2, portMAX_DELAY); 
		start_bp_state = port_pin_get_input_level(BUTTON_0_PIN);
	
		if ((prev_start_bp_state == true) && (start_bp_state == false) && (g_mode == NORMAL_MODE)){
			g_mode = OBSERVATION_MODE;
		}
		
		prev_start_bp_state = start_bp_state;
	}
}

/*************************************** 
		code t�che : Affichage 
****************************************/ 

void vCodeAffichage (void * pvParameters){
	Resultat_t resultat;
	char str_val[10];
		
	for(;;){
		
	   if( xQueueReceive( ResultQueue,&( resultat ),( TickType_t ) portMAX_DELAY ) == pdPASS )
	   {
		    // reset partie droite de l'�cran
			gfx_mono_draw_filled_rect(75, 0, GFX_MONO_LCD_WIDTH, GFX_MONO_LCD_HEIGHT, GFX_PIXEL_CLR);
		   
		    // affichage  des r�sultats sur l'�cran
		   	itoa(resultat.MesMin, str_val, 10);
		   	gfx_mono_draw_string(str_val, 75, 0, &sysfont);
			itoa(resultat.Moyenne, str_val, 10);
			gfx_mono_draw_string(str_val, 75, 7, &sysfont);
			itoa(resultat.MesMax, str_val, 10);
			gfx_mono_draw_string(str_val, 75, 14, &sysfont);
			itoa(resultat.periodeObservation, str_val, 10);
			gfx_mono_draw_string(str_val, 75, 21, &sysfont);
	   }
	}
}

/*************************************** 
		code t�che : Observation 
****************************************/ 

void vCodeObservation(void * pvParameters) {
	
	/* private type */
	enum Etat_t {NON_OBSERVATION, OBSERVE};
	enum fsm_state_t {REPOS, ATTENTE, MESURE, ATTENTE_MESURE, FIN_MESURE};
		
	/* private variable */
	enum Etat_t Etat = NON_OBSERVATION;
	enum fsm_state_t fsm_state = REPOS;
	Resultat_t resultat;
	uint16_t T;
	uint8_t Sortie = 0;
	uint8_t Entree = 0;
	uint8_t NbMes = 0;
	uint16_t Somme;
	uint8_t NbMesToDO = 5;
	uint16_t Duree_Att = 0;

	for (;;) /* infinite loop */
	{
		xSemaphoreTake(xSem_H1, portMAX_DELAY); 
		
		Entree = get_Entree();
		set_Sortie(Sortie);
		
		resultat.periodeObservation = EndMeasure();
		StartMeasure();
		
		switch (fsm_state){
			case REPOS:
				Etat = NON_OBSERVATION;
				Sortie = 0;
				
				if (g_mode == OBSERVATION_MODE) {
					T = 0;
					NbMes = 0;
					Somme = 0;
					Duree_Att = rand () % 1001 + 2000;
					fsm_state = ATTENTE;
				}
				break;
			case ATTENTE:
				Etat = OBSERVE;
				Sortie = 0;
				
				if (T < Duree_Att) {
					T = T + 1;	
				} else {
					T = 0;
					Sortie = rand() % 7 + 1;
					fsm_state = MESURE;
				}
				break;
			case MESURE:
				Etat = OBSERVE;
				
				if (Entree != Sortie) {
					T = T + 1;
				} else if ((Entree == Sortie) && (NbMes == 0)) {
					resultat.MesMin = T;
					resultat.MesMax = T;
					Somme = T;
						
					NbMes = NbMes + 1;
					fsm_state = ATTENTE_MESURE;
				} else if ((Entree == Sortie) && (NbMes != 0) && (NbMes != NbMesToDO-1)) {
					if (T < resultat.MesMin) resultat.MesMin = T;
					if (T > resultat.MesMax) resultat.MesMax = T;
					Somme = Somme + T;
					
					NbMes = NbMes + 1;
					fsm_state = ATTENTE_MESURE;
				} else if ((Entree == Sortie) && (NbMes == NbMesToDO-1)) {
					if (T < resultat.MesMin) resultat.MesMin = T;
					if (T > resultat.MesMax) resultat.MesMax = T;
					Somme = Somme + T;
				
					NbMes = NbMes + 1;
					resultat.Moyenne = Somme/NbMesToDO;
					if( xQueueSend( ResultQueue,( void * ) &resultat, ( TickType_t ) 0 ) != pdPASS )
					{/* Failed to post the message */}
					fsm_state = FIN_MESURE;
				}
				break;
			case ATTENTE_MESURE:
				Etat = OBSERVE;
				Sortie = 0;
				
				if (Entree ==  0) {
					T = 0;
					Duree_Att = rand () % 1001 + 2000;
					fsm_state = ATTENTE;
				}
				break;
			case FIN_MESURE:
				Sortie = 0;
				Etat = NON_OBSERVATION;
				
				if (Entree == 0) {
					g_mode = NORMAL_MODE;
					fsm_state = REPOS;
				}
				break;
			default:
				fsm_state = REPOS;
				break;
		}
		
		// Allumage de la LED0 en fonction de l'�tat
		if (Etat == OBSERVE) {
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
		} else {
			port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
		}
	}	
}


