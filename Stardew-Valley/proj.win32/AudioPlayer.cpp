/****************************************************************
 * Project Name:  Stardew-Valley
 * File Name:     AudioPlayer.cpp
 * File Function: ��Ƶ���淽���Ķ���
 * Author:        �ܽ�˶
 * Update Date:   2024/12/07
 * License:       MIT License
 ****************************************************************/
#include "AudioEngine.h"
#include "proj.win32/Constant.h"
#include "AudioPlayer.h"
// �����ռ�

// ��Ƶ��������
int g_backgroundMusicSign = DEFAULT_MUSIC_SIGN;
int g_soundEffectSign = DEFAULT_MUSIC_SIGN;
float g_backgroundMusicVolumn = DEFAULT_MUSIC_VOLUMN;
float g_soundEffectVolumn = DEFAULT_MUSIC_VOLUMN;

// ��Ƶ���淽��
void audioPlayer(const std::string& audioPath, bool isLoop)
{
    if (isLoop) {
        if (g_backgroundMusicSign != DEFAULT_MUSIC_SIGN) {
            cocos2d::experimental::AudioEngine::stop(g_backgroundMusicSign);
        }
        g_backgroundMusicSign = cocos2d::experimental::AudioEngine::play2d(audioPath, isLoop);
        cocos2d::experimental::AudioEngine::setVolume(g_backgroundMusicSign, g_backgroundMusicVolumn);
    }
    else {
        g_soundEffectSign = cocos2d::experimental::AudioEngine::play2d(audioPath, isLoop);
        cocos2d::experimental::AudioEngine::setVolume(g_soundEffectSign, g_soundEffectVolumn);
    }
}
