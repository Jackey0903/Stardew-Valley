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
    // ���ڶԻ�����ʾ�ĳ�Ա����
    cocos2d::LayerColor* _dialogueLayer = nullptr;   // �Ի��򱳾���
    cocos2d::Sprite* _npcFace = nullptr;             // NPCͷ����
    cocos2d::ui::Text* _dialogueLabel = nullptr;     // ��ʾ�Ի��ı��Ŀؼ�
    int _dialogueIndex = 0;                          // ��ǰ��ʾ�ĶԻ��������
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
