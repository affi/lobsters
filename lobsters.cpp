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

  //bool m_arduino; // send tag detections to serial port?

  double m_dAzimuth;
  double m_cAzimuth;
  double m_dAltitude;
  double m_cAltitude;
  double m_tolerance;
  double m_LeftThruster;
  double m_RightThruster;
  double m_TopThruster;
  double m_dDistance;
  double m_cDistance;

  //Serial m_serial;

public:

  // default constructor
  PartyTime() :
    // default settings, most can be modified through command line options (see below)
    m_dAzimuth(10),
    m_cAzimuth(3),
    m_dAltitude(7),
    m_cAltitude(2),
    m_tolerance(10),
    m_LeftThruster(0),
    m_RightThruster(0),
    m_TopThruster(0)
   {}

  // changing the tag family

// not stolen code here

  void loop() {
    while (true) {
    	//if we're too far left
    	if (m_cAzimuth < (m_dAzimuth - m_tolerance)) {
    		// turn right
    		m_LeftThruster = 12;
    	}
    	//if we're too far right
    	else if (m_cAzimuth > (m_dAzimuth + m_tolerance)) {
    		// turn left
    		m_RightThruster = 12;
    	}
    	// if we're tipped too far downward
    	if (m_cAltitude < (m_dAltitude - m_tolerance)) {
    	    //turn upwards
    		m_TopThruster = -12;
    	}
    	//if we're tipped too far upward
    	else if (m_cAltitude > (m_dAltitude + m_tolerance)) {
    	    // turn upwards
    		m_TopThruster = 12;
    	// if we're oriented correctly
    	if (m_cAzimuth < (m_dAzimuth + m_tolerance) && m_cAzimuth > (m_dAzimuth - m_tolerance) && m_cAltitude < (m_dAltitude + m_tolerance) && m_cAltitude > (m_dAltitude - m_tolerance)) {
    		//move forwards
    		m_LeftThruster = 12;
    		m_RightThruster = 12;
    	}
    }
  }

  int main() {
	PartyTime party;
	// party.setup();
	party.loop();
  }



