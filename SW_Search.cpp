
#include "SW_Search.h"

template<class In, class Code>
void SW_Search<In, Code>::operator()(In dataStart, In dataEnd, Code winSize, Code maxCountSize, Code* index, Code* count)
{
	typedef typename std::iterator_traits<In>::difference_type difference_type;

	*index = *count = Code();

	In winStart = dataStart;
	std::advance(winStart, -static_cast<difference_type>(winSize));

	for(; winStart != dataStart; **winStart)
	{
		In winP = winStart;

		for (In dataP = dataStart; *winP == *dataP ; ++dataP)
		{
			if (dataP == dataEnd) break;
			++winP;
		}

		size_t sz = std::distance(winStart, winP);
		if (sz > *count)
		{
			*index = std::distance(winStart, dataStart);
			*count = sz;
		}

		if (*count > maxCountSize)
		{
			*count = maxCountSize;
			break;
		}

	}

}

template<class In, class Code>
SlidingWindow< In,  Code>::SlidingWindow(In s, In e, 
	Code maxWinSize, Code maxCountSize):
	dataStart_(s), dataEnd_(e), curIndex_(s), 
	winSize_(0), maxWinSize_(maxWinSize), 
	maxCountSize_(maxCountSize)
{
	if (std::distance(s, e) < maxWinSize_)
		maxWinSize_ = std::distance(s, e);
}

template<class In, class Code, class Container>
SlidingWindow<In, Code, Container>::SlidingWindow(
	const Container& container, Code maxWinSize, Code maxCountSize):
	dataStart_(container.begin()), dataEnd_(container.end()), curIndex_(container.begin()),
	winSize_(0), maxWinSize_(maxWinSize), maxCountSize_(maxCountSize)
	{

	}

template<class In, class Code, template<class, class> class SW_Search>
	void SlidingWindow<In, Code, SW_Search>::search(SW_Search<In, Code>& search,
	Code* index, Code* count) const
	{
		search(curIndex_, dataEnd_, winSize_, maxCountSize_, index, count);
	}

template<class In, class Code>
value_type SlidingWindow<In, Code>::value() const
{
	return(curIndex_ == dataEnd_);
}

template<class In, class Code>
void SlidingWindow<In, Code>::inc (size_t n)
{
	if (static_cast<size_t>(std::distance(curIndex_, dataEnd_)) < n)
	{
		curIndex_ = dataEnd_;
	}
	else
	{
		std::advance(curIndex_, n);
	}

	if(winSize_ < maxWinSize_)
	{
		winSize_ += n;
		if(winSize_ > maxWinSize_)
		{
			winSize_ = maxWinSize_;
		}
	}
}


