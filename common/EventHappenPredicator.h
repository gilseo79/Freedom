#pragma once

#include <memory>
#include <string>
#include <vector>
#include "IpcEventId.h"
#include "IpcEventWatchInfo.h"

class IpcEventInfoObjMgr;

class EventHappenPredicator 
{
	public:
		EventHappenPredicator();
		EventHappenPredicator(IpcEventInfoObjMgr& objMgr, IpcEventId tarEventId, const std::string& watcherAlias = "");
		EventHappenPredicator(IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias, const std::string& watcherAlias = "");
		EventHappenPredicator(IpcEventInfoObjMgr& objMgr, const std::vector<IpcEventId>& tarEventIdVec);
		EventHappenPredicator(IpcEventInfoObjMgr& objMgr, const std::vector<std::string>& tarEventAliasVec);
		virtual bool predicate() const;
		bool addIpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, IpcEventId tarEventId, const std::string& watcherAlias = "");
		bool addIpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias, const std::string& watcherAlias = "");
		std::size_t addIpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, const std::vector<IpcEventId>& tarEventIdVec);
		std::size_t addIpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, const std::vector<std::string>& tarEventAliasVec);
		void updateWatchInfoEventSeqs(IpcEventInfoObjMgr& objMgr);
		std::vector<EventIdType> getTarEventIds() const; 	

	private:
		std::vector<std::shared_ptr<IpcEventWatchInfo>>	eventWatchInfoVec_;
};


