/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : Constant.h
 * �ļ�����      : ��������ͷ�ļ�
 * ����          : �ܽ�˶
 * ��������      : 2024/12/04
 * ���֤        : MIT License
 ****************************************************************/
#pragma once

#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include <string>

 // Ӧ�ó�������
constexpr int DESIGN_RESOLUTION_WIDTH = 1280;                               // ��Ʒֱ��ʿ��
constexpr int DESIGN_RESOLUTION_HEIGHT = 720;                               // ��Ʒֱ��ʸ߶�
constexpr int SMALL_RESOLUTION_WIDTH = 960;                                 // С�ֱ��ʿ��
constexpr int SMALL_RESOLUTION_HEIGHT = 540;                                // С�ֱ��ʸ߶�
constexpr int MEDIUM_RESOLUTION_WIDTH = 1280;                               // �зֱ��ʿ��
constexpr int MEDIUM_RESOLUTION_HEIGHT = 720;                               // �зֱ��ʸ߶�
constexpr int LARGE_RESOLUTION_WIDTH = 1920;                                // ��ֱ��ʿ��
constexpr int LARGE_RESOLUTION_HEIGHT = 1080;                               // ��ֱ��ʸ߶�
constexpr float FRAME_RATE = 60.0f;                                         // Ӧ�ó���֡��
const std::string APPLICATION_TITLE = u8"��¶�� Stardew-Valley";      // Ӧ�ó������

constexpr int DEFAULT_MUSIC_SIGN = -1;                                      // ȱʡ���ֱ�ʶ
constexpr float DEFAULT_MUSIC_VOLUMN = 0.5f;                                // ȱʡ������С

// ������������
constexpr float SCENE_TRANSITION_DURATION = 0.3f;                           // �����л�ʱ��
constexpr float SETTING_BUTTON_SCALE = 12.0f;                               // ���ð�ť���ű���
constexpr float SETTING_BUTTON_OFFSET = 50.0f;                              // ���ð�ťƫ����
constexpr float SETTINGS_SCENE_SLIDER_OFFSET_X = 50.0f; 				    // ���ó���������X��ƫ����
constexpr float SETTINGS_SCENE_BGMSLIDER_OFFSET_Y = 80.0f; 				// ���ó����������ֻ�����Y��ƫ����
constexpr float SETTINGS_SCENE_EFFSLIDER_OFFSET_Y = -70.0f; 				// ���ó�����Ч������Y��ƫ����

constexpr float SETTINGS_SCENE_RETURN_MENU_BUTTON_OFFSET_X = 330.0f; 		// ���ó������ز˵���ťX��ƫ����
constexpr float SETTINGS_SCENE_RETURN_MENU_BUTTON_OFFSET_Y = -190.0f; 		// ���ó������ز˵���ťY��ƫ����











#endif // !_CONSTANT_H_