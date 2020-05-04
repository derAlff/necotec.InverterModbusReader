//============================================================================
// Name        : Modbus Reader V0.0.3
// Author      : Oliver Alff
// Version     :
// Copyright   : Open Source Project GPL v3
// Description : Read Modbus Registers from Solarinverters (SMA, Fronius)
//============================================================================

#include <iostream>
#include <stdio.h>
#include <modbus.h>
#include <modbus-tcp.h>
#include <string.h>
#include <string>
#include <cstring>
#include <fstream>
#include "registers.h"
#include <errno.h>
#include <unistd.h>
#include <cstdlib>
#include <sstream>
#include <ctime>


using namespace std;
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


int listCount = 0; 		//Count the lines from document
int actualLine = 0;		//Count the lines from document xxx_slaves
//int sumSlaves = 0;	  //is the same like "actualLine"????
int csvLineLength = 0;
int currentSlave = 0;		//for the actually slave to read registers

string fileLine;	//Variable for the string from txt
string homePath = "/home/oliver/Schreibtisch/";
string pathIpWr = "ip";
string pathWrMan = "manufr";
string pathDatalogger = "dl";
string pathCsv = "data.csv";

string ipAddresses[32]; 		//Array for 32 Inverters
string manufracturers[32];		//Array for 32 Manufracturers
string checkDataloggers[32];	//Array for 32 definitions of dataloggers
string slaves[32];				//Array for max 32 slaves!

string csvStrings[10];			//Array for CSV lines

time_t timestamp;
tm *now;

bool getLiner = 1;


string getLists()
{
	fstream ipAdr, wrMan, datalogger;

	//open files
	ipAdr.open((homePath + pathIpWr).c_str());
	wrMan.open((homePath + pathWrMan).c_str());
	datalogger.open((homePath + pathDatalogger).c_str());

	while(getline(ipAdr, fileLine))
	{
		ipAddresses[listCount] = string(fileLine);		//get string from txt and set to array
		getline(wrMan, fileLine);					//get line from manufracturers txt
		manufracturers[listCount] = fileLine;		//write manufracturer to array
		getline(datalogger, fileLine);				//get line from dataloggers
		checkDataloggers[listCount] = fileLine;		//write datalogger to array

		listCount++;
	}

	ipAdr.close();		//Close file ip
	wrMan.close();		//close file manufracturers
	datalogger.close();	//close file datalogger

	cout << "IP-Address\t|\tManufracturer\t|\tDatalogger" << endl;
	//write ips and manufracturers to console
	for (int i = 0; i < listCount; i++)
	{
		cout << ipAddresses[i] << "\t|\t " << manufracturers[i] << "\t\t|\t " << checkDataloggers[i] << endl;
	}

	return "";
}

string getSlaves(int arrayNumber)
{
	bool whileLoop = 1;	//for the while-loop downside

	fstream slaveId;
	slaveId.open((homePath + ipAddresses[arrayNumber] + "_slaves").c_str()); 	//open file with slave-id(s)

	while(whileLoop)
	{
		if (getLiner)
		{
			if(getline(slaveId, fileLine))		//get line from file
			{
				slaves[actualLine] = fileLine;	//put slaveID in array
				actualLine++;					//increment actual line
			}
			else{
				whileLoop = 0;
			}

		}
	}

	//getLiner = 0;

	//print slaveIDs to console
	cout << "SlaveID" << endl;
	for (int i = 0; i < actualLine; i++)
	{
		cout << slaves[i] << endl;
	}

	slaveId.close();	//clode file
	//actualLine = 0;		//reset actual line

	return "";
}

