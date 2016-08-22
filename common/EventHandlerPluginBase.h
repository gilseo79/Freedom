#pragma once
#include <string>

class EventHandlerPluginInfo
{
	public:
		const std::string& getName() const { return name_; }
		unsigned short getVersionLow() const { return verLow_; }
		unsigned short getVersionHigh() const { return verHigh_; }

	private:
		unsigned int	verLow_:16;
		unsigned int	verHigh_:16;
		std::string		name_;
};

class EventHandlerPluginBase
{
	public:
		friend class EventHandlerPluginInfo;
	
		EventHandlerPluginBase() { setPluginInfo(); }
		virtual void setPluginInfo() = 0;

		virtual	bool setup() = 0;
		virtual	bool preprocess() = 0;
		virtual	bool process() = 0;
		virtual	bool postprocess() = 0;

		virtual ~EventHandlerPluginBase(){}

		const EventHandlerPluginInfo& getPluginInfo() const { return info_; }

	protected:
		EventHandlerPluginInfo	info_;
};
