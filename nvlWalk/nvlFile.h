#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<Windows.h>
#include<algorithm>
#include<math.h>
#include"vec_plugin.h"
#include"MD5.h"

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
			if (!_walked[_source[i]])
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
		if (!_tmpTable.size() || !_filenames.size()) return;
		for (size_t i = 0; i < _size; i++)
		{
			if (_tmpTable[i] != _filenames[i])
				std::wcout << i << L":\n" << _tmpTable[i] << _filenames[i];
		}
	}
};
struct tTJSArray
{
	std::wstring _source;
	std::vector<tTJSArray> _sub_array;
	std::vector<std::wstring> _elements;
	void deleteRepeat()
	{
		//std::set<tTJSArray>_temp_sub_array(_sub_array.begin(), _sub_array.end());
		std::set<std::wstring>_temp_elements(_elements.begin(), _elements.end());
		//_sub_array.clear();
		_elements.clear();
		//_sub_array.assign(_temp_sub_array.begin(), _temp_sub_array.end());
		_elements.assign(_temp_elements.begin(), _temp_elements.end());
	}
	void operator+=(const tTJSArray& _b)
	{
		_source += L'\n' + _b._source;
		_sub_array.insert(_sub_array.end(), _b._sub_array.begin(), _b._sub_array.end());
		_elements.insert(_elements.end(), _b._elements.begin(), _b._elements.end());
		//deleteRepeat();
		//DeleteRepeat(_sub_array);
		//DeleteRepeat(_elements);
	}
};

class nvlFile
{
private:
	const size_t MAX_FILE_SIZE = 20 * 1024 * 1024;
	wchar_t* _base_address;
	wchar_t* _init_filename;
	std::wstring _init_content;
	std::vector<std::wstring> _splited_init_content;

	std::map<std::wstring, std::vector<std::wstring> > _file_content_table;
	std::vector<std::wstring> _missing_filenames;
	FileTable _file_table;

	tTJSArray NULLTJSArray;

	//std::vector<LPCWSTR> _list;

	void _FindBaseInit();
	void _ReadInit();
	void _ReadFile(const std::wstring& _address, void* _Dest);
	void _ReadFile(const std::wstring& _address);

	std::vector<std::wstring> _FindKSFile();
	bool _IsKSFile(const std::wstring& _address);
	std::wstring _GuessKSFile(const std::wstring& _address);
	void _ReadKSFile();
	bool _ExistKSFile(const std::wstring& _address);
	void _RebuildKSFileList(const std::wstring& _init_address_regex);
	void _RenameKSFile(const std::wstring& _init_address);

	tTJSArray _SortArrayFromTJSFile(const std::wstring& _address);
	tTJSArray _SortArrayFromTJS(const std::wstring& _source);

	std::wstring _CalculateMD5(const std::wstring& _address);
	void _MatchMD5(const std::wstring& _address1, const std::wstring& _address2);

	std::wstring _NormalizeFileName(const std::wstring& _address);
	std::wstring _FulfillFileName(const std::wstring& _source);
	std::vector<std::wstring> _SplitScriptLines(const std::wstring& _source);
	std::vector<std::wstring> _FindStorage(const std::vector<std::wstring>& _source);
public:
	std::map<std::wstring, std::wstring> _hash_filename_table;
	std::map<std::wstring, std::wstring> _MD5_known_filename_table;
	std::map<std::wstring, std::wstring> _MD5_unknown_filename_table;

	void MatchMD5(const std::wstring& _address1, const std::wstring& _address2);
	void RenameKSFile(const std::wstring& _init_address);
	std::wstring GuessKSFile(const std::wstring& _address);
	void RebuildKSFileList(const std::wstring& _init_address);
	tTJSArray SortArrayFromTJSFile(const std::wstring& _address);
	tTJSArray SortArrayFromTJS(const std::wstring& _source);
	nvlFile(LPCWSTR _address);
};