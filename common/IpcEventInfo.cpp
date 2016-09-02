#include "IpcEventInfo.h"
#include <iostream>

using namespace std;

IpcEventInfo::IpcEventInfo(): eventType_(0), dataSize_(0), dataType_(0)
{
}

IpcEventInfo::IpcEventInfo(unsigned char eventType): eventType_(eventType), dataSize_(0), dataType_(0)
{
}

bool			IpcEventInfo::setUserData(const std::string& data)
{
	if (data.size() >= MaxEventInfoUserDataSize) {
		return false;
	}

	dataType_ = IEUT_STRING;
	dataSize_ = data.size();
	memcpy(data_, data.c_str(), dataSize_);
	data_[dataSize_] = 0;

	return true;
}

bool			IpcEventInfo::setUserData(const char* data, unsigned char dataSize, unsigned char dataType)
{
	if (IEUT_BYTES == dataType) {
		dataType_ = IEUT_BYTES;
	}
	else if (IEUT_BOOL == dataType) {
		dataType_ = IEUT_BOOL;
	}
	else if (IEUT_CHAR == dataType) {
		dataType_ = IEUT_CHAR;
	}
	else if (IEUT_UCHAR == dataType) {
		dataType_ = IEUT_UCHAR;
	}
	else if (IEUT_SHORT == dataType) {
		dataType_ = IEUT_SHORT;
	}
	else if (IEUT_USHORT == dataType) {
		dataType_ = IEUT_USHORT;
	}
	else if (IEUT_INT == dataType) {
		dataType_ = IEUT_INT;
	}
	else if (IEUT_UINT == dataType) {
		dataType_ = IEUT_UINT;
	}
	else if (IEUT_FLOAT == dataType) {
		dataType_ = IEUT_FLOAT;
	}
	else if (IEUT_DOUBLE == dataType) {
		dataType_ = IEUT_DOUBLE;
	}
	else if (IEUT_LONGLONG == dataType) {
		dataType_ = IEUT_LONGLONG;
	}
	else if (IEUT_ULONGLONG == dataType) {
		dataType_ = IEUT_ULONGLONG;
	}
	else if (IEUT_FIXEDFLOAT == dataType) {
		dataType_ = IEUT_FIXEDFLOAT;
	}
	else if (IEUT_STRING == dataType) {
		if (dataSize >= MaxEventInfoUserDataSize) {
			return false;
		}

		dataType_ = IEUT_STRING;
		dataSize_ = dataSize;
		memcpy(data_, data, dataSize_);
		data_[dataSize_] = 0;

		return true;
	}
	else {
		return false;
	}

	dataSize_ = dataSize;
	memcpy(data_, data, dataSize_);

	return true;
}


boost::any		IpcEventInfo::getUserDataAny() const
{
	boost::any data;

	if (IEUT_BYTES == dataType_) {
		data = (const char*)data_;
	}
	else if (IEUT_BOOL == dataType_) {
		data = *((const bool*)data_);
	}
	else if (IEUT_CHAR == dataType_) {
		data = *((const char*)data_);
	}
	else if (IEUT_UCHAR == dataType_) {
		data = *((const unsigned char*)data_);
	}
	else if (IEUT_SHORT == dataType_) {
		data = *((const short*)data_);
	}
	else if (IEUT_USHORT == dataType_) {
		data = *((const unsigned short*)data_);
	}
	else if (IEUT_INT == dataType_) {
		data = *((const int*)data_);
	}
	else if (IEUT_UINT == dataType_) {
		data = *((const unsigned int*)data_);
	}
	else if (IEUT_FLOAT == dataType_) {
		data = *((const float*)data_);
	}
	else if (IEUT_DOUBLE == dataType_) {
		data = *((const double*)data_);
	}
	else if (IEUT_LONGLONG == dataType_) {
		data = *((const long long*)data_);
	}
	else if (IEUT_ULONGLONG == dataType_) {
		data = *((const unsigned long long*)data_);
	}
	else if (IEUT_FIXEDFLOAT == dataType_) {
		data = *((const FixedFloat*)data_);
	}
	else if (IEUT_STRING == dataType_) {
		data = string(data_);
	}
	
	return data;
}

