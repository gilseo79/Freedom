#pragma once

#include <string.h>
#include <string>
#include "FixedFloat.h"
#include "boost/any.hpp"

class IpcEventInfoObjMgr;

class IpcEventId
{
	public:
		friend class IpcEventInfoObjMgr;
		IpcEventId():id_(0){}
		inline unsigned short get() const { return id_; }
		operator unsigned short() const { return id_; }
	private:
		inline void set(unsigned short id){ id_ = id; }
		unsigned short 	id_;
};

const int MaxEventInfoUserDataSize	= 11;

class IpcEventInfo {
	public:
		friend class IpcEventInfoObjMgr;
		enum {IEUT_NONE, IEUT_BOOL, IEUT_CHAR, IEUT_UCHAR, IEUT_SHORT, IEUT_USHORT, IEUT_INT, IEUT_UINT, 
			IEUT_FLOAT, IEUT_DOUBLE, IEUT_LONGLONG, IEUT_ULONGLONG, IEUT_BYTES, IEUT_STRING, IEUT_FIXEDFLOAT};

		IpcEventInfo();
		unsigned short 			getIpcEventSeq() const;
		void					incIpcEventSeq();
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
		inline const char*		getUserData() const { return data_; }
		boost::any				getUserDataAny() const;
		inline unsigned int		getUserDataSize() const { return dataSize_; }	
		inline unsigned int		getUserDataType() const { return dataType_; }	
		static const char* 		getUserDataTypeStr(unsigned int dataType);
		const char* 			getUserDataTypeStr() const { return getUserDataTypeStr(dataType_); }

		void					printAll() const;

	private:
		unsigned short 	eventSeq_;
		unsigned char	dataSize_:4;
		unsigned char	dataType_:4;
		char			data_[MaxEventInfoUserDataSize];
};


