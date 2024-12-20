// NpcLeah.h
#ifndef __NPC_LEAH_H__
#define __NPC_LEAH_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class NpcLeah : public cocos2d::Sprite
{
public:
    // ��������
    static NpcLeah* createWithPosition(const cocos2d::Vec2& position, const std::string& spriteFile = "Leah.png");

    // ��ʼ������
    bool initWithPosition(const cocos2d::Vec2& position, const std::string& spriteFile);

private:
    // �����¼�����
    void setupTouchListener();

    // ��ʾ�Ի�
    void showDialogue();

    // ��ʾѡ��
    void showOptions();

    // �رնԻ�
    void closeDialogue();

    // �Ի���ر���
    cocos2d::LayerColor* _dialogueLayer;
    cocos2d::Sprite* _npcFace;
    cocos2d::ui::Text* _dialogueLabel;
    int _dialogueIndex;

    // �Ի����ݽṹ��
    struct DialogueLine {
        std::string text;
        std::string faceImage;
    };

    std::vector<DialogueLine> _npcDialogues;
};

#endif // __NPC_LEAH_H__
