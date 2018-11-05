#include "sbndaq-artdaq-core/Overlays/SBND/NevisTPC/NevisTPCTypes.hh"

std::ostream & sbnddaq::operator  << (std::ostream & os, NevisTPCHeader const & h){

	os << " NevisTPCHeader." << std::endl;
	os << std::hex << std::setfill('0') << std::setw(4);
	os << "Slot[" << (int)h.getSlot() << "], FEMID["  << (int)h.getFEMID()  << "], RAW[0x";
	os << std::setw(8) << h.id_and_slot << "]" << std::endl;

	os << std::hex << std::setfill('0') << std::setw(4);
	os << "ADCWordCount[" << h.getADCWordCount()  << "], RAW[0x" << h.word_count << "]" << std::endl;
	os << "EventNum[" << h.getEventNum() << "], RAW[0x" << h.event_num << "]" <<std::endl;
	os << "FrameNum[" << h.getFrameNum() << "], RAW[0x" << h.frame_num << "]" <<std::endl;
	os << "Checksum[" << h.getChecksum() << "], RAW[0x" << h.checksum << "]" <<std::endl;
	os << "TrigSample2mhz[" << h.get2mhzSample() << "], TrigFrame[" << h.getTrigFrame() 
	   << "], RAW[0x" << h.trig_frame_sample << "]" << std::endl;

	return os;
}

