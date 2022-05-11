#pragma once
#include<iostream>
#include<vector>
#include<string>

template<typename T>
void DeleteRepeat(std::vector<T>& _source);

template<typename T>
std::wstring VectorToWstring(const std::vector<T>& _source, const wchar_t* _splitFlag);

template<>
std::wstring VectorToWstring(const std::vector<std::wstring>& _source, const wchar_t* _splitFlag);

template<>
std::wstring VectorToWstring(const std::vector<std::string>& _source, const wchar_t* _splitFlag);