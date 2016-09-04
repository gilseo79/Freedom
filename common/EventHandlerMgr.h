#pragma once

#include <thread>
#include <mutex>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include "IpcEventId.h"

class IpcUniqueEventIdQueueMgr;
class IpcEventInfoObjMgr;
class EventHandler;

struct EventIdHasher
{
	std::size_t operator()(const IpcEventId& k) const
	{
		using std::hash;
		return hash<EventIdType>()(k.get());
	}
};
typedef std::unordered_map<IpcEventId, std::unordered_set<std::shared_ptr<EventHandler>>, 
		EventIdHasher>  									EventIdHandlerMap;
typedef std::unordered_set<std::shared_ptr<EventHandler>>  							EventHandlerSet;

class EventHandlerMgr
{
	public:
		EventHandlerMgr(IpcUniqueEventIdQueueMgr* eventIdQueueMgr, IpcEventInfoObjMgr* eventInfoObjMgr, unsigned int eventQueueId);
		~EventHandlerMgr();
		bool	addEventHandler(EventHandler* eventHandler);
		void	removeEventHandler(EventHandler* eventHandler);

	private:
		void	run();

		IpcUniqueEventIdQueueMgr* 	eventIdQueueMgr_;
		IpcEventInfoObjMgr* 		eventInfoObjMgr_;
		unsigned int				eventQueueId_;
		std::thread*				eventConsumer_;
		std::mutex					mutex_;
		EventIdHandlerMap			eventIdHandlerMap_;
		EventHandlerSet				eventHandlerSet_;
};