std::string	IpcEventInfo::getUserDataAsStr() const
{
	std::string data;

	if (IEUT_NONE == dataType_) {
		data = "(NONE)";
	}
	else if (IEUT_BYTES == dataType_) {
		char tmp[10] = {0,};
		for (int i = 0; i < dataSize_; ++i) {
			sprintf(tmp, "%02X ", (unsigned char)data_[i]);
			data += tmp;
		}
	}
	else if (IEUT_BOOL == dataType_) {
		data = std::to_string(*((const bool*)data_));
	}
	else if (IEUT_CHAR == dataType_) {
		data = std::to_string(*((const char*)data_));
	}
	else if (IEUT_UCHAR == dataType_) {
		data = std::to_string(*((const unsigned char*)data_));
	}
	else if (IEUT_SHORT == dataType_) {
		data = std::to_string(*((const short*)data_));
	}
	else if (IEUT_USHORT == dataType_) {
		data = std::to_string(*((const unsigned short*)data_));
	}
	else if (IEUT_INT == dataType_) {
		data = std::to_string(*((const int*)data_));
	}
	else if (IEUT_UINT == dataType_) {
		data = std::to_string(*((const unsigned int*)data_));
	}
	else if (IEUT_FLOAT == dataType_) {
		data = std::to_string(*((const float*)data_));
	}
	else if (IEUT_DOUBLE == dataType_) {
		data = std::to_string(*((const double*)data_));
	}
	else if (IEUT_LONGLONG == dataType_) {
		data = std::to_string(*((const long long*)data_));
	}
	else if (IEUT_ULONGLONG == dataType_) {
		data = std::to_string(*((const unsigned long long*)data_));
	}
	else if (IEUT_FIXEDFLOAT == dataType_) {
		data = std::to_string((double)*((const FixedFloat*)data_));
	}
	else if (IEUT_STRING == dataType_) {
		data = string(data_);
	}
	else {
		data = "(UNKNOWN)";
	}

	return data;
}

void			IpcEventInfo::printAll() const 
{
	cout << "Seq: " << getEventSeq() << ", Type: " << getEventType() << ",DataType: " << getUserDataTypeStr() 
		<< "(" << getUserDataType() << "),Size: " << getUserDataSize() << ", Data: " << getUserDataAsStr() << endl;
}

		
const char* 		IpcEventInfo::getUserDataTypeStr(unsigned int dataType) 
{
	if (IEUT_NONE == dataType) {
		return "NONE";
	}
	else if (IEUT_BYTES == dataType) {
		return "BYTES";
	}
	else if (IEUT_BOOL == dataType) {
		return "BOOL";
	}
	else if (IEUT_CHAR == dataType) {
		return "CHAR";
	}
	else if (IEUT_UCHAR == dataType) {
		return "UCHAR";
	}
	else if (IEUT_SHORT == dataType) {
		return "SHORT";
	}
	else if (IEUT_USHORT == dataType) {
		return "USHORT";
	}
	else if (IEUT_INT == dataType) {
		return "INT";
	}
	else if (IEUT_UINT == dataType) {
		return "UINT";
	}
	else if (IEUT_FLOAT == dataType) {
		return "FLOAT";
	}
	else if (IEUT_DOUBLE == dataType) {
		return "DOUBLE";
	}
	else if (IEUT_LONGLONG == dataType) {
		return "LONGLONG";
	}
	else if (IEUT_ULONGLONG == dataType) {
		return "ULONGLONG";
	}
	else if (IEUT_FIXEDFLOAT == dataType) {
		return "FIXEDFLOAT";
	}
	else if (IEUT_STRING == dataType) {
		return "STRING";
	}
	else {
		return "UNKNOWN_DATA_TYPE";
	}
}
