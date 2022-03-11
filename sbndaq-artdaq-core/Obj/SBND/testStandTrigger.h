#ifndef testStandTrigger_h
#define testStandTrigger_h

#include <cstddef>
#include <vector>

namespace sbndaq {

  struct testStandTrigger{

    size_t nAboveThreshold;

    std::vector<int> PMT_chA;
    std::vector<int> PMT_chB;
    std::vector<int> PMT_chC;
    std::vector<int> PMT_chD;
    std::vector<int> PMT_chE;
    std::vector<int> RWM;

    testStandTrigger() {}

  };

} // namespace sbndaq

#endif
