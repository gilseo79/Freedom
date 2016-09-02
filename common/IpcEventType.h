#pragma once
#include <string>

enum IpcEventType {
	IET_DEFAULT = 0,
	IET_CUR_PRICE,
	IET_HIGH_PRICE,
	IET_LOW_PRICE,
	IET_CUR_OFFER_HOGA,
	IET_CUR_ASK_HOGA,
	IET_CUR_IV,
	IET_HIGH_IV,
	IET_LOW_IV,
	IET_CUR_DELTA,
	IET_HIGH_DELTA,
	IET_LOW_DELTA,
	IET_CUR_GAMMA,
	IET_HIGH_GAMMA,
	IET_LOW_GAMMA,
	IET_CUR_THETA,
	IET_HIGH_THETA,
	IET_LOW_THETA,
	IET_CUR_VEGA,
	IET_HIGH_VEGA,
	IET_LOW_VEGA,
	IET_CUR_VIX,
	IET_HIGH_VIX,
	IET_LOW_VIX,

	IET_EVENT_WATCH_DEFAULT = 128
};

extern const char* getIpcEventTypePrefix(unsigned int ipcEventType);
extern const char* getIpcEventTypeStr(unsigned int ipcEventType);
