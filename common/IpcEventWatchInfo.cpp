#include "IpcEventWatchInfo.h"
#include "IpcEventInfo.h"
#include "IpcEventInfoObjMgr.h"
#include <iostream>

IpcEventWatchInfo::IpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, IpcEventId tarEventId)
	:tarIpcEventInfo_(nullptr), tarPrevEventSeq_(0)
{
	if ((tarIpcEventInfo_ = const_cast<IpcEventInfo*>(objMgr.getEvent(tarEventId))) != nullptr) {
		if (objMgr.registerEvent(ipcEventId_, "", IET_EVENT_WATCH_DEFAULT)) {
			tarPrevEventSeq_ = tarIpcEventInfo_->getEventSeq();
			tarIpcEventId_ = tarEventId;
		}
	}
}

IpcEventWatchInfo::IpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias)
	:tarIpcEventInfo_(nullptr), tarPrevEventSeq_(0)
{
	if ((tarIpcEventInfo_ = const_cast<IpcEventInfo*>(objMgr.getEvent(tarEventAlias, &tarIpcEventId_))) != nullptr) {
		if (objMgr.registerEvent(ipcEventId_, "", IET_EVENT_WATCH_DEFAULT)) {
			tarPrevEventSeq_ = tarIpcEventInfo_->getEventSeq();
		}
	}
}

IpcEventWatchInfo::IpcEventWatchInfo(const std::string& eventAlias, IpcEventInfoObjMgr& objMgr, IpcEventId tarEventId)
	:tarIpcEventInfo_(nullptr), tarPrevEventSeq_(0)
{
	if ((tarIpcEventInfo_ = const_cast<IpcEventInfo*>(objMgr.getEvent(tarEventId))) != nullptr) {
		if (objMgr.registerEvent(ipcEventId_, eventAlias, IET_EVENT_WATCH_DEFAULT)) {
			tarPrevEventSeq_ = tarIpcEventInfo_->getEventSeq();
			tarIpcEventId_ = tarEventId;
		}
	}
}

IpcEventWatchInfo::IpcEventWatchInfo(const std::string& eventAlias, IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias)
	:tarIpcEventInfo_(nullptr), tarPrevEventSeq_(0)
{
	if ((tarIpcEventInfo_ = const_cast<IpcEventInfo*>(objMgr.getEvent(tarEventAlias, &tarIpcEventId_))) != nullptr) {
		if (objMgr.registerEvent(ipcEventId_, eventAlias, IET_EVENT_WATCH_DEFAULT)) {
			tarPrevEventSeq_ = tarIpcEventInfo_->getEventSeq();
		}
	}
}

bool IpcEventWatchInfo::isDiffEventSeq() const 
{ 
	if (tarIpcEventInfo_ == nullptr) {
		return false;
	}
	//std::cout << "IpcSeq: " <<tarIpcEventInfo_->getEventSeq() << ", " << tarPrevEventSeq_ << std::endl;
	return tarIpcEventInfo_->getEventSeq() != tarPrevEventSeq_;
}

void IpcEventWatchInfo::updatePrevEventSeq() 
{
	if (tarIpcEventInfo_ != nullptr) {
		tarPrevEventSeq_ = tarIpcEventInfo_->getEventSeq();
	}
}

bool IpcEventWatchInfo::isBound() const 
{
	if (ipcEventId_.get() == IpcEventId::INVALID_ID || 
			tarIpcEventId_.get() == IpcEventId::INVALID_ID) {
		return false;
	}

	return ipcEventId_.get() != tarIpcEventId_.get();
}

