/****************************************************************
  * ��Ŀ��        : Stardew-Valley
  * �ļ���        : Npc.h
  * �ļ�����      : Npc���ʵ��
  * ����          : �ܽ�˶�����ƽ�
  * ��������      : 2024/12/19
  * ���֤        : MIT License
****************************************************************/

#ifndef __NPC_H__
#define __NPC_H__

#include "cocos2d.h"
// ���� UI ���ͷ�ļ�
#include "ui/CocosGUI.h"

// ����Ի��нṹ�壬�����ı���ͷ��ͼ��
struct DialogueLine {
    std::string text;       // �Ի��ı�
    std::string faceImage;  // ͷ��ͼ���ļ�·��
};

// ���� Npc �࣬�̳��� cocos2d::Sprite
class Npc : public cocos2d::Sprite
{
private:
    // �Ի���ر���
    cocos2d::LayerColor* _dialogueLayer;        // �Ի���
    cocos2d::Sprite* _npcFace;                  // NPC ͷ��
    cocos2d::ui::Text* _dialogueLabel;          // �Ի��ı���ǩ
    std::vector<std::string> options;           // ѡ���б�
    int _dialogueIndex;                         // ��ǰ�Ի�����

    std::vector<DialogueLine> _npcDialogues;    // NPC �Ի�����

    // �����¼�������
    void setupTouchListener();

    // ��ʾ�Ի�
    void showDialogue();

    // ��ʾѡ��
    void showOptions();

    // �رնԻ�
    void closeDialogue();

public:
    std::string npcName;  // NPC ����

    // ����������ʹ��ָ��λ�úͲ������� Npc ����
    static Npc* createWithPosition(
        const cocos2d::Vec2& position,
        const std::string& npcName,
        std::vector<DialogueLine>& npcDialogues,
        std::vector<std::string>& option);

    // ��ʼ������
    bool initWithPosition(const cocos2d::Vec2& position, const std::string& spriteFile);

};

#endif // __NPC_H__

