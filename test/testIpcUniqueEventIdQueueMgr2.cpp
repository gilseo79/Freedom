#include "IpcUniqueEventIdQueueMgr.h"
#include <string>
#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>

int main() 
{
	IpcUniqueEventIdQueueMgr* mgr = IpcUniqueEventIdQueueMgr::createInstance(10000);

	while (1) {
		try {
			unsigned int id = mgr->pop(0, 10000);
			std::cout << "id: " << id << std::endl;
		}
		catch(timeout_error e) {
			std::cout << e.what() << std::endl;
			break;
		}
	}

	while (1) {
		try {
			unsigned int id = mgr->pop(1, 10000);
			std::cout << "id: " << id << std::endl;
		}
		catch(timeout_error e) {
			std::cout << e.what() << std::endl;
			break;
		}
	}

	return 0;
}
