#pragma once
#include "CommonTypeDefs.h"

class IpcEventInfoObjMgr;

class IpcEventId
{
	public:
		friend class IpcEventInfoObjMgr;
		IpcEventId():id_(0){}
		inline EventIdType get() const { return id_; }
		operator EventIdType() const { return id_; }
	protected:
		inline void set(EventIdType id){ id_ = id; }
		EventIdType 	id_;
};

