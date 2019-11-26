#include "LZ77Decompresser.h"

using namespace std;

void LZ77Decompresser::SplitCompressed()
{
	stringstream ss(_input);
	string tok;

	while (getline(ss, tok, ' '))
	{
		_splited.push_back(tok);
	}
}

int LZ77Decompresser::Decompress(string* output)
{

	SplitCompressed();

	for (unsigned int i = 0; i < _splited.size(); ++i)
	{
		if (_splited[i].size() == 0)
		{
			continue;
		}

		vector<string> ss_input = split(_splited[i], ',');

		if (_splited[i][4] == ',')
		{
			ss_input[2] = ',';
		}
		else if (ss_input.size() == 2)
		{
			ss_input.push_back(" ");
		}

		//if ((int)ss_input)

		int p = stoi(ss_input[0]), l = stoi(ss_input[1]);
		string ch;

		if (ss_input[2][0] == '0')
		{
			ch = ' ';
		}
		else
		{
			ch = ss_input[2];
		}

		if (p != 0)
		{
			int result_len = (int)result.length();
			for (int x = 0; x < l; x++)
			{
				result += result[result_len - p + x];
			}
		}

		if (ch[0] != '0' || ch[0] != NULL)
		{
			result += ch;
		}


	return result;
}
