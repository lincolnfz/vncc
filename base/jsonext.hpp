#pragma once
#include <string>
#include <json.hpp>


#ifndef _WSTRING
#define _WSTRING
class WString : public std::wstring {
public:
	WString() = default;
	WString(const std::wstring& in) { assign(in); }
	WString(const wchar_t* in) { assign(in); }
	const std::wstring& operator=(const std::wstring& in) { assign(in); return *this; }
	operator const wchar_t* () const { return this->c_str(); }
};
#endif

#ifndef _JSONDATASTRUCTPARENT
#define _JSONDATASTRUCTPARENT
struct __JsonDataStruct {};
#endif // !_JSONDATASTRUCTPARENT


#ifndef JSONSTRUCT
#define JSONSTRUCT(name) struct name : __JsonDataStruct
#endif
				 //void from_json(const json& j, _TeamInfo& s)
#define FROM_JSON(type) 	void from_json(const json& j, type& s)
#define TO_JSON(type) void to_json(json& j, const type& s)


#define FROMJSVALUE(name)  	if((j.find(#name) != j.cend()) && !j.at(#name).is_null()){ j.at(#name).get_to(s.name); } else { \
								_ASSERT(false); \
								}
								
#define OPTIONJSVALUE(name) if((j.find(#name) != j.cend()) && !j.at(#name).is_null()){ j.at(#name).get_to(s.name); }else{ \
							 }

#define TOJSVALUE(name) j[#name] = s.name



namespace nlohmann
{
	// Convert a wide Unicode string to an UTF8 string
	/*static std::string utf8_encode(const std::wstring& wstr)
	{
		if (wstr.empty()) return std::string();
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}

	// Convert an UTF8 string to a wide Unicode String
	static std::wstring utf8_decode(const std::string& str)
	{
		if (str.empty()) return std::wstring();
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}*/

	/*template <typename T>
	struct adl_serializer<std::unique_ptr<T>> {
		static void to_json(json& j, const std::unique_ptr<T>& opt) {
			j = opt ? json(*opt) : json(nullptr);
		}

		static void from_json(const json& j, std::unique_ptr<T>& opt) {
			opt = !j.is_null() ? std::make_unique<T>(j.get<T>()) : nullptr;
		}
	};

	static void to_json(json& j, const std::string str) {
		j = str;
	}

	static void from_json(const json& j, std::string& str) {
		if (j.is_string())
			str = j.get<std::string>();
		else
			str = j.dump();
	}*/

}


#include <vector>
#include <list>
#include <array>
#include <deque>
#include <forward_list>
#include <set>
#include <unordered_set>

template <typename T>
struct IsConvableJsonArray { static constexpr bool value = false; };
template <typename Arg>
struct IsConvableJsonArray<std::vector<Arg>> { static constexpr bool value = true; };
template <typename Arg>
struct IsConvableJsonArray<std::list<Arg>> { static constexpr bool value = true; };
template <typename Arg>
struct IsConvableJsonArray<std::forward_list<Arg>> { static constexpr bool value = true; };
template <typename Arg>
struct IsConvableJsonArray<std::deque<Arg>> { static constexpr bool value = true; };
template <typename Arg, size_t _Size>
struct IsConvableJsonArray<std::array<Arg, _Size>> { static constexpr bool value = true; };
template <typename... Arg>
struct IsConvableJsonArray<std::set<Arg...>> { static constexpr bool value = true; };
template <typename... Arg>
struct IsConvableJsonArray<std::multiset<Arg...>> { static constexpr bool value = true; };
template <typename... Arg>
struct IsConvableJsonArray<std::unordered_set<Arg...>> { static constexpr bool value = true; };
template <typename... Arg>
struct IsConvableJsonArray<std::unordered_multiset<Arg...>> { static constexpr bool value = true; };


template <typename T>
struct IsConvableJsonMap { static constexpr bool value = false; };
template <typename Arg>
struct IsConvableJsonMap<std::map<std::string, Arg>> { static constexpr bool value = true; };
template <typename Arg>
struct IsConvableJsonMap<std::multimap<std::string, Arg>> { static constexpr bool value = true; };
template <typename Arg>
struct IsConvableJsonMap<std::unordered_multimap<std::string, Arg>> { static constexpr bool value = true; };
template <typename Arg>
struct IsConvableJsonMap<std::unordered_map<std::string, Arg>> { static constexpr bool value = true; };


#define _CHECKTYPE(typeCheck,typeStd) std::is_same<typename std::decay<typeCheck>::type,typeStd>::value

template <typename TP>
bool QuickGetJsonData(const nlohmann::json& json, TP& value)
{
	if (!json.is_null())
	{
		bool bTypeOK = false;
		if (_CHECKTYPE(TP, bool))
		{
			bTypeOK = json.is_boolean();
		}
		else if (std::is_integral<TP>::value || std::is_floating_point<TP>::value || std::is_enum<TP>::value)
		{
			bTypeOK = json.is_number();
		}
		else if (_CHECKTYPE(TP, std::string) || _CHECKTYPE(TP, WString))
		{
			bTypeOK = json.is_string();
		}
		else if (IsConvableJsonArray<TP>::value)
		{
			bTypeOK = json.is_array();
		}
		else if (IsConvableJsonMap<TP>::value)
		{
			bTypeOK = json.is_object();
		}
		else if (std::is_class<TP>::value && std::is_base_of<__JsonDataStruct, TP>::value)
		{
			bTypeOK = json.is_object();
		}
		

		if (bTypeOK)
		{
			bool bRet = false;
			try
			{
				json.get_to(value);
				bRet = true;
			}
			catch (...)
			{
				//printLogMsg(eLOG_LEVEL_WARN, "json", "json invalid"); 
			}
			return bRet;
		}
	}
	
	return false;
}

template <typename TP>
bool RootJsonString_help(const char* pdata, TP& value) {
	bool bret = false;
	nlohmann::json js;
	try {
		js = nlohmann::json::parse(pdata, nullptr, true);
		bret = QuickGetJsonData(js, value);
	}
	catch (nlohmann::json::parse_error& e) {
		//printLogMsg(eLOG_LEVEL_ERROR, "eEmuClient", "json fial");
	}
	catch (...) {
		//printLogMsg(eLOG_LEVEL_ERROR, "eEmuClient", "json unknow fial");
	}
	return bret;
}

template <typename TP>
bool QuickGetJsonData(const char* pdata, TP& value) {
	bool bret = false;
	nlohmann::json js;
	try {
		js = nlohmann::json::parse(pdata, nullptr, true);
		if (js.is_string()) {
			//有的数据最外层是string,这里做二次解析
			std::string strjson = js;
			bret = RootJsonString_help(strjson.c_str(), value);
		}
		else {
			bret = QuickGetJsonData(js, value);
		}
	}
	catch (nlohmann::json::parse_error& e) {
		//printLogMsg(eLOG_LEVEL_ERROR, "eEmuClient", "json fial");
	}
	catch (...) {
		//printLogMsg(eLOG_LEVEL_ERROR, "eEmuClient", "json unknow fial");
	}
	return bret;
}