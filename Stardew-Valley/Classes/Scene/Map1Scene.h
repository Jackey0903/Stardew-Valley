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

    // ���ڶԻ�����ʾ�ĳ�Ա����
    cocos2d::LayerColor* _dialogueLayer = nullptr;   // �Ի��򱳾���
    cocos2d::Sprite* _npcFace = nullptr;             // NPCͷ����
    cocos2d::ui::Text* _dialogueLabel = nullptr;     // ��ʾ�Ի��ı��Ŀؼ�
    int _dialogueIndex = 0;                          // ��ǰ��ʾ�ĶԻ��������
    void showDialogue();
};

#endif
