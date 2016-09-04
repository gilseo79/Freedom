#pragma once
#include "CommonTypeDefs.h"

class IpcEventInfoObjMgr;
class EventHandlerMgr;

class IpcEventId
{
	public:
		enum {INVALID_ID = 0};
		friend class IpcEventInfoObjMgr;
		friend class EventHandlerMgr;
		IpcEventId():id_(0){}
		inline EventIdType get() const { return id_; }
		operator EventIdType() const { return id_; }
		bool operator==(const IpcEventId& rhs) const { return this->id_== rhs.id_; }
		
	protected:
		explicit IpcEventId(EventIdType id):id_(id){}
		inline void set(EventIdType id){ id_ = id; }
		EventIdType 	id_;
};

