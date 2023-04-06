#pragma once

#ifndef UTIL_HEADER
#define UTIL_HEADER

#include <format>
#include <filesystem>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;
namespace fs = std::filesystem;

std::wstring GetActiveProcessName();
std::wstring GetActiveWindowTitle();
std::string utf8_encode(const std::wstring& wstr);
std::wstring utf8_decode(const std::string& str);
std::string timestring();
std::string GetClipboardText();

#endif