#include "EventHappenPredicator.h"
#include "IpcEventInfoObjMgr.h"
#include <functional>
#include <iostream>
#include <string>

EventHappenPredicator::EventHappenPredicator()
{
}

EventHappenPredicator::EventHappenPredicator(IpcEventInfoObjMgr& objMgr, IpcEventId tarEventId, const std::string& watcherAlias)
{
	addIpcEventWatchInfo(objMgr, tarEventId, watcherAlias);
}

EventHappenPredicator::EventHappenPredicator(IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias, const std::string& watcherAlias)
{
	addIpcEventWatchInfo(objMgr, tarEventAlias, watcherAlias);
}

EventHappenPredicator::EventHappenPredicator(IpcEventInfoObjMgr& objMgr, const std::vector<IpcEventId>& tarEventIdVec)
{
	size_t cnt = addIpcEventWatchInfo(objMgr, tarEventIdVec);
	//std::cout << "EventHappenPredicator::EventHappenPredicator() : tarEventCnt: " << cnt << std::endl;
}

EventHappenPredicator::EventHappenPredicator(IpcEventInfoObjMgr& objMgr, const std::vector<std::string>& tarEventAliasVec)
{
	size_t cnt = addIpcEventWatchInfo(objMgr, tarEventAliasVec);
	//std::cout << "EventHappenPredicator::EventHappenPredicator() : tarEventCnt: " << cnt << std::endl;
}

bool EventHappenPredicator::predicate() const 
{
	bool ret = true;
	
	for (auto watchInfo:eventWatchInfoVec_) {

		if (!watchInfo->isDiffEventSeq()) {
			ret = false;
			break;
		}
	}

	return ret;
}

bool EventHappenPredicator::addIpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, IpcEventId tarEventId, const std::string& watcherAlias) 
{
	auto info = std::make_shared<IpcEventWatchInfo>(watcherAlias, objMgr, tarEventId);
	if (!info->isBound()) {
		return false;
	}

	eventWatchInfoVec_.push_back(info);
	return true;
}

bool EventHappenPredicator::addIpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias, const std::string& watcherAlias) 
{
	auto info = std::make_shared<IpcEventWatchInfo>(watcherAlias, objMgr, tarEventAlias);
	if (!info->isBound()) {
		return false;
	}

	eventWatchInfoVec_.push_back(info);
	return true;
}

std::size_t EventHappenPredicator::addIpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, const std::vector<IpcEventId>& tarEventIdVec)
{
	std::size_t success_cnt = 0;
	std::for_each(tarEventIdVec.cbegin(), tarEventIdVec.cend(), 
			[&](IpcEventId ipcEventId){ 
				if (this->addIpcEventWatchInfo(objMgr, ipcEventId)) {
					++success_cnt;
				}
			}
			);
	return success_cnt;
}

std::size_t EventHappenPredicator::addIpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, const std::vector<std::string>& tarEventAliasVec)
{
	std::size_t success_cnt = 0;
	std::for_each(tarEventAliasVec.cbegin(), tarEventAliasVec.cend(), 
			[&](const std::string& ipcEventAlias){ 
				if (this->addIpcEventWatchInfo(objMgr, ipcEventAlias)) {
					++success_cnt;
				}
			}
			);
	return success_cnt;
}

void EventHappenPredicator::updateWatchInfoEventSeqs(IpcEventInfoObjMgr& objMgr) 
{
	for (auto eventWatchInfo:eventWatchInfoVec_) {
		eventWatchInfo->updatePrevEventSeq();
		objMgr.updateEvent(eventWatchInfo->getEventId());
	}
}

std::vector<EventIdType> EventHappenPredicator::getTarEventIds() const
{
	std::vector<EventIdType> res;
	for (auto watch_info:eventWatchInfoVec_) {
		res.push_back(EventIdType(watch_info->getTarEventId()));
	}

	return res;
}
