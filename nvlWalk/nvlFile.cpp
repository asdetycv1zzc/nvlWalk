#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cstring>
#include<Windows.h>
#include"nvlFile.h"
#include<io.h>
#include"str_plugin.h"
#include"vec_plugin.h"
#include<set>

#pragma warning( push )
#pragma warning( disable : 4700 )
#pragma warning( pop )

using namespace std;

void nvlFile::_FindBaseInit()
{
	auto _size = wstrlen(_init_filename);
	size_t i = _size;
	while (*(_init_filename + i) != L'\\')
	{
		i--;
	}
	size_t _pos = i;
	_base_address = new wchar_t[_pos + 2];
	for (i = 0; i <= _pos + 1; i++)
	{
		_base_address[i] = _init_filename[i];
	}
	_base_address[_pos + 1] = L'\0';
	return;
}
void nvlFile::_ReadInit()
{
	setlocale(LC_CTYPE, "");
	HANDLE handle = CreateFile(_init_filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	_init_content.resize(GetFileSize(handle, NULL));
	_ReadFile(wstring(_init_filename), const_cast<wchar_t*>(_init_content.c_str()));
	_file_content_table[_NormalizeFileName(_init_filename)] = _SplitScriptLines(_init_content);
	_file_table.setWalked(_NormalizeFileName(_init_filename));
	CloseHandle(handle);
}
void nvlFile::_ReadFile(const wstring& _address)
{
	if (_file_table.isWalked(_address))return;
	setlocale(LC_CTYPE, "");
	fstream _f(_address, ios::in | ios::binary);
	HANDLE handle = CreateFile(_address.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS) return;
	wstring _buffer;
	auto _size = GetFileSize(handle, NULL);
	_buffer.resize(_size);
	_f.read((char*)_buffer.c_str(), sizeof(wchar_t) * _size);
	_file_content_table[_NormalizeFileName(_address)] = _SplitScriptLines(_buffer);
	_file_table.setWalked(_NormalizeFileName(_address));
	CloseHandle(handle);
}
void nvlFile::_ReadFile(const wstring& _address, void* _Dest)
{
	if (_file_table.isWalked(_address))return;
	setlocale(LC_CTYPE, "");
	fstream _f(_address, ios::in | ios::binary);
	HANDLE handle = CreateFile(_address.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS) return;
	auto _size = GetFileSize(handle, NULL);
	//_Dest = new wchar_t[_size + 1];
	//wchar_t* _buffer = new wchar_t[_size + 1];
	_f.read((char*)_Dest, sizeof(wchar_t) * _size);
	//*((wchar_t*)_Dest + _size + 1) = L'\0';
	//wmemcpy((wchar_t*)_Dest, _buffer, _size + 1);
	memset((wchar_t*)_Dest + _size / 2, L'\0', 4); // Divide 2 For Unknown Reason, but just works
	//memcpy(_Dest, _buffer, _size + 1);
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
wstring nvlFile::_GuessKSFile(const wstring& _address)
{
	if (_address.find(L".") != wstring::npos) return L"";
	wchar_t* _buffer = new wchar_t[MAX_FILE_SIZE / 10];
	_ReadFile(_address, _buffer);
	wstring _temp(_buffer);
	delete[] _buffer;
	_buffer = nullptr;

	auto _content = _SplitScriptLines(_temp);
	if (_content.size() < 1) return L"";
	if (_content[1].find(L".ks") != wstring::npos)
		return _content[1].substr(1);
	return L"";
}
void nvlFile::_ReadKSFile()
{
	FileTable _tmpTable = _file_table;
	_tmpTable.deleteRepeat();
	if (_tmpTable.isKSAllWalked())
		return;
	auto _size = _tmpTable._filenames.size();
	for (size_t i = 0; i < _size; i++)
	{
		_tmpTable = _file_table;
		_tmpTable.deleteRepeat();
		if (_tmpTable.isKSAllWalked())
			return;
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
				_missing_filenames.push_back(_NormalizeFileName(_tmpTable._filenames[i]));
				DeleteRepeat(_missing_filenames);
			}
		}
	}
	_ReadKSFile();
}
void nvlFile::_RebuildKSFileList(const wstring& _init_address_regex)
{
	auto _base_address = _init_address_regex.substr(0, _init_address_regex.size() - 1);
	struct _finddata_t _fileinfo;
	long handle = _findfirst(WString2String(_init_address_regex).c_str(), &_fileinfo);
	_findnext(handle, &_fileinfo);
	_findnext(handle, &_fileinfo);
	do
	{
		auto _result = _GuessKSFile(_base_address + String2WString(_fileinfo.name));
		if (!_result.empty())
		{
			_hash_filename_table[_NormalizeFileName(String2WString(_fileinfo.name))] = _result;
#if true
			wprintf(L"%s -> %s\n", _NormalizeFileName(String2WString(_fileinfo.name)).c_str(), _result.c_str());
#endif
		}

	} while (!_findnext(handle, &_fileinfo));
}
void nvlFile::_RenameKSFile(const wstring& _init_address)
{
	auto _init_address_regex = _init_address + L'*';
	_RebuildKSFileList(_init_address_regex);
	for (auto i = _hash_filename_table.begin(); i != _hash_filename_table.end();)
	{
		auto _source_name = _init_address + (*i).first;
		auto _dest_name = _init_address + (*i).second;
		MoveFile(_source_name.c_str(), _dest_name.c_str());
		i++;
	}
}
bool nvlFile::_ExistKSFile(const wstring& _address)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(_address.c_str(), &fd);
	BOOL bFilter = (FALSE == FALSE) ? TRUE : fd.dwFileAttributes & FALSE;
	BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;
	FindClose(hFind);
	return RetValue;

}

