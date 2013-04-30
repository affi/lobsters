//============================================================================
// Name        : lobsters.cpp
// Author      : Janille M Maragh
// Version     :
// Copyright   : 2013
// Description : Hello World in C++, Ansi-style
//============================================================================

using namespace std;

#include <iostream>
#include <cstring>
#include <vector>
#include <sys/time.h>
#include "Serial.h"
#include <stdio.h>
#include <math.h>

// Needed for getopt / command line options processing
#include <unistd.h>
extern int optind;
extern char *optarg;

const string usage = "\n"
		"Usage:\n"
		//put usage text here
		"\n";

// utility function to provide current system time
double tic() {
	struct timeval t;
	gettimeofday(&t, NULL);
	return ((double) t.tv_sec + ((double) t.tv_usec) / 1000000.);
}

//stolen code here
class PartyTime {

	// serial ports open?
	bool m_arduino; // send directions to serial port?
	bool m_serialLucille; //receive information from acoustics system
	bool m_serialJackie; //receive information from sensors

	//current orientation of lobsterbot, from Jackie
	double m_pitch;
	double m_roll;
	double m_yaw; //
	double m_headingIs;

	// current position of lobsterbot, from Lucille
	double m_Xis;
	double m_Yis;

	//goal position/orientation of lobsterbot
	double m_Xgoal; //obtained from path
	double m_Ygoal; //obtained from path

	//thruster commands
	double m_LeftThrust;
	double m_RightThrust;
	double m_TopThrust;

	//path variables
	double m_Xwidth;
	double m_Ywidth;
	double m_numCoords;
	double m_path[][];
	int ind;

	double depthTic;
	double headingTic;

	Serial m_serial;

public:

	// default constructor
	PartyTime() :
			// default settings
			m_pitch(0), m_roll(0), m_yaw(0),

			m_arduino(false),

			m_LeftThrust(0), m_RightThrust(0), m_TopThrust(0),

			ind(0),

			m_Xwidth(10), m_Ywidth(10), m_numCoords(121),

			depthTic(0), headingTic(0), depthIs(0), headingIs(0)

	{
	}

	void setup() {
		// prepare serial port for communication with Arduino
		if (m_arduino) {
			m_serial.open("/dev/ttyACM0");
		}
		// create the path
		double m_path[110][2]; //each row is one x,y coordinate
		int n, m;
		// fill in the elements of the array
		for (n = 0; n < m_numCoords; n++) {
			m_path[n][1] = n / 11;
			if (m_path[n][1] % 2 == 0) {
				m_path[n][0] = n % 11;
			} else {
				m_path[n][0] = 10 - (n % 11);
			}
		}
	}

	void loop() {
		while (true) {
			double depthIs;
			double Xis;
			double Yis;
			double pitch;
			double roll;
			double yaw;
			int emergency;
			// Lucille is sending in information from acoustics
			if (m_serialLucille) {
				depthIs = 0;
				Xis = 0;
				Yis = 0;
			}
			// Jackie is sending in information from sensors
			if (m_serialJackie) {
				pitch = 0;
				roll = 0;
				yaw = 0;
				emergency = 0;

			}
			if (emergency == 1) {
				break;
			}

			double Xgoal = [ind][0];;
			double Ygoal = [ind][1];

			//calculate heading using pitch/roll/yaw
			double headingIs = 0; //read in from serial port

			if (Xgoal - r) < Xis < (Xgoal + r) && (Ygoal - r) < Yis < (Ygoal + r) {
				ind++;
			}

			// carrotCurr is our current target
			carrotCurr = path(ind);
			// carrotPrev is our last target
			if (ind == 0) {
				carrotPrev = path(0);
				else {
					carrotPrev = path(ind - 1);
				}
			}

			// calculates heading relative to +y direction
			double headingGoal = 90 - atan2((Ygoal - Yis), (Xgoal - Xis));
			// heading error to be used for PD control
			double headingErr = headingGoal - headingIs;

			//PD control of left thruster
			m_LeftThrust = (m_k1 * headingErr) + (m_k2 * (headingIs - headingTic));
			//PD control of right thruster
			m_RightThrust = -(m_k1 * headingErr)
					- (m_k2 * (headingIs - headingTic));
			//PD control of depth
			m_TopThrust = (m_k3 * depthErr) + (m_k4 * (depthIs - depthTic));

			if (m_arduino) {
				// thruster commands printed to serial port to arduino here
				m_serial.print(m_LeftThrust);
				m_serial.print(",");
				m_serial.print(m_RightThrust);
				m_serial.print(",");
				m_serial.print(m_TopThrust);
				m_serial.print("\n");
			}
			double headingTic = headingIs;
			double depthTic = depthIs;
			}
		}

}

int main() {
	PartyTime party;
	party.setup();
	party.loop();
}

