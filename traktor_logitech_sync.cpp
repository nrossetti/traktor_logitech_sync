#include "LogitechLEDLib.h"
#include "json.hpp"
#include "tchar.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <thread>
#include <fstream>

std::unordered_map<std::string, LogiLed::KeyName> keyNameMap = {
    {"ESC", LogiLed::ESC},
    {"F1", LogiLed::F1},
    {"F2", LogiLed::F2},
    {"F3", LogiLed::F3},
    {"F4", LogiLed::F4},
    {"F5", LogiLed::F5},
    {"F6", LogiLed::F6},
    {"F7", LogiLed::F7},
    {"F8", LogiLed::F8},
    {"F9", LogiLed::F9},
    {"F10", LogiLed::F10},
    {"F11", LogiLed::F11},
    {"F12", LogiLed::F12},
    {"PRINT_SCREEN", LogiLed::PRINT_SCREEN},
    {"SCROLL_LOCK", LogiLed::SCROLL_LOCK},
    {"PAUSE_BREAK", LogiLed::PAUSE_BREAK},
    {"TILDE", LogiLed::TILDE},
    {"ONE", LogiLed::ONE},
    {"TWO", LogiLed::TWO},
    {"THREE", LogiLed::THREE},
    {"FOUR", LogiLed::FOUR},
    {"FIVE", LogiLed::FIVE},
    {"SIX", LogiLed::SIX},
    {"SEVEN", LogiLed::SEVEN},
    {"EIGHT", LogiLed::EIGHT},
    {"NINE", LogiLed::NINE},
    {"ZERO", LogiLed::ZERO},
    {"MINUS", LogiLed::MINUS},
    {"EQUALS", LogiLed::EQUALS},
    {"BACKSPACE", LogiLed::BACKSPACE},
    {"INSERT", LogiLed::INSERT},
    {"HOME", LogiLed::HOME},
    {"PAGE_UP", LogiLed::PAGE_UP},
    {"NUM_LOCK", LogiLed::NUM_LOCK},
    {"NUM_SLASH", LogiLed::NUM_SLASH},
    {"NUM_ASTERISK", LogiLed::NUM_ASTERISK},
    {"NUM_MINUS", LogiLed::NUM_MINUS},
    {"TAB", LogiLed::TAB},
    {"Q", LogiLed::Q},
    {"W", LogiLed::W},
    {"E", LogiLed::E},
    {"R", LogiLed::R},
    {"T", LogiLed::T},
    {"Y", LogiLed::Y},
    {"U", LogiLed::U},
    {"I", LogiLed::I},
    {"O", LogiLed::O},
    {"P", LogiLed::P},
    {"OPEN_BRACKET", LogiLed::OPEN_BRACKET},
    {"CLOSE_BRACKET", LogiLed::CLOSE_BRACKET},
    {"BACKSLASH", LogiLed::BACKSLASH},
    {"KEYBOARD_DELETE", LogiLed::KEYBOARD_DELETE},
    {"END", LogiLed::END},
    {"PAGE_DOWN", LogiLed::PAGE_DOWN},
    {"NUM_SEVEN", LogiLed::NUM_SEVEN},
    {"NUM_EIGHT", LogiLed::NUM_EIGHT},
    {"NUM_NINE", LogiLed::NUM_NINE},
    {"NUM_PLUS", LogiLed::NUM_PLUS},
    {"CAPS_LOCK", LogiLed::CAPS_LOCK},
    {"A", LogiLed::A},
    {"S", LogiLed::S},
    {"D", LogiLed::D},
    {"F", LogiLed::F},
    {"G", LogiLed::G},
    {"H", LogiLed::H},
    {"J", LogiLed::J},
    {"K", LogiLed::K},
    {"L", LogiLed::L},
    {"SEMICOLON", LogiLed::SEMICOLON},
    {"APOSTROPHE", LogiLed::APOSTROPHE},
    {"ENTER", LogiLed::ENTER},
    {"NUM_FOUR", LogiLed::NUM_FOUR},
    {"NUM_FIVE", LogiLed::NUM_FIVE},
    {"NUM_SIX", LogiLed::NUM_SIX},
    {"LEFT_SHIFT", LogiLed::LEFT_SHIFT},
    {"Z", LogiLed::Z},
    {"X", LogiLed::X},
    {"C", LogiLed::C},
    {"V", LogiLed::V},
    {"B", LogiLed::B},
    {"N", LogiLed::N},
    {"M", LogiLed::M},
    {"COMMA", LogiLed::COMMA},
    {"PERIOD", LogiLed::PERIOD},
    {"FORWARD_SLASH", LogiLed::FORWARD_SLASH},
    {"RIGHT_SHIFT", LogiLed::RIGHT_SHIFT},
    {"ARROW_UP", LogiLed::ARROW_UP},
    {"NUM_ONE", LogiLed::NUM_ONE},
    {"NUM_TWO", LogiLed::NUM_TWO},
    {"NUM_THREE", LogiLed::NUM_THREE},
    {"NUM_ENTER", LogiLed::NUM_ENTER},
    {"LEFT_CONTROL", LogiLed::LEFT_CONTROL},
    {"LEFT_WINDOWS", LogiLed::LEFT_WINDOWS},
    {"LEFT_ALT", LogiLed::LEFT_ALT},
    {"SPACE", LogiLed::SPACE},
    {"RIGHT_ALT", LogiLed::RIGHT_ALT},
    {"RIGHT_WINDOWS", LogiLed::RIGHT_WINDOWS},
    {"APPLICATION_SELECT", LogiLed::APPLICATION_SELECT},
    {"RIGHT_CONTROL", LogiLed::RIGHT_CONTROL},
    {"ARROW_LEFT", LogiLed::ARROW_LEFT},
    {"ARROW_DOWN", LogiLed::ARROW_DOWN},
    {"ARROW_RIGHT", LogiLed::ARROW_RIGHT},
    {"NUM_ZERO", LogiLed::NUM_ZERO},
    {"NUM_PERIOD", LogiLed::NUM_PERIOD},
    {"G_1", LogiLed::G_1},
    {"G_2", LogiLed::G_2},
    {"G_3", LogiLed::G_3},
    {"G_4", LogiLed::G_4},
    {"G_5", LogiLed::G_5},
    {"G_6", LogiLed::G_6},
    {"G_7", LogiLed::G_7},
    {"G_8", LogiLed::G_8},
    {"G_9", LogiLed::G_9},
    {"G_LOGO", LogiLed::G_LOGO},
    {"G_BADGE", LogiLed::G_BADGE}
};

