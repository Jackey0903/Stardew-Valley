/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : Map2Scene.h
 * �ļ�����      : ����2������Ĺ���ʵ��
 * ����          : ���ƽܡ��ܽ�˶��������
 * ��������      : 2024/12/21
 * ���֤        : MIT License
 ****************************************************************/

#ifndef __MAP2_SCENE_H__    // ��ֹ�ظ�����ͷ�ļ�
#define __MAP2_SCENE_H__

#include "BaseMapScene.h"    // ������ೡ��
#include "Object/Npc.h"      // ����NPC��

 // Map2Scene ��̳��� BaseMapScene��ʵ���˳���2�ľ��幦��
class Map2Scene : public BaseMapScene
{
public:
    // ����������һ�� Map2Scene ʵ��
    static cocos2d::Scene* createScene();

    // ��ʼ�������������Ի���
    virtual bool init() override;

    // ���볡��ʱ���ã������Ի���
    virtual void onEnter() override;

    // ʹ�ú괴�� Map2Scene ����
    CREATE_FUNC(Map2Scene);

private:
    bool _isPlanting;                             // �Ƿ�������ֲ����
    bool _isPlantingTree;                         // �Ƿ�������ֲ��ľ
    std::vector<cocos2d::Sprite*> _crops;         // �洢���������
    std::vector<cocos2d::Sprite*> _trees;         // �洢��ľ������
    cocos2d::Sprite* _npc;                        // NPC�������

    // ���ڶԻ�����ʾ�ĳ�Ա����
    cocos2d::LayerColor* _dialogueLayer = nullptr;   // �Ի��򱳾���
    cocos2d::Sprite* _npcFace = nullptr;             // NPCͷ����
    cocos2d::ui::Text* _dialogueLabel = nullptr;     // ��ʾ�Ի��ı��Ŀؼ�
    int _dialogueIndex = 0;                          // ��ǰ��ʾ�ĶԻ��������

    std::vector<DialogueLine> _npcLewisDialogues;    // NPC Lewis�ĶԻ�����
    std::vector<std::string> LewisOption;            // NPC Lewis�ĶԻ�ѡ��

    // ��ʼ���¼������������̡���꣩
    void initEventListeners();

    // ���̰����¼��ص�
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // �����ͷ��¼��ص�
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // ��갴���¼��ص�
    void onMouseDown(cocos2d::Event* event);

    // ��ֲ����ĺ���
    void plantCropAt(const cocos2d::Vec2& locationInMap);

    // �ո�����ĺ���
    void harvestCropAt(const cocos2d::Vec2& locationInMap);

    // ��ֲ��ľ�ĺ���
    void plantTreeAt(const cocos2d::Vec2& locationInMap);

    // ������ľ�ĺ���
    void fellTreeAt(const cocos2d::Vec2& locationInMap);
};

#endif  // __MAP2_SCENE_H__
