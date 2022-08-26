#include "artdaq-core/Data/detail/RawFragmentHeader.hh"

namespace sbndaq {

  class BernCRTFragmentSerial {
  public:
    artdaq::detail::RawFragmentHeader::sequence_id_t sequence_id;
    artdaq::detail::RawFragmentHeader::fragment_id_t fragment_id;
    artdaq::detail::RawFragmentHeader::timestamp_t   timestamp;
    
  private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
      ar & sequence_id;
      ar & fragment_id;
      ar & timestamp;
    }
  };
}
