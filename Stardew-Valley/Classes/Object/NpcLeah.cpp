// NpcLeah.cpp
#include "NpcLeah.h"

USING_NS_CC;

NpcLeah* NpcLeah::createWithPosition(const Vec2& position, const std::string& spriteFile)
{
    NpcLeah* npc = new (std::nothrow) NpcLeah();
    if (npc && npc->initWithPosition(position, spriteFile))
    {
        npc->autorelease();
        return npc;
    }
    CC_SAFE_DELETE(npc);
    return nullptr;
}

bool NpcLeah::initWithPosition(const Vec2& position, const std::string& spriteFile)
{
    if (!Sprite::initWithFile(spriteFile))
    {
        return false;
    }

    this->setPosition(position);
    this->setScale(0.5f); // 根据需要调整缩放

    // 初始化对话内容
    _npcDialogues = {
        {"Hello, Jackey", "Leah_1.png"},
        {"You look lonely", "Leah_2.png"},
        {"Here is the thing, there is something I have always wanted to say to you", "Leah_3.png"},
        {"Would you like to be my lover?", "Leah_4.png"}
    };
    _dialogueIndex = 0;
    _dialogueLayer = nullptr;
    _npcFace = nullptr;
    _dialogueLabel = nullptr;

    // 设置触摸监听器
    setupTouchListener();

    return true;
}

void NpcLeah::setupTouchListener()
{
    auto npcListener = EventListenerTouchOneByOne::create();
    npcListener->setSwallowTouches(true);
    npcListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        // 将触摸点转换为NPC的局部坐标
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

void NpcLeah::showDialogue()
{
    if (_dialogueLayer)
    {
        // 对话已经在显示中
        return;
    }

    // 初始化对话层
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    _dialogueLayer = LayerColor::create(Color4B(0, 0, 0, 180),
        visibleSize.width,
        visibleSize.height / 2);
    _dialogueLayer->setPosition(origin.x, origin.y);
    Director::getInstance()->getRunningScene()->addChild(_dialogueLayer, 20);

    // 显示NPC头像
    _npcFace = Sprite::create(_npcDialogues[_dialogueIndex].faceImage);
    _npcFace->setScale(0.5f);
    _npcFace->setAnchorPoint(Vec2(0, 1));
    _npcFace->setPosition(Vec2(10, visibleSize.height / 2 - 10));
    _dialogueLayer->addChild(_npcFace, 1);

    // 显示对话文本
    _dialogueLabel = ui::Text::create("", "Arial", 24);
    _dialogueLabel->setAnchorPoint(Vec2(0, 1));
    _dialogueLabel->setPosition(Vec2(400, visibleSize.height / 2 - 160)); // 修改为右边位置
    _dialogueLabel->setContentSize(Size(
        visibleSize.width + 200, // 适当调整宽度
        visibleSize.height / 2 + 100)); // 适当调整高度
    _dialogueLabel->setColor(Color3B::RED);
    _dialogueLayer->addChild(_dialogueLabel, 1);

    _dialogueLabel->setString(_npcDialogues[_dialogueIndex].text);

    // 设置对话触摸监听器
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
            // 下一句对话
            _dialogueLabel->setString(_npcDialogues[_dialogueIndex].text);
            _npcFace->setTexture(_npcDialogues[_dialogueIndex].faceImage);
        }
        else {
            // 对话结束，显示选项
            this->showOptions();
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(dialogListener, _dialogueLayer);
}

void NpcLeah::showOptions()
{
    if (!_dialogueLayer) return;

    _dialogueLabel->setString(""); // 清空文本
    _npcFace->setTexture("Leah_4.png"); // 将头像设为默认或保持最后头像

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建选项文本
    auto option1Label = Label::createWithSystemFont("God, I can do it.", "Arial", 24);
    auto option2Label = Label::createWithSystemFont("I love you.", "Arial", 24);
    auto option3Label = Label::createWithSystemFont("Sorry, but I have something more important.", "Arial", 24);

    // 创建选项菜单项
    auto option1 = MenuItemLabel::create(option1Label, [this](Ref* sender) {
        CCLOG("选择了 'God, I can do it.'");
        // 根据选择执行对应逻辑...
        this->closeDialogue();
        });

    auto option2 = MenuItemLabel::create(option2Label, [this](Ref* sender) {
        CCLOG("选择了 'I love you.'");
        // 根据选择执行对应逻辑...
        this->closeDialogue();
        });

    auto option3 = MenuItemLabel::create(option3Label, [this](Ref* sender) {
        CCLOG("选择了 'Sorry, but I have something more important.'");
        // 根据选择执行对应逻辑...
        this->closeDialogue();
        });

    // 创建菜单并设置位置
    auto menu = Menu::create(option1, option2, option3, nullptr);
    menu->alignItemsVerticallyWithPadding(20);
    menu->setPosition(visibleSize.width * 0.7f, visibleSize.height / 4);
    _dialogueLayer->addChild(menu, 2);
}

void NpcLeah::closeDialogue()
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
