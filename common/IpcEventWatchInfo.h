#pragma once

#include "CommonConstants.h"
#include "CommonTypeDefs.h"
#include "IpcEventId.h"
#include "IpcEventInfo.h"

class IpcEventWatchInfo: public EventSeqInfo 
{
	public:
		IpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, IpcEventId tarEventId);
		IpcEventWatchInfo(IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias);
		IpcEventWatchInfo(const std::string& eventAlias, IpcEventInfoObjMgr& objMgr, IpcEventId tarEventId);
		IpcEventWatchInfo(const std::string& eventAlias, IpcEventInfoObjMgr& objMgr, const std::string& tarEventAlias);

		IpcEventId 			getEventId() const;
		IpcEventId 			getTarEventId() const;
		unsigned short		getTarPrevEventSeq() const;
		bool				isDiffEventSeq() const;
		void				updatePrevEventSeq();
		bool				isBound() const;

	private:
		IpcEventId						ipcEventId_;
		IpcEventId						tarIpcEventId_;
};


