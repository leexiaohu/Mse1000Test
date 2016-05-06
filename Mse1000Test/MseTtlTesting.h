#pragma once

#include "MseTtlModule.h"
#include "ModuleData.h"

class MseTtlTesting
{
public:
	/** The constructor.
	* @param clientIp The IP address of the analog module to communicate with
	*/
	//MseTtlTesting(const char* clientIp);
	MseTtlTesting(MseTtlModule* module);
	/** The destructor
	*   Deletes the MseTtlModule object if one was created during initialize
	*/
	~MseTtlTesting(void);

	/**
	* This method is used to create the Udp client and server connection, tell the MSE1000 our ip and port,
	* and read the initial settings of the module and devices
	*
	* @param mseIpAddress The ip address of the MSE1000 module to talk to
	*
	* @return true on success
	*/
	bool initialize();

	/**
	* This method will display the position values returned from the module for each channel.
	*
	* @return true on success
	*/
	bool getPositions();
	bool maseter(double num);

private:
	bool initialized_; /**< Whether the module has been initialized yet */
	char clientIp_[SIZE_IP_ADDRESS]; /**< The IP address of the module */
	MseTtlModule* module_; /**< The pointer to the module object */
};

