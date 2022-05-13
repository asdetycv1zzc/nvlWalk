#include<iostream>
#include<Windows.h>
#include"nvlFile.h"
#include<fstream>
#include "str_plugin.h"
using namespace std;

int main()
{
	nvlFile _test(L"N:\\REWITEMP\\Gaokao Love 100Days\\screenshot\\data\\title.ks");
	//auto _content = _test.SortArrayFromTJSFile(L"N:\\REWITEMP\\Gaokao Love 100Days\\screenshot\\data\\macro\\event_main.tjs");
	//auto _content = _test.GuessKSFile(wstring(L"N:\\REWITEMP\\Gaokao Love 100Days\\Extract\\Data\\00AA12747D3D9DE0B0B65ABE"));
	//_test.RebuildKSFileList(L"N:\\REWITEMP\\Gaokao Love 100Days\\Extract\\Data\\*");
	/*
	_test.RebuildKSFileList(L"N:\\REWITEMP\\Gaokao Love 100Days\\Extract\\Data\\*");
	wstring _result;
	for (auto i = _test._hash_filename_table.begin(); i != _test._hash_filename_table.end();)
	{
		_result += L"scenario/" + (*i).second + L"\r\n";
		_result += L"scenario_cht/" + (*i).second + L"\r\n";
		_result += L"scenario_eng/" + (*i).second + L"\r\n";
		_result += (*i).second + L"\r\n";
		i++;
	}
	fstream _f;
	_f.open(L"1.lst", ios::out | ios::binary);
	_f.write((char*)_result.c_str(), sizeof(wchar_t) * _result.size());
	_f.close();*/
	/*
	_test.MatchMD5(L"N:\\REWITEMP\\Gaokao Love 100Days\\Extract\\Data\\", L"E:\\TRANSPLANT_TEMP\\GK\\");
	map<wstring, wstring>_unknown_to_known;
	for (auto i = _test._MD5_unknown_filename_table.begin(); i != _test._MD5_unknown_filename_table.end();)
	{
		if (!_test._MD5_known_filename_table.at((*i).second).empty())
		{
			_unknown_to_known[(*i).first] = _test._MD5_known_filename_table[(*i).second];
		}
		i++;
	}*/
	/*
	for (auto i = _unknown_to_known.begin(); i != _unknown_to_known.end();)
	{
		wprintf(L"%s		->		%s\n", (*i).first.c_str(), (*i).second.c_str());
		i++;
	}*/
	HANDLE handle = CreateFile(L"1.lst", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS) return 1;
	wstring _buffer;
	auto _size = GetFileSize(handle, NULL);
	_buffer.resize(_size);
	fstream _f;
	_f.open(L"1.lst", ios::in | ios::binary);
	_f.read((char*)_buffer.c_str(), sizeof(wchar_t) * _size);
	_f.close();
	auto _temp = splitwstr(_buffer, L"\r\n");
	_size = _temp.size();
	for (size_t i = 0; i < _size; i++)
	{
		if(_temp[i].find(L'/') != wstring::npos)
			_temp[i] = _temp[i].substr(_temp[i].find_last_of(L'/') + 1);
	}
	DeleteRepeat(_temp);
	_size = _temp.size();
	wstring _result;
	_result.resize(20 * 1024 * 1024);
	for (size_t i = 0; i < _size; i++)
	{
		_result.append(_temp[i] + L"\r\n");
		_result.append(L"scenario_cht/" + _temp[i] + L"\r\n");
		_result.append(L"scenario_eng/" + _temp[i] + L"\r\n");
		_result.append(L"scenario/" + _temp[i] + L"\r\n");
		_result.append(L"ui/" + _temp[i] + L"\r\n");
		_result.append(L"ui_eng/" + _temp[i] + L"\r\n");
		_result.append(L"ui_cht/" + _temp[i] + L"\r\n");
		_result.append(L"macro/" + _temp[i] + L"\r\n");
		_result.append(L"macro_eng/" + _temp[i] + L"\r\n");
		_result.append(L"macro_cht/" + _temp[i] + L"\r\n");
		_result.append(L"face/" + _temp[i] + L"\r\n");
		_result.append(L"system/" + _temp[i] + L"\r\n");
		_result.append(L"kagplugin/" + _temp[i] + L"\r\n");
		_result.append(L"main/" + _temp[i] + L"\r\n");
		_result.append(L"sysscn/" + _temp[i] + L"\r\n");
		_result.append(L"others/" + _temp[i] + L"\r\n");
		_result.append(L"nvl/" + _temp[i] + L"\r\n");
		_result.append(L"rule/" + _temp[i] + L"\r\n");
		_result.append(L"init/" + _temp[i] + L"\r\n");
		_result.append(L"thum/" + _temp[i] + L"\r\n");
		_result.append(L"uipsd/" + _temp[i] + L"\r\n");
		_result.append(L"evimage/" + _temp[i] + L"\r\n");
	}
	return 0;
}