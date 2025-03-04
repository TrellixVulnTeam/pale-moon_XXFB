#ifndef _COMMON_UTILS_H
#define _COMMON_UTILS_H

#define UTIL_BEGIN_NAMESPACE namespace util {
#define UTIL_END_NAMESPACE }
#define USING_UTIL_NAMESPACE using namespace util;

#include <memory>
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <chrono>


UTIL_BEGIN_NAMESPACE

using dictionary = void;

enum DatePrecision
{
    PRECISION_NULL = 0,
    PRECISION_YEAR = 1,
    PRECISION_MONTH = 2,
    PRECISION_DAY = 3,
    PRECISION_HOUR = 4,
    PRECISION_MINUTE = 5,
    PRECISION_SECOND = 6,
    PRECISION_MSEC = 7,
    PRECISION_USEC = 8,
    PRECISION_NANO100 = 9
};

struct TIMESTAMP_STRUCT
{
    short           year{};
    unsigned short  month{};  // [1, 12]
    unsigned short  day{};    // [1, 31]
    unsigned short  hour{};   // [0, 23]
    unsigned short  minute{}; // [0, 59]
    unsigned short  second{}; // [0, 60] including leap second
    unsigned long   fraction{};
};

struct TIME_STRUCT
{
    unsigned short  hour;
    unsigned short  minute;
    unsigned short  second;
};

void GetCurTimestamp(TIMESTAMP_STRUCT &st);
TIMESTAMP_STRUCT GetCurTimestamp();
std::string GetCurTimestampStr(bool with_fraction);
std::time_t TimestampToTime(const TIMESTAMP_STRUCT &st);
std::time_t GetCurTimeT();
void TimeToTimestamp(std::time_t tt, TIMESTAMP_STRUCT &ts);
void TimeToTM(std::time_t tt, std::tm& stm);
std::tm TimeToTM(std::time_t tt);
bool StrToTimestamp(const std::string &s, TIMESTAMP_STRUCT &ts);
std::string TimestampToStr(const TIMESTAMP_STRUCT &st, bool with_fraction);
std::string& TimestampToStr(const TIMESTAMP_STRUCT &st, bool with_fraction, std::string& str);
std::string TimeTToStr(std::time_t tm);
std::string& TimeTToStr(std::time_t tm, std::string& str);
std::time_t StrToTimeT(const std::string &str);
long LocalUtcTimeDiff();

bool ParseTimestamp(const char *s, TIMESTAMP_STRUCT &timestamp);
bool ParseTimestamp(const std::string &s, TIMESTAMP_STRUCT &timestamp);
bool ParseTimestamp(const char *s, TIMESTAMP_STRUCT &timestamp, DatePrecision& precision);
bool ParseTime(const char *s, TIME_STRUCT &ts);
bool ParseTime(const std::string &s, TIME_STRUCT &ts);
bool ParseTime(const char *s, TIME_STRUCT &time_struct, DatePrecision& precision);

void SleepMs(long ms);
unsigned long long GetTimeInMs64();
// (t2 - t1) to miniseconds
static long GetTimeDiffInMs(const std::chrono::time_point<std::chrono::system_clock>& t1,
    const std::chrono::time_point<std::chrono::system_clock>& t2)
{
    return static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
}
// (now - from_point) to miniseconds
static inline long GetElapsedTimeFromInMs(const std::chrono::time_point<std::chrono::system_clock>& from_point)
{
    return GetTimeDiffInMs(from_point, std::chrono::system_clock::now());
}
bool GetFileModifyTime(const char *path, std::time_t& mtime);
std::string GetCurrentPath();
std::string GetCurrentProcessName();

bool FileExists(const std::string& file_path);
long long FileSize(const std::string& file_path);

// find files (including folders) using file specifier
// path_specifier: e.g., abc/*.*, tuple: (file name/dir name, is directory)
bool FindFiles(const std::string& path_specifier,
    std::vector<std::tuple<std::string, bool>>& items);
