#include <cstddef>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/unordered_map.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <stdexcept>
#include <functional>
#include <string>
#include <iostream>

struct ID{};
struct ALIAS{};

using namespace boost::multi_index;
typedef boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager> CharAllocator;
typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> MyShmString;

typedef unsigned int    						KeyType1;
typedef MyShmString								KeyType2;

struct ValueType1
{
	ValueType1(unsigned int id, const char* alias, CharAllocator& charAllocator):first(id), second(alias, charAllocator){}

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


int main() 
{
	boost::interprocess::shared_memory_object::remove("MySharedMemory");
	boost::interprocess::managed_shared_memory segment(boost::interprocess::open_or_create,"MySharedMemory", 65536);
	ShmemAllocator1 allocator =  segment.get_allocator<ValueType1>();
	CharAllocator ca = segment.get_allocator<char>();
	CharAllocator ca1 = ca;

	//Construct the multi_index in shared memory
	BidirectIdAliasMap *es = segment.construct<BidirectIdAliasMap>
		("My MultiIndex Container")            //Container's name in shared memory
		( BidirectIdAliasMap::ctor_args_list()
		  , allocator);  //Ctor parameters
	/*
	   BidirectIdAliasMap aa;
	   ValueType1 v;
	   v.first = 1;

	   aa.insert(v);
	*/
	ValueType1 v1(10, "humm", ca);
	es->insert(v1);
	v1.first = 2;
	v1.second = MyShmString("herher", ca);
	es->insert(v1);

	v1.first = 5;
	v1.second = MyShmString("", ca);
	es->insert(v1);

	v1.first = 13;
	v1.second = MyShmString("", ca);
	es->insert(v1);
	auto it = es->get<0>().begin();
	while (it != es->get<0>().end()) {
		std::cout<< it->first << ", " << it->second << std::endl;
		++it;
	}

	auto sit = es->get<1>().find(MyShmString("herher", ca));
		std::cout<< sit->first << ", " << sit->second << std::endl;
	return 0;
}

