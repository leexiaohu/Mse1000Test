#include "stdafx.h"
#include "MseTtlTesting.h"

#include <iostream>
#include <iomanip>


MseTtlTesting::MseTtlTesting(MseTtlModule* module) :
	initialized_(false)
{
	module_ = module;
}

MseTtlTesting::~MseTtlTesting(void)
{
	if (0 != module_)
		delete module_;

	module_ = 0;
	initialized_ = false;
}
//初始化TTL_MT1271参数配置
bool MseTtlTesting::initialize()
{
	MseResults retVal;

	if (0 != module_)
	{
		MseResults retVal;
		for (int i = 0; i < module_->getNumChannels(); ++i)
		{
			//设置编码器处于连接状态
			retVal = module_->setChannelPresence(1, i);
			if (RESPONSE_OK != retVal.getCode())
			{
				std::cout << "Could not set the channel presence for TTL module: " << MseResults::showRespCode(retVal.getCode()) << " channel " << i << std::endl;
				return false;
			}
			//设置引用标志:one
			retVal = module_->initReferencing(i, REFERENCE_MARK_NONE, 0);
			if (RESPONSE_OK != retVal.getCode())
			{
				std::cout << "Could not set the referencing mode for TTL module: " << MseResults::showRespCode(retVal.getCode()) << " channel " << i << std::endl;
				return false;
			}
			//设置误差补偿
			bool boolRetVal = module_->setErrorCompensation(1, i);
			if (!boolRetVal)
			{
				std::cout << "Could not set the channel error compensation for TTL module: " << MseResults::showRespCode(retVal.getCode()) << " channel " << i << std::endl;
				return false;
			}
			//设置编码器类型：长度计
			boolRetVal = module_->setEncoderType(ENCODER_TYPE_GAUGE, i);
			if (!boolRetVal)
			{
				std::cout << "Could not set the encoder type for TTL module: " << MseResults::showRespCode(retVal.getCode()) << " channel " << i << std::endl;
				return false;
			}
			//设置信号周期：2um 内部细分倍数:5倍 （MT1271固定不变）
			boolRetVal = module_->setSignalPeriod(2.0, TTL_INTERPOLATION_X5, i);
			if (!boolRetVal)
			{
				std::cout << "Could not set the signal period for TTL module: " << MseResults::showRespCode(retVal.getCode()) << " channel " << i << std::endl;
				return false;
			}
			//计数方向：正向
			retVal = module_->setCountingDirection(0, i);
			if (RESPONSE_OK != retVal.getCode())
			{
				std::cout << "Could not set the counting direction for TTL module: " << MseResults::showRespCode(retVal.getCode()) << " channel " << i << std::endl;
				return false;
			}
			//设置度量单位：mm
			boolRetVal = module_->setUom(UOM_MM, i);
			if (!boolRetVal)
			{
				std::cout << "Could not set the signal period for TTL module: " << MseResults::showRespCode(retVal.getCode()) << " channel " << i << std::endl;
				return false;
			}
		}
		initialized_ = true;
		std::cout << "Congratulations,parameters setting completed!" << std::endl;
	}
	return true;
}
//获取位置所有编码器信息
bool MseTtlTesting::getPositions()
{
	if (0 != module_)
	{
		MseResults retVal;
		double pos[MAX_CHANNELS_PER_MODULE];
		double newResolution = 0.0;
		/*获取编码器的分辨率
		for (int i = 0; i < module_->getNumChannels(); ++i)
		{	
			bool boolRetVal = module_->getResolution(&newResolution, i);
			if (!boolRetVal)
			{
				std::cout << "Could not get the resolution for TTL module: " << MseResults::showRespCode(retVal.getCode()) << " channel " << i << std::endl;
				return false;
			}
			std::cout << "The resolution for channel " << i << " is " << newResolution << std::endl;			
		}
		*/
		//获取编码器位置
		retVal = module_->getPositions(pos, MAX_CHANNELS_PER_MODULE, COUNT_REQUEST_LATEST);
		if (RESPONSE_OK != retVal.getCode())
		{
			std::cout << "Could not get values from the TTL module: " << MseResults::showRespCode(retVal.getCode()) << std::endl;
			return false;
		}
		std::cout << "---------------------- Positions ---------------------- " << std::endl;
		for (int i = 0; i < module_->getNumChannels(); ++i)
		{
			std::cout << "Channel " << i << " position = " << pos[i] <<
				" offset: " << module_->getDeviceOffset(i) << std::endl;
		}
	}
	else
	{
		std::cout << "Module has not been initialized" << std::endl;
		return false;
	}
	return true;
}
/*校准：把编码器任意一位置校正为指定值(一般把自然垂直状态设为0，表示把初始位置清零)
*每次清零只对当次测量有效，如果下次还要从零开始测量，要重新清零一次
*/
bool MseTtlTesting::maseter(double num) {
	if (0 != module_)
	{
		MseResults retVal;
		double pos[MAX_CHANNELS_PER_MODULE];
		retVal = module_->getPositions(pos, MAX_CHANNELS_PER_MODULE, COUNT_REQUEST_LATEST);
		if (RESPONSE_OK != retVal.getCode())
		{
			std::cout << "Could not get values from the TTL module: " << MseResults::showRespCode(retVal.getCode()) << std::endl;
			return false;
		}
		double oldoffset, newoffset;
		for (int i = 0; i < module_->getNumChannels(); ++i)
		{
			oldoffset = module_->getDeviceOffset(i);
			newoffset = oldoffset + num - pos[i];
			module_->setDeviceOffset(i, newoffset);
		}
	}
	else
	{
		std::cout << "Module has not been initialized" << std::endl;
		return false;
	}
	return true;
}