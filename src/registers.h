/*
 * registers.h
 *
 *  Created on: 12.02.2019
 *      Author: oliver
 */

#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <iostream>
#include <stdint.h>		//uint8_t usw...
#include <string>

/*
 *SMA
 * */

//Die Register von SMA
int smaRegs[9] = {
						 30055,				//Hersteller				0
						 40067,				//Seriennummer				1
						 30247,				//aktueller Fehlercode		2
						 30201,				//Status					3
						 30775,				//Aktuelle AC Leistung (W!)	4
						 30513,				//Gesamtertrag (Wh!)		5
						 30517,				//Ertrag Heute (Wh!)		6
};


//Registerlängen von SMA
int smaRegLength[9] ={	2,
							2,
							2,
							2,
							2,
							4,
							4
};

std::string smaDatatype[9] = {
								"string",
								"int",
								"int",
								"int",
								"1000",
								"1000",
								"1000"
};

/*
 *Fronius
 * */

//Die Register von Fronius
int froniusRegs[9] = {
							40004,				//Hersteller
							40052,				//Seriennummer
							213,				//aktueller Fehlercode
							40108,				//Status
							499,				//Aktuelle AC Leistung (W!)
							40186,				//Gesamtertrag (Wh!)
							501,				//Ertrag Heute (Wh!)
};

//Registerlängen von Fronius
int froniusRegLength[9] ={
								4,				//Hersteller
								4,				//Seriennummer
								2,				//Aktueller Fehlercode
								1,				//Status
								2,				//Aktuelle AC Leistung (in W!)
								4,				//Gesamtertrag (in Wh!)
								4				//Ertrag heute (in Wh!)
};

std::string froDatatype[9] = {
								"string",
								"string",
								"int",
								"int",
								"1000",
								"1000",
								"1000"

};

//Die Register von SolarEdge
int solaredgeRegs[9] = {
							40003,				//Hersteller
							40052,				//Seriennummer
							40108,				//aktueller Fehlercode
							40108,				//Status
							40083,				//Aktuelle AC Leistung (W!)
							40093,				//Gesamtertrag (Wh!)
							40093,				//Ertrag Heute (Wh!)
};

//Registerlängen von Fronius
int solaredgeRegLength[9] ={
								5,				//Hersteller
								4,				//Seriennummer
								1,				//Aktueller Fehlercode
								1,				//Status
								1,				//Aktuelle AC Leistung (in W!)
								2,				//Gesamtertrag (in Wh!)
								2				//Ertrag heute (in Wh!)
};

std::string sleDatatype[9] = {
								"string",
								"string",
								"int",
								"int",
								"1000",
								"1000",
								"1000"

};

//Die Beschreibung der Register
std::string regCovers[9] = {	 "Hersteller",
								 "Seriennummer",
								 "aktueller Fehlercode",
								 "Status",
								 "Aktuelle AC Leistung (kW)",
								 "Gesamtertrag (kWh)",
								 "Ertrag Heute (kWh)",
};

#endif /* REGISTERS_H_ */
