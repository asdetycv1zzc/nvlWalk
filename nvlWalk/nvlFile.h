#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<Windows.h>
#include<algorithm>
#include<math.h>

struct FileTable
{
	std::vector<std::wstring> _filenames;
	std::map<std::wstring, bool> _walked; //All filenames are normalized
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
	void setWalked(const std::wstring& _Val)
	{
		_walked[_Val] = true;
	}
	void deleteRepeat()
	{
		std::set<std::wstring>_temp(_filenames.begin(), _filenames.end());
		_filenames.clear();
		_filenames.assign(_temp.begin(), _temp.end());
	}
	void pushIn(const std::vector < std::wstring > _source)
	{
		deleteRepeat();
		_filenames.insert(_filenames.end(), _source.begin(), _source.end());
		auto _size = _source.size();
		for (size_t i = 0; i < _size; i++)
		{
			if(!_walked[_source[i]])
				_walked[_source[i]] = false;
		}
		deleteRepeat();
		Validate();
	}
	bool isKSAllWalked()
	{
		for (auto i = _walked.begin(); i != _walked.end();)
		{
			if ((*i).first.find(L".ks") != std::wstring::npos)
			{
				if (!(*i).second)
					return false;
			}
			i++;
		}
		return true;
	}
	bool isAllWalked()
	{
		for (auto i = _walked.begin(); i != _walked.end();)
		{
			if (!(*i).second) return false;
			i++;
		}
		return true;
	}
	void Validate()
	{
		std::vector<std::wstring> _tmpTable;
		for (auto i = _walked.begin(); i != _walked.end();)
		{
			_tmpTable.push_back((*i).first);
			i++;
		}
		if (_tmpTable.size() == _filenames.size()) return;
		auto _size = max(_tmpTable.size(), _filenames.size());
		for (size_t i = 0; i < _size; i++)
		{
			if (_tmpTable[i] != _filenames[i])
				std::wcout << i << L":\n" << _tmpTable[i] << _filenames[i];
		}
	}
};
class nvlFile
{
private:
	wchar_t* _base_address;
	wchar_t* _init_filename;
	std::wstring _init_content;
	std::vector<std::wstring> _splited_init_content;

	std::map<std::wstring, std::vector<std::wstring> > _file_content_table;
	std::vector<std::wstring> _missing_filenames;
	FileTable _file_table;

	//std::vector<LPCWSTR> _list;

	void _FindBaseInit();
	void _ReadInit();
	void _ReadFile(const std::wstring& _address);

	std::vector<std::wstring> _FindKSFile();
	bool _IsKSFile(const std::wstring& _address);
	void _ReadKSFile();
	bool _ExistKSFile(const std::wstring& _address);

	std::vector<std::wstring> _ReadTJSFile(const std::wstring& _address);

	std::wstring _NormalizeFileName(const std::wstring& _address);
	std::wstring _FulfillFileName(const std::wstring& _source);
	std::vector<std::wstring> _SplitScriptLines(const std::wstring& _source);
	std::vector<std::wstring> _FindStorage(const std::vector<std::wstring>& _source);
public:
	nvlFile(LPCWSTR _address);
};