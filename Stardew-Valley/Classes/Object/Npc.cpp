/****************************************************************
  * 项目名        : Stardew-Valley
  * 文件名        : Npc.cpp
  * 文件功能      : Npc类的实现
  * 作者          : 曹津硕、胡浩杰
  * 更新日期      : 2024/12/19
  * 许可证        : MIT License
****************************************************************/

#include "Npc.h"
// 引入STL头文件，用于使用 std::vector
#include <vector>
// 使用 cocos2d 命名空间
USING_NS_CC;

// 使用指定的位置、名称、对话和选项创建 Npc 对象
Npc* Npc::createWithPosition(const Vec2& position, const std::string& npcName, std::vector<DialogueLine>& npcDialogues, std::vector<std::string>& option)
{
    // 构建精灵文件路径
    std::string spriteFile = npcName + "/" + npcName + ".png";
    // 使用 C++11 的 nothrow，防止内存分配失败抛出异常
    Npc* npc = new (std::nothrow) Npc();
    npc->_npcDialogues = npcDialogues;
    npc->npcName = npcName;
    npc->options = option;
    if (npc && npc->initWithPosition(position, spriteFile))
    {
        npc->autorelease();
        return npc;
    }
    // 安全删除对象
    CC_SAFE_DELETE(npc);
    return nullptr;
}

// 初始化 Npc 对象
bool Npc::initWithPosition(const Vec2& position, const std::string& spriteFile)
{
    // 初始化精灵，如果失败则返回 false
    if (!Sprite::initWithFile(spriteFile))
    {
        return false;
    }

    this->setPosition(position);
    this->setScale(0.5f); // 根据需要调整缩放比例

    // 初始化对话内容
    for (int i = 0; i < 4; i++) {
        // 使用 C++11 的 std::to_string 将整数转换为字符串
        _npcDialogues[i].faceImage = npcName + "/" + npcName + "_" + std::to_string(i + 1) + ".png";
    }
    // 输出调试信息
    CCLOG("npcImage: %s", _npcDialogues[0].faceImage.c_str());
    CCLOG("npcImage: %s", _npcDialogues[1].faceImage.c_str());
    CCLOG("npcImage: %s", _npcDialogues[2].faceImage.c_str());
    CCLOG("npcImage: %s", _npcDialogues[3].faceImage.c_str());
    _dialogueIndex = 0;
    _dialogueLayer = nullptr; // C++11 中使用 nullptr，替代旧的 NULL
    _npcFace = nullptr;
    _dialogueLabel = nullptr;

    // 设置触摸监听器
    setupTouchListener();

    return true;
}

// 设置触摸监听器
void Npc::setupTouchListener()
{
    auto npcListener = EventListenerTouchOneByOne::create();
    npcListener->setSwallowTouches(true);
    // 使用 C++11 的 Lambda 表达式，简化回调函数的定义
    npcListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        // 将触摸点转换为节点坐标
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);
        // 判断触摸点是否在节点范围内
        if (rect.containsPoint(locationInNode))
        {
            this->showDialogue();
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(npcListener, this);
}

// 显示对话
void Npc::showDialogue()
{
    if (_dialogueLayer)
    {
        // 对话已经在显示中
        return;
    }

    // 获取可见区域大小和原点
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建一个半透明的对话层
    _dialogueLayer = LayerColor::create(Color4B(0, 0, 0, 180),
        visibleSize.width,
        visibleSize.height / 2);
    _dialogueLayer->setPosition(origin.x, origin.y);
    Director::getInstance()->getRunningScene()->addChild(_dialogueLayer, 20);

    // 显示 NPC 头像
    _npcFace = Sprite::create(_npcDialogues[_dialogueIndex].faceImage);
    _npcFace->setScale(0.5f);
    _npcFace->setAnchorPoint(Vec2(0, 1));
    _npcFace->setPosition(Vec2(10, visibleSize.height / 2 - 10));
    _dialogueLayer->addChild(_npcFace, 1);

    // 显示对话文本
    _dialogueLabel = ui::Text::create("", "Arial", 24);
    _dialogueLabel->setAnchorPoint(Vec2(0, 1));
    _dialogueLabel->setPosition(Vec2(400, visibleSize.height / 2 - 160));
    _dialogueLabel->setContentSize(Size(
        visibleSize.width + 200,
        visibleSize.height / 2 + 100));
    _dialogueLabel->setColor(Color3B::RED);
    _dialogueLayer->addChild(_dialogueLabel, 1);

    _dialogueLabel->setString(_npcDialogues[_dialogueIndex].text);

    // 设置对话层的触摸监听器
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
            // 显示下一句对话
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

// 显示选项
void Npc::showOptions()
{
    if (!_dialogueLayer) return;

    _dialogueLabel->setString(""); // 清空文本
    _npcFace->setTexture(npcName + "/" + npcName + "_4.png"); // 设置默认头像

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建选项文本标签
    auto option1Label = Label::createWithSystemFont(options[0], "Arial", 24);
    auto option2Label = Label::createWithSystemFont(options[1], "Arial", 24);
    auto option3Label = Label::createWithSystemFont(options[2], "Arial", 24);

    // 创建菜单项，并使用 C++11 的 Lambda 表达式定义回调函数
    auto option1 = MenuItemLabel::create(option1Label, [this](Ref* sender) {
        CCLOG("选择了：'%s'", options[0].c_str());
        this->closeDialogue();
        });

    auto option2 = MenuItemLabel::create(option2Label, [this](Ref* sender) {
        CCLOG("选择了：'%s'", options[1].c_str());
        this->closeDialogue();
        });

    auto option3 = MenuItemLabel::create(option3Label, [this](Ref* sender) {
        CCLOG("选择了：'%s'", options[2].c_str());
        this->closeDialogue();
        });

    // 创建菜单并设置位置
    auto menu = Menu::create(option1, option2, option3, nullptr);
    menu->alignItemsVerticallyWithPadding(20);
    menu->setPosition(visibleSize.width * 0.7f, visibleSize.height / 4);
    _dialogueLayer->addChild(menu, 2);
}

// 关闭对话
void Npc::closeDialogue()
{
    if (_dialogueLayer)
    {
        _dialogueLayer->removeFromParent();
        _dialogueLayer = nullptr; // C++11 中使用 nullptr
        _npcFace = nullptr;
        _dialogueLabel = nullptr;
        _dialogueIndex = 0;
    }
}

