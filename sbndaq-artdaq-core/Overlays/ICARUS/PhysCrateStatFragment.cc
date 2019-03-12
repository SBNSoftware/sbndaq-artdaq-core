#include "sbndaq-artdaq-core/Overlays/ICARUS/PhysCrateStatFragment.hh"
#include <iostream>

#include "cetlib_except/exception.h"

std::ostream & icarus::operator << (std::ostream & os, struct PhysCrateStatBlock const & m) {

  os << "\nPhysCrateStatBlock" << std::dec
     << "\n\tSTAT Token: " << m.token
     << "\n\tFIFO Status 1: " << std::hex << m.fifo_status_1
     << "\n\tFIFO Status 2: " << std::hex << m.fifo_status_2
    //<< "\n\tFIFO Status: " << std::hex << m.fifo_status
     << "\n\tAbs time: " << std::dec << m.abstime
     << "\n\tMin time: " << m.mintime
     << "\n\tPacket format version: " << m.pkt_fmt_ver
     << "\n\tCrate id: " << m.crate_id
     << "\n\tSize: " << m.size;

  return os;
}

std::ostream & icarus::operator << (std::ostream & os, PhysCrateStatFragment const & f) {
  os << *(f.StatBlock());
  return os;
}
bool icarus::PhysCrateStatFragment::Verify() const { return true; }

