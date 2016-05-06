#include "stdafx.h"
#include "MseNetworking.h"
#include "MseTtlTesting.h"
#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <iomanip>

MseNetworking::MseNetworking(const char* clientIp)
{
	strcpy_s(clientIP_, clientIp);
}

MseNetworking::~MseNetworking()
{
}

/**
*通过广播创建模块链
*/
bool MseNetworking::createChain()
{
	MseResults retVal;
	std::cout << "createChain: Please wait while the MSE 1000 modules are detected and ordered" << std::endl;
	retVal = mse1000_.createChain(clientIP_,MSE1000_CLIENT_DEFAULT_PORT,false,"255.255.255.255");
	if (RESPONSE_OK != retVal.getCode()) {
		std::cout << "Can not create the chain via broadcasting: " << MseResults::showRespCode(retVal.getCode()) << std::endl;
		return false;
	}
	std::cout << "createChain:Finished detected and ordered the modle chain" << std::endl;

	return true;
}

/**
手动创建模块链（一般不用）
*/
bool MseNetworking::createManualChain()
{
	MseResults retVal;
	retVal = mse1000_.addModule(MODULE_ID_TTL_BASE, "172.31.46.2",false);
	if (RESPONSE_OK != retVal.getCode())
	{
		std::stringstream ss;
		ss << "Error: " << MseResults::showRespCode(retVal.getCode());
		std::cout << ss.str().c_str() << std::endl;
	}

	retVal = mse1000_.addModule(MODULE_ID_TTL_4X, "172.31.46.3", false);
	if (RESPONSE_OK != retVal.getCode())
	{
		std::stringstream ss;
		ss << "Error: " << MseResults::showRespCode(retVal.getCode());
		std::cout << ss.str().c_str() << std::endl;
	}

	retVal = mse1000_.addModule(MODULE_ID_TTL_4X, "172.31.46.4", false);
	if (RESPONSE_OK != retVal.getCode())
	{
		std::stringstream ss;
		ss << "Error: " << MseResults::showRespCode(retVal.getCode());
		std::cout << ss.str().c_str() << std::endl;
	}

	retVal = mse1000_.addModule(MODULE_ID_TTL_4X, "182.168.1.5", false);
	if (RESPONSE_OK != retVal.getCode())
	{
		std::stringstream ss;
		ss << "Error: " << MseResults::showRespCode(retVal.getCode());
		std::cout << ss.str().c_str() << std::endl;
	}

	retVal = mse1000_.addModule(MODULE_ID_TTL_4X, "182.168.1.6", false);
	if (RESPONSE_OK != retVal.getCode())
	{
		std::stringstream ss;
		ss << "Error: " << MseResults::showRespCode(retVal.getCode());
		std::cout << ss.str().c_str() << std::endl;
	}
	return true;
}


