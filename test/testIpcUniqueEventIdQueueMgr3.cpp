#include "IpcUniqueEventIdQueueMgr.h"
#include <string>
#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>

int main() 
{
	IpcUniqueEventIdQueueMgr* mgr = IpcUniqueEventIdQueueMgr::createInstance(10000);
	for (int i = 0; i < 3; ++i) {
		mgr->push(IpcUniqueEventIdQueueMgr::EQ_ALL, i * 3);
		mgr->push(1, i * 10);
	}

	/*
	std::cout << mgr->size() << std::endl;
	while (1) {
		try {
			unsigned int id = mgr->pop(1000);
			std::cout << "id: " << id << std::endl;
		}
		catch(timeout_error e) {
			std::cout << e.what() << std::endl;
			break;
		}
	}
	*/

	/*
	while (1) {
		try {
			unsigned int id = mgr->pop(0, 1000);
			std::cout << "id: " << id << std::endl;
		}
		catch(timeout_error e) {
			std::cout << e.what() << std::endl;
			break;
		}
	}

	while (1) {
		try {
			unsigned int id = mgr->pop(1, 1000);
			std::cout << "id: " << id << std::endl;
		}
		catch(timeout_error e) {
			std::cout << e.what() << std::endl;
			break;
		}
	}
	*/

	return 0;
}
