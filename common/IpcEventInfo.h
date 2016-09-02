#pragma once

#include <string.h>
#include <string>
#include "CommonConstants.h"
#include "CommonTypeDefs.h"
#include "FixedFloat.h"
#include "boost/any.hpp"
#include "IpcEventId.h"
#include "EventSeqInfo.h"

class IpcEventInfoObjMgr;

class IpcEventInfo: public EventSeqInfo 
{
	public:
		friend class IpcEventInfoObjMgr;
		enum {IEUT_NONE, IEUT_BOOL, IEUT_CHAR, IEUT_UCHAR, IEUT_SHORT, IEUT_USHORT, IEUT_INT, IEUT_UINT, 
			IEUT_FLOAT, IEUT_DOUBLE, IEUT_LONGLONG, IEUT_ULONGLONG, IEUT_BYTES, IEUT_STRING, IEUT_FIXEDFLOAT};

		IpcEventInfo();
		IpcEventInfo(unsigned char eventType);
		inline EventSeqType 	getEventSeq() const { return getSeq(); } 
		inline unsigned int 	getEventType() const { return eventType_; } 
		inline const char*		getUserData() const { return data_; }
		boost::any				getUserDataAny() const;
		std::string				getUserDataAsStr() const;
		inline unsigned int		getUserDataSize() const { return dataSize_; }	
		inline unsigned int		getUserDataType() const { return dataType_; }	
		static const char* 		getUserDataTypeStr(unsigned int dataType);
		const char* 			getUserDataTypeStr() const { return getUserDataTypeStr(dataType_); }

		void					printAll() const;

	protected:
		inline void				incEventSeq() { incSeq();}
		bool					setUserData(const char* data, unsigned char dataSize, unsigned char dataType = IEUT_BYTES);
		template<typename T>
		bool					setUserData(const T& data) {
			if (typeid(bool) == typeid(data)) {
				dataType_ = IEUT_BOOL;
			}
			else if (typeid(char) == typeid(data)) {
				dataType_ = IEUT_CHAR;
			}
			else if (typeid(unsigned char) == typeid(data)) {
				dataType_ = IEUT_UCHAR;
			}
			else if (typeid(short) == typeid(data)) {
				dataType_ = IEUT_SHORT;
			}
			else if (typeid(unsigned short) == typeid(data)) {
				dataType_ = IEUT_USHORT;
			}
			else if (typeid(int) == typeid(data)) {
				dataType_ = IEUT_INT;
			}
			else if (typeid(unsigned int) == typeid(data)) {
				dataType_ = IEUT_UINT;
			}
			else if (typeid(float) == typeid(data)) {
				dataType_ = IEUT_FLOAT;
			}
			else if (typeid(double) == typeid(data)) {
				dataType_ = IEUT_DOUBLE;
			}
			else if (typeid(long long) == typeid(data)) {
				dataType_ = IEUT_LONGLONG;
			}
			else if (typeid(unsigned long long) == typeid(data)) {
				dataType_ = IEUT_ULONGLONG;
			}
			else if (typeid(FixedFloat) == typeid(data)) {
				dataType_ = IEUT_FIXEDFLOAT;
			}
			else {
				return false;
			}

			dataSize_ = sizeof(data);
			memcpy(data_, (const char*)&data, dataSize_);

			return true;
		}

		bool					setUserData(const std::string& data);
		inline void				setEventType(unsigned char eventType) { eventType_ = eventType; }

	protected:
		unsigned char	eventType_;
		unsigned char	dataSize_:4;
		unsigned char	dataType_:4;
		char			data_[MaxEventInfoUserDataSize];
};

