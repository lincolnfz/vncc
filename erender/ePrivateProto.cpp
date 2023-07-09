#include "ePrivateProto.h"
#include "base/jsonext.hpp"


void ePrivateProto::GenOutProto(const unsigned long reqId, const int cmdType, const std::string& data, const char* lbh_token, std::string& out) {
	nlohmann::json root;
	root["cmdType"] = cmdType;
	root["device"] = 4;
#if defined(_MULTIDESK_VER)
	root["device"] = 5; //pc端
#endif
	
	root["stt"] = "";
	root["data"] = data.c_str();
	root["reqid"] = reqId;
#if defined(_KAOPU_VER)
	root["version"] = 1057;
#endif
	root["t"] = lbh_token;
	out = root.dump();
}


std::string ePrivateProto::Connect(const char* lang, const char* token, const int deviceLevel, const char* lbh_token) {
	std::string out;
    std::unique_lock<std::mutex> lock{ _cs };
	unsigned long reqid = ++_reqid;
    lock.unlock();
	nlohmann::json root;
	root["cusType"] = 4;
#if defined(_MULTIDESK_VER)
	root["cusType"] = 5;
#endif

    //root["language"] = lang;
#if defined(_MAINLAND_VER) || defined(_MULTIDESK_VER)
	//国内版本不需要设置语言
    //auto got = root.find("language");
    //root.erase(got);
#endif
	root["channe"] = "";
	root["cilpContent"] = "";
	root["width"] = 0;
	root["height"] = 0;
	root["hidePackageNames"] = "";
	root["inWidth"] = 0;
	root["inDensity"] = 0;
	root["tk"] = token;
	root["deviceLevel"] = deviceLevel;
	GenOutProto(reqid, CMD_ACTION_PREPARE_CONNECT, root.dump(), lbh_token, out);
	return out;
}

std::string ePrivateProto::SwitchLang(const char* lang, const char* lbh_token) {
	std::string out;
    std::unique_lock<std::mutex> lock{ _cs };
	unsigned long reqid = ++_reqid;
    lock.unlock();
	//nlohmann::json root;
	//root["language"] = lang;
	//root["cusType"] = 4;
	GenOutProto(reqid, CMD_ACTION_CHANG_LANGUAGE, std::string(lang), lbh_token, out);
	return out;
}

std::string ePrivateProto::Heartbeat(const char* lbh_token) {
	std::string out;
    std::unique_lock<std::mutex> lock{ _cs };
	unsigned long reqid = ++_reqid;
    lock.unlock();
	GenOutProto(reqid, CMD_ACTION_GP_HEART, "", lbh_token, out);
	return out;
}

std::string ePrivateProto::HighVolume() {
	std::string out = "input keyevent KEYCODE_VOLUME_UP";
	//unsigned long reqid = ++_reqid;
	//GenOutProto(reqid, CMD_ACTION_SHELL, "input keyevent KEYCODE_VOLUME_UP", out);
	return out;
}

std::string ePrivateProto::LowVolume() {
	std::string out = "input keyevent KEYCODE_VOLUME_DOWN";
	//unsigned long reqid = ++_reqid;
	//GenOutProto(reqid, CMD_ACTION_SHELL, "input keyevent KEYCODE_VOLUME_DOWN", out);
	return out;
}

std::string ePrivateProto::Home() {
	std::string out = "input keyevent KEYCODE_HOME";
	//unsigned long reqid = ++_reqid;
	//GenOutProto(reqid, CMD_ACTION_SHELL, "input keyevent KEYCODE_HOME", out);
	return out;
}

std::string ePrivateProto::Back() {
	std::string out = "input keyevent KEYCODE_BACK";
	//unsigned long reqid = ++_reqid;
	//GenOutProto(reqid, CMD_ACTION_SHELL, "input keyevent KEYCODE_BACK", out);
	return out;
}

std::string ePrivateProto::AppSwitch() {
	std::string out = "input keyevent KEYCODE_APP_SWITCH";
	//unsigned long reqid = ++_reqid;
	//GenOutProto(reqid, CMD_ACTION_SHELL, "input keyevent KEYCODE_APP_SWITCH", out);
	return out;
}

std::string ePrivateProto::SetClipBoard(const char* data, const char* lbh_token) {
	std::string out;
    std::unique_lock<std::mutex> lock{ _cs };
	unsigned long reqid = ++_reqid;
    lock.unlock();
	GenOutProto(reqid, CMD_ACTION_SET_CLIP_BOARD, data, lbh_token, out);
	return out;
}

std::string ePrivateProto::GetInstallAppInfo(const char* lbh_token) {
	std::string out;
    std::unique_lock<std::mutex> lock{ _cs };
	unsigned long reqid = ++_reqid;
    lock.unlock();
	nlohmann::json root;
	root["isNeedIcon"] = "true";
	root["type"] = 2;
	GenOutProto(reqid, CMD_ACTION_GET_INSTALLED_APP_INFO, root.dump(), lbh_token, out);
	return out;
}

std::string ePrivateProto::MouseAction(float x, float y, int action) {
	std::string out;
    std::unique_lock<std::mutex> lock{ _cs };
	unsigned long reqid = ++_reqid;
    lock.unlock();
	nlohmann::json root;
	root["actionIndex"] = 0;
	root["action"] = action;
	root["count"] = 1;
	nlohmann::json points;
	nlohmann::json pt;
	pt["pid"] = 0;
	pt["x"] = x;
	pt["y"] = y;
	points.push_back(pt);
	root["points"] = points;
	//GenOutProto(reqid, CMD_ACTION_TOUCH, root.dump(), out);
	out = root.dump();
	return out;
}

std::string ePrivateProto::RootApps(std::vector<std::string>& changeapps, std::vector<std::string>& rootapps, const char* lbh_token) {
	std::string out;
    //EnterCriticalSection(&_cs);
    std::unique_lock<std::mutex> lock{ _cs };
	unsigned long reqid = ++_reqid;
    //LeaveCriticalSection(&_cs);
    lock.unlock();
	nlohmann::json root;
	root["changeApps"] = changeapps;
	root["rootApps"] = rootapps;
	GenOutProto(reqid, CMD_ACTION_ROOT_APP, root.dump(), lbh_token, out);
	return out;
}

std::string ePrivateProto::GetDeviceInfo(const char* lbh_token) {
	std::string out;
    std::unique_lock<std::mutex> lock{ _cs };
	unsigned long reqid = ++_reqid;
    lock.unlock();

	GenOutProto(reqid, CMD_ACTION_GET_DEVICEINFO, "", lbh_token, out);
	return out;
}

std::string ePrivateProto::ScreenCaptrue(int width, int height, bool bzip, float Resolution, float Definition, bool isSaveAlbum, const char* lbh_token) {
	std::string out;
    std::unique_lock<std::mutex> lock{ _cs };
	unsigned long reqid = ++_reqid;
    lock.unlock();

	nlohmann::json root;
	root["width"] = width;
	root["height"] = height;
	root["isZip"] = bzip;
	root["Resolution"] = Resolution;
	root["Definition"] = Definition;
	root["isSaveAlbum"] = isSaveAlbum;
	GenOutProto(reqid, CMD_ACTION_SCREENCAP, root.dump(), lbh_token, out);
	return out;
}

std::string ePrivateProto::GenNormal(const int cmdType, const std::string& data, const char* lbh_token) {
	std::string out;
    std::unique_lock<std::mutex> lock{ _cs };
	unsigned long reqid = ++_reqid;
    lock.unlock();

	GenOutProto(reqid, cmdType, data, lbh_token, out);
	return out;
}
