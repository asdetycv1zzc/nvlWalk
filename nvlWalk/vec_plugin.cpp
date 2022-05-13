#include<iostream>
#include<vector>
#include<string>
#include"vec_plugin.h"
#include<set>
using namespace std;

template<typename T>
void DeleteRepeat(vector<T>& _source)
{
	set<T> _temp(_source.begin(), _source.end());
	_source.clear();
	_source.assign(_temp.begin(), _temp.end());
}
template<>
void DeleteRepeat(vector<wstring>& _source)
{
	set<wstring> _temp(_source.begin(), _source.end());
	_source.clear();
	_source.assign(_temp.begin(), _temp.end());
}
template<typename T>
void DeleteEmpty(vector<T>& _source)
{
	for (auto i = _source.begin(); i != _source.end();)
	{
		if (!(*(int*)(*i)))
			_source.erase(i);
		else
			i++;
	}
}
template<>
void DeleteEmpty(vector<wstring>& _source)
{
	for (auto i = _source.begin(); i != _source.end();)
	{
		if ((*i).empty())
			_source.erase(i);
		else
			i++;
	}
}
template<typename T>
wstring VectorToWstring(const vector<T>& _source, const wchar_t* _splitFlag)
{
	auto _size = _source.size();
	wchar_t* _buffer;
	for (size_t i = 0; i < _size; i++)
	{
		swprintf(_buffer, L"%s%s", _source[i], _splitFlag);
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
	if (_result.empty()) return _result;
	_result.pop_back();
	return _result;
}
string WString2String(const wstring& ws)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = ws.c_str();
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	char* chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	std::string strResult = chDest;
	delete[]chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}
// string => wstring
wstring String2WString(const string& s)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const char* chSrc = s.c_str();
	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
	wchar_t* wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[]wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}