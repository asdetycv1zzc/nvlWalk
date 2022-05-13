#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<set>

template<typename T>
void DeleteRepeat(std::vector<T>& _source);

template<typename T>
void DeleteEmpty(std::vector<T>& _source);
template<>
void DeleteEmpty(std::vector<std::wstring>& _source);

template<typename T>
std::wstring VectorToWstring(const std::vector<T>& _source, const wchar_t* _splitFlag);

template<>
std::wstring VectorToWstring(const std::vector<std::wstring>& _source, const wchar_t* _splitFlag);