wstring nvlFile::_CalculateMD5(const wstring& _address)
{
	wchar_t* _buffer = new wchar_t[MAX_FILE_SIZE];
	_ReadFile(_address, _buffer);
	wstring _content(_buffer);
	delete[] _buffer;
	auto _result = GetDataMD5(_content);
	return String2WString(_result);
}
void nvlFile::_MatchMD5(const wstring& _address1, const wstring& _address2)
{
	auto _base_address1 = _address1;
	auto _base_address2 = _address2;
	struct _finddata_t _fileinfo;
	long handle = _findfirst(WString2String(_address1 + L'*').c_str(), &_fileinfo);
	_findnext(handle, &_fileinfo);
	_findnext(handle, &_fileinfo);
	do
	{
		auto _result = _CalculateMD5(_base_address1 + String2WString(_fileinfo.name));
		if (!_result.empty())
		{
			//_hash_filename_table[_NormalizeFileName(String2WString(_fileinfo.name))] = _result;
			_MD5_known_filename_table[_result] = _NormalizeFileName(String2WString(_fileinfo.name));
#if true
			wprintf(L"%s -> %s\n", _NormalizeFileName(String2WString(_fileinfo.name)).c_str(), _result.c_str());
#endif
		}

	} while (!_findnext(handle, &_fileinfo));
	handle = _findfirst(WString2String(_address2 + L'*').c_str(), &_fileinfo);
	_findnext(handle, &_fileinfo);
	_findnext(handle, &_fileinfo);
	do
	{
		if (_fileinfo.attrib == _A_SUBDIR)continue;
		auto _result = _CalculateMD5(_base_address2 + String2WString(_fileinfo.name));
		if (!_result.empty())
		{
			//_hash_filename_table[_NormalizeFileName(String2WString(_fileinfo.name))] = _result;
			_MD5_unknown_filename_table[_NormalizeFileName(String2WString(_fileinfo.name))] = _result;
#if true
			wprintf(L"%s -> %s\n", _NormalizeFileName(String2WString(_fileinfo.name)).c_str(), _result.c_str());
#endif
		}

	} while (!_findnext(handle, &_fileinfo));
	return;
}

tTJSArray nvlFile::_SortArrayFromTJSFile(const wstring& _address)
{
	tTJSArray _result;
	_ReadFile(_address);
	auto& _content = _file_content_table[_NormalizeFileName(_address)];
	auto _size = _content.size();
	for (size_t i = 0; i < _size; i++)
	{
		_result._sub_array.push_back(_SortArrayFromTJS(_content[i]));
		_result._source += _content[i];
	}
	DeleteEmpty(_result._elements);
	return _result;
}
tTJSArray nvlFile::_SortArrayFromTJS(const wstring& _source)
{
	tTJSArray _result;
	size_t _sub_array_count = 0;
	_result._source = _source;
	const wstring _mark = L" , ";
	if (_source.size() < 2) return NULLTJSArray;
	if (_source.find(L'[') == wstring::npos || _source.find(L']') == wstring::npos) return NULLTJSArray;
	_sub_array_count = Count(_source, L'[');
	if (Count(_source, L'[') != Count(_source, L']')) return NULLTJSArray;
	auto _init_pos = _source.find_first_of(L'[');
	size_t _end_pos = 0;
	for (size_t i = 0; i < _sub_array_count; i++)
	{
		if (i == 1 || i == _sub_array_count - 1)
		{
			_init_pos = _source.find_first_of(L'[', _end_pos);
			_end_pos = _source.find_first_of(L'[', _init_pos + 1);
			if (_end_pos == wstring::npos)  //特判，即只有一个子数组
				_end_pos = _source.find_first_of(L']', _init_pos);
			auto _array = _source.substr(_init_pos + 1, _end_pos - _init_pos - 1);
			auto _elements = splitwstr(_array, _mark);
			_result._elements.insert(_result._elements.end(), _elements.begin(), _elements.end());
			_end_pos = _init_pos;
			continue;
		}
		_init_pos = _source.find_first_of(L'[', _end_pos);
		_end_pos = _source.find_first_of(L'[', _init_pos);
		if (_end_pos == wstring::npos)  //特判，即只有一个子数组
			_end_pos = _source.find_first_of(L']', _init_pos);
		auto _array = _source.substr(_init_pos, _end_pos - _init_pos);
		_result += _SortArrayFromTJS(_array);
	}
	return _result;
}

wstring nvlFile::_FulfillFileName(const wstring& _source)
{
	if (!_base_address) return nullptr;
	if (_source.find(_base_address) != wstring::npos) return _source;
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

void nvlFile::MatchMD5(const wstring& _address1, const wstring& _address2)
{
	return _MatchMD5(_address1, _address2);
}
void nvlFile::RenameKSFile(const wstring& _init_address)
{
	return _RenameKSFile(_init_address);
}
wstring nvlFile::GuessKSFile(const wstring& _address)
{
	return _GuessKSFile(_address);
}
void nvlFile::RebuildKSFileList(const wstring& _init_address)
{
	return _RebuildKSFileList(_init_address);
}
tTJSArray nvlFile::SortArrayFromTJSFile(const wstring& _address)
{
	return _SortArrayFromTJSFile(_address);
}
tTJSArray nvlFile::SortArrayFromTJS(const wstring& _source)
{
	return SortArrayFromTJS(_source);
}
nvlFile::nvlFile(LPCWSTR _address)
{
	_init_filename = const_cast<WCHAR*>(_address);
	_FindBaseInit();
	_ReadInit();
	_splited_init_content = _SplitScriptLines(_init_content);
	_file_table.pushIn(_FindStorage(_splited_init_content));
	_ReadKSFile();
	wcout << VectorToWstring<wstring>(_missing_filenames, L"\n");
}