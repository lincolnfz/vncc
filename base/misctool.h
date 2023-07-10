#pragma once

#include <string>
#include <iconv.h>
#include <time.h>
#include <assert.h>
#include <sys/utsname.h>
#include <spdlog/spdlog.h>
#include <regex>

#define GUID_KEY_LEN 37
std::string CreateGuid();

#ifdef __GNUC__
#    define GCC_VERSION_AT_LEAST(x,y) (__GNUC__ > (x) || __GNUC__ == (x) && __GNUC_MINOR__ >= (y))
#else
#    define GCC_VERSION_AT_LEAST(x,y) 0
#endif

#if GCC_VERSION_AT_LEAST(3,1)
#    define attribute_deprecated __attribute__((deprecated))
#elif defined(_MSC_VER)
#    define attribute_deprecated __declspec(deprecated)
#else
#    define attribute_deprecated
#endif

//void E_ASSERT(const bool);

#ifdef _DEBUG
#define DO_ASSERT(expr) assert(expr)
#else
#define DO_ASSERT(expr) 
#endif

#define _ASSERT( expr ) {\
                            if(!(expr)){ \
                                char szTemp[1028] = {0}; \
                                snprintf(szTemp, sizeof(szTemp)-1, "assert(false) %s:%d,  fun: %s,  %s, %s \n", __FILE__, __LINE__ , __FUNCTION__, __DATE__, __TIME__); \
                            } \
                            DO_ASSERT(expr); \
                        }

#define SAVE_FREE(point) { \
    if(point){ \
        free(point); \
        point = nullptr; \
    } \
}

#define SAVE_DELETE(point) { \
    if(point){ \
        delete point; \
        point = nullptr; \
    } \
}
            
std::string generate_guid();

int getnow_ts(struct timespec &now_ts);

/**
 * @brief Get the utcnow ts object
 * 
 * @param now_ts 
 * @return int 
 */
int get_now_utc_ts(struct timespec &now_ts);

/**
 * @brief 得到当前utc时间串
 * 
 * @param strTime 
 * @return int 
 */
attribute_deprecated
int utc_time_str(std::string& strTime);



/**
 * @brief 获取系统到当前的开机时间
 * 
 * @return long 
 */
long getNowTickMs();


/**
 * @brief Get the Utc 时间戳(秒)
 * 
 * @return long 
 */
long getUtcTickSec();


/**
 * @brief Get the Utc 时间戳(微秒)
 * 
 * @return long 
 */
unsigned long getUtcTickus();

/**
 * @brief Get the Utc 时间戳(毫秒)
 * 
 * @return long 
 */
unsigned long getUtcTickms();

unsigned int hash_str(std::string &str);

template< typename... Args >
std::string string_sprintf(const char* format, Args... args) {
    int length = std::snprintf(nullptr, 0, format, args...);
    if (length <= 0) {
        return std::string("");
    }
    char* buf = new char[length + 1];
    std::snprintf(buf, length + 1, format, args...);

    std::string str(buf);
    delete[] buf;
    return str;
}

void handle_for_sigpipe();

int setsigmask();

std::string GetStringMd5(const char* pszData);

std::string GetStringSha256(const char* pszData);

std::vector<std::string> stringSplit(const std::string& str, char delim);

/**
 * @brief Set the Current Thread High Priority object
 * 
 * @param value 
 */
void setCurrentThreadHighPriority(bool value);

void getNowTimeFmt(std::string& out);

bool testTcpPortUsed(unsigned short usPort);

bool IsWindow(uint64_t id);

std::string GetExeDirectory();
