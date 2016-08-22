#include "IpcIpcEventWatchInfo.h"

IpcEventWatchInfo::IpcEventWatchInfo(EventIdType eventId, IpcEventInfoObjMgr& objMgr, EventIdType tarEventId):IpcEventInfo(eventId), tarIpcEventInfo_(nullptr){
	if ((tarIpcEventInfo_ = objMgr.get(tarEventId)) != nullptr) {
		tarPrevEventSeq_ = tarIpcEventInfo_->getEventSeq();
		this->registerSelf(objMgr);
	}
}

IpcEventWatchInfo::IpcEventWatchInfo(EventIdType eventId, IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias):IpcEventInfo(eventId), tarIpcEventInfo_(nullptr){
	if ((tarIpcEventInfo_ = objMgr.get(tarEventAlias)) != nullptr) {
		tarPrevEventSeq_ = tarIpcEventInfo_->getEventSeq();
		this->registerSelf(objMgr);
	}
}

IpcEventWatchInfo::IpcEventWatchInfo(const char* eventAlias, IpcEventInfoObjMgr& objMgr, EventIdType tarEventId): tarIpcEventInfo_(nullptr){
	if ((tarIpcEventInfo_ = objMgr.get(tarEventId)) != nullptr) {
		tarPrevEventSeq_ = tarIpcEventInfo_->getEventSeq();
		this->registerSelf(objMgr, eventAlias);
	}
}

IpcEventWatchInfo::IpcEventWatchInfo(const char* eventAlias, IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias): tarIpcEventInfo_(nullptr){
	if ((tarIpcEventInfo_ = objMgr.get(tarEventAlias)) != nullptr) {
		tarPrevEventSeq_ = tarIpcEventInfo_->getEventSeq();
		this->registerSelf(objMgr, eventAlias);
	}
}

EventIdType 	IpcEventWatchInfo::getTarEventId() const { 
	if (tarIpcEventInfo_ == nullptr) {
		return 0;
	}

	return tarIpcEventInfo_->getEventId(); 
}

IpcEventInfo*		IpcEventWatchInfo::getTarIpcEventInfo() { return tarIpcEventInfo_; }
EventIdType 	IpcEventWatchInfo::getTarPrevEventSeq() const { return tarPrevEventSeq_; }
bool			IpcEventWatchInfo::isDiffEventSeq() const { 
	if (tarIpcEventInfo_ == nullptr) {
		return false;
	}

	return tarIpcEventInfo_->getEventSeq() != tarPrevEventSeq_;
}

void			IpcEventWatchInfo::updatePrevEventSeq() {
	if (tarIpcEventInfo_ != nullptr) {
		tarPrevEventSeq_ = tarIpcEventInfo_->getEventSeq();
	}
}

bool			IpcEventWatchInfo::isBound() const {
	if (ipcEventId_ == 0 || tarIpcEventId_ == 0) {
		return false;
	}

	return ipcEventId_ != tarIpcEventId_;
}

