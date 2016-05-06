#pragma once
#include "MseInterface.h"

class MseNetworking
{
public:
	MseNetworking(const char* clientIp);
	~MseNetworking();
	bool createChain();
	bool createManualChain();
	bool testChain();
	MseInterface* getChain();
private:
	char clientIP_[SIZE_IP_ADDRESS];
	MseInterface mse1000_;
	
public:
	bool initModule(const int& moduleNum);
	bool MseNetworking::getModuleData(const int& moduleNum);
	bool masterModule(const int& moduleNum);
	MODULE_ID MseNetworking::getModuleType(const int& moduleNum);
};

