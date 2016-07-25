#include "IpcUniqueEventIdQueueMgr.h"
#include <string>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

using namespace std;

timeout_error::timeout_error(unsigned int timeoutMiliSec)
{
	reason_ = "timeout : " + to_string(timeoutMiliSec) + " msec";
}

const char * timeout_error::what () const throw ()
{
	return reason_.c_str();	
}

IpcUniqueEventIdQueueMgr*		IpcUniqueEventIdQueueMgr::instance_ = nullptr;
boost::interprocess::named_mutex IpcUniqueEventIdQueueMgr::ipcUniqueEventIdQueueMgrMutex_(boost::interprocess::open_or_create, "IpcUniqueEventIdQueueMgrMutex");

bool IpcUniqueEventIdQueueMgr::push(unsigned int queueUnitId, unsigned int eventId) 
{
	boost::interprocess::scoped_lock<boost::interprocess::named_mutex> sl(ipcUniqueEventIdQueueMgrMutex_);
	if (queueUnitId == EQ_ALL) {
		auto&& it = eventIdQueueUnitMap_->begin();
		while (it != eventIdQueueUnitMap_->end()) {
			it->second->push(eventId);
			++it;
		}

		return true;
	}
	else {
		auto&& it = eventIdQueueUnitMap_->find(queueUnitId);
		if (it != eventIdQueueUnitMap_->end()) {
			it->second->push(eventId);
			return true;
		}

		return false;
	}
}


unsigned int IpcUniqueEventIdQueueMgr::pop(unsigned int queueUnitId, unsigned int timeoutMiliSec)
{
	EventIdQueueUnitMap::iterator it;
	{
		boost::interprocess::scoped_lock<boost::interprocess::named_mutex> sl(ipcUniqueEventIdQueueMgrMutex_);
		it = eventIdQueueUnitMap_->find(queueUnitId);
		if (it == eventIdQueueUnitMap_->end()) {
			throw invalid_argument("invalid argument - no existed queueUnitId: " + to_string(queueUnitId));
		}
	}

	return it->second->pop(timeoutMiliSec);
}

size_t	IpcUniqueEventIdQueueMgr::size(unsigned int queueUnitId)
{
	boost::interprocess::scoped_lock<boost::interprocess::named_mutex> sl(ipcUniqueEventIdQueueMgrMutex_);
	auto&& it = eventIdQueueUnitMap_->find(queueUnitId);
	if (it == eventIdQueueUnitMap_->end()) {
		throw invalid_argument("invalid argument - no existed queueUnitId: " + to_string(queueUnitId));
	}

	return it->second->size();
}

std::size_t	IpcUniqueEventIdQueueMgr::countQueueUnit()
{
	boost::interprocess::scoped_lock<boost::interprocess::named_mutex> sl(ipcUniqueEventIdQueueMgrMutex_);
	return eventIdQueueUnitMap_->size();
}

bool IpcUniqueEventIdQueueMgr::init()
{
	eventIdQueueUnitMap_ = segment_->find_or_construct<EventIdQueueUnitMap>("EVENTID_QUEUE_UNIT_MAP") 
		      ( 5, boost::hash<unsigned int>(), std::equal_to<unsigned int>()  
				      , segment_->get_allocator<ValueType>());
	return true;
}

bool IpcUniqueEventIdQueueMgr::addEventIdQueueUnit(unsigned int unitId) 
{
	boost::interprocess::scoped_lock<boost::interprocess::named_mutex> sl(ipcUniqueEventIdQueueMgrMutex_);
	auto&& iter = eventIdQueueUnitMap_->find(unitId);
	if (iter != eventIdQueueUnitMap_->end()) {
		return false;
	}

	string shm_unit_id = "EVENTID_QUEUE_UNIT_INSTANCE_" + to_string(unitId);
	IpcUniqueEventIdQueueUnit* queueUnit = segment_->find_or_construct<IpcUniqueEventIdQueueUnit>(shm_unit_id.c_str())
		(unitId, segment_);
	eventIdQueueUnitMap_->insert(make_pair(unitId, queueUnit));
	return true;
}

void IpcUniqueEventIdQueueMgr::removeEventIdQueueUnit(unsigned int unitId)
{
	bool removed = false;
	{
		boost::interprocess::scoped_lock<boost::interprocess::named_mutex> sl(ipcUniqueEventIdQueueMgrMutex_);
		auto&& res = eventIdQueueUnitMap_->find(unitId);

		if (res != eventIdQueueUnitMap_->end()) {
			eventIdQueueUnitMap_->erase(res);
			removed = true;
		}
	}

	if (removed) {
		string shm_unit_id = "EVENTID_QUEUE_UNIT_INSTANCE_" + to_string(unitId);
		segment_->destroy<IpcUniqueEventIdQueueUnit>(shm_unit_id.c_str());
	}
}

