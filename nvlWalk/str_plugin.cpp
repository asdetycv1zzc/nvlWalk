#include<iostream>
#include<string>
#include"str_plugin.h"
using namespace std;

size_t Count(const wstring& _source,const wstring& _Val)
{
	size_t _result = 0;
	if (_source.find(_Val) == wstring::npos) return _result;
	auto _init_pos = 0;
	while (_source.find(_Val,_init_pos) != wstring::npos)
	{
		_result++;
		_init_pos = _source.find(_Val, _init_pos) + 1;
	}
	return _result;
}
void Delete(wstring& _source, const wchar_t _Val)
{
	if (_source.find(_Val) == wstring::npos) return;
	for (auto i = _source.begin(); i != _source.end();)
	{
		if (*i == _Val)
			_source.erase(i);
		else 
			i++;
	}
	return;
}