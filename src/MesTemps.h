/*
 * MesTemps.h
 *
 * Created: 16/12/2019 00:49:39
 *  Author: pasquier-o
 */ 


#ifndef MESTEMPS_H_
#define MESTEMPS_H_

//Function prototype definition
void ConfigureMeasure(void);
void StartMeasure(void);
uint16_t EndMeasure(void);

// lien vers les variables globales
extern struct tc_module tc_instance;


#endif /* MESTEMPS_H_ */