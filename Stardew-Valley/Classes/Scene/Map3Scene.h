// Map3Scene.h
#ifndef __MAP3_SCENE_H__
#define __MAP3_SCENE_H__

#include "BaseMapScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h" 
#include "Object/Npc.h"
class Map3Scene : public BaseMapScene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void onEnter() override;

    CREATE_FUNC(Map3Scene);

private:
    cocos2d::Sprite* _npc;

    // 用于对话框显示的成员变量
    cocos2d::LayerColor* _dialogueLayer = nullptr;   // 对话框背景层
    cocos2d::Sprite* _npcFace = nullptr;             // NPC头像精灵
    cocos2d::ui::Text* _dialogueLabel = nullptr;     // 显示对话文本的控件
    int _dialogueIndex = 0;                          // 当前显示的对话语句索引
    void showDialogue();
    void showOptions();  // 显示回答选项
    void closeDialogue();
    std::vector<DialogueLine> _npcLeahDialogues;
    std::vector<std::string> LeahOption;
};

#endif // __MAP3_SCENE_H__
