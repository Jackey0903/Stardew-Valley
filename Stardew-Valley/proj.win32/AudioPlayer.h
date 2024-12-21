/****************************************************************
 * Project Name:  Stardew-Valley
 * File Name:     AudioPlayer.h
 * File Function: 音频引擎方法的定义
 * Author:        曹津硕
 * Update Date:   2024/12/07
 * License:       MIT License
 ****************************************************************/

#pragma once
#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

#include "AudioEngine.h"
#include "proj.win32/Constant.h"

// 音频引擎方法
void audioPlayer(const std::string& audioPath, bool isLoop);

#endif // !_AUDIO_PLAYER_H_