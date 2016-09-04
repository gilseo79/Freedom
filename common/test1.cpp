#include <iostream>
#include <boost/any.hpp>
#include <unordered_map>
#include "IpcEventInfo.h"
#include "IpcEventInfoObjMgr.h"
#include "IpcEventWatchInfo.h"
#include "EventHappenPredicator.h"
#include <string>
#include <vector>
#include "EventHandlerPluginTest.h"
#include "EventHandler.h"
#include "EventHandlerMgr.h"
#include "IpcUniqueEventIdQueueMgr.h"
#include <chrono>
#include <thread>


using namespace std;

struct A {
	unsigned short a;
};

struct B {
	unsigned short b;
	char c[12];
};

typedef pair<A,B> C;

	char buf[8] = {'a','b','c','d','e','f','g', 0};
boost::any get_test(int a) 
{
	boost::any ret;
	if (a == 0) {
		ret = *((int*)buf);
	}
	else if (a == 1) {
		ret = (const char*)buf;
	}

	return ret;
}


template<typename T>
void print_id(const T& val)
{
	int ret = 0;
	if (typeid(bool)== typeid(val)) ret = 0;
	else if (typeid((char)'a')== typeid(val)) ret = 1;
	else if (typeid((unsigned char)'a')== typeid(val)) ret = 2;
	else if (typeid((short)0)== typeid(val)) ret = 3;
	else if (typeid((unsigned short)0)== typeid(val)) ret = 4;
	else if (typeid((int)0)== typeid(val)) ret = 5;
	else if (typeid((unsigned int)0)== typeid(val)) ret = 6;

	cout << ret << endl;
}

int main()
{
	/*
	cout << sizeof(A) << endl;
	cout << sizeof(B) << endl;
	cout << sizeof(C) << endl;
	auto x = get_test(1);

	cout << boost::any_cast<const char*>(x) << endl;

	print_id(true);
	print_id((unsigned int)10);

	IpcEventInfo ie;
	bool y = true;
	ie.setUserData(y);
	boost::any&& z = ie.getUserDataAny();


	cout << boost::any_cast<bool>(z) << endl;
	ie.incEventSeq();
	ie.printAll();
	unsigned short p = 22;
	ie.setUserData(p);
	cout << boost::any_cast<unsigned short>(ie.getUserDataAny()) << endl;
	ie.printAll();
	ie.setUserData(std::string("301L9217"));
	cout << boost::any_cast<std::string>(ie.getUserDataAny()) << endl;
	ie.printAll();
	ie.setUserData(double(3.141592));
	cout << boost::any_cast<double>(ie.getUserDataAny()) << endl;
	ie.printAll();
	*/

	boost::interprocess::shared_memory_object::remove("IPC_EVENT_OBJ_MGR_SEGMENT");
	boost::interprocess::shared_memory_object::remove("IPC_UNIQUE_EVENTID_QUEUE_MGR_SHARED_SEGMENT");

	IpcUniqueEventIdQueueMgr* queueMgr = IpcUniqueEventIdQueueMgr::createInstance(100);
	queueMgr->addEventIdQueueUnit(0, 100);
	IpcEventInfoObjMgr* mgr = IpcEventInfoObjMgr::getInstance();
	EventHandlerMgr* eventHandlerMgr = new EventHandlerMgr(queueMgr, mgr, 0);
	IpcEventId testId, testId2, testId3, tetsId;
	if (!mgr->registerEvent(testId, "ahahah")) {
		mgr->getEventId("ahahah", testId);
	}

	if (!mgr->registerEvent(testId2, "sss", IET_CUR_PRICE)) {
		mgr->getEventId("sss", testId2);
	}

	if (!mgr->registerEvent(testId3, "hhh")) {
		mgr->getEventId("hhh", testId3);
	}
	mgr->registerEvent(tetsId);

	vector<string> alias_list = {"ahahah", "sss", "hhh"};

	EventHappenPredicator* ep = new EventHappenPredicator(*mgr, alias_list);
	EventHandlerPluginTest* testHandler = new EventHandlerPluginTest;
	EventHandler* eh = new EventHandler(ep, testHandler);
	eventHandlerMgr->addEventHandler(eh);
	size_t registed_cnt = 3;
	mgr->updateEvent(testId, FixedFloat(3141592.0)  / FixedFloat(10000.0));
	cout << "oh:" <<  testId.get() << endl;

	queueMgr->push(IpcUniqueEventIdQueueMgr::EQ_ALL, testId.get());
	this_thread::sleep_for(chrono::milliseconds(100));

	mgr->updateEvent(testId2, std::string("I'minShm"));
	cout << "oh:" <<  testId2.get() << endl;
	queueMgr->push(IpcUniqueEventIdQueueMgr::EQ_ALL, testId2.get());
	this_thread::sleep_for(chrono::milliseconds(100));

	mgr->updateEvent(testId3, "OhOh!");
	cout << "oh:" <<  testId3.get() << endl;
	queueMgr->push(IpcUniqueEventIdQueueMgr::EQ_ALL, testId3.get());
	this_thread::sleep_for(chrono::milliseconds(100));
	cout << "oh:" <<  tetsId << endl;
	const IpcEventInfo* pinfo = mgr->getEvent("sss");
	cout << testId2.get() << ", " << pinfo->getEventSeq() << ", " << boost::any_cast<std::string>(pinfo->getUserDataAny()) << endl;
	cout << endl << endl;
	mgr->updateEvent("sss", true);
	char tmp[10] = {'a','t','c',0,'X',2, (char)0xff};
	mgr->updateEvent(tetsId, tmp, sizeof tmp, IpcEventInfo::IEUT_BYTES);

	this_thread::sleep_for(chrono::seconds(3));
	IpcEventInfo i2;
	mgr->getEvent(tetsId, i2);
	mgr->printAll();

	cout << endl << endl;

	i2.printAll();

	mgr->printAll();
	return 0;
}
