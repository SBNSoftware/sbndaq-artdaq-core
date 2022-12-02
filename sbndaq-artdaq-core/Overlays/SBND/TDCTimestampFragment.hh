#ifndef sbndaq_Overlays_SBND_TDCTimestampFragment_hh
#define sbndaq_Overlays_SBND_TDCTimestampFragment_hh

#include "artdaq-core/Data/Fragment.hh"

#include <array>
#include <ostream>

namespace sbndaq {

class TDCTimestampFragment;
std::ostream& operator<<(std::ostream&, TDCTimestampFragment const&);

struct TDCTimestamp;
std::ostream& operator<<(std::ostream&, TDCTimestamp const&);
/*
inline TraceStreamer& operator<<(TraceStreamer& x, sbndaq::TDCTimestampFragment const& r) {
  std::stringstream ss;
  ss << r;
  return x << ss.str().c_str();
}
*/
/**
 * FMC-TDC time-stamp descriptor
 */
struct TDCTimestamp {
  typedef uint64_t data_t;
  static constexpr size_t const size_words = 5ul;
  static constexpr size_t size_bytes = size_words * sizeof(TDCTimestamp::data_t);
  static constexpr size_t name_length = 8;
  using name_t = std::array<char, name_length>;
  struct vals_t {
    data_t seconds;
    data_t coarse : 32;
    data_t frac : 32;
    data_t channel : 8;
    data_t tai2epoch_offset : 8;
    data_t unused : 16;
    name_t name;
  };
  union {
    vals_t vals;
    std::array<data_t, size_words> data = {};
  };

  using value_type = std::array<data_t, size_words>::value_type;
  using iterator = std::array<data_t, size_words>::iterator;
  using const_iterator = std::array<data_t, size_words>::const_iterator;

  // Provides a nano-second approximation of the timestamp.
  uint64_t nanoseconds() const {
    uint64_t ns = 0;
    ns += vals.coarse * 8;
    ns += vals.frac * 8 / 4096;
    return ns;
  }

  uint64_t timestamp_ns() const {
    uint64_t ns = 0;
    ns += vals.seconds * 1000000000;
    ns += nanoseconds();
    return ns;
  }
};

static_assert(sizeof(TDCTimestamp) == TDCTimestamp::size_words * sizeof(TDCTimestamp::data_t),
              "TDCTimestamp size changed");

class TDCTimestampFragment {
 public:
  struct Metadata {
    typedef uint64_t data_t;
    data_t reserved;
    static constexpr size_t const size_words = 1ul;
  };

  static_assert(sizeof(Metadata) == Metadata::size_words * sizeof(Metadata::data_t),
                "TDCTimestampFragment::Metadata size changed");

  explicit TDCTimestampFragment(artdaq::Fragment const& f) : artdaq_Fragment_(f) {}

  Metadata const* metadata() const { return artdaq_Fragment_.metadata<Metadata>(); }
  sbndaq::TDCTimestamp const* getTDCTimestamp() const {
    return reinterpret_cast<TDCTimestamp const*>(artdaq_Fragment_.dataBeginBytes());
  }

 private:
  artdaq::Fragment const& artdaq_Fragment_;
};
}  // namespace sbndaq
#endif /* sbndaq_Overlays_SBND_TDCTimestampFragment_hh */
