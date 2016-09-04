#include "EventHandlerPluginTest.h"
#include <iostream>
#include <string>

using namespace std;

void EventHandlerPluginTest::setPluginInfo()
{
	cout << "EventHandlerPluginTest::setPluginInfo()" << endl;
}

bool EventHandlerPluginTest::setup()
{
	cout << "EventHandlerPluginTest::setup()" << endl;
	return true;
}

bool EventHandlerPluginTest::preprocess()
{
	cout << "EventHandlerPluginTest::preprocess()" << endl;
	return true;
}

bool EventHandlerPluginTest::process()
{
	cout << "EventHandlerPluginTest::process()" << endl;
	return true;
}

bool EventHandlerPluginTest::postprocess()
{
	cout << "EventHandlerPluginTest::postprocess()" << endl;
	return true;
}

