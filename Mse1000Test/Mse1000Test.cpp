// Mse1000Test.cpp : �������̨Ӧ�ó������ڵ㡣
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
				//ͨ���㲥��������MSE1000ģ��
				RET_SUCCESS = broadcastTest->createChain();
				if (RET_SUCCESS == false) {
					continue;
				}
			}
			else if (input == "b") {
				//������������״̬
				if (RET_SUCCESS == false) {
					std::cout << "ģ������δ������" << std::endl;
					continue;
				}
				broadcastTest->testChain();
			}
			else if (input == "c") {
				//��ʼ����������������
				if (RET_SUCCESS == false) {
					std::cout << "ģ������δ������" << std::endl;
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
				//��������ģ��
				if (RET_SUCCESS == false) {
					std::cout << "ģ������δ������" << std::endl;
					continue;
				}
				for (size_t i = 0; i < modulechain->getNumModules(); i++)
				{
					std::cout << "Mastting the " << i << " moudle" << std::endl;
					broadcastTest->masterModule(i);
				}
			}else if (input == "e") {
				//��ȡ����ģ���λ����Ϣ
				if (RET_SUCCESS == false) {
					std::cout << "ģ������δ������" << std::endl;
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

