/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Constant.h
 * 文件功能      : 常量定义头文件
 * 作者          : 曹津硕
 * 更新日期      : 2024/12/22
 * 许可证        : MIT License
 ****************************************************************/
#pragma once

#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include <string>

 // 应用程序设置
constexpr int DESIGN_RESOLUTION_WIDTH = 1280;                               // 设计分辨率宽度
constexpr int DESIGN_RESOLUTION_HEIGHT = 720;                               // 设计分辨率高度
constexpr int SMALL_RESOLUTION_WIDTH = 960;                                 // 小分辨率宽度
constexpr int SMALL_RESOLUTION_HEIGHT = 540;                                // 小分辨率高度
constexpr int MEDIUM_RESOLUTION_WIDTH = 1280;                               // 中分辨率宽度
constexpr int MEDIUM_RESOLUTION_HEIGHT = 720;                               // 中分辨率高度
constexpr int LARGE_RESOLUTION_WIDTH = 1920;                                // 大分辨率宽度
constexpr int LARGE_RESOLUTION_HEIGHT = 1080;                               // 大分辨率高度
constexpr float FRAME_RATE = 60.0f;                                         // 应用程序帧率
const std::string APPLICATION_TITLE = u8"星露谷 Stardew-Valley";      // 应用程序标题

constexpr int DEFAULT_MUSIC_SIGN = -1;                                      // 缺省音乐标识
constexpr float DEFAULT_MUSIC_VOLUMN = 0.5f;                                // 缺省音量大小

// 启动场景设置
constexpr float SCENE_TRANSITION_DURATION = 0.3f;                           // 场景切换时间
constexpr float SETTING_BUTTON_SCALE = 12.0f;                               // 设置按钮缩放比例
constexpr float SETTING_BUTTON_OFFSET = 50.0f;                              // 设置按钮偏移量
constexpr float SETTINGS_SCENE_SLIDER_OFFSET_X = 50.0f; 				    // 设置场景滑动条X轴偏移量
constexpr float SETTINGS_SCENE_BGMSLIDER_OFFSET_Y = 80.0f; 				// 设置场景背景音乐滑动条Y轴偏移量
constexpr float SETTINGS_SCENE_EFFSLIDER_OFFSET_Y = -70.0f; 				// 设置场景音效滑动条Y轴偏移量
constexpr float SETTINGS_SCENE_RETURN_MENU_BUTTON_OFFSET_X = 330.0f; 		// 设置场景返回菜单按钮X轴偏移量
constexpr float SETTINGS_SCENE_RETURN_MENU_BUTTON_OFFSET_Y = -190.0f; 		// 设置场景返回菜单按钮Y轴偏移量

// 地图选择场景设置
constexpr float MAP2_BUTTON_OFFSET_X = -290.0f;                              // 地图2按钮X轴偏移量
constexpr float MAP2_BUTTON_OFFSET_Y = -410.0f;							    // 地图2按钮Y轴偏移量
constexpr float MAP3_BUTTON_OFFSET_X = 0.0f;							    // 地图3按钮X轴偏移量
constexpr float MAP3_BUTTON_OFFSET_Y = -410.0f;							    // 地图3按钮Y轴偏移量
constexpr float MAP1_BUTTON_OFFSET_X = 290.0f;							    // 地图1按钮X轴偏移量
constexpr float MAP1_BUTTON_OFFSET_Y = -410.0f;							    // 地图1按钮Y轴偏移量

// 背包场景设置
constexpr float SPEED_BAR_OFFSET_Y = -110.0f;                               // 速度滑动条Y轴偏移量
constexpr float SIZE_BAR_OFFSET_Y = 0.0f;								    // 大小滑动条Y轴偏移量
constexpr float BAR_OFFSET_X = 140.0f;									    // 滑动条X轴偏移量
constexpr float ITEM_PADDING = 47.5f;									    // 物品间距
constexpr float ITEM_OFFSET_X = 10.00f;									    // 物品X轴偏移量
constexpr float ITEM_OFFSET_Y = 20.00f;									    // 物品Y轴偏移量

//触摸键位置
constexpr float KEYS_CENTER_X = 200.0f;
constexpr float KEYS_CENTER_Y = 200.0f;
constexpr float KEYS_RADIUS = 80.0f;
constexpr float KEYS_BACKPACK_X = 1200.0f;
constexpr float KEYS_BACKPACK_Y = 500.0f;
constexpr float KEYS_MAP_X = 1200.0f;
constexpr float KEYS_MAP_Y = 600.0f;







#endif // !_CONSTANT_H_