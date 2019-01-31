#ifndef sbndaq_Overlays_SpectratimeEvent_hh
#define sbndaq_Overlays_SpectratimeEvent_hh

#include "artdaq-core/Data/Fragment.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace sbndaq{

	struct SpectratimeEvent;

	struct SpectratimeFragmentMetadata;
}

struct sbndaq::SpectratimeEvent
{
  // $PTNTA
  uint32_t timeStamp;
  uint16_t oscillatorQuality;
  int32_t  ppsDifference;        // ns
  int32_t  finePhaseComparator;  // ns
  uint16_t status;
  uint16_t message;
  uint16_t transferQuality;
  
  // $PTNTS,B
  uint16_t actualFrequency; // steps of 5.12e-13
  uint16_t holdoverFrequency; // steps of 5.12e-13
  uint16_t eepromFrequency; // steps of 5.12e-13
  uint16_t loopTimeConstantMode; // 0:fixed value, 1:automatic
  uint16_t loopTimeConstantInUse; // 100 to 999999 seconds
  float sigmaPPS; // in approx ns

  // $GPRMC
  char messageStatus; // A: valid; V: not valid
  float latitude;    // XXYY.YYYY  X=Degrees  Y=minutes
  char hemisphereNS;  // N or S
  float longitude;   // XXYY.YYYY  X=Degrees  Y=minutes
  char hemisphereEW;  // E or W

  // MISC
  struct timespec systemTime;
  struct timespec systemTimer;
  float   systemDifference;
  char location[22];  // 41°50.31'N 88°16.17'W
  char timeStampString[15];  // 20181207181920
};

struct sbndaq::SpectratimeFragmentMetadata{

public:
  SpectratimeFragmentMetadata() {};

};

#endif

