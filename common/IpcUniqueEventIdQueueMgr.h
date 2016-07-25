#pragma once

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/unordered_map.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <stdexcept>

typedef boost::interprocess::managed_shared_memory::segment_manager                         SegmentManager;

using namespace boost::multi_index;
typedef multi_index_container<
	unsigned int,
	indexed_by<
		sequenced<>,           
		hashed_unique<identity<unsigned int> >
	>,
	boost::interprocess::managed_shared_memory::allocator<unsigned int>::type
> IpcUniqueEventIdQueue;

struct timeout_error: public std::exception
{
	explicit timeout_error(unsigned int timeoutMiliSec);
	const char * what () const throw ();
	private:
		std::string		reason_;
};

const unsigned int DefaultIpcUniqueEventQueueSize		= 16 * 1000;

class IpcUniqueEventIdQueueMgr
{
	public:
		enum {EQ_ALL = -1};
		bool push(unsigned int queueUnitId, unsigned int eventId);
		unsigned int pop(unsigned int queueUnitId, unsigned int timeoutMiliSec = 0);
		std::size_t	size(unsigned int queueUnitId);
		bool addEventIdQueueUnit(unsigned int unitId);
		void removeEventIdQueueUnit(unsigned int unitId);
		std::size_t	countQueueUnit();

		static IpcUniqueEventIdQueueMgr* 	createInstance(unsigned int maxQueueSize);
		static IpcUniqueEventIdQueueMgr* 	getInstance();
		static void 						releaseInstance();

	private:
		class IpcUniqueEventIdQueueUnit
		{
			public:
				IpcUniqueEventIdQueueUnit(unsigned int unitId, boost::interprocess::managed_shared_memory* segment);
				~IpcUniqueEventIdQueueUnit();
				bool push(unsigned int eventId);
				unsigned int pop(unsigned int timeoutMiliSec = 0);
				std::size_t	size();

			private:
				IpcUniqueEventIdQueueUnit& operator=(const IpcUniqueEventIdQueueUnit& rhs){}

				unsigned int 											unitId_;
				boost::interprocess::offset_ptr<IpcUniqueEventIdQueue>	eventIdQueue_;
				boost::interprocess::interprocess_mutex					eventIdQueueMutex_;
				boost::interprocess::interprocess_condition				eventIdQueueCondition_;
		};

		typedef unsigned int    											KeyType;
		typedef boost::interprocess::offset_ptr<IpcUniqueEventIdQueueUnit> 	MappedType;
		typedef std::pair<const KeyType, MappedType> 						ValueType;

		typedef boost::interprocess::allocator<ValueType, boost::interprocess::managed_shared_memory::segment_manager> ShmemAllocator;

		typedef boost::unordered_map
			< KeyType               , MappedType
			, boost::hash<KeyType>  , std::equal_to<KeyType>
			, ShmemAllocator> EventIdQueueUnitMap;

		bool init();
		void releaseQueueUintMap();
		IpcUniqueEventIdQueueMgr(){}
		IpcUniqueEventIdQueueMgr& operator=(const IpcUniqueEventIdQueueMgr& rhs){}
		static IpcUniqueEventIdQueueMgr*				instance_;
		boost::interprocess::managed_shared_memory*		segment_;
		static boost::interprocess::named_mutex			ipcUniqueEventIdQueueMgrMutex_;
		EventIdQueueUnitMap*							eventIdQueueUnitMap_;
};
