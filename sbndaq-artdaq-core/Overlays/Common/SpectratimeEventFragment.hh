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

struct sbndaq::SpectratimeEvent{
	char time[7];
	char quality;
	short phase;
	char status;
};

struct sbndaq::SpectratimeFragmentMetadata{

public:
  SpectratimeFragmentMetadata() {};

};

#endif

