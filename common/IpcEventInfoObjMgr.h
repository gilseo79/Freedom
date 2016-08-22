#pragma once

#include <cstddef>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/unordered_map.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include "CommonTypeDefs.h"
#include "IpcEventInfo.h"

using namespace boost::multi_index;

struct ID{};
struct ALIAS{};

class IpcEventInfoObjMgr 
{
	public:
		typedef EventIdType    							KeyType;
		typedef IpcEventInfo							MappedType;
		typedef std::pair<KeyType, MappedType> 			ValueType;

		typedef boost::interprocess::allocator<ValueType, boost::interprocess::managed_shared_memory::segment_manager> ShmemAllocator;

		typedef boost::unordered_map
			< KeyType               , MappedType
			, boost::hash<KeyType>  , std::equal_to<KeyType>
			, ShmemAllocator> IpcEventInfoObjMap;

		typedef boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager> CharAllocator;
		typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> MyShmString;
		
		typedef EventIdType    							KeyType1;
		typedef MyShmString								KeyType2;

		struct ValueType1
		{
			ValueType1(EventIdType id, const char* alias, CharAllocator& charAllocator):first(id), second(alias, charAllocator){}

			KeyType1	first;
			KeyType2	second;
		};

		typedef boost::interprocess::allocator<ValueType1, boost::interprocess::managed_shared_memory::segment_manager> ShmemAllocator1;

		BOOST_STATIC_CONSTANT(unsigned,from_offset=offsetof(ValueType1,first));
		BOOST_STATIC_CONSTANT(unsigned,to_offset  =offsetof(ValueType1,second));

		typedef multi_index_container<
			ValueType1,
			indexed_by<
				hashed_unique<
				tag<ID>, member_offset<ValueType1,KeyType1,from_offset> >,
			hashed_non_unique<
				tag<ALIAS>, member_offset<ValueType1,KeyType2,to_offset> >
				>,
			boost::interprocess::managed_shared_memory::allocator<ValueType1>::type
		> BidirectIdAliasMap;

		// member functions
		bool 				registerEvent(IpcEventId& ipcEventId, const char* alias = nullptr);

		const IpcEventInfo* getEvent(const IpcEventId& ipcEventId) const;
		const IpcEventInfo* getEvent(EventIdType ipcEventId) const;
		const IpcEventInfo* getEvent(const std::string& ipcEventAlias) const;

		bool 				getEvent(EventIdType ipcEventId, IpcEventInfo& ipcEventInfo) const;
		bool			 	getEvent(const IpcEventId& ipcEventId, IpcEventInfo& ipcEventInfo) const;
		bool			 	getEvent(const std::string& ipcEventAlias, IpcEventInfo& ipcEventInfo) const;

		bool 				getEventId(const std::string& ipcEventAlias, IpcEventId& ipcEventId) const;
		EventIdType 		getEventId(const std::string& ipcEventAlias) const;

		bool 				removeEvent(EventIdType ipcEventId);
		bool 				removeEvent(const IpcEventId& ipcEventId);
		bool 				removeEvent(const std::string& ipcEventAlias);

		template<typename T>
		bool 				updateEvent(EventIdType ipcEventId, const T& data) {
			boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lg(*ipcEventInfoObjMapMutex_);
			auto&& res = ipcEventInfoObjMap_->find(ipcEventId);
			if (res == ipcEventInfoObjMap_->end()) {
				return false;
			}

			boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lg1(*ipcEventInfoMutex_);
			if (res->second.setUserData(data)) {
				res->second.incIpcEventSeq();
				return true;
			}

			return false;
		}

		template<typename T>
		bool 				updateEvent(const IpcEventId& ipcEventId, const T& data) {
			if (ipcEventId == 0) {
				return false;
			}

			return updateEvent(ipcEventId.get(), data);
		}

		template<typename T>
		bool 				updateEvent(const std::string& ipcEventAlias, const T& data) {
			EventIdType ipcEventId = getEventId(ipcEventAlias);
			if (ipcEventId == 0) {
				return false;
			}

			return updateEvent(ipcEventId, data);
		}

		bool 				updateEvent(EventIdType ipcEventId, const char* data = nullptr, unsigned char dataSize = 0, unsigned char dataType = IpcEventInfo::IEUT_NONE);
		bool 				updateEvent(const IpcEventId& ipcEventId, const char* data = nullptr, unsigned char dataSize = 0, unsigned char dataType = IpcEventInfo::IEUT_NONE);
		bool				updateEvent(const std::string& ipcEventAlias, const char* data = nullptr, unsigned char dataSize = 0, unsigned char dataType = IpcEventInfo::IEUT_NONE);
			
		void 				printAll() const;

		static IpcEventInfoObjMgr* getInstance();

	private:
		static IpcEventInfoObjMgr* createInstance();
		EventIdType 		allocIpcEventInfoObjId();
		bool 				init();

	private: //variables 
		static IpcEventInfoObjMgr*							instance_;
		static boost::interprocess::named_mutex				instanceMutex_;

		IpcEventInfoObjMgr(){}
		IpcEventInfoObjMgr& operator=(const IpcEventInfoObjMgr& rhs) {}

		IpcEventInfoObjMap*									ipcEventInfoObjMap_;
		mutable boost::interprocess::interprocess_mutex*	ipcEventInfoObjMapMutex_;
		BidirectIdAliasMap*									eventIdAliasMap_;
		mutable boost::interprocess::interprocess_mutex*	ipcEventInfoMutex_;
		EventIdType*										curIpcEventInfoObjId_;
		mutable boost::interprocess::interprocess_mutex*	curIpcEventInfoObjIdMutex_;
		boost::interprocess::managed_shared_memory*			segment_;
};

