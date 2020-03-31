#include <string>

#include "format.h"
#include "linux_parser.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
long mesureSeconds = 0;
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    mesureSeconds = seconds;
    long hour = seconds/3600;
	seconds = seconds%3600;
	long min = seconds/60;
	seconds = seconds%60;
    std::string strh = std::to_string(hour);
        if(strh.size() == 1)
            strh = "0" + strh;
    std::string strm = std::to_string(min);
        if(strm.size() == 1)
            strm = "0" + strm;
    std::string strs = std::to_string(seconds);
        if(strs.size() == 1)
            strs = "0" + strs;
    std::string str = strh + ":" + strm + ":" + strs;
    return string(str); }