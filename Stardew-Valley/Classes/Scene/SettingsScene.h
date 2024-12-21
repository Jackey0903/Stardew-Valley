/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : SettingsScene.h
 * 文件功能      : 设置场景类实现
 * 作者          : 曹津硕
 * 更新日期      : 2024/12/20
 * 许可证        : MIT License
 ****************************************************************/

#ifndef _SETTINGS_SCENE_H_
#define _SETTINGS_SCENE_H_

#include "cocos2d.h"


class SettingsScene : public cocos2d::Scene {
public:
    // 创建场景
    static cocos2d::Scene* createScene();

    // 初始化场景
    virtual bool init();

    // 实现 SettingsScene 类的 create 方法
    CREATE_FUNC(SettingsScene);
};

#endif // !_SETTINGS_SCENE_H_