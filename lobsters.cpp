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
  return ((double)t.tv_sec + ((double)t.tv_usec)/1000000.);
}

//stolen code here
class PartyTime {

  bool m_arduino; // send directions to serial port?

  double m_pitch; //from Jackie
  double m_roll; //from Jackie
  double m_yaw; //from Jackie

//  double m_dAzimuth;
//  double m_cAzimuth;
//  double m_dAltitude;
//  double m_cAltitude;
//  double m_tolerance;
//  double m_LeftThruster;
//  double m_RightThruster;
//  double m_TopThruster;
//  double m_dDistance;
//  double m_cDistance;

  double m_Xis; //from Lucille
  double m_Xgoal; //obtained from path
  double m_Yis; //from Lucille
  double m_Ygoal; //obtained from path
  double m_headingIs; //calculated here, referenced to magnetic north
  double m_headingGoal; //calculated here
  double m_path[]; //array of all the path coordinates



  Serial m_serial;

public:

  // default constructor
  PartyTime() :
    // default settings, most can be modified through command line options (see below)
	m_pitch(0),
	m_roll(0),
	m_yaw(0),

	m_arduino(false),

//	m_dAzimuth(0),
//    m_cAzimuth(0),
//    m_dAltitude(0),
//    m_cAltitude(0),
//    m_tolerance(0),
    m_LeftThrust(0),
    m_RightThrust(0),
    m_TopThrust(0)

   {}

  void setup() {
    // prepare serial port for communication with Arduino
    if (m_arduino) {
      m_serial.open("/dev/ttyACM0");
    }
  }

// not stolen code here

  void loop() {
    while (true) {
    	double headingErr;
    	double depthErr;
    	double headingCurr;
    	double headingPrev;
    	double headingGoal;
    	//if Xcurr and Ycurr are within range o
    	//then index of carrotCurr = next
    	//else index of carrotCurr  = same
    	headingCurr = path(ind);
    	if (ind == 0) {
    		headingPrev = path(0);
    		else {
    			headingPrev = path(ind - 1);
    		}
    	}
    	headingErr = headingGoal - headingCurr;
    	headingGoal = 90 - atan((Ygoal - Yis)/(Xgoal - Xis));





    	//PD control of left thruster
    	m_LeftThrust = (m_k1 * headingErr) + (m_k2 * (headingCurr - headingPrev));
    	//PD control of right thruster
    	m_RightThrust = -(m_k1 * headingErr) - (m_k2 * (headingCurr - headingPrev));
    	//PD control of depth
    	m_TopThrust = (m_k3 * depthErr

//    	//if we're too far left
//    	if (m_cAzimuth < (m_dAzimuth - m_tolerance)) {
//    		// turn right
//    		m_LeftThruster = 12;
//    	}
//    	//if we're too far right
//    	else if (m_cAzimuth > (m_dAzimuth + m_tolerance)) {
//    		// turn left
//    		m_RightThruster = 12;
//    	}
//    	// if we're tipped too far downward
//    	if (m_cAltitude < (m_dAltitude - m_tolerance)) {
//    	    //turn upwards
//    		m_TopThruster = -12;
//    	}
//    	//if we're tipped too far upward
//    	else if (m_cAltitude > (m_dAltitude + m_tolerance)) {
//    	    // turn upwards
//    		m_TopThruster = 12;
//    	// if we're oriented correctly
//    	if (m_cAzimuth < (m_dAzimuth + m_tolerance) && m_cAzimuth > (m_dAzimuth - m_tolerance) && m_cAltitude < (m_dAltitude + m_tolerance) && m_cAltitude > (m_dAltitude - m_tolerance)) {
//    		//move forwards
//    		m_LeftThruster = 12;
//    		m_RightThruster = 12;
//    	}

    	if (m_arduino) {
			// here put information to be printed to serial port
    		// m_serial.print(detections[0].id);
			 // m_serial.print(",");
			 // m_serial.print(translation(0));
			 // m_serial.print(",");
			 // m_serial.print(translation(1));
			  //m_serial.print(",");
			  //m_serial.print(translation(2));
			  //m_serial.print("\n");
			}
		}
    }
  }

  int main() {
	PartyTime party;
	party.setup();
	party.loop();
  }



