#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>
#include<Windows.h>
#include"nvlFile.h"
#include"str_plugin.h"
#include <set>

using namespace std;

void nvlFile::_FindBaseInit()
{
	auto _size = wstrlen(_init_address);
	size_t i = _size;
	while (*(_init_address + i) != L'\\')
	{
		i--;
	}
	size_t _pos = i;
	_base_address = new wchar_t[_pos + 2];
	for (i = 0; i <= _pos + 1; i++)
	{
		_base_address[i] = _init_address[i];
	}
	_base_address[_pos + 1] = L'\0';
	return;
}
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
	_file_content_table[_NormalizeFileName(_init_address)] = _SplitScriptLines(_init_content);
	_file_table.setWalked(_NormalizeFileName(_init_address));
	CloseHandle(handle);
	return;
}
void nvlFile::_ReadFile(const wstring& _address)
{
	if (_file_table.isWalked(_address))return;
	setlocale(LC_CTYPE, "");
	fstream _f(_address, ios::in | ios::binary);
	size_t i = 0;
	HANDLE handle = CreateFile(_init_address, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS) return;
	wstring _buffer;
	_buffer.resize(GetFileSize(handle, NULL));
	while (!_f.eof())
	{
		_f.read((char*)&_buffer[i], sizeof(wchar_t));
		i++;
	}
	_file_content_table[_NormalizeFileName(_address)] = _SplitScriptLines(_buffer);
	_file_table.setWalked(_NormalizeFileName(_address));
	CloseHandle(handle);
}

vector<wstring> nvlFile::_FindKSFile()
{
	vector<wstring> _result;
	auto _size = _file_table._filenames.size();
	for (size_t i = 0; i < _size; i++)
	{
		if (_file_table._filenames[i].find(L".ks") != wstring::npos && !_file_table.isWalked(_file_table._filenames[i]))
		{
			_result.push_back(_file_table._filenames[i]);
		}
	}
	return _result;
}
bool nvlFile::_IsKSFile(const wstring& _source)
{
	if (_source.find(L".ks") != wstring::npos)
		return true;
	else
		return false;
}
void nvlFile::_ReadKSFile()
{
	FileTable _tmpTable = _file_table;
	if (_tmpTable.isKSAllWalked()) 
		return;
	_tmpTable.deleteRepeat();
	auto _size = _tmpTable._filenames.size();
	for (size_t i = 0; i < _size; i++)
	{
		_tmpTable = _file_table;
		if (_tmpTable.isKSAllWalked())
			return;
		_tmpTable.deleteRepeat();
		if (_IsKSFile(_tmpTable._filenames[i]))
		{
			if (_ExistKSFile(_FulfillFileName(_tmpTable._filenames[i])))
			{
				_ReadFile(_FulfillFileName(_tmpTable._filenames[i]));
				_file_table.pushIn(_FindStorage(_file_content_table[_tmpTable._filenames[i]]));
				_file_table.deleteRepeat();
			}
			else
			{
				_file_table.setWalked(_NormalizeFileName(_tmpTable._filenames[i]));
			}
		}
	}
	_ReadKSFile();
}
bool nvlFile::_ExistKSFile(const std::wstring& _address)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(_address.c_str(), &fd);
	BOOL bFilter = (FALSE == FALSE) ? TRUE : fd.dwFileAttributes & FALSE;
	BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;
	FindClose(hFind);
	return RetValue;

}

wstring nvlFile::_FulfillFileName(const wstring& _source)
{
	if (!_base_address) return nullptr;
	return _base_address + _source;
}
wstring nvlFile::_NormalizeFileName(const wstring& _source)
{
	if (_source.find(L'\\') == wstring::npos) return _source;
	auto _beginPos = _source.find_last_of(L'\\');
	return _source.substr(_beginPos + 1);
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
	_init_address = const_cast<WCHAR*>(_address);
	_FindBaseInit();
	_ReadInit();
	_splited_init_content = _SplitScriptLines(_init_content);
	_file_table.pushIn(_FindStorage(_splited_init_content));
	_ReadKSFile();
}