#include <iostream>
#include <boost/any.hpp>
#include <unordered_map>
#include "IpcEventInfo.h"
#include "IpcEventInfoObjMgr.h"

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
	ie.incIpcEventSeq();
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

	//boost::interprocess::shared_memory_object::remove("IPC_EVENT_OBJ_MGR_SEGMENT");
	IpcEventInfoObjMgr mgr;
	mgr.init();
	IpcEventId testId, testId2, testId3, tetsId;
	if (!mgr.registerEvent(testId, "ahahah")) {
		mgr.getEventId("ahahah", testId);
	}
	cout << "oh:" <<  testId.get() << endl;
	if (!mgr.registerEvent(testId2, "sss")) {
		mgr.getEventId("sss", testId2);
	}
	mgr.updateEvent(testId2, std::string("I'minShm"));
	cout << "oh:" <<  testId2.get() << endl;
	if (!mgr.registerEvent(testId3, "hhh")) {
		mgr.getEventId("hhh", testId3);
	}
	cout << "oh:" <<  testId3.get() << endl;
	mgr.registerEvent(tetsId);
	cout << "oh:" <<  tetsId << endl;
	const IpcEventInfo* pinfo = mgr.getEvent("sss");
	cout << testId2.get() << ", " << pinfo->getIpcEventSeq() << ", " << boost::any_cast<std::string>(pinfo->getUserDataAny()) << endl;
	cout << endl << endl;
	mgr.printAll();
	return 0;
}