using json = nlohmann::json;
using Color = std::array<int, 3>;

std::unordered_map<std::string, std::vector<LogiLed::KeyName>> keyZones;

COLORREF GetPixelColorAt(int x, int y) {
    HDC hDC = GetDC(NULL);
    if (hDC == NULL) {
        std::cerr << "Failed to get DC" << std::endl;
        return CLR_INVALID;
    }

    COLORREF color = GetPixel(hDC, x, y);
    ReleaseDC(NULL, hDC);
    return color;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    const DWORD TITLE_SIZE = 1024;
    WCHAR windowTitle[TITLE_SIZE];

    GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);
    if (wcsstr(windowTitle, L"Traktor") != NULL) {
        *(HWND*)lParam = hwnd;
        return FALSE;
    }
    return TRUE;
}

void LoadConfiguration() {
    std::ifstream configFile("keyzones.json");
    if (!configFile.is_open()) {
        std::cerr << "Unable to open keyzones.json file.\n";
        return;
    }

    json config;
    configFile >> config;

    for (auto& zone : config["zones"].items()) {
        std::string zoneName = zone.key();
        std::cout << "Loading zone: " << zoneName << std::endl;
        std::vector<LogiLed::KeyName> keyNames;

        for (auto& keyNameStr : zone.value()) {
            if (keyNameMap.find(keyNameStr) != keyNameMap.end()) {
                keyNames.push_back(keyNameMap[keyNameStr]);
            }
        }

        keyZones[zoneName] = keyNames;
    }
}

void MonitorAreaForBlinking(HWND hwnd, int startX, int endX, int y, const std::string& zoneName) {
    int x = startX;
    bool isMonitoringPixel = false;
    DWORD lastColorChangeTime = GetTickCount();
    bool lastColorWasRed = false;

    while (true) {
        DWORD currentTime = GetTickCount();
        COLORREF color = GetPixelColorAt(x, y);
        int red = GetRValue(color);
        int green = GetGValue(color);
        int blue = GetBValue(color);

        if (!isMonitoringPixel) {

            if (red == 13 && green == 13 && blue == 13) {
                isMonitoringPixel = true;
                lastColorChangeTime = currentTime;
                std::cout << "Pixel found in Zone " << zoneName << ": RGB(" << red << ", " << green << ", " << blue << ")\n";
            }

            x++;
            if (x > endX) {
                x = startX;
                std::cout << "Reached end, looping back in Zone " << zoneName << ".\n";
            }
        }
        else {
            if ((red == 163 && green == 13 && blue == 13 && !lastColorWasRed) ||
                (red == 13 && green == 13 && blue == 13 && lastColorWasRed)) {
                std::cout << "Blink detected in Zone " << zoneName << "! New color: RGB(" << red << ", " << green << ", " << blue << ")\n";
                lastColorWasRed = !lastColorWasRed;
                lastColorChangeTime = currentTime;
                if (lastColorWasRed) {
                    for (const auto& key : keyZones[zoneName]) {
                        LogiLedSetLightingForKeyWithKeyName(key, 100, 0, 0);
                    }
                }
                else {
                    for (const auto& key : keyZones[zoneName]) {
                        LogiLedSetLightingForKeyWithKeyName(key, 100, 100, 100);
                    }
                }
            }
            else if (currentTime - lastColorChangeTime > 2000 && lastColorWasRed) {
                std::cout << "Blinking stopped in Zone " << zoneName << ".\n";
                lastColorWasRed = false;

                for (const auto& key : keyZones[zoneName]) {
                    LogiLedSetLightingForKeyWithKeyName(key, 100,100,100);
                }
            }
        }

        Sleep(10);
    }
}

int _tmain(int argc, _TCHAR* argv[]) {
    if (!LogiLedInit()) {
        std::cerr << "LogiLedInit() failed." << std::endl;
        return 1;
    }
    LoadConfiguration();
    LogiLedSaveCurrentLighting();

    HWND hwndTraktor = NULL;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&hwndTraktor));
    
    LogiLedSetLighting(100, 100, 100);

    if (hwndTraktor == NULL) {
        std::cerr << "Traktor window not found!\n";
        LogiLedShutdown();
        return 1;
    }

    std::cout << "LED SDK Initialized" << std::endl;
    LogiLedSetTargetDevice(LOGI_DEVICETYPE_ALL);

    int startX1 = 40, endX1 = 776, y1 = 308;
    int startX2 = 1060, endX2 = 1793, y2 = 308;

    std::string zoneName1 = "left_keys";
    std::string zoneName2 = "right_keys";

    std::thread thread1(MonitorAreaForBlinking, hwndTraktor, startX1, endX1, y1, zoneName1);
    std::thread thread2(MonitorAreaForBlinking, hwndTraktor, startX2, endX2, y2, zoneName2);

    thread1.join();
    thread2.join();

    LogiLedRestoreLighting();
    LogiLedShutdown();
    return 0;
}