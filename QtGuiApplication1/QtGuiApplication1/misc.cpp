#include "misc.h"

std::vector<std::string> string_split_delim(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;

	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}



float vector_mean(std::vector<float>& v)
{
	float mean=0;
	for (int i = 0; i < v.size(); i++)
	{
		mean += v[i];
	}
	mean /= v.size();
	return mean;
}
float vector_standard_deviation(std::vector<float>& v)
{
	float mean = vector_mean(v);
	float deviance;
	float sum = 0;
	for (int i = 0; i < v.size(); i++)
	{
		sum += (v[i] - mean)*(v[i] - mean);
	}
	deviance = sqrt(sum / (v.size() - 1));

	return deviance;
}