#include "IpcEventInfoObjMgr.h"
#include <iostream>
#include "CommonConstants.h"

using namespace std;
using namespace boost::interprocess;


boost::interprocess::named_mutex	IpcEventInfoObjMgr::instanceMutex_{open_or_create, "IPC_EVENT_INFO_OBJ_MGR_MUTEX"};
IpcEventInfoObjMgr*					IpcEventInfoObjMgr::instance_ = nullptr;

IpcEventInfoObjMgr* IpcEventInfoObjMgr::getInstance()
{
	if (instance_ == nullptr) {
		instance_ = createInstance();
	}

	return instance_;
}

IpcEventInfoObjMgr* IpcEventInfoObjMgr::createInstance()
{
	scoped_lock<named_mutex> lg(instanceMutex_);
	if (instance_ == nullptr) {
		IpcEventInfoObjMgr* instance = new IpcEventInfoObjMgr;
		if (instance->init()) {
			instance_ = instance;
		}
		else {
			delete instance;
		}
	}

	return instance_;
}

bool IpcEventInfoObjMgr::init()
{
	segment_ = new managed_shared_memory(open_or_create, "IPC_EVENT_OBJ_MGR_SEGMENT", IpcEventObjMgrSegmentSize);
	ipcEventInfoObjMap_ = segment_->find_or_construct<IpcEventInfoObjMap>("IPC_EVENT_INFO_OBJ_MAP") 
		      (IpcEventObjMapBucketSize, boost::hash<EventIdType>(), std::equal_to<EventIdType>()  
				      ,segment_->get_allocator<ValueType>());
	eventIdAliasMap_ = segment_->find_or_construct<BidirectIdAliasMap>("IPC_EVENT_INFO_ALIAS_ID_MAP")
		(BidirectIdAliasMap::ctor_args_list(), segment_->get_allocator<ValueType1>());
	ipcEventInfoObjMapMutex_ = segment_->find_or_construct<interprocess_mutex>("IPC_EVENT_OBJ_MAP_MUTEX")();
	curIpcEventInfoObjId_ = segment_->find_or_construct<EventIdType>("CUR_IPC_EVENT_INFO_OBJ_ID")(1);
	curIpcEventInfoObjIdMutex_ = segment_->find_or_construct<interprocess_mutex>("CUR_IPC_EVENT_INFO_OBJ_ID_MUTEX")();
	ipcEventInfoMutex_ = segment_->find_or_construct<interprocess_mutex>("IPC_EVENT_INFO_MUTEX")();

	return true;
}

bool IpcEventInfoObjMgr::registerEvent(IpcEventId& ipcEventId, const char* alias)
{
	if (ipcEventId.get() != 0) {
		cout << "Already existing IpcEventInfo Id: " << ipcEventId << endl;
		return false;
	}

	scoped_lock<interprocess_mutex> lg(*ipcEventInfoObjMapMutex_);
	CharAllocator charAllocator = segment_->get_allocator<char>();
	ValueType1 kv1(0, alias == nullptr?"":alias, charAllocator);
	if (alias != nullptr) {
		auto& aliasMap = eventIdAliasMap_->get<1>();
		auto&& it = aliasMap.find(kv1.second);

		if (it != aliasMap.end()) {
			cout << "Already existing IpcEventInfo Alias: " << alias << endl;
			return false;
		}
	}

	kv1.first = allocIpcEventInfoObjId();
	ipcEventInfoObjMap_->insert(make_pair(kv1.first, IpcEventInfo()));
	eventIdAliasMap_->insert(kv1);
	ipcEventId.set(kv1.first);

	return true;
}

EventIdType IpcEventInfoObjMgr::allocIpcEventInfoObjId() {
	scoped_lock<interprocess_mutex> lg(*curIpcEventInfoObjIdMutex_);
	return (*curIpcEventInfoObjId_)++;
}

const IpcEventInfo* IpcEventInfoObjMgr::getEvent(EventIdType ipcEventId) const
{
	scoped_lock<interprocess_mutex> lg(*ipcEventInfoObjMapMutex_);
	auto&& res = ipcEventInfoObjMap_->find(ipcEventId);
	if (res == ipcEventInfoObjMap_->end()) {
		return nullptr;
	}

	return &(res->second);
}

const IpcEventInfo* IpcEventInfoObjMgr::getEvent(const IpcEventId& ipcEventId) const
{
	if (ipcEventId == 0) {
		return nullptr;
	}

	return getEvent(ipcEventId.get());
}

const IpcEventInfo* IpcEventInfoObjMgr::getEvent(const std::string& ipcEventAlias) const
{
	if (ipcEventAlias.empty()) {
		return nullptr;
	}

	scoped_lock<interprocess_mutex> lg(*ipcEventInfoObjMapMutex_);
	auto&& aliasIt = eventIdAliasMap_->get<1>().find(MyShmString(ipcEventAlias.c_str(), segment_->get_allocator<char>()));
	if (aliasIt == eventIdAliasMap_->get<1>().end()) {
		return nullptr;
	}

	auto&& res = ipcEventInfoObjMap_->find(aliasIt->first);
	if (res == ipcEventInfoObjMap_->end()) {
		return nullptr;
	}

	return &(res->second);
}

