#pragma once

#include "CommonConstants.h"
#include "CommonTypeDefs.h"
#include "IpcEventId.h"
#include <string>

class IpcEventInfo;
class IpcEventInfoObjMgr;

class IpcEventWatchInfo
{
	public:
		IpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, IpcEventId tarEventId);
		IpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias);
		IpcEventWatchInfo(const std::string& eventAlias, IpcEventInfoObjMgr& objMgr, IpcEventId tarEventId);
		IpcEventWatchInfo(const std::string& eventAlias, IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias);

		inline IpcEventId 		getEventId() const { return ipcEventId_; }
		inline IpcEventId 		getTarEventId() const { return tarIpcEventId_; }
		inline IpcEventInfo*	getTarIpcEventInfo() { return tarIpcEventInfo_; }
		inline EventSeqType		getTarPrevEventSeq() const { return tarPrevEventSeq_; }
		bool					isDiffEventSeq() const;
		void					updatePrevEventSeq();
		bool					isBound() const;

	private:
		IpcEventInfo*					tarIpcEventInfo_;
		EventSeqType					tarPrevEventSeq_;
		IpcEventId						tarIpcEventId_;
		IpcEventId						ipcEventId_;
};


