#include "EventHandler.h"
#include <iostream>


EventHandler::EventHandler(EventHappenPredicator* predicator, EventHandlerPluginBase* handler, unsigned int eventHandlerLife)
	:eventHandlerLife_(eventHandlerLife), eventExecutionCnt_(0), predicator_(predicator), handler_(handler)
{
}

bool EventHandler::isLiveEvent() const 
{
	if (eventHandlerLife_ == EHL_PERMANENCE) {
		return true;
	}

	if (eventHandlerLife_ > eventExecutionCnt_) {
		return true;
	}

	return false;
}

unsigned int EventHandler::getEventExecutionCnt() const 
{ 
	return eventExecutionCnt_; 
}

void EventHandler::process(IpcEventInfoObjMgr& objMgr)
{
	if (predicator_->predicate() == true) {
		handler_->preprocess();
		handler_->process();
		handler_->postprocess();
		predicator_->updateWatchInfoEventSeqs(objMgr);
		++eventExecutionCnt_;
		//std::cout << "EventHandler::process() - predicate: true" << std::endl;
	}
	else {
		//std::cout << "EventHandler::process() - predicate: false" << std::endl;
	}
}

