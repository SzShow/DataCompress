
template<class T, class Code>
bool LZ77Decode(const std::vector<Code>& packed, std::vecor<T>* data,
								size_t indexBit, typename std::vector<T>::size_type dataCount);
