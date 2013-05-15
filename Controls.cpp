/************************************************************/
/*    NAME: Janille Maragh                                              */
/*    ORGN: MIT                                             */
/*    FILE: Controls.cpp                                        */
/*    DATE: May 14, 2013                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Controls.h"
#include "Serial.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>


using namespace std;

int desired_rudder;
int desired_thrust;
int desired_elevator;
int m_iterations;
int m_timewarp;
bool m_arduino;
Serial m_serial;

//---------------------------------------------------------
// Constructor

Controls::Controls() {
	m_iterations = 0;
	m_timewarp = 1;
	desired_rudder = 0;
	desired_thrust = 0;
	desired_elevator = 0;
	m_arduino = true;
	if (m_arduino) {
		m_serial.open("/dev/tty.usbmodem411");
	}
}

//---------------------------------------------------------
// Destructor

Controls::~Controls() {

}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Controls::OnNewMail(MOOSMSG_LIST &NewMail) {
	MOOSMSG_LIST::iterator p;

	for (p = NewMail.begin(); p != NewMail.end(); p++) {
		CMOOSMsg &msg = *p;

		// look through the messages, and
		// if the message is DESIRED_RUDDER, extract the value
		// and assign it to desired_rudder
		// if the message is DESIRED_THRUST, extract the value
		// and assign it to desired_thrust

		if (msg.m_sKey == "DESIRED_RUDDER") {
			MOOSTrace("Received DESIRED_RUDDER, value is %lf\n", msg.m_dfVal);
			desired_rudder = msg.m_dfVal;
		}

		if (msg.m_sKey == "DESIRED_THRUST") {
			MOOSTrace("Received DESIRED_THRUST, value is %lf\n", msg.m_dfVal);
			desired_thrust = msg.m_dfVal;
		}
		if (msg.m_sKey == "DESIRED_ELEVATOR") {
			MOOSTrace("Received DESIRED_ELEVATOR, value is %lf\n", msg.m_dfVal);
			desired_elevator = msg.m_dfVal;
		}

#if 0 // Keep these around just for template
		string key = msg.GetKey();
		string comm = msg.GetCommunity();
		double dval = msg.GetDouble();
		string sval = msg.GetString();
		string msrc = msg.GetSource();
		double mtime = msg.GetTime();
		bool mdbl = msg.IsDouble();
		bool mstr = msg.IsString();
#endif
	}
	return (true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Controls::OnConnectToServer() {
	// register for variables here
	// possibly look at the mission file?
	// m_MissionReader.GetConfigurationParam("Name", <string>);
	// m_Comms.Register("VARNAME", 0);

	RegisterVariables();
	return (true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Controls::Iterate() {
	m_iterations++;

	cout << "XIteration " << m_iterations;
	cout << ", desired_rudder = ," << desired_rudder;
	cout << ", desired_thrust = ," << desired_thrust;
	cout << ", desired_elevator = " << desired_elevator;
	cout << endl;

	// send desired rudder and desired thrust to the arduino:
	//m_Comms.Notify("DESIRED_RUDDER", rudder);
	//m_Comms.Notify("DESIRED_THRUST", thrust);


	if (m_arduino) {
		// thruster commands printed to serial port to arduino here
		m_serial.print(desired_rudder);
		m_serial.print("+");
		m_serial.print(desired_thrust);
		m_serial.print(",");
		m_serial.print(desired_elevator);
		m_serial.print("\n");
	}

	return (true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open



bool Controls::OnStartUp() {
	cout << "pControls starting...." << endl;

	list<string> sParams;
	m_MissionReader.EnableVerbatimQuoting(false);
	if (m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
		list<string>::iterator p;
		for (p = sParams.begin(); p != sParams.end(); p++) {
			string original_line = *p;
			string param = stripBlankEnds(toupper(biteString(*p, '=')));
			string value = stripBlankEnds(*p);

			if (param == "FOO") {
				//handled
			} else if (param == "BAR") {
				//handled
			}
		}
	}

	m_timewarp = GetMOOSTimeWarp();
	Serial serial;
	serial.open("/dev/tty.usbmodem411", 115200); // might need to change to your USB port
	RegisterVariables();
	return (true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Controls::RegisterVariables() {
	m_Comms.Register("DESIRED_RUDDER", 0);
	m_Comms.Register("DESIRED_THRUST", 0);
	m_Comms.Register("DESIRED_ELEVATOR", 0);

}

