#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<Windows.h>
#include<algorithm>

struct FileTable
{
	std::vector<std::wstring> _filenames;
	std::map<std::wstring, bool> _walked;
	bool isWalked(size_t _a)
	{
		return _walked[_filenames[_a]];
	}
	bool isWalked(const std::wstring& _Val)
	{
		auto _pos = find(_filenames.begin(), _filenames.end(), _Val);
		if (_pos == _filenames.end())
			return false;
		return _walked[_filenames[_pos - _filenames.begin()]];
	}
	void setWalked(size_t _a)
	{
		_walked[_filenames[_a]] = true;
	}
	void deleteRepeat()
	{
		std::set<std::wstring>_temp(_filenames.begin(), _filenames.end());
		_filenames.clear();
		_filenames.assign(_temp.begin(), _temp.end());
	}
	void pushIn(const std::vector < std::wstring > _source)
	{
		_filenames.insert(_filenames.end(), _source.begin(), _source.end());
		auto _size = _source.size();
		for (size_t i = 0; i < _size; i++)
		{
			_walked[_filenames[i]] = false;
		}
	}
};
class nvlFile
{
private:
	LPCWSTR _init_address = NULL;
	std::wstring _init_content;
	std::vector<std::wstring> _splited_init_content;
	FileTable _file_table;

	std::vector<LPCWSTR> _list;

	void _ReadInit();
	std::vector<std::wstring> _SplitScriptLines(const std::wstring& _source);
	std::vector<std::wstring> _FindStorage(const std::vector<std::wstring>& _source);
public:
	nvlFile(LPCWSTR _address);
};