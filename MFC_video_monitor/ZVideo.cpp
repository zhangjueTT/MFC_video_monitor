#include "stdafx.h"
#include "ZVideo.h"

// ��̬������ͷ�ļ������������ļ��ж��塣
ZVideo* ZVideo::instance = nullptr;
std::mutex ZVideo::mux;


// ��������
ZVideo& ZVideo::getInstance() {
	if (instance==nullptr) {
		// ����
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


std::string ZVideo::getFormat() {
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


bool ZVideo::init(HWND screen_hwnd) {
	inst = libvlc_new(0, NULL);						/* Create and initialize a libvlc instance. */
	if (inst == nullptr) return false;

	// ��ʼ������·��
	m = libvlc_media_new_path(inst,this->url);		// ����ΪӢ��·��  
	if (m == nullptr) return false;

	mp = libvlc_media_player_new_from_media(m);		/* Create a media player playing environement */
	if (mp == nullptr) return false;

	// ���ò��Ŵ���
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


// ���ܷ���

bool ZVideo::begin() {
	// �����ļ�
	if (mp == nullptr) return false;

	// IDLE/CLOSE=0, OPENING=1, PLAYING=3, PAUSED=4, STOPPING=5, ENDED=6, ERROR=7
	if (libvlc_media_player_get_state(mp)!=libvlc_Playing) {
		libvlc_media_player_play(mp);      /* play the media_player */
		rate = 1.0;
	}
	return true;
}

bool ZVideo::pause() {
	// �л���ͣ��ʼ
	if (mp == nullptr) return false;
	libvlc_media_player_pause(mp);
	return true;
}
bool ZVideo::close() {
	if (mp == nullptr) return false;
	libvlc_media_player_stop(mp);

	// ״̬ת��
	delete url;
	url = nullptr;
	rate = 1.0;
	return true;
}
bool ZVideo::fastForword() {
	if (mp == nullptr) return false;

	if (rate < 2&& libvlc_media_player_get_state(mp) == libvlc_Playing) {
		rate = rate + 0.25;
	}

	libvlc_media_player_set_rate(mp,rate);
	return true;
}
bool ZVideo::fastRewind() {
	if (mp == nullptr) return false;

	if (rate > 0.5&&libvlc_media_player_get_state(mp) == libvlc_Playing) {
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

int ZVideo::getVolumn() {
	if (mp == nullptr) return false;	
	return libvlc_audio_get_volume(mp);
}


bool ZVideo::setVolumn(int volume) {
	if (mp == nullptr) return false;
	libvlc_audio_set_volume(mp, volume);
	return true;
}

void ZVideo::setProgress(float posf) {
	if (mp == nullptr) return;
	libvlc_media_player_set_position(mp, posf);
}

// ����������
float ZVideo::getRate() {
	return this->rate;
}

// IDLE/CLOSE=0, OPENING=1, PLAYING=3, PAUSED=4, STOPPING=5, ENDED=6, ERROR=7
int ZVideo::getPlayerState() {
	if (mp == nullptr) return 0;
	return libvlc_media_player_get_state(mp);
}

int ZVideo::getDuration() {
	if (mp == nullptr) return 0;
	return libvlc_media_player_get_length(mp) / 1000;
}

int ZVideo::getCurrent() {
	if (mp == nullptr) return 0;
	return libvlc_media_player_get_time(mp) / 1000;
}





// ����������
ZVideo::ZVideo() {
}

ZVideo::~ZVideo() {
	if(mp!=nullptr) libvlc_media_player_release(mp);    /* Free the media_player */
	if (inst != nullptr) libvlc_release(inst);			/* Decrement the reference count of a libvlc instance*/
	if (m != nullptr)libvlc_media_release(m);			/* No need to keep the media now */
}
