#include "util.h"
#include <ctime>

#pragma warning(disable:4996)
std::wstring GetProcessNameFromId(DWORD processId)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
		return L"";

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(snapshot, &entry))
	{
		CloseHandle(snapshot);
		return L"";
	}

	do
	{
		if (entry.th32ProcessID == processId)
		{
			std::wstring name(entry.szExeFile);
			CloseHandle(snapshot);
			return name;
		}
	} while (Process32Next(snapshot, &entry));

	CloseHandle(snapshot);
	return L"";
}

std::wstring GetActiveProcessName() {
	HWND hwnd = GetForegroundWindow();
	DWORD processId = 0;

	if (!hwnd) return L"";

	GetWindowThreadProcessId(hwnd, &processId);
	if (!processId) return L"";

	return GetProcessNameFromId(processId);
}

std::wstring GetActiveWindowTitle()
{
	HWND hwnd = GetForegroundWindow();
	if (hwnd == NULL)
		return L"";

	int length = GetWindowTextLength(hwnd);
	if (length == 0)
		return L"";

	std::wstring title;
	title.resize((size_t)length + 1);

	int result = GetWindowText(hwnd, &title[0], length + 1);
	if (result == 0 || result > length)
		return L"";

	title.resize(result);
	return title;
}

BOOL SetClipboardText(string text) {
	int size = ::MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
	if (size < 0) return FALSE;

	if (::OpenClipboard(NULL)) {
		::EmptyClipboard();
		HGLOBAL hGlobal = ::GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, (size + 1) * sizeof(WCHAR));
		if (hGlobal != NULL) {
			LPWSTR lpszData = (LPWSTR)::GlobalLock(hGlobal);
			if (lpszData != nullptr) {
				::MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, lpszData, size);
				::GlobalUnlock(hGlobal);
				::SetClipboardData(CF_UNICODETEXT, hGlobal);
			}
		}
		::CloseClipboard();
		return true;
	}
	return false;
}

std::string GetClipboardText()
{
	if (!OpenClipboard(0)) return "";
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr) return "";

	char* pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
		return "";

	std::string text(pszText);
	GlobalUnlock(hData);
	CloseClipboard();
	return text;
}

std::wstring GetClipboardTextW()
{
	if (!OpenClipboard(0)) return L"";
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr) return L"";

	wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
	if (pszText == nullptr)
		return L"";

	std::wstring text(pszText);
	GlobalUnlock(hData);
	CloseClipboard();
	return text;
}


BOOL SetClipboardTextW(const wstring text) {
	int size = text.length();
	if (size < 0 || !OpenClipboard(NULL)) return FALSE;

	::EmptyClipboard();

	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, ((size_t)size + 1) * sizeof(WCHAR));
	if (hGlobal != NULL) {
		wchar_t* lpszData = (wchar_t*)::GlobalLock(hGlobal);

		if (lpszData != nullptr) {
			wcscpy_s(lpszData, size, text.c_str());
			::GlobalUnlock(hGlobal);
			::SetClipboardData(CF_UNICODETEXT, hGlobal);
		}
	}

	::CloseClipboard();
	return true;
}
// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}
// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string& str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

std::string timestring() {
	std::time_t now = std::time(nullptr);
	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
	return std::string(buffer);
}
