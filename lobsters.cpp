//============================================================================
// Name        : lobsters.cpp
// Author      : Janille M Maragh
// Version     :
// Copyright   : 2013
// Description : Hello World in C++, Ansi-style
//============================================================================

using namespace std;

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sys/time.h>
#include "Serial.h"
#include <stdio.h>
#include <math.h>
#include <fstream>
//#include <stdlib.h>
#define PI (3.141592653589793)

// Needed for getopt / command line options processing
#include <unistd.h>
extern int optind;
extern char *optarg;

const string usage = "\n"
		"Usage:\n"
		"  M [OPTION...] [deviceID]\n" "Options:\n"
		"  R  -?          Show help options\n"
		"  L              Arduino (send tag ids over serial port)\n"
		"\n";

// utility function to provide current system time
double tic() {
	struct timeval t;
	gettimeofday(&t, NULL);
	return ((double) t.tv_sec + ((double) t.tv_usec) / 1000000.);
}

//http://www.webalice.it/fede.tft/serial_port/serial_port.html
class Demo {

	// serial ports open?
	bool m_arduino; // send directions to serial port?
	bool m_fileStream; // send data to text file?
	bool m_terminal; // print commands to terminal?

	bool m_manual; // are we in manual mode?
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
	double m_path[121][2];
	int ind;

	double depthTic;
	double headingTic;
	double headingIs;
	double m_depthGoal;

	//PD controller constants
	double m_k1;
	double m_k2;
	double m_k3;
	double m_k4;

	//stuff we get from arduino
	float pitch;
	float roll;
	float yaw;
	float depthIs;
	int emergency;

	//defines square around target point - in the square == at the target
	double m_r;

	Serial m_serial;

public:

	// default constructor
	Demo() :
	// default settings
	m_pitch(0), m_roll(0), m_yaw(0),

	m_fileStream(false), m_arduino(false), m_terminal(true),

	m_serialLucille(false), m_serialJackie(false),

	m_manual(true),

	m_LeftThrust(0), m_RightThrust(0), m_TopThrust(0),

	ind(0),

	m_Xwidth(10), m_Ywidth(10), m_numCoords(121),

	depthTic(0), headingTic(0), depthIs(0), headingIs(0), m_depthGoal(1),

	m_k1(0), m_k2(0), m_k3(0), m_k4(0),

	m_r(0.25)

	{
	}

	void parse(string s) {
		cout << "s = |" << s << "|" << endl;

		// try to parse the line
		int num = sscanf(s.c_str(), "%f,%f,%f,%f,%d", &pitch,&roll,&yaw,&depthIs,
				&emergency);
		cout << "num = " << num << endl;
		cout << ", pitch = " << pitch << endl;

	}

	void setup() {
		// prepare serial port for communication with Arduino
		if (m_arduino) {
			m_serial.open("/dev/ttyACM0");
		}

		// create the path
		int n, m;
		// fill in the elements of the array
		for (n = 0; n < m_numCoords; n++) {
			m_path[n][1] = n / 11;
			if ((int) trunc(m_path[n][1]) % 2 == 0) {
				m_path[n][0] = n % 11;
			} else {
				m_path[n][0] = 10 - (n % 11);
			}
		}
	}

	void loop() {
		while (true) {
			if (m_manual == false) {
				cout << "automatic mode" << endl;
				double Xis;
				double Yis;
				double pitch;
				double roll;
				double yaw;
				int emergency;
				// Lucille is sending in information from acoustics
				if (m_serialLucille) {
					Xis = 0;
					Yis = 0;
				}
				if (emergency == 1) {
					break;
				}

				double Xgoal = m_path[ind][0];
				double Ygoal = m_path[ind][1];

				//calculate heading using pitch/roll/yaw
				double headingIs = 0; //read in from serial port

				if ((Xgoal - m_r) < Xis && Xis < (Xgoal + m_r)
						&& (Ygoal - m_r) < Yis && Yis < (Ygoal + m_r)) {
					if (ind < 120) {
						ind++;
					}
				}

				// calculates heading relative to +y direction
				double headingGoal = 90 - atan2((Ygoal - Yis), (Xgoal - Xis));
				// heading & depth error to be used for PD control
				double headingErr = headingGoal - headingIs;
				double depthErr = m_depthGoal - depthIs;

				if (headingErr > (PI)) {
					headingErr = headingErr - (2 * PI);
				} else if (headingErr < (PI)) {
					headingErr = headingErr + (2 * PI);
				}
				//PD control of left thruster
				m_LeftThrust = (m_k1 * headingErr)
						+ (m_k2 * (headingIs - headingTic));
				//PD control of right thruster
				m_RightThrust = -(m_k1 * headingErr)
						- (m_k2 * (headingIs - headingTic));
				//PD control of depth
				m_TopThrust = (m_k3 * depthErr) + (m_k4 * (depthIs - depthTic));

				if (m_terminal) {
					cout << "mterminal" << endl;
				}
				if (m_fileStream) {
					ofstream myfile("scrapData.txt");
					if (myfile.is_open()) {
						myfile << "This is a line.\n";
						myfile << "This is another line.\n";
						myfile.close();
					}
				}
				if (m_arduino) {
					// thruster commands printed to serial port to arduino here
					m_serial.print(m_LeftThrust);
					m_serial.print(" ");
					m_serial.print(m_RightThrust);
					m_serial.print(" ");
					m_serial.print(m_TopThrust);
					m_serial.print("\n");
				}
				double headingTic = headingIs;
				double depthTic = depthIs;
			}
		}

	}
}
;

int main() {
	Serial serial;
	serial.open("/dev/tty.usbmodem411", 9600); // might need to change to your USB port
	Demo demo;
	// read and parse one line at a time
	while (true) {

		string s = serial.readBytesUntil('\n');
		//cout << "read " << s.length() << " characters" << endl;
		//cout << s << endl;
		//sleep(1);
		demo.parse(s);
		}
	//demo.setup();
	//demo.loop();

}

