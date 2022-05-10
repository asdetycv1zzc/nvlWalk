#include<iostream>
#include<vector>
#include<fstream>
#include<Windows.h>
#include"nvlFile.h"
#include"str_plugin.h"
#include <set>

using namespace std;

void nvlFile::_ReadInit()
{
	setlocale(LC_CTYPE, "");
	fstream _f(_init_address, ios::in | ios::binary);
	size_t i = 0;
	HANDLE handle = CreateFile(_init_address, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	_init_content.resize(GetFileSize(handle, NULL));
	while (!_f.eof())
	{
		_f.read((char*)&_init_content[i], sizeof(wchar_t));
		i++;
	}
	return;
}
vector<wstring> nvlFile::_SplitScriptLines(const wstring& _source)
{
	auto _size = _source.size();
	wstring _buffer;
	vector<wstring> _result;
	for (size_t i = 0; i < _size; i++)
	{
		if (_source[i] != L'\r' && _source[i] != L'\n')
		{
			_buffer.push_back(_source[i]);
			continue;
		}
		if (_source[i] == L'\r' || _source[i] == L'\n')
		{
			if (_buffer.size() > 0)
			{
				_result.push_back(_buffer);
				_buffer.clear();
			}
		}
	}
	return _result;
}
vector<wstring> nvlFile::_FindStorage(const vector<wstring>& _source)
{
	const wchar_t* _mark = L"storage=\"";
	auto _size = _source.size();
	auto _judge_size = wstring(_mark).size();
	vector<wstring> _result;
	for (size_t i = 0; i < _size; i++)
	{
		if (_source[i].size() < _judge_size) continue;
		if (_source[i].find(_mark) == wstring::npos) continue;
		auto _num = Count(_source[i], _mark);
		size_t _offset = 0;
		for (size_t j = 0; j < _num; j++)
		{
			auto _init_pos = _source[i].find(_mark, _offset);
			auto _end_pos = _source[i].find(L'\"', _init_pos + _judge_size + 1);
			auto _temp = _source[i].substr(_init_pos + _judge_size, _end_pos - _init_pos - _judge_size);
			Delete(_temp, L'\"');
			_result.push_back(_temp);
			_offset = _init_pos;
		}
	}
	set<wstring>s(_result.begin(), _result.end());
	_result.assign(s.begin(), s.end());
	return _result;
}
nvlFile::nvlFile(LPCWSTR _address)
{
	_init_address = _address;
	_ReadInit();
	_splited_init_content = _SplitScriptLines(_init_content);
	_file_table.pushIn(_FindStorage(_splited_init_content));
	
}