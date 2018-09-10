#pragma once   //和ifndef一个作用

#include <string>
#include <mutex>
#include "vlc/vlc.h" 
#pragma comment(lib,"libvlc.lib")
#pragma comment(lib,"libvlccore.lib")



class ZVideo {
public:
	bool  ZVideo::init(HWND screen_hwnd);
	static ZVideo& getInstance();
	std::string getName();
	char* getUrl();
	void setUrl(CStringA a_url);
	std::string getFormat();
	virtual ~ZVideo();

	// VLC中文字符转码问题
	char* ZVideo::unicode2UTF8(CStringW& unicodeString);

	/* 功能方法 */
	bool begin();
	bool close();
	bool pause();
	bool fastForword();
	bool fastRewind();
	bool storeImage();
	bool storeVideoFragment();
	bool setVolumn();
	bool setProgress();

private:
	ZVideo();

	static ZVideo *instance;
	static std::mutex mux;

	std::string name{ "my_video" };
	std::string format;



	// 播放相关组件
	libvlc_instance_t * inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;

	// 相关参数
	char* url;
	float rate{1};
	long length{ 0 };
};




