#include "EventHandlerMgr.h"
#include "EventHandler.h"
#include "IpcEventInfoObjMgr.h"
#include "IpcUniqueEventIdQueueMgr.h"
#include "EventHandlerPluginBase.h"
#include "CommonStructs.h"
#include <memory>
#include <iostream>
#include <string>

EventHandlerMgr::EventHandlerMgr(IpcUniqueEventIdQueueMgr* eventIdQueueMgr, IpcEventInfoObjMgr* eventInfoObjMgr, unsigned int eventQueueId)
	:eventIdQueueMgr_(eventIdQueueMgr), eventInfoObjMgr_(eventInfoObjMgr), eventQueueId_(eventQueueId), eventConsumer_(nullptr)
{
	eventConsumer_ = new std::thread([&](){ run(); });
}

EventHandlerMgr::~EventHandlerMgr()
{
	if (eventConsumer_ != nullptr) {
		delete eventConsumer_;
	}
}

bool	EventHandlerMgr::addEventHandler(EventHandler* eventHandler)
{
	std::lock_guard<std::mutex> lock(mutex_);
	auto res = eventHandlerSet_.insert(std::shared_ptr<EventHandler>(eventHandler));

	if (res.second == true) {
		auto&& ids = eventHandler->predicator_->getTarEventIds();

		for (auto id:ids) {
			auto& handle_map = eventIdHandlerMap_[IpcEventId(id)];
			handle_map.insert(*res.first);
		}
	}

	return true;
}

void	EventHandlerMgr::removeEventHandler(EventHandler* eventHandler)
{
}

void	EventHandlerMgr::run()
{
	//std::cout << "Start - EventHandlerMgr::run()\n";
	while (1) {
		try {
			IpcEventId eventId(eventIdQueueMgr_->pop(eventQueueId_, 1000));
			std::lock_guard<std::mutex> lock(mutex_);
			auto iter = eventIdHandlerMap_.find(eventId);
			if (iter == eventIdHandlerMap_.end()) {
				continue;
			}

			//std::cout << "EventHandlerMgr::run() - Found a EventHandler for Id: " << eventId.get() << std::endl;
			for (auto event_handler:iter->second) {
				event_handler->process(*eventInfoObjMgr_);

				if (event_handler->isLiveEvent() == false) {
					eventHandlerSet_.erase(event_handler);
				}
			}
		}
		catch(timeout_error e) {
			continue;		
		}
	}
}


