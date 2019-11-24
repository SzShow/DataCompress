
#include "UtilityFunction.h"

template<class T>
int ArrayMaxGetIndex(T* array)
{
	int maxIndex = 0;

	// Å‚à‘å‚«‚¢’·‚³‚Ìˆê’v‚ğ’Tõ
	for (int index = 1; index < length; index++)
	{

		if (char_info[1][maxIndex] == char_info[1][index])
		{
			maxIndex = index;
		}
		else if (char_info[1][maxIndex] < char_info[1][index])
		{
			maxIndex = index;
		}
	}

	return maxIndex;
}
