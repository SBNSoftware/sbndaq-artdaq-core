#include "sbndaq-artdaq-core/Overlays/FragmentType.hh"

#include <algorithm>
#include <cassert>
#include <string>
#include <map>


namespace {
  static std::map<sbndaq::detail::FragmentType, std::string> const
  names{
    {sbndaq::detail::FragmentType::MISSED,           "MISSED"}, 

    //Common
    {sbndaq::detail::FragmentType::CAENV1730,        "CAENV1730"},
    {sbndaq::detail::FragmentType::SpectratimeEvent, "SPECTRATIMEVENT"},
    {sbndaq::detail::FragmentType::BERNCRT,          "BERNCRT"},
    {sbndaq::detail::FragmentType::BERNCRTZMQ,       "BERNCRTZMQ"},

    //ICARUS
    {sbndaq::detail::FragmentType::PHYSCRATEDATA,    "PHYSCRATEDATA"},
    {sbndaq::detail::FragmentType::PHYSCRATESTAT,    "PHYSCRATESTAT"},

    //SBND
    {sbndaq::detail::FragmentType::NevisTPC,         "NEVISTPC"},
    {sbndaq::detail::FragmentType::PTB,              "PTB"},

    //Simulators
    {sbndaq::detail::FragmentType::DummyGenerator,   "DUMMYGENERATOR"},

    {sbndaq::detail::FragmentType::INVALID,          "UNKNOWN"}
  };
}

sbndaq::FragmentType
sbndaq::toFragmentType(std::string t_string)
{
  std::transform(t_string.begin(),
                 t_string.end(),
                 t_string.begin(),
                 toupper);
  for(auto it = names.begin(); it != names.end(); ++it)
    if(t_string == it->second)
      return static_cast<FragmentType>(it->first);
  return FragmentType::INVALID;
}

std::string
sbndaq::fragmentTypeToString(FragmentType val)
{
  if (val < FragmentType::INVALID) {
    return names.at(val);
  }
  else {
    return "INVALID/UNKNOWN"; //AA: why is it INVALID/UNKNOWN rather than UNKNOWN, as defined in the list above?
  }
}

std::map< artdaq::Fragment::type_t, std::string > sbndaq::makeFragmentTypeMap()
{
      auto output = artdaq::Fragment::MakeSystemTypeMap();
      for (auto name : names)
      {
               output[name.first] = name.second;
      }
      return output;
}
