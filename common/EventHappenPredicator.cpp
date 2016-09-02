#include "EventHappenPredicator.h"

bool EventHappenPredicator::predicate() const 
{
	bool ret = true;

	for (IpcEventWatchInfo* watchInfo:eventWatchInfoVec_) {

		if (!watchInfo->isDiffEventSeq()) {
			ret = false;
			break;
		}
	}

	return ret;
}

bool EventHappenPredicator::addIpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, IpcEventId tarEventId, const char* watcherAlias) 
{
	IpcEventWatchInfo* info = new IpcEventWatchInfo(watcherAlias, objMgr, tarEventId);
	if (!info->isBound()) {
		delete info;
		return false;
	}

	eventWatchInfoVec_.push_back(info);
	return true;
}

bool EventHappenPredicator::addIpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias, const char* watcherAlias) 
{
	return addIpcEventWatchInfo(objMgr, objMgr.getId(tarEventAlias), watcherAlias);
}

void EventHappenPredicator::updateWatchInfoEventSeqs() 
{
	for (IpcEventWatchInfo* eventWatchInfo:eventWatchInfoVec_) {
		eventWatchInfo->updatePrevEventSeq();
		eventWatchInfo->incEventSeq();
	}
}

