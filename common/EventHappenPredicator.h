#pragma once

class EventHappenPredicator 
{
	public:
		virtual bool predicate() const {
			bool ret = true;

			for (IpcEventWatchInfo* watchInfo:eventWatchInfoVec_) {

				if (!watchInfo->isDiffEventSeq()) {
					ret = false;
					break;
				}
			}

			return ret;
		}

		bool addIpcEventWatchInfo(EventInfoObjMgr& objMgr, unsigned int tarEventId, const char* watcherAlias = nullptr) {
			IpcEventWatchInfo* info = new IpcEventWatchInfo(watcherAlias, objMgr, tarEventId);
			if (!info->isBound()) {
				delete info;
				return false;
			}
			
			eventWatchInfoVec_.push_back(info);
			return true;
		}

		bool addIpcEventWatchInfo(EventInfoObjMgr& objMgr, const std::string& tarEventAlias, const char* watcherAlias = nullptr) {
			return addIpcEventWatchInfo(objMgr, objMgr.getId(tarEventAlias), watcherAlias);
		}

		void updateWatchInfoEventSeqs() {
			for (IpcEventWatchInfo* eventWatchInfo:eventWatchInfoVec_) {
				eventWatchInfo->updatePrevEventSeq();
				eventWatchInfo->incEventSeq();
			}
		}

	private:
		std::vector<IpcEventWatchInfo*>	eventWatchInfoVec_;
};


