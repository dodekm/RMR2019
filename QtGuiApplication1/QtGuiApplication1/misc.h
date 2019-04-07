#pragma once
#include <vector>
#include <string>

std::vector<std::string> string_split_delim(const std::string& str, const std::string& delim);


float vector_mean(std::vector<float>& v);
float vector_standard_deviation(std::vector<float>& v);

template <typename T>int sign(T x)
{
	return (((x) < 0) ? -1 : ((x) > 0));
}

