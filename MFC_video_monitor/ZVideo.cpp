#include "stdafx.h"
#include "ZVideo.h"

// 静态变量，头文件中声明，类文件中定义。
ZVideo* ZVideo::instance = nullptr;
std::mutex ZVideo::mux;


// 单例函数
ZVideo& ZVideo::getInstance() {
	if (instance==nullptr) {
		// 加锁
		mux.lock();
		if (instance == nullptr) {
			instance = new ZVideo();
		}
		mux.unlock();
	}
	return *instance;
}


std::string ZVideo::getName() {
	return "1";
}



char* ZVideo::getUrl() {
	return this->url;
}

void ZVideo::setUrl(CStringA a_url) {

	USES_CONVERSION;
	CStringW w_url;
	w_url.Format(L"%s", A2W(a_url));
	
	char* url = unicode2UTF8(w_url);

	this->url = url;
}

std::string ZVideo::getFormat() {
	return "1";
}

bool ZVideo::init(HWND screen_hwnd) {
	inst = libvlc_new(0, NULL);						/* Create and initialize a libvlc instance. */
	if (inst == nullptr) return false;

	// 初始化播放路径
	m = libvlc_media_new_path(inst,this->url);		// 必须为英文路径  
	if (m == nullptr) return false;

	mp = libvlc_media_player_new_from_media(m);		/* Create a media player playing environement */
	if (mp == nullptr) return false;

	// 获取电影长度
	length = (long)libvlc_media_player_get_length(mp);
	//name = libvlc_media_player_get_title(mp);

	// 设置播放窗口
	libvlc_media_player_set_hwnd(mp, screen_hwnd);
}


char* ZVideo::unicode2UTF8(CStringW& unicodeString)
{
	int stringLength = ::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), NULL, 0, NULL, NULL);
	char* UTF8String = new char[stringLength + 1];
	::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), UTF8String, stringLength, NULL, NULL);
	UTF8String[stringLength] = '\0';

	return UTF8String;
}


// 功能方法

bool ZVideo::begin() {
	// 播放文件
	if (mp == nullptr) return false;
	libvlc_media_player_play(mp);      /* play the media_player */
	return true;
}

bool ZVideo::pause() {
	// 切换暂停开始
	if (mp == nullptr) return false;
	libvlc_media_player_pause(mp);
	return true;
}
bool ZVideo::close() {
	if (mp == nullptr) return false;
	libvlc_media_player_stop(mp);
	return true;
}
bool ZVideo::fastForword() {
	if (mp == nullptr) return false;

	if (rate < 2) {
		rate = rate + 0.25;
	}

	libvlc_media_player_set_rate(mp,rate);
	return true;
}
bool ZVideo::fastRewind() {
	if (mp == nullptr) return false;

	if (rate > 0.5) {
		rate = rate - 0.25;
	}
	libvlc_media_player_set_rate(mp, rate);
	return true;
}

bool ZVideo::storeImage() {
	if (mp == nullptr) return false;

	return true;
}
bool ZVideo::storeVideoFragment() {
	if (mp == nullptr) return false;

	return true;
}

bool ZVideo::setVolumn() {
	if (mp == nullptr) return false;
	libvlc_audio_set_volume_cb();

	return true;
}

bool ZVideo::setProgress() {
	if (mp == nullptr) return false;

	/*Get the current movie time (in ms).*/
	libvlc_media_player_get_time(mp);
	libvlc_media_player_get_position(mp);

	libvlc_media_player_set_time(mp, 1);
	libvlc_media_player_set_position(mp, 0.1);
	return true;
}


// 构造与析构
ZVideo::ZVideo() {
}

ZVideo::~ZVideo() {
	if(mp!=nullptr) libvlc_media_player_release(mp);    /* Free the media_player */
	if (inst != nullptr) libvlc_release(inst);			/* Decrement the reference count of a libvlc instance*/
	if (m != nullptr)libvlc_media_release(m);			/* No need to keep the media now */
}
