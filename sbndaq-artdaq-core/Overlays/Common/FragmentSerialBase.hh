#ifndef sbndaq_Overlays_SBND_FragmentSerialBase_hh
#define sbndaq_Overlays_SBND_FragmentSerialBase_hh

#include "artdaq-core/Data/detail/RawFragmentHeader.hh"

namespace sbndaq {

  class FragmentSerialBase;
  std::ostream& operator<<(std::ostream&, FragmentSerialBase const&);

  class FragmentSerialBase {
  public:
    artdaq::detail::RawFragmentHeader::type_t        fragment_type;
    artdaq::detail::RawFragmentHeader::sequence_id_t sequence_id;
    artdaq::detail::RawFragmentHeader::fragment_id_t fragment_id;
    artdaq::detail::RawFragmentHeader::timestamp_t   timestamp;
    
  private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
      ar & fragment_type;
      ar & sequence_id;
      ar & fragment_id;
      ar & timestamp;
    }
  };
}

#endif
