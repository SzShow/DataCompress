

template<class T, class Code, template<class, class> class SW_SearchType>
bool LZ77Encode(const std::vector<T>& data,
								std::vector<code>* packed,
								size_t indexBit,
								SW_SearchType<typename std::vector<T>::const_iterator, code>& swSearch);


