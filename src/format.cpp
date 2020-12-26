#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) { 

    string Time;

    long hr, min, sec;

    hr = seconds/3600;
    (hr < 10) ? (Time = "0" + std::to_string(hr)) : (Time = std::to_string(hr)) ;
       
    min = (seconds - hr*3600)/60;
    (min < 10) ? (Time += (":0" + std::to_string(min))) : (Time += (":"+std::to_string(min))) ;

    sec = (seconds - hr*3600 - min*60);
    (sec < 10) ? (Time += (":0" + std::to_string(sec))) : (Time += (":"+std::to_string(sec))) ;
        
    return Time; }