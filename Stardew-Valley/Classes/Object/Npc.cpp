/****************************************************************
  * ��Ŀ��        : Stardew-Valley
  * �ļ���        : Npc.cpp
  * �ļ�����      : Npc���ʵ��
  * ����          : �ܽ�˶�����ƽ�
  * ��������      : 2024/12/19
  * ���֤        : MIT License
****************************************************************/

#include "Npc.h"
// ����STLͷ�ļ�������ʹ�� std::vector
#include <vector>
// ʹ�� cocos2d �����ռ�
USING_NS_CC;

// ʹ��ָ����λ�á����ơ��Ի���ѡ��� Npc ����
Npc* Npc::createWithPosition(const Vec2& position, const std::string& npcName, std::vector<DialogueLine>& npcDialogues, std::vector<std::string>& option)
{
    // ���������ļ�·��
    std::string spriteFile = npcName + "/" + npcName + ".png";
    // ʹ�� C++11 �� nothrow����ֹ�ڴ����ʧ���׳��쳣
    Npc* npc = new (std::nothrow) Npc();
    npc->_npcDialogues = npcDialogues;
    npc->npcName = npcName;
    npc->options = option;
    if (npc && npc->initWithPosition(position, spriteFile))
    {
        npc->autorelease();
        return npc;
    }
    // ��ȫɾ������
    CC_SAFE_DELETE(npc);
    return nullptr;
}

// ��ʼ�� Npc ����
bool Npc::initWithPosition(const Vec2& position, const std::string& spriteFile)
{
    // ��ʼ�����飬���ʧ���򷵻� false
    if (!Sprite::initWithFile(spriteFile))
    {
        return false;
    }

    this->setPosition(position);
    this->setScale(0.5f); // ������Ҫ�������ű���

    // ��ʼ���Ի�����
    for (int i = 0; i < 4; i++) {
        // ʹ�� C++11 �� std::to_string ������ת��Ϊ�ַ���
        _npcDialogues[i].faceImage = npcName + "/" + npcName + "_" + std::to_string(i + 1) + ".png";
    }
    // ���������Ϣ
    CCLOG("npcImage: %s", _npcDialogues[0].faceImage.c_str());
    CCLOG("npcImage: %s", _npcDialogues[1].faceImage.c_str());
    CCLOG("npcImage: %s", _npcDialogues[2].faceImage.c_str());
    CCLOG("npcImage: %s", _npcDialogues[3].faceImage.c_str());
    _dialogueIndex = 0;
    _dialogueLayer = nullptr; // C++11 ��ʹ�� nullptr������ɵ� NULL
    _npcFace = nullptr;
    _dialogueLabel = nullptr;

    // ���ô���������
    setupTouchListener();

    return true;
}

// ���ô���������
void Npc::setupTouchListener()
{
    auto npcListener = EventListenerTouchOneByOne::create();
    npcListener->setSwallowTouches(true);
    // ʹ�� C++11 �� Lambda ���ʽ���򻯻ص������Ķ���
    npcListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        // ��������ת��Ϊ�ڵ�����
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);
        // �жϴ������Ƿ��ڽڵ㷶Χ��
        if (rect.containsPoint(locationInNode))
        {
            this->showDialogue();
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(npcListener, this);
}

// ��ʾ�Ի�
void Npc::showDialogue()
{
    if (_dialogueLayer)
    {
        // �Ի��Ѿ�����ʾ��
        return;
    }

    // ��ȡ�ɼ������С��ԭ��
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // ����һ����͸���ĶԻ���
    _dialogueLayer = LayerColor::create(Color4B(0, 0, 0, 180),
        visibleSize.width,
        visibleSize.height / 2);
    _dialogueLayer->setPosition(origin.x, origin.y);
    Director::getInstance()->getRunningScene()->addChild(_dialogueLayer, 20);

    // ��ʾ NPC ͷ��
    _npcFace = Sprite::create(_npcDialogues[_dialogueIndex].faceImage);
    _npcFace->setScale(0.5f);
    _npcFace->setAnchorPoint(Vec2(0, 1));
    _npcFace->setPosition(Vec2(10, visibleSize.height / 2 - 10));
    _dialogueLayer->addChild(_npcFace, 1);

    // ��ʾ�Ի��ı�
    _dialogueLabel = ui::Text::create("", "Arial", 24);
    _dialogueLabel->setAnchorPoint(Vec2(0, 1));
    _dialogueLabel->setPosition(Vec2(400, visibleSize.height / 2 - 160));
    _dialogueLabel->setContentSize(Size(
        visibleSize.width + 200,
        visibleSize.height / 2 + 100));
    _dialogueLabel->setColor(Color3B::RED);
    _dialogueLayer->addChild(_dialogueLabel, 1);

    _dialogueLabel->setString(_npcDialogues[_dialogueIndex].text);

    // ���öԻ���Ĵ���������
    auto dialogListener = EventListenerTouchOneByOne::create();
    dialogListener->setSwallowTouches(true);
    dialogListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        auto target = event->getCurrentTarget();
        Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());

        Size size = _dialogueLayer->getContentSize();
        Rect rect(0, 0, size.width, size.height);
        if (rect.containsPoint(locationInNode)) {
            return true;
        }
        return false;
        };

    dialogListener->onTouchEnded = [this](Touch* touch, Event* event) {
        _dialogueIndex++;
        if (_dialogueIndex < static_cast<int>(_npcDialogues.size())) {
            // ��ʾ��һ��Ի�
            _dialogueLabel->setString(_npcDialogues[_dialogueIndex].text);
            _npcFace->setTexture(_npcDialogues[_dialogueIndex].faceImage);
        }
        else {
            // �Ի���������ʾѡ��
            this->showOptions();
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(dialogListener, _dialogueLayer);
}

// ��ʾѡ��
void Npc::showOptions()
{
    if (!_dialogueLayer) return;

    _dialogueLabel->setString(""); // ����ı�
    _npcFace->setTexture(npcName + "/" + npcName + "_4.png"); // ����Ĭ��ͷ��

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // ����ѡ���ı���ǩ
    auto option1Label = Label::createWithSystemFont(options[0], "Arial", 24);
    auto option2Label = Label::createWithSystemFont(options[1], "Arial", 24);
    auto option3Label = Label::createWithSystemFont(options[2], "Arial", 24);

    // �����˵����ʹ�� C++11 �� Lambda ���ʽ����ص�����
    auto option1 = MenuItemLabel::create(option1Label, [this](Ref* sender) {
        CCLOG("ѡ���ˣ�'%s'", options[0].c_str());
        this->closeDialogue();
        });

    auto option2 = MenuItemLabel::create(option2Label, [this](Ref* sender) {
        CCLOG("ѡ���ˣ�'%s'", options[1].c_str());
        this->closeDialogue();
        });

    auto option3 = MenuItemLabel::create(option3Label, [this](Ref* sender) {
        CCLOG("ѡ���ˣ�'%s'", options[2].c_str());
        this->closeDialogue();
        });

    // �����˵�������λ��
    auto menu = Menu::create(option1, option2, option3, nullptr);
    menu->alignItemsVerticallyWithPadding(20);
    menu->setPosition(visibleSize.width * 0.7f, visibleSize.height / 4);
    _dialogueLayer->addChild(menu, 2);
}

// �رնԻ�
void Npc::closeDialogue()
{
    if (_dialogueLayer)
    {
        _dialogueLayer->removeFromParent();
        _dialogueLayer = nullptr; // C++11 ��ʹ�� nullptr
        _npcFace = nullptr;
        _dialogueLabel = nullptr;
        _dialogueIndex = 0;
    }
}

