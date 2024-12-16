// Map1Scene.cpp
#include "Map1Scene.h"

USING_NS_CC;

Scene* Map1Scene::createScene()
{
    return Map1Scene::create();
}

bool Map1Scene::init()
{
    if (!BaseMapScene::init())
        return false;

    // 设置地图文件和缩放
    loadMap("Map/Map1/map1.tmx", 5.0f);
    initPlayer();

    return true;
}

void Map1Scene::onEnter()
{
    BaseMapScene::onEnter();

    // 在Map3上查找npc对象层并创建NPC
    if (_tiledMap)
    {
        // 获取名为"Npc"的对象层
        auto npcLayer = _tiledMap->getObjectGroup("Npc");
        if (npcLayer)
        {
            auto npcObjects = npcLayer->getObjects();
            for (auto& obj : npcObjects)
            {
                auto npcObject = obj.asValueMap();
                if (npcObject["name"].asString() == "Npc")
                {
                    // 获取NPC在地图坐标系中的位置（Tiled左下为(0,0)）
                    float npcX = npcObject["x"].asFloat();
                    float npcY = npcObject["y"].asFloat();
                    Vec2 npcPosition(npcX, npcY);

                    _npc = Sprite::create("Bouncer.png");
                    _npc->setScale(1.0f); // 增大NPC的缩放，使点击区域更大
                    _npc->setPosition(npcPosition);
                    _tiledMap->addChild(_npc, 15);


                    // 设置点击事件监听器
                    auto npcListener = EventListenerTouchOneByOne::create();
                    npcListener->setSwallowTouches(true);
                    npcListener->onTouchBegan = [this](Touch* touch, Event* event) {
                        // 将屏幕坐标转换为_tiledMap坐标
                        auto locationInView = touch->getLocation();
                        auto locationInMap = _tiledMap->convertToNodeSpace(locationInView);
                        CCLOG("Touch at screen: (%.2f,%.2f)", locationInView.x, locationInView.y);
                        CCLOG("Touch in map coords: (%.2f,%.2f)", locationInMap.x, locationInMap.y);

                        Rect boundingBox = _npc->getBoundingBox();
                        CCLOG("NPC boundingBox: origin(%.2f,%.2f), size(%.2f,%.2f)",
                            boundingBox.origin.x, boundingBox.origin.y,
                            boundingBox.size.width, boundingBox.size.height);
                        // boundingBox与locationInMap均在_tiledMap坐标系中
                        if (boundingBox.containsPoint(locationInMap))
                        {
                            showDialogue();
                            return true;
                        }
                        return false;
                        };
                    _eventDispatcher->addEventListenerWithSceneGraphPriority(npcListener, _npc);
                }
            }
        }
    }
}

void Map1Scene::showDialogue()
{
    // 对话内容
    static std::vector<std::string> npcDialogues = {
        "Hey，man！",
        "Wish u can do it",
        "Need Help？",
        "Ok，bye！"
    };
    static int currentIndex = 0; // 当前对话索引

    // 如果对话框已经存在并在显示中，则点击可以继续下一句
    // 为此，可以将对话框定义为Map3Scene的成员变量，让后续点击可访问
    // 若这只是第一次创建，可在Map3Scene.h中声明：
    // LayerColor* _dialogueLayer;
    // Label* _dialogueLabel;
    // Sprite* _npcFace;
    // int _dialogueIndex;
    // npcDialogues为static本地也行，或作为成员变量。

    // 如果对话框不存在，创建对话框UI
    if (!_dialogueLayer)
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();

        // 创建半屏高的对话框背景（下半屏）
        _dialogueLayer = LayerColor::create(Color4B(0, 0, 0, 180),
            visibleSize.width,
            visibleSize.height / 2);
        _dialogueLayer->setPosition(origin.x, origin.y);
        this->addChild(_dialogueLayer, 20);

        // 在对话框左侧放NPC头像
        _npcFace = Sprite::create("npc_face.png"); // 替换为您的NPC头像图片
        _npcFace->setScale(2.0f); // 放大头像
        _npcFace->setAnchorPoint(Vec2(0, 1));
        _npcFace->setPosition(Vec2(10, visibleSize.height / 2 - 10));
        _dialogueLayer->addChild(_npcFace, 1);

        // 在对话框右侧放对话文本Label
        _dialogueLabel = cocos2d::ui::Text::create("", "Arial", 24);
        _dialogueLabel->setAnchorPoint(Vec2(0, 1));
        _dialogueLabel->setPosition(Vec2(_npcFace->getContentSize().width * 2 + 30,
            visibleSize.height / 2 - 10));
        _dialogueLabel->setContentSize(Size(
            visibleSize.width - (_npcFace->getContentSize().width * 2 + 50),
            visibleSize.height / 2 - 20));
        // 给Label设置多行显示，dimensions使其自动换行
        _dialogueLabel->setColor(Color3B::WHITE);
        _dialogueLayer->addChild(_dialogueLabel, 1);

        // 初始化对话索引
        _dialogueIndex = 0;
        _dialogueLabel->setString(npcDialogues[_dialogueIndex]);

        // 为对话框添加点击事件（整个对话框区域可点）
        auto dialogListener = EventListenerTouchOneByOne::create();
        dialogListener->setSwallowTouches(true);
        dialogListener->onTouchBegan = [this](Touch* touch, Event* event) {
            auto target = event->getCurrentTarget();
            Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());

            // 获取对话框大小来判断点击是否在其范围内
            Size size = _dialogueLayer->getContentSize();
            Rect rect(0, 0, size.width, size.height);
            if (rect.containsPoint(locationInNode)) {
                return true; // 在对话框区域内，消耗触摸
            }
            return false;
            };

        dialogListener->onTouchEnded = [this](Touch* touch, Event* event) {
            // 点击对话框后显示下一句对话
            _dialogueIndex++;
            if (_dialogueIndex < (int)npcDialogues.size()) {
                _dialogueLabel->setString(npcDialogues[_dialogueIndex]);
            }
            else {
                // 对话结束，移除对话框
                _dialogueLayer->removeFromParent();
                _dialogueLayer = nullptr;
                _npcFace = nullptr;
                _dialogueLabel = nullptr;
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(dialogListener, _dialogueLayer);
    }
    else
    {
        // 如果对话框已经存在，但此种逻辑下不需要额外处理
        // 因为点击逻辑由onTouchEnded处理
    }
}