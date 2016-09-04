#pragma once

#include <memory>
#include "EventHappenPredicator.h"
#include "EventHandlerPluginBase.h"

class EventHandlerMgr;

class EventHandler
{
	public:
		friend class EventHandlerMgr;
		enum {EHL_PERMANENCE = 0};
		EventHandler(EventHappenPredicator* predicator, EventHandlerPluginBase* handler, unsigned int eventHandlerLife = EHL_PERMANENCE);
		bool isLiveEvent() const;
		unsigned int getEventExecutionCnt() const;
		virtual void process(IpcEventInfoObjMgr& objMgr); 

	private:
		unsigned int							eventHandlerLife_;
		unsigned int							eventExecutionCnt_;
		std::shared_ptr<EventHappenPredicator>	predicator_;
		std::shared_ptr<EventHandlerPluginBase>	handler_;
};

