#pragma once

template<class In, class Code>
struct SW_Search
{
	void operator(In dataStart, In dataEnd, Code winSize, Code maxCountSize, Code* index, Code* count);
};

template<class In, class Code>
void SW_Search<In, Code>::operator()(In dataStart, In dataEnd, Code winSize, Code maxCountSize, Code* index, Code* count);

template<class In, class Code>
class SlidingWindow
{
private:
	In dataStart_;
	In dataEnd_;
	In curIndex_;
	Code winSize_;
	Code maxWinSize_;
	Code maxCountSize_;

public:
	SlidingWindow(In s, In e, Code maxWinSize, Code maxCountSize)
		:dataStart_(s), dataEnd_(e), curIndex_(s),
		winSize_(0), maxWinSize_(maxWinSize), maxCountSize_(maxCountSize);

	template<class Container>
		SlidingWindow(const Container& container, Code maxWinSize, Code maxCountSize)
			: dataStart_(container.begin()), dataEnd_(container.end()), curIndex_(container.begin()),
			winSize_(0), maxWinSize_(maxWinSize_), maxCountSize_(maxCountSize_);

	template<template<class, class> class SW_Search>
		void search(SW_Search<In, Code>& search, Code* index, Code* count);

	value_type value();

	bool empty();

	void inc(size_t n);
};

template<class In, class Code>
void SlidingWindow<In, Code>::inc(size_t n);
