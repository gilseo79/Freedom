#include "IpcEventType.h"

const char* getIpcEventTypePrefix(unsigned int ipcEventType)
{
	switch(ipcEventType) {
		case IET_CUR_PRICE: 
			return "PC";
		case IET_HIGH_PRICE: 
			return "PH";
		case IET_LOW_PRICE: 
			return "PL";
		case IET_CUR_OFFER_HOGA: 
			return "HO";
		case IET_CUR_ASK_HOGA: 
			return "HA";
		case IET_CUR_IV: 
			return "IC";
		case IET_HIGH_IV: 
			return "IH";
		case IET_LOW_IV: 
			return "IL";
		case IET_CUR_DELTA: 
			return "GDC";
		case IET_HIGH_DELTA: 
			return "GDH";
		case IET_LOW_DELTA: 
			return "GDL";
		case IET_CUR_GAMMA: 
			return "GGC";
		case IET_HIGH_GAMMA: 
			return "GGH";
		case IET_LOW_GAMMA: 
			return "GGL";
		case IET_CUR_THETA: 
			return "GTC";
		case IET_HIGH_THETA: 
			return "GTH";
		case IET_LOW_THETA: 
			return "GTL";
		case IET_CUR_VEGA: 
			return "GVC";
		case IET_HIGH_VEGA: 
			return "GVH";
		case IET_LOW_VEGA: 
			return "GVL";
		case IET_CUR_VIX: 
			return "VIXC";
		case IET_HIGH_VIX: 
			return "VIXH";
		case IET_LOW_VIX: 
			return "VIXL";
		case IET_EVENT_WATCH_DEFAULT:
			return "EW";
		default:
			return "";
	}
}

const char* getIpcEventTypeStr(unsigned int ipcEventType)
{
	switch(ipcEventType) {
		case IET_CUR_PRICE: 
			return "CUR_PRICE";
		case IET_HIGH_PRICE: 
			return "HIGH_PRICE";
		case IET_LOW_PRICE: 
			return "LOW_PRICE";
		case IET_CUR_OFFER_HOGA: 
			return "CUR_OFFER_HOGA";
		case IET_CUR_ASK_HOGA: 
			return "CUR_ASK_HOGA";
		case IET_CUR_IV: 
			return "CUR_IV";
		case IET_HIGH_IV: 
			return "HIGH_IV";
		case IET_LOW_IV: 
			return "LOW_IV";
		case IET_CUR_DELTA: 
			return "CUR_DELTA";
		case IET_HIGH_DELTA: 
			return "HIGH_DELTA";
		case IET_LOW_DELTA: 
			return "LOW_DELTA";
		case IET_CUR_GAMMA: 
			return "CUR_GAMMA";
		case IET_HIGH_GAMMA: 
			return "HIGH_GAMMA";
		case IET_LOW_GAMMA: 
			return "LOW_GAMMA";
		case IET_CUR_THETA: 
			return "CUR_THETA";
		case IET_HIGH_THETA: 
			return "HIGH_THETA";
		case IET_LOW_THETA: 
			return "LOW_THETA";
		case IET_CUR_VEGA: 
			return "CUR_VEGA";
		case IET_HIGH_VEGA: 
			return "HIGH_VEGA";
		case IET_LOW_VEGA: 
			return "LOW_VEGA";
		case IET_CUR_VIX: 
			return "CUR_VIX";
		case IET_HIGH_VIX: 
			return "HIGH_VIX";
		case IET_LOW_VIX: 
			return "LOW_VIX";
		case IET_EVENT_WATCH_DEFAULT:
			return "EVENT_WATCH_DEFAULT";
		default:
			return "";
	}
}