bool FindFiles(const std::string& path_specifier,
    std::vector<std::string>& filenames);
// tuple: (file name/dir name, is directory)
bool GetSubsInFolder(const std::string& folder_pathname,
    std::vector<std::tuple<std::string, bool>>& sub_items);
bool GetFilesInFolder(const std::string& folder_pathname,
    std::vector<std::string>& filenames);
bool GetPathnamesInFolder(const std::string& folder_pathname,
    std::vector<std::string>& pathnames);

bool DirExists(const std::string& pathname);
bool MakeDir(const std::string& pathname);
bool RmDir(const std::string& pathname, bool recursive);
// Remove files/folders, specifier: file specifier with *, ?
bool Rm(const std::string& specifier, bool recursive);

bool GetLine(std::istream &is, std::string &line);
bool GetLine(std::ifstream &fs, std::string &line);
void ParseCsvLine(std::vector<std::string> &strs, const std::string &line, char delimiter);
void ParseCsvLine(std::vector<std::string> &strs, const char* line, char delimiter);
void ParseCsvLineInPlace(std::vector<char *> &strs, char *line, char delimiter,
    bool ignore_double_quote = false);
std::vector<std::string> StringSplit(const std::string &line, char delimiter);
std::vector<char *> StringSplitInPlace(std::string &line, char delimiter);

// return the tuple (start, end), start <= index <= end
std::vector<std::tuple<int, int>> SplitIntoSubsBlocks(int task_count, int element_count);
// n_parts: is the suggested count of the parts. the actural may be different
bool SplitBigData(char *data, size_t len, char delimiter, int n_blocks,
    std::vector<char *>& blocks);
bool ReadAllFromFile(const std::string& pathname, std::string &data);
bool WriteStrToFile(const std::string& pathname, const std::string &data);
int StringReplace(std::string &str_base, const std::string &str_src,
    const std::string &str_des);
int StringReplace(std::wstring &wstr_base, const std::wstring &wstr_src,
    const std::wstring &wstr_des);
int StringReplaceChar(std::string &str_base, char ch_src, char ch_des);
int StringReplaceChar(std::wstring &wstr_base, wchar_t ch_src, wchar_t ch_des);

void MakeUpper(std::string &str);
void MakeLower(std::string &str);
std::string& LeftTrimString(std::string& str);
std::string& RightTrimString(std::string& str);
std::string& TrimString(std::string& str);

std::wstring& StrToWStr(const std::string &str, std::wstring& wstr);
std::string& WStrToStr(const std::wstring &wstr, std::string& str);
std::wstring StrToWStr(const std::string &str);
std::string WStrToStr(const std::wstring &wstr);

#ifdef _WIN32
std::wstring utf82ws(const char *s);
std::wstring& utf82ws(const char *s, std::wstring& ws);
std::string ws2utf8(const wchar_t *ws);
std::string& ws2utf8(const wchar_t *ws, std::string& s);
std::string ws2gb2312(const wchar_t *wstr);
std::wstring gb2312_2_ws(const char *src);
#endif

bool ReadAllDataFromFile(const std::string& pathname, std::vector<unsigned char> &data);
bool WriteAllDataToFile(const std::string& pathname, const std::vector<unsigned char> &data);

void Base64Encode(const char *str, std::string& ret);
void Base64Decode(const char *str, std::string& ret);
static inline std::string Base64Encode(const char *str)
{
    std::string ret;
    Base64Encode(str, ret);
    return ret;
}
static inline std::string Base64Decode(const char *str)
{
    std::string ret;
    Base64Decode(str, ret);
    return ret;
}

bool LocaleToLangChar(const std::string& locale, char& lang_char);
bool LocaleToLangChar(const char* locale, char& lang_char);
bool IsUtf8String(const std::string& str);

UTIL_END_NAMESPACE

#endif // _COMMON_UTILS_H
