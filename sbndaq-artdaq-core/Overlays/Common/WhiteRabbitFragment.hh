#ifndef sbndaq_Overlays_WhiteRabbitEvent_hh
#define sbndaq_Overlays_WhiteRabbitEvent_hh

#include "artdaq-core/Data/Fragment.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace sbndaq
{
  struct WhiteRabbitEvent;
  struct WhiteRabbitFragmentMetadata;
  enum
  {
    WR_DIO_N_STAMP = 16 /* At least 5 * 3 */
  };
}

struct sbndaq::WhiteRabbitEvent
{
  uint16_t command;	/* from user */
  uint16_t channel;	/* 0..4 or mask from user */
  uint32_t value;	/* for DAC or I/O */
  uint32_t flags;
  uint32_t nstamp;	/* from kernel, if IN_STAMP */
  struct timespec timeStamp[WR_DIO_N_STAMP];
};

struct sbndaq::WhiteRabbitFragmentMetadata
{
  public:
    WhiteRabbitFragmentMetadata() {};
};

#endif

