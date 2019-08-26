#ifndef __ConfigurationFile_hpp__
#define __ConfigurationFile_hpp__

#include <iostream>

class ConfigurationFile {
public:

	static ConfigurationFile &instance() {
		static ConfigurationFile instance;
	
		return instance;
	}

    size_t GetTCRTime();

    size_t GetTPS();

    bool UseDragon();

private:
    ConfigurationFile();

    size_t tcr_time_= 1300;
    size_t tps_ = 37000000;
    bool use_dragon_ = false;
};


#endif