IpcUniqueEventIdQueueMgr* 	IpcUniqueEventIdQueueMgr::createInstance(unsigned int maxQueueSize)
{
	boost::interprocess::scoped_lock<boost::interprocess::named_mutex> sl(ipcUniqueEventIdQueueMgrMutex_);
	if (instance_ == nullptr) {
		IpcUniqueEventIdQueueMgr* instance = new IpcUniqueEventIdQueueMgr;
		instance->segment_ = new boost::interprocess::managed_shared_memory(boost::interprocess::open_or_create, "IPC_UNIQUE_EVENTID_QUEUE_MGR_SHARED_SEGMENT", 64 * maxQueueSize);

		if (instance->init() == false) {
			boost::interprocess::shared_memory_object::remove("IPC_UNIQUE_EVENTID_QUEUE_MGR_SHARED_SEGMENT");
			delete instance->segment_;
			delete instance;

			return nullptr;
		}

		instance_ = instance;
	}

	return instance_;
}

IpcUniqueEventIdQueueMgr* 	IpcUniqueEventIdQueueMgr::getInstance()
{
	return instance_;
}

void 						IpcUniqueEventIdQueueMgr::releaseInstance()
{
	boost::interprocess::scoped_lock<boost::interprocess::named_mutex> sl(ipcUniqueEventIdQueueMgrMutex_);

	if (instance_ != nullptr) {
		instance_->releaseQueueUintMap();
		boost::interprocess::shared_memory_object::remove("IPC_UNIQUE_EVENTID_QUEUE_MGR_SHARED_SEGMENT");
		delete instance_;
		instance_ = nullptr;
	}
		
	boost::interprocess::named_mutex::remove("IpcUniqueEventIdQueueMgrMutex");
}

void IpcUniqueEventIdQueueMgr::releaseQueueUintMap()
{
	if (eventIdQueueUnitMap_ != nullptr) {
		auto&& it = eventIdQueueUnitMap_->begin();
		while (it != eventIdQueueUnitMap_->end()) {
			string shm_unit_id = "EVENTID_QUEUE_UNIT_INSTANCE_" + to_string(it->first);

			segment_->destroy<IpcUniqueEventIdQueueUnit>(shm_unit_id.c_str());
			it = eventIdQueueUnitMap_->erase(it);
		}

		segment_->destroy_ptr(eventIdQueueUnitMap_);
		eventIdQueueUnitMap_ = nullptr;
	}
}

IpcUniqueEventIdQueueMgr::IpcUniqueEventIdQueueUnit::IpcUniqueEventIdQueueUnit(unsigned int unitId, boost::interprocess::managed_shared_memory* segment)
{
	unitId_ = unitId;
	string event_id_unit = "IPC_UNIQUE_EVENTID_QUEUE_" + to_string(unitId);
	eventIdQueue_ = segment->find_or_construct<IpcUniqueEventIdQueue>(event_id_unit.c_str())
		(IpcUniqueEventIdQueue::ctor_args_list(), segment->get_allocator<unsigned int>());
}

bool IpcUniqueEventIdQueueMgr::IpcUniqueEventIdQueueUnit::push(unsigned int eventId) 
{
	boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> sl(eventIdQueueMutex_);
	auto&& res = eventIdQueue_->get<1>().insert(eventId);
	eventIdQueueCondition_.notify_one();
	return res.second;
}

unsigned int IpcUniqueEventIdQueueMgr::IpcUniqueEventIdQueueUnit::pop(unsigned int timeoutMiliSec)
{
	boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> sl(eventIdQueueMutex_);
	if (eventIdQueue_->get<0>().empty()) {
		if (timeoutMiliSec) {
			bool res = eventIdQueueCondition_.timed_wait(sl, boost::posix_time::microsec_clock::universal_time() +
					boost::posix_time::milliseconds(timeoutMiliSec));
			if (res == false) {
				throw timeout_error(timeoutMiliSec);
			}
		}
		else {
			eventIdQueueCondition_.wait(sl);
		}
	}

	unsigned int ret = eventIdQueue_->front();
	eventIdQueue_->pop_front();

	return ret;
}

size_t	IpcUniqueEventIdQueueMgr::IpcUniqueEventIdQueueUnit::size()
{
	boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> sl(eventIdQueueMutex_);
	if (instance_ == nullptr) {
		return 0;
	}

	return eventIdQueue_->size();
}

IpcUniqueEventIdQueueMgr::IpcUniqueEventIdQueueUnit::~IpcUniqueEventIdQueueUnit()
{
	boost::interprocess::managed_shared_memory segment(boost::interprocess::open_only, "IPC_UNIQUE_EVENTID_QUEUE_MGR_SHARED_SEGMENT");
	string event_id_unit = "IPC_UNIQUE_EVENTID_QUEUE_" + to_string(unitId_);
	segment.destroy<IpcUniqueEventIdQueue>(event_id_unit.c_str());
}
