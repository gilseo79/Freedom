#pragma once

#include <memory>
#include "IpcEventId.h"
#include "IpcEventInfoObjMgr.h"
#include "IpcEventWatchInfo.h"

class EventHappenPredicator 
{
	public:
		virtual bool predicate() const;
		bool addIpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, IpcEventId tarEventId, const char* watcherAlias = nullptr);
		bool addIpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias, const char* watcherAlias = nullptr);
		void updateWatchInfoEventSeqs();

	private:
		std::vector<std::shared_ptr<IpcEventWatchInfo>>	eventWatchInfoVec_;
};


