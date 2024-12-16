// Map1Scene.h
#ifndef __MAP1_SCENE_H__
#define __MAP1_SCENE_H__

#include "BaseMapScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h" 

class Map1Scene : public BaseMapScene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void onEnter() override;
    CREATE_FUNC(Map1Scene);
private:
    cocos2d::Sprite* _npc;

    // 用于对话框显示的成员变量
    cocos2d::LayerColor* _dialogueLayer = nullptr;   // 对话框背景层
    cocos2d::Sprite* _npcFace = nullptr;             // NPC头像精灵
    cocos2d::ui::Text* _dialogueLabel = nullptr;     // 显示对话文本的控件
    int _dialogueIndex = 0;                          // 当前显示的对话语句索引
    void showDialogue();
};

#endif
