#include "NevisTBFragment.hh"

using namespace sbndaq;

std::string NevisTrigger_Header::debugInfo() const noexcept
{
  std::ostringstream os;

  os << " Busy[ " << getBusyFlag() << " ],"
     << " 2MHzSample[ " << get2MHzSampleNumber() << " ],"
     << " 16MHzRemainder[ " << get16MHzRemainderNumber() << " ],"
     << " RAW[0x" << trig_time << "]" << std::endl;

  os << " Frame[ " << getFrame() << " ],"
     << " TriggerNumber[ " << getTriggerNumber() << " ],"
     << " RAW[0x" << frame_1 << " 0x" << frame_2_and_ntrig_1 << " 0x" << ntrig_2 << "]"
     << std::endl;

  return os.str();
}

size_t NevisTrigger_Header::getWordCount() const noexcept
{
  return ( sizeof(sbndaq::NevisTrigger_Data) / sizeof(uint32_t) ); //raw_data_type) );                                                                                                        
}

std::string NevisTrigger_Data::debugInfo() const noexcept
{
  std::ostringstream os;

  //  os << "Object " << demangle(typeid(this)) << "."<< std::endl;
  os << std::hex << std::setfill('0') << std::setw(8);
  os << " RAW1[0x" << trig_data_1 << "], RAW2[0x" << trig_data_2 << "]" << std::endl;
  os << "   PMT[0x" << getPMTTrigData() << "]" << std::endl;
  os << "       PC[ " << Trig_PC() << " ]" << std::endl;
  os << "      EXT[ " << Trig_EXT() << " ]" << std::endl;
  os << "    CALIB[ " << Trig_Calib() << " ]" << std::endl;
  os << "   Active[ " << Trig_Active() << " ]" << std::endl;
  os << "     VETO[ " << Trig_Veto() << " ]" << std::endl;
  os << "    GATE1[ " << Trig_Gate1() << " ]" << std::endl;
  os << "    GATE2[ " << Trig_Gate2() << " ]" << std::endl;
  os << " GATEFAKE[ " << Trig_GateFake() << " ]" << std::endl;
  os << " BEAMFAKE[ " << Trig_BeamFake() << " ]" << std::endl;
  os << "    SPARE[ " << Trig_Spare1() << " ]" << std::endl;
  os << "    Phase[0x" << getPhase() << "]" << std::endl;

  return os.str();
}

std::string NevisTrigger_Trailer::debugInfo() const noexcept
{
  std::ostringstream os;
  //  os << "Object " << demangle(typeid(this)) << "."<< std::endl;
  os << std::hex << std::setfill('0') << std::setw(8);

  os << " Trailer1[0x" << trailer1 << "], Trailer2[0x" << trailer2 << "]" << std::endl;
  return os.str();
}
