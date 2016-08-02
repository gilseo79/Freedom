#pragma once

#include <cstddef>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/unordered_map.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include "IpcEventInfo.h"

using namespace boost::multi_index;

struct ID{};
struct ALIAS{};

class IpcEventInfoObjMgr 
{
	public:
		typedef unsigned short    						KeyType;
		typedef IpcEventInfo							MappedType;
		typedef std::pair<KeyType, MappedType> 			ValueType;

		typedef boost::interprocess::allocator<ValueType, boost::interprocess::managed_shared_memory::segment_manager> ShmemAllocator;

		typedef boost::unordered_map
			< KeyType               , MappedType
			, boost::hash<KeyType>  , std::equal_to<KeyType>
			, ShmemAllocator> IpcEventInfoObjMap;

		typedef boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager> CharAllocator;
		typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> MyShmString;
		
		typedef unsigned short    						KeyType1;
		typedef MyShmString								KeyType2;

		struct ValueType1
		{
			ValueType1(unsigned short id, const char* alias, CharAllocator& charAllocator):first(id), second(alias, charAllocator){}

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


		IpcEventInfoObjMgr();


		bool 				init();
		bool 				registerEvent(IpcEventId& ipcEventId, const char* alias = nullptr);
		const IpcEventInfo* getEvent(const IpcEventId& ipcEventId) const;
		const IpcEventInfo* getEvent(unsigned short ipcEventId) const;
		const IpcEventInfo* getEvent(const std::string& ipcEventAlias) const;
		bool 				getEventId(const std::string& ipcEventAlias, IpcEventId& ipcEventId) const;
		unsigned short 		getEventId(const std::string& ipcEventAlias) const;
		bool 				removeEvent(unsigned short ipcEventId);
		bool 				removeEvent(const IpcEventId& ipcEventId);
		bool 				removeEvent(const std::string& ipcEventAlias);

		template<typename T>
		bool 				updateEvent(unsigned short ipcEventId, const T& data) {
			boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lg(*ipcEventInfoObjMapMutex_);
			auto&& res = ipcEventInfoObjMap_->find(ipcEventId);
			if (res == ipcEventInfoObjMap_->end()) {
				return false;
			}

			boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lg1(*ipcEventInfoMutex_);
			bool updated = res->second.setUserData(data);
			if (updated) {
				res->second.incIpcEventSeq();
			}

			return updated;
		}

		bool 				updateEvent(unsigned short ipcEventId, const char* data = nullptr, unsigned char dataSize = 0, unsigned char dataType = IpcEventInfo::IEUT_NONE);
		bool 				updateEvent(const IpcEventId& ipcEventId, const char* data = nullptr, unsigned char dataSize = 0, unsigned char dataType = IpcEventInfo::IEUT_NONE);
		bool				updateEvent(const std::string& ipcEventAlias, const char* data = nullptr, unsigned char dataSize = 0, unsigned char dataType = IpcEventInfo::IEUT_NONE);
			
		void 				printAll() const;


	private:
		bool update(unsigned short ipcEventId, const char* data){}
		unsigned short allocIpcEventInfoObjId();

		IpcEventInfoObjMap*									ipcEventInfoObjMap_;
		mutable boost::interprocess::interprocess_mutex*	ipcEventInfoObjMapMutex_;
		BidirectIdAliasMap*									eventIdAliasMap_;
		mutable boost::interprocess::interprocess_mutex*	ipcEventInfoMutex_;
		unsigned short*										curIpcEventInfoObjId_;
		mutable boost::interprocess::interprocess_mutex*	curIpcEventInfoObjIdMutex_;
		boost::interprocess::managed_shared_memory*			segment_;
};

