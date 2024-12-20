// Map1Scene.h
#ifndef __MAP1_SCENE_H__
#define __MAP1_SCENE_H__

#include "BaseMapScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h" 
#include "Object/Npc.h"

class Map1Scene : public BaseMapScene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void onEnter() override;
    CREATE_FUNC(Map1Scene);
private:
    bool _isCollectingStone;
    cocos2d::Sprite* _npc;
    void showOptions();
    void closeDialogue();
    // 用于对话框显示的成员变量
    cocos2d::LayerColor* _dialogueLayer = nullptr;   // 对话框背景层
    cocos2d::Sprite* _npcFace = nullptr;             // NPC头像精灵
    cocos2d::ui::Text* _dialogueLabel = nullptr;     // 显示对话文本的控件
    int _dialogueIndex = 0;                          // 当前显示的对话语句索引
    std::vector<cocos2d::Sprite*> _mines;
    void initEventListeners();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void collectStoneAt(const cocos2d::Vec2& locationInMap);
    void addManualMines();
    std::vector<DialogueLine> _npcDemiDialogues;
    std::vector<std::string> DemiOption;
};

#endif
