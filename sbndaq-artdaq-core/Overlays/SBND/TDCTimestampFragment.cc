#include "sbndaq-artdaq-core/Overlays/SBND/TDCTimestampFragment.hh"

#include "cetlib_except/exception.h"

std::ostream& sbndaq::operator<<(std::ostream& os, TDCTimestampFragment const& f) {
  os << "TDCTimestampFragment:{" << *f.getTDCTimestamp() << "}\n";

  return os;
}

std::ostream& sbndaq::operator<<(std::ostream& os, TDCTimestamp const& t) {
  os << "TDCTimestamp:{" << std::left;
  os << " name=" << std::setw(8) << std::string(std::begin(t.vals.name), std::end(t.vals.name)).c_str();
  os << std::right;
  os << " chan=" << std::setw(1) << t.vals.channel;
  os << " secs=" << std::setw(10) << t.vals.seconds;
  os << " nano=" << std::setw(9) << t.nanoseconds();
  os << " coarse=" << std::setw(9) << t.vals.coarse;
  os << " frac=" << std::setw(4) << t.vals.frac;
  os << std::left;
  os << " tai2epoch_offset=" << std::setw(2) << t.vals.tai2epoch_offset;
  os << " }";

  return os;
}

