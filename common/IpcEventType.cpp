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
			return "DC";
		case IET_HIGH_DELTA: 
			return "DH";
		case IET_LOW_DELTA: 
			return "DL";
		case IET_CUR_GAMMA: 
			return "GC";
		case IET_HIGH_GAMMA: 
			return "GH";
		case IET_LOW_GAMMA: 
			return "GL";
		case IET_CUR_THETA: 
			return "TC";
		case IET_HIGH_THETA: 
			return "TH";
		case IET_LOW_THETA: 
			return "TL";
		case IET_CUR_VEGA: 
			return "VC";
		case IET_HIGH_VEGA: 
			return "VH";
		case IET_LOW_VEGA: 
			return "VL";
		case IET_CUR_VIX: 
			return "VIXC";
		case IET_HIGH_VIX: 
			return "VIXH";
		case IET_LOW_VIX: 
			return "VIXL";
		default:
			return "";
	}
}
