#pragma once
#include "EventHandlerPluginBase.h"

class EventHandlerPluginTest : public EventHandlerPluginBase
{
	public:
		virtual void setPluginInfo();

		virtual	bool setup();
		virtual	bool preprocess();
		virtual	bool process();
		virtual	bool postprocess();
};
