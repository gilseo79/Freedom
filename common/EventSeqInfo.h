#pragma once
#include "CommonTypeDefs.h"

class EventSeqInfo 
{
	public:
		EventSeqInfo():seq_(0){}
		inline EventSeqType getSeq() const { return seq_; }
		inline void			incSeq(short unit = 1) { seq_ += unit; }
		inline void			setSeq(EventSeqType seq) { seq_ = seq; }

	protected:
		EventSeqType 	seq_;
};

