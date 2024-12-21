// Npc.cpp
#include "Npc.h"
#include <vector>
USING_NS_CC;

Npc* Npc::createWithPosition(const Vec2& position, const std::string& npcName, std::vector<DialogueLine>& npcDialogues,std::vector<std::string>& option)
{
	std::string spriteFile = "../Resources/"+npcName + static_cast<std::string>("/") + npcName + static_cast<std::string>(".png");
    Npc* npc = new (std::nothrow) Npc();
    npc->_npcDialogues = npcDialogues;
    npc->npcName = npcName;
    npc->options = option;
    if (npc && npc->initWithPosition(position, spriteFile))
    {
        npc->autorelease();
        return npc;
    }
    CC_SAFE_DELETE(npc);
    return nullptr;
}

bool Npc::initWithPosition(const Vec2& position, const std::string& spriteFile)
{
    if (!Sprite::initWithFile(spriteFile))
    {
        return false;
    }

    this->setPosition(position);
    this->setScale(0.5f); // ������Ҫ��������

    // ��ʼ���Ի�����

    for (int i = 0; i < 4; i++) {
        _npcDialogues[i].faceImage = npcName + "/" + npcName + "_" + std::to_string(i + 1) + ".png";
    }
	CCLOG("npcImage: %s", _npcDialogues[0].faceImage.c_str());
	CCLOG("npcImage: %s", _npcDialogues[1].faceImage.c_str());
	CCLOG("npcImage: %s", _npcDialogues[2].faceImage.c_str());
	CCLOG("npcImage: %s", _npcDialogues[3].faceImage.c_str());
    _dialogueIndex = 0;
    _dialogueLayer = nullptr;
    _npcFace = nullptr;
    _dialogueLabel = nullptr;

    // ���ô���������
    setupTouchListener();

    return true;
}

void Npc::setupTouchListener()
{
    auto npcListener = EventListenerTouchOneByOne::create();
    npcListener->setSwallowTouches(true);
    npcListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        // ��������ת��ΪNPC�ľֲ�����
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);
        if (rect.containsPoint(locationInNode))
        {
            this->showDialogue();
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(npcListener, this);
}

void Npc::showDialogue()
{
    if (_dialogueLayer)
    {
        // �Ի��Ѿ�����ʾ��
        return;
    }

    // ��ʼ���Ի���
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    _dialogueLayer = LayerColor::create(Color4B(0, 0, 0, 180),
        visibleSize.width,
        visibleSize.height / 2);
    _dialogueLayer->setPosition(origin.x, origin.y);
    Director::getInstance()->getRunningScene()->addChild(_dialogueLayer, 20);

    // ��ʾNPCͷ��
    _npcFace = Sprite::create(_npcDialogues[_dialogueIndex].faceImage);
    _npcFace->setScale(0.5f);
    _npcFace->setAnchorPoint(Vec2(0, 1));
    _npcFace->setPosition(Vec2(10, visibleSize.height / 2 - 10));
    _dialogueLayer->addChild(_npcFace, 1);

    // ��ʾ�Ի��ı�
    _dialogueLabel = ui::Text::create("", "Arial", 24);
    _dialogueLabel->setAnchorPoint(Vec2(0, 1));
    _dialogueLabel->setPosition(Vec2(400, visibleSize.height / 2 - 160)); // �޸�Ϊ�ұ�λ��
    _dialogueLabel->setContentSize(Size(
        visibleSize.width + 200, // �ʵ��������
        visibleSize.height / 2 + 100)); // �ʵ������߶�
    _dialogueLabel->setColor(Color3B::RED);
    _dialogueLayer->addChild(_dialogueLabel, 1);

    _dialogueLabel->setString(_npcDialogues[_dialogueIndex].text);

    // ���öԻ�����������
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
            // ��һ��Ի�
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

void Npc::showOptions()
{
    if (!_dialogueLayer) return;

    _dialogueLabel->setString(""); // ����ı�
    _npcFace->setTexture(npcName + "/" + npcName + "_4.png"); // ��ͷ����ΪĬ�ϻ򱣳����ͷ��

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // ����ѡ���ı�
    auto option1Label = Label::createWithSystemFont(options[0], "Arial", 24);
    auto option2Label = Label::createWithSystemFont(options[1], "Arial", 24);
    auto option3Label = Label::createWithSystemFont(options[2], "Arial", 24);

    // ����ѡ��˵���
    auto option1 = MenuItemLabel::create(option1Label, [this](Ref* sender) {
        CCLOG("ѡ���� 'God, I can do it.'");
        // ����ѡ��ִ�ж�Ӧ�߼�...
        this->closeDialogue();
        });

    auto option2 = MenuItemLabel::create(option2Label, [this](Ref* sender) {
        CCLOG("ѡ���� 'I love you.'");
        // ����ѡ��ִ�ж�Ӧ�߼�...
        this->closeDialogue();
        });

    auto option3 = MenuItemLabel::create(option3Label, [this](Ref* sender) {
        CCLOG("ѡ���� 'Sorry, but I have something more important.'");
        // ����ѡ��ִ�ж�Ӧ�߼�...
        this->closeDialogue();
        });

    // �����˵�������λ��
    auto menu = Menu::create(option1, option2, option3, nullptr);
    menu->alignItemsVerticallyWithPadding(20);
    menu->setPosition(visibleSize.width * 0.7f, visibleSize.height / 4);
    _dialogueLayer->addChild(menu, 2);
}

void Npc::closeDialogue()
{
    if (_dialogueLayer)
    {
        _dialogueLayer->removeFromParent();
        _dialogueLayer = nullptr;
        _npcFace = nullptr;
        _dialogueLabel = nullptr;
        _dialogueIndex = 0;
    }
}