bool MseNetworking::testChain()
{
	MODULE_ID moduleType;
	unsigned long hwId = 0;
	unsigned char hwRev = 0;
	unsigned short numAxes;
	unsigned char isConnected = 0;
	MseResults retVal;
	MseModule* baseBoard;
	ModuleData moduleData;


	for (unsigned int moduleIndex = 0; moduleIndex < mse1000_.getNumModules(); ++moduleIndex)
	{
		baseBoard = mse1000_.getModule(moduleIndex);
		if (0 == baseBoard)
		{
			std::cout << "Module " << moduleIndex << "returned NULL" << std::endl;
			continue;
		}

		if (moduleIndex == 0)
		{
			std::cout << "MSElibrary version = " << baseBoard->getLibraryVersion() << std::endl;
		}

		retVal = baseBoard->getModuleType(&moduleType, &hwId, &hwRev, &numAxes);
		if (RESPONSE_OK != retVal.getCode())
		{
			std::cout << "Could not get the module type from module " << moduleIndex << " : " << MseResults::showRespCode(retVal.getCode()) << std::endl;
			continue;
		}

		std::cout << "Module[" << moduleIndex << "] is of type " << MseModule::showModuleType(moduleType) << std::endl;
		moduleData = baseBoard->getModuleData();
		std::cout << "	IP = " << moduleData.ipAddress << std::endl;
		std::cout << "	Netmask = " << moduleData.netmask << std::endl;

		switch (moduleType)
		{
		case MODULE_ID_TTL_BASE:
		case MODULE_ID_TTL_4X:
		case MODULE_ID_TTL_8X: 
		{
			MseTtlModule* module = mse1000_.getTtlModule(moduleIndex);
			if (0 == module)
			{
				std::cout << "Module " << moduleIndex << " is NULL " << MseResults::showRespCode(retVal.getCode()) << std::endl;
				continue;
			}
			unsigned long counts[MAX_CHANNELS_PER_MODULE];
			retVal = module->getCounts(counts, module->getNumChannels(),COUNT_REQUEST_LATEST);
			if (RESPONSE_OK != retVal.getCode()) {
				std::cout << "Could not get counts from TTL module: " << MseResults::showRespCode(retVal.getCode()) << std::endl;
				continue;
			}
			std::cout << "---------------------- Counts ---------------------- " << std::endl;
			for (unsigned int i = 0; i < module->getNumChannels(); ++i)
			{
				
				retVal=module->getChannelPresence(&isConnected, i);
				if (RESPONSE_OK == retVal.getCode()) {
					std::string s_conncttion = isConnected == 1 ? " connected " : " not connected ";
					std::cout << "Channel[" << i << "]" << s_conncttion << "to a encoder" << std::endl;
				}
				std::cout << "Channel[" << i << "] = " << counts[i] << std::endl;
			}
			std::cout << "---------------------------------------------------- " << std::endl;
		}
		break;

		case MODULE_ID_PNEUMATIC:
		default:
			break;
		}
	}
	return true;
}
MseInterface* MseNetworking::getChain()
{
	return &mse1000_;
}
//初始化模块参数
bool MseNetworking::initModule(const int& moduleNum)
{
	MseTtlModule* ttlModule;
	MODULE_ID moduleType = getModuleType(moduleNum);

	switch (moduleType)
	{
	case MODULE_ID_TTL_BASE:
	case MODULE_ID_TTL_4X:
	case MODULE_ID_TTL_8X:
	{
		ttlModule = mse1000_.getTtlModule(moduleNum);
		MseTtlTesting *msettltesting = new MseTtlTesting(ttlModule);
		msettltesting->initialize();
	}
	break;
	default:
		break;
	}
	return true;
}
//校正编码器为指定值
bool MseNetworking::masterModule(const int& moduleNum)
{
	MseTtlModule* ttlModule;
	MODULE_ID moduleType = getModuleType(moduleNum);

	switch (moduleType)
	{
	case MODULE_ID_TTL_BASE:
	case MODULE_ID_TTL_4X:
	case MODULE_ID_TTL_8X:
	{
		ttlModule = mse1000_.getTtlModule(moduleNum);
		MseTtlTesting *msettltesting = new MseTtlTesting(ttlModule);
		//编码器清零或者设为制定值
		msettltesting->maseter(0.000);
	}
	break;
	default:
		break;
	}
	return true;
}
//获取模块数据
bool MseNetworking::getModuleData(const int& moduleNum) {
	MseTtlModule* ttlModule;
	MODULE_ID moduleType= getModuleType(moduleNum);
	switch (moduleType)
	{
	case MODULE_ID_TTL_BASE:
	case MODULE_ID_TTL_4X:
	case MODULE_ID_TTL_8X:
	{
		ttlModule = mse1000_.getTtlModule(moduleNum);
		MseTtlTesting *msettltesting = new MseTtlTesting(ttlModule);
		msettltesting->getPositions();
	}
	break;
	default:
		break;
	}
	return true;
}
//获取模块类型
MODULE_ID MseNetworking::getModuleType(const int& moduleNum) {
	MseModule* module;
	MODULE_ID moduleType;
	MseResults retVal;
	unsigned long hwId = 0;
	unsigned char hwRev = 0;
	unsigned short numAxes;
	module = mse1000_.getModule(moduleNum);
	if (0 == module) {
		std::cout << "Module " << moduleNum << "returned NULL" << std::endl;
		return MODULE_ID_NONE;
	}
	retVal = module->getModuleType(&moduleType, &hwId, &hwRev, &numAxes);
	if (RESPONSE_OK != retVal.getCode())
	{
		std::cout << "Could not get the module type from module " << moduleNum
			<< " : " << MseResults::showRespCode(retVal.getCode()) << std::endl;
		return MODULE_ID_NONE;
	}
	return moduleType;
}