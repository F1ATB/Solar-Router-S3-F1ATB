#pragma once
#include <Arduino.h>

//Paramètres pour RTE
extern byte TempoRTEon ;
extern int LastHeureRTE ;
extern int LTARFbin ;  //Code binaire  des tarifs

void Call_RTE_data();

