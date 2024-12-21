// Map2Scene.h
#ifndef __MAP2_SCENE_H__
#define __MAP2_SCENE_H__

#include "BaseMapScene.h"
#include "Object/Npc.h"
class Map2Scene : public BaseMapScene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void onEnter() override;

    CREATE_FUNC(Map2Scene);

private:
    bool _isPlanting;
    bool _isPlantingTree;
    std::vector<cocos2d::Sprite*> _crops;
    std::vector<cocos2d::Sprite*> _trees;
    void initEventListeners();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void plantCropAt(const cocos2d::Vec2& locationInMap);
    void harvestCropAt(const cocos2d::Vec2& locationInMap);
    void plantTreeAt(const cocos2d::Vec2& locationInMap);
    void fellTreeAt(const cocos2d::Vec2& locationInMap);
    cocos2d::Sprite* _npc;
    void showOptions();
    void closeDialogue();
    // ���ڶԻ�����ʾ�ĳ�Ա����
    cocos2d::LayerColor* _dialogueLayer = nullptr;   // �Ի��򱳾���
    cocos2d::Sprite* _npcFace = nullptr;             // NPCͷ����
    cocos2d::ui::Text* _dialogueLabel = nullptr;     // ��ʾ�Ի��ı��Ŀؼ�
    int _dialogueIndex = 0;                          // ��ǰ��ʾ�ĶԻ��������
	std::vector<DialogueLine> _npcLewisDialogues;
	std::vector<std::string> LewisOption;
};

#endif