string convertHex(uint64_t value, int registerCount, string manufracturer,int type, string cover)
{
	stringstream stream;			//for converting dec to hex
	stream << std::hex << value;	//convert the dec to hex
	string output = stream.str();	//convert hex to string
	long int n = value;
	string datatype;
	float outputFloat = value;

	//load datatype
	if(manufracturer == "fro")
	{
		datatype = froDatatype[type];
	}
	if(manufracturer == "sma")
	{
		datatype = smaDatatype[type];
	}
	if(manufracturer == "sle")
	{
		datatype = sleDatatype[type];
	}

	if(datatype == "string")
	{
		char d[20];
		char b[20];
		memset(b, 0, 20);
		int c = 0;
		while(n > 0) {
			d[c++] = (char)(n % 256);
			n /= 256;
		}
		int c1,c2 = 0;
		for (c1 = c - 1, c2 = 0; c1 >= 0; c1--, c2++)
		b[c2] = d[c1];

	    //printf("%s '%s'\n",cover, b);
		cout << cover << ": " << b << endl;
		return b;
	}
	if(datatype == "int")
	{
		cout << cover << ": " << patch::to_string(value) << endl;
		return patch::to_string(value);
	}
	if(datatype == "1000")
	{
		cout << cover << ": " << patch::to_string(outputFloat /1000) << endl;
		return patch::to_string(outputFloat /1000);
	}
	if(datatype == "1000000")
	{
		cout << cover << ": "<< patch::to_string(outputFloat / 1000000) << endl;
		return patch::to_string(outputFloat / 1000000);
	}

	//cout << "1. " << output.c_str() << endl;	//write hex-String to console

	//cout << "2. " << patch::to_string(value) << endl;

	return "";
}

void writeCsv(string csvString)
{
	/*for(int i = 0; i < csvLineLength; i++)
	{
		csvStrings[0] += "Hersteller, Seriennummer, Aktueller Fehlercode, Status, Momentanleistung [kW], Gesamtertrag [kWh], Tagesertrag [kWh]";
	}
	cout << csvStrings[0] << endl;*/
	csvStrings[0] += (csvString + ";");

}

void writeCsvFile()
{
	//fstream file((homePath + pathCsv).c_str(), ios::out);
	fstream file((homePath + pathCsv).c_str(), ios::app);

	file << csvStrings[0] << endl;
	//file << "\n" << endl;
	file.close();

	/*
	FILE* file;
	file = fopen("/home/oliver/Schreibtisch/data.csv", "w");
	fprintf(file, "\n");
	fprintf(file, "%s", csvStrings[0].c_str());
	fclose(file);
	*/
}

string getTimestamp(){

	timestamp = time(0);
	now = localtime(&timestamp);
	string timestring;
	int hour, min;

	hour = now -> tm_hour;
	min = now -> tm_min;

	if (hour <10)
	{
		timestring = "0" + patch::to_string(now -> tm_hour);
	}
	else {
		timestring = patch::to_string(now -> tm_hour);
	}

	timestring += " : ";

	if (min < 10)
	{
		timestring += "0" + patch::to_string(now -> tm_min);
	}
	else
	{
		timestring += patch::to_string(now -> tm_min);
	}

	return (patch::to_string(now -> tm_hour) + ":" + patch::to_string(now -> tm_min));
}