bool 	IpcEventInfoObjMgr::getEvent(EventIdType ipcEventId, IpcEventInfo& ipcEventInfo) const
{
	const IpcEventInfo* info = getEvent(ipcEventId);
	if (info == nullptr) {
		return false;
	}

	scoped_lock<interprocess_mutex> lg(*ipcEventInfoMutex_);
	ipcEventInfo = *info;
	return true;
}

bool	IpcEventInfoObjMgr::getEvent(const IpcEventId& ipcEventId, IpcEventInfo& ipcEventInfo) const
{
	return getEvent(ipcEventId.get(), ipcEventInfo);
}

bool	IpcEventInfoObjMgr::getEvent(const std::string& ipcEventAlias, IpcEventInfo& ipcEventInfo) const
{
	EventIdType ipcEventId = getEventId(ipcEventAlias);
	return getEvent(ipcEventId, ipcEventInfo);
}

EventIdType 	IpcEventInfoObjMgr::getEventId(const std::string& ipcEventAlias) const
{
	if (ipcEventAlias.empty()) {
		return 0;
	}

	scoped_lock<interprocess_mutex> lg(*ipcEventInfoObjMapMutex_);
	auto&& aliasIt = eventIdAliasMap_->get<1>().find(MyShmString(ipcEventAlias.c_str(), segment_->get_allocator<char>()));
	if (aliasIt == eventIdAliasMap_->get<1>().end()) {
		return 0;
	}

	return aliasIt->first;
}

bool IpcEventInfoObjMgr::getEventId(const std::string& ipcEventAlias, IpcEventId& ipcEventId) const
{
	ipcEventId.set(getEventId(ipcEventAlias));
	if (ipcEventId.get() == 0) {
		return false;
	}
	
	return true;
}

bool IpcEventInfoObjMgr::removeEvent(EventIdType ipcEventId)
{
	if (ipcEventId == 0) {
		return false;
	}

	scoped_lock<interprocess_mutex> lg(*ipcEventInfoObjMapMutex_);
	auto&& res = ipcEventInfoObjMap_->erase(ipcEventId);
	if (res == 0) {
		return false;
	}
	
	eventIdAliasMap_->get<0>().erase(ipcEventId);

	return true;
}

bool IpcEventInfoObjMgr::removeEvent(const IpcEventId& ipcEventId)
{
	if (ipcEventId.get() == 0) {
		return false;
	}

	return removeEvent(ipcEventId.get());
}

bool IpcEventInfoObjMgr::removeEvent(const std::string& ipcEventAlias)
{
	return removeEvent(getEventId(ipcEventAlias));
}

bool IpcEventInfoObjMgr::updateEvent(EventIdType ipcEventId, const char* data, unsigned char dataSize, unsigned char dataType)
{
	if (ipcEventId == 0) {
		return false;
	}

	scoped_lock<interprocess_mutex> lg(*ipcEventInfoObjMapMutex_);
	auto&& res = ipcEventInfoObjMap_->find(ipcEventId);
	if (res == ipcEventInfoObjMap_->end()) {
		return false;
	}

	scoped_lock<interprocess_mutex> lg1(*ipcEventInfoMutex_);

	if (data != nullptr || dataSize != 0 || dataType != IpcEventInfo::IEUT_NONE) {
		if (data != nullptr && dataSize != 0 && dataType != IpcEventInfo::IEUT_NONE) {
			if (!res->second.setUserData(data, dataSize, dataType)) {
				return false;
			}
		}
		else {

			if (data != nullptr && dataSize == 0 && dataType == IpcEventInfo::IEUT_NONE) {
				if (!res->second.setUserData(std::string(data))) {
					return false;
				}
			}
			else {
				return false;
			}
		}
	}

	res->second.incIpcEventSeq();
	return true;
}

bool IpcEventInfoObjMgr::updateEvent(const IpcEventId& ipcEventId, const char* data, unsigned char dataSize, unsigned char dataType)
{
	return updateEvent(ipcEventId.get(), data, dataSize, dataType);
}

bool IpcEventInfoObjMgr::updateEvent(const std::string& ipcEventAlias, const char* data, unsigned char dataSize, unsigned char dataType)
{
	return updateEvent(getEventId(ipcEventAlias), data, dataSize, dataType);
}

void IpcEventInfoObjMgr::printAll() const{
	{
		scoped_lock<interprocess_mutex> lg(*curIpcEventInfoObjIdMutex_);
		cout << "Next Event ID: " << *curIpcEventInfoObjId_ << endl;
	}
	cout << "Registered IpcEventInfoObjs\n";

	scoped_lock<interprocess_mutex> lg(*ipcEventInfoObjMapMutex_);
	IpcEventInfoObjMap::const_iterator it = ipcEventInfoObjMap_->cbegin();
	while (it != ipcEventInfoObjMap_->cend()) {
		cout << "ID: " << it->first << endl;
		it->second.printAll();
		++it;
	}
}

