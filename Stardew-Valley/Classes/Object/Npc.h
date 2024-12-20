// Npc.h
#ifndef __NPC_H__
#define __NPC_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
struct DialogueLine {
    std::string text;
    std::string faceImage;
};
class Npc : public cocos2d::Sprite
{
private:
    // �Ի���ر���
    cocos2d::LayerColor* _dialogueLayer;
    cocos2d::Sprite* _npcFace;
    cocos2d::ui::Text* _dialogueLabel;
    std::vector<std::string> options;;
    int _dialogueIndex;


    std::vector<DialogueLine> _npcDialogues;
    // �����¼�����
    void setupTouchListener();

    // ��ʾ�Ի�
    void showDialogue();

    // ��ʾѡ��
    void showOptions();

    // �رնԻ�
    void closeDialogue();

public:
    std::string npcName;
    // ��������
    static Npc* createWithPosition(const cocos2d::Vec2& position, const std::string& npcName
                                  , std::vector<DialogueLine>& npcDialogues
                                  , std::vector<std::string>& option);

    // ��ʼ������
    bool initWithPosition(const cocos2d::Vec2& position, const std::string& spriteFile);

};

#endif // __NPC_H__
