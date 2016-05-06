// Mse1000Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
//#include <winsock2.h>
#include "MseNetworking.h"

int main()
{
	char* clientIpAddress = "172.31.46.88";
	bool RET_SUCCESS = false;
	std::cout << "My first MSE1000 Project!" << std::endl;
	
	MseNetworking* broadcastTest = new MseNetworking(clientIpAddress);
	MseInterface* modulechain = 0;
	std::string input;
	if (broadcastTest)
	{
		while (true) {
			std::cout << "Input your choice{a|b|c|d|e|exit}:" << std::endl;
			std::cin >> input;
			if (input == "a") {
				//通过广播搜索所有MSE1000模块
				RET_SUCCESS = broadcastTest->createChain();
				if (RET_SUCCESS == false) {
					continue;
				}
			}
			else if (input == "b") {
				//检查编码器连接状态
				if (RET_SUCCESS == false) {
					std::cout << "模块链尚未建立！" << std::endl;
					continue;
				}
				broadcastTest->testChain();
			}
			else if (input == "c") {
				//初始化编码器参数设置
				if (RET_SUCCESS == false) {
					std::cout << "模块链尚未建立！" << std::endl;
					continue;
				}
				modulechain = broadcastTest->getChain();
				for (size_t i = 0; i < modulechain->getNumModules(); i++)
				{
					broadcastTest->initModule(i);
					std::cout << "The Module " << i << " datas" << std::endl;
					//broadcastTest->getModuleData(i);
				}
			}
			else if (input == "d") {
				//清零所有模块
				if (RET_SUCCESS == false) {
					std::cout << "模块链尚未建立！" << std::endl;
					continue;
				}
				for (size_t i = 0; i < modulechain->getNumModules(); i++)
				{
					std::cout << "Mastting the " << i << " moudle" << std::endl;
					broadcastTest->masterModule(i);
				}
			}else if (input == "e") {
				//获取所有模块的位置信息
				if (RET_SUCCESS == false) {
					std::cout << "模块链尚未建立！" << std::endl;
					continue;
				}
				for (size_t i = 0; i < modulechain->getNumModules(); i++)
				{
					std::cout << "The Module " << i << " datas" << std::endl;
					broadcastTest->getModuleData(i);
				}
			}
			else if (input == "exit") {
				break;
			}
		}
	}
	std::cout << "Press enter to quit!" << std::endl;
	getchar();
    return 0;
}

