#include<iostream>
#include<vector>
#include<string>
#include"vec_plugin.h"
using namespace std;

template<typename T>
void DeleteRepeat(vector<T>& _source)
{
	set<T>_temp(_source.begin(), _source.end());
	_source.clear();
	_source.assign(_temp.begin(), _temp.end());
}
template<typename T>
wstring VectorToWstring(const vector<T>& _source, const wchar_t* _splitFlag)
{
	auto _size = _source.size();
	wchar_t* _buffer;
	for (size_t i = 0; i < _size; i++)
	{
		swprintf(_buffer, L"%s%s", _source[i],_splitFlag);
	}
	wstring _result(_buffer);
	_result.pop_back();
	return _result;
}
template<>
wstring VectorToWstring(const vector<wstring>& _source, const wchar_t* _splitFlag)
{
	auto _size = _source.size();
	wstring _result;
	for (size_t i = 0; i < _size; i++)
	{
		_result += _source[i] + _splitFlag;
	}
	_result.pop_back();
	return _result;
}