void readRegister()
{
	uint16_t tabReg[64];		//for saving register data
	uint64_t result = 0;		//return of function
	string value;				//i don't know :)
	modbus_t * ctx;				//for the modbus class
	string returnString;			//for other returns -_-... i don't know :)

	//int arrayCounter;


	//cout << "Array number: " << arrayNumber << endl;

	int rc;						//Read Register from Modbus Class
	//int i;
	int counter = 0;			//Counter for Register (Registers are saved in "registers.h"

	try{
		for (int i = 0; i < listCount; i++)
			{
				getSlaves(i);	//get the Slaves of IP Address (sum of slaves in "actualLine")

				//loop for read registers and write data to console
				while (currentSlave < actualLine)
				{
					for(int j = 0; j < 7; j++)
					{
						ctx = modbus_new_tcp(ipAddresses[i].c_str(), 502);	// New Modbus TCP connection

						//print two lines on console for overview
						cout << "\n\nManufracturer \"" << manufracturers[i] << "\" with slave-ID " << slaves[currentSlave] << endl;
						cout << "Current IP-Address: \"" << ipAddresses[i] << "\" with " << counter +1 << ". register address." << endl;

						//Connect to modbus device
						if (modbus_connect(ctx) == -1) {
							fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
							modbus_free(ctx);
							//return -1;
						}
						else{
							cout << "Connection OK!" << endl;
						}


						if (ctx == NULL) {
							fprintf(stderr, "Unable to allocate libmodbus context\n");
							//return -1;
						}
						else{
							cout << "ctx != NULL" << endl;
						}

						//Set modbus slave id to device
						if(modbus_set_slave(ctx, atoi(slaves[currentSlave].c_str())) == 0)
						{
							cout << "Set Slave..." << endl;
							cout << "Current SlaveID: " << slaves[currentSlave] << endl;
						}
						else{
							cout << "hmmmm.... doch nicht *g" << endl;
						}

						//read register
						if(manufracturers[i] == "fro")
						{
							rc = modbus_read_registers(ctx, froniusRegs[counter], froniusRegLength[counter], tabReg);
							cout << "Fronius Register Address: " << froniusRegs[counter] << " with a length of: " << froniusRegLength[counter] << endl;
						}
						if(manufracturers[i] == "sma")
						{
							rc = modbus_read_registers(ctx, smaRegs[counter], smaRegLength[counter], tabReg);
							cout << "SMA Register Address: " << smaRegs[counter] << " with a length of: " << smaRegLength[counter] << endl;
						}
						if(manufracturers[i] == "sle")
						{
							rc = modbus_read_registers(ctx, solaredgeRegs[counter], solaredgeRegLength[counter], tabReg);
							cout << "SolarEdge Register Address: " << solaredgeRegs[counter] << " with a length of: " << solaredgeRegLength[counter] << endl;
						}

						if (rc == -1) {
							cout << "RC ERROR" << endl;
							fprintf(stderr, "%s\n", modbus_strerror(errno));
							//return -1;
						}
						else{
							cout << "rc OK!"<< endl;
						}

						returnString = "";

						//Write formatted register to console
						for (int k=0; k < rc; k++) {
							printf("reg[%d]=%d (0x%X)\n", k, tabReg[k], tabReg[k]);

							//format register to string
							returnString += tabReg[k];
						}

						cout << "Size of \"rc\" is: " << rc << endl;

						switch(rc)
							{

								case 1:
									result = tabReg[0];
									break;
								case 2:
									result = ((uint64_t)tabReg[0]) << 16 | tabReg[1];
									break;
								case 3:
									result = ((uint64_t)tabReg[0] << 32) | ((uint64_t)tabReg[1] << 16) | (uint64_t)tabReg[2];
									break;
								case 4:
									result = ((uint64_t)tabReg[0] << 48) | ((uint64_t)tabReg[1] << 32) | ((uint64_t)tabReg[2] << 16) | (uint64_t)tabReg[3];
									break;
								case 5:
									result = ((uint64_t)tabReg[0] << 64) | ((uint64_t)tabReg[1] << 48) | ((uint64_t)tabReg[2] << 32) | ((uint64_t)tabReg[3] << 16) | (uint64_t)tabReg[4];
									break;
							}

						//Convert Hex to string
						convertHex(result, counter, manufracturers[i], j, regCovers[j]);
						writeCsv(convertHex(result, counter, manufracturers[i], j, regCovers[j]));

						counter++;			//increment counter for Registers
						modbus_close(ctx);	//close modbus connction
						modbus_free(ctx);	//give free space for a new modbus connection

						sleep(1);			//wait one seconds
					}
					cout << "\n\n########################################################################\n\n" << endl;
					counter = 0;			//after a loop, set the register counter to zero
					currentSlave++;			//after a loop, increment the current slave

					//csvLineLength++;			//increment  the CSV line length
				}

			}
	}
	catch(exception){
		cout << "Error by TRY." << endl;
	}


	//return 0;
}

void removeSlaves()
{
	for(int i = 0; i < 32; i++)
	{
		slaves[i] = "";
	}
}

int main() {
	cout << "Modbus Reader V0.0.3 (c)Oliver Alff" << endl; // prints

	cout << "Initializing" << endl << endl;

	//get lists
	getLists();

	while(1)
	{
		csvStrings[0] == "";
		csvStrings[0] += (getTimestamp() + ";");

		//read modbus data
		readRegister();

		cout << getTimestamp() << endl;

		//write csv
		writeCsvFile();
		cout << csvStrings[0] << endl;
		csvStrings[0] = "";
		currentSlave = 0;
		actualLine = 0;

		//sleep(900);
		sleep(10);
	}

	return 0;
}
