#pragma once
#include <thread>
#include <mutex>
#include <memory>
#include <unordered_set>
#include "IpcEventId.h"
#include "EventHappenPredicator.h"
#include "EventHandlerPluginBase.h"

class EventHandler
{
	public:
		enum {EHL_PERMANENCE = 0}
		EventHandler(const EventHappenPredicator* predicator, const EventHandlerPluginBase* handler, unsigned int eventHandlerLife = EHL_PERMANENCE):
			eventHandlerLife_(eventHandlerLife), eventExecutionCnt_(0), predicator_(predicator), handler_(handler) {}

		bool isLiveEvent() const {
			if (eventHandlerLife_ == EHL_PERMANENCE) {
				return true;
			}

			if (eventHandlerLife_ > eventExecutionCnt_) {
				return true;
			}

			return false;
		}

		unsigned int getEventExecutionCnt() const { return eventExecutionCnt_; }

	private:
		unsigned int							eventHandlerLife_;
		unsigned int							eventExecutionCnt_;
		std::shared_ptr<EventHappenPredicator>	predicator_;
		std::shared_ptr<EventHandlerPluginBase>	handler_;
};

typedef std::unordered_map<IpcEventId, std::set<std::shared_ptr<EventHandler> > >  	EventIdHandlerMap;
typedef std::unordered_set<std::shared_ptr<EventHandler>>  							EventHandlerSet;

class EventHandlerMgr
{
	public:
		EventHandlerMgr();
		~EventHandlerMgr();
		bool	addEventHandler(const EventHandler* eventHandler);
		void	removeEventHandler(const EventHandler* eventHandler);

	private:
		void	run();

		std::thread*			eventConsumer_;
		std::mutex				mutex_;
		EventIdHandlerMap		eventIdHandlerMap_;
		EventHandlerSet			eventHandlerSet_;
};
