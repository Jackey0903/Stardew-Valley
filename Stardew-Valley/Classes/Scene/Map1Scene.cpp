/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Map1Scene.cpp
 * 文件功能      : 场景1派生类的功能实现
 * 作者          : 胡浩杰、曹津硕、胡正华
 * 更新日期      : 2024/12/21
 * 许可证        : MIT License
 ****************************************************************/

#include "Scene/Map1Scene.h"
#include "ui/CocosGUI.h"          // 引入Cocos2d UI模块
#include "Player/Player.h"        // 引入Player类
#include "MapLayer.h"             // 引入MapLayer类
#include "Object/Mine.h"         // 引入Mine类
#include "Object/Npc.h"          // 引入Npc类
USING_NS_CC;

Scene* Map1Scene::createScene()
{
    return Map1Scene::create();  // 创建并返回Map1Scene实例
}

bool Map1Scene::init()
{
    if (!BaseMapScene::init())   // 调用基类初始化函数，确保初始化成功
        return false;

    // 设置地图文件和缩放比例
    loadMap("Map/Map1/map1.tmx", 5.0f);
    initPlayer();                // 初始化玩家
    _isCollectingStone = false;  // 初始化是否正在收集矿石

    // 初始化事件监听器
    initEventListeners();
    this->scheduleUpdate();      // 调度更新函数

    return true;
}

void Map1Scene::addManualMines()
{
    // 定义矿石的位置（根据地图的坐标系）
    std::vector<Vec2> minePositions = {
        Vec2(80, 100), Vec2(90, 120), Vec2(120, 70),
        Vec2(150, 90), Vec2(200, 190), Vec2(170, 270),
        Vec2(200, 190), Vec2(275, 200), Vec2(250, 190),
        Vec2(200, 270), Vec2(250, 270), Vec2(150, 250)
    };

    // 遍历矿石位置并创建矿石实例
    for (const auto& pos : minePositions)
    {
        std::vector<std::string> a = { "Mine_1.png", "Mine_2.png", "Mine_3.png" };
        auto mine = Mine::createWithPosition(pos, a[random() % a.size()]);
        if (mine)
        {
            mine->setScale(0.1f);  
            _tiledMap->addChild(mine, 30);  // 将矿石添加到地图层
            _mines.push_back(mine);  // 将矿石添加到矿石列表
        }
        else
        {
            CCLOG("Failed to create mine at position: (%f, %f)", pos.x, pos.y); 
        }
    }

    CCLOG("Manual mines added: %lu", _mines.size());  // 输出矿石数量
}

void Map1Scene::onEnter()
{
    BaseMapScene::onEnter();   // 调用基类的onEnter方法

    if (_tiledMap)
    {
        auto npcLayer = _tiledMap->getObjectGroup("Npc_Demi");
        if (npcLayer)
        {
            // 遍历对象层中的所有对象
            auto npcObjects = npcLayer->getObjects();
            for (auto& obj : npcObjects)
            {
                auto npcObject = obj.asValueMap();
                if (npcObject["name"].asString() == "Npc_Demi")
                {
                    // 获取NPC在地图坐标系中的位置
                    float npcX = npcObject["x"].asFloat();
                    float npcY = npcObject["y"].asFloat();
                    Vec2 npcPosition(npcX, npcY);

                    // 定义NPC对话内容
                    _npcDemiDialogues = {
                        {"Hey,buddy!", ""},
                        {"My name is Demi,and I am a miner.", ""},
                        {"There are many minerals here that can be mined.", ""},
                        {"Do you want to join me?", ""}
                    };

                    DemiOption = {
                        {"Great! I want to join you."},
                        {"No,thank you.I plan to leave here."},
                        {"Never mind,I’ll just mine it myself."}
                    };

                    // 创建 NPC 并添加到地图中
                    auto npcDemi = Npc::createWithPosition(npcPosition, "Demi", _npcDemiDialogues, DemiOption);
                    if (npcDemi)
                    {
                        npcDemi->setScale(0.1f);  // 增大NPC的缩放，使点击区域更大
                        _tiledMap->addChild(npcDemi, 25);  // 将NPC添加到地图层
                    }
                }
            }
        }

        addManualMines();   
        initEventListeners(); // 初始化事件监听器
        this->scheduleUpdate();  // 调度更新
    }
}

void Map1Scene::initEventListeners()
{
    // 初始化键盘事件监听器
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Map1Scene::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Map1Scene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // 初始化鼠标事件监听器
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Map1Scene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Map1Scene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_U)
    {
        _isCollectingStone = true;  // 按U键开始收集矿石
    }
}

void Map1Scene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_U)
    {
        _isCollectingStone = false; // 松开U键停止收集矿石
    }
}

void Map1Scene::onMouseDown(Event* event)
{
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
    auto mouseButton = mouseEvent->getMouseButton();
    Vec2 locationInView(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    auto locationInMap = _tiledMap->convertToNodeSpace(locationInView);  // 将视图坐标转换为地图坐标

    if (_isCollectingStone)
    {
        if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT)
        {
            collectStoneAt(locationInMap);  // 如果按下鼠标左键，则进行收集矿石
        }
    }
}

void Map1Scene::collectStoneAt(const Vec2& locationInMap)
{
    Vec2 playerPositionInMap = _tiledMap->convertToNodeSpace(_player->getPosition());

    auto hoeSprite = Sprite::create("Hoe_1.png");
    hoeSprite->setScale(1.0f);
    hoeSprite->setAnchorPoint(Vec2(0.5f, 0.5f));

    // 将斧子精灵添加到玩家精灵的子节点中
    _player->getPlayerSprite()->addChild(hoeSprite, 40);
    hoeSprite->setPosition(Vec2(60, 120));  // 设置斧子的位置（玩家右手边）

    // 创建动画帧
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrame::create("Hoe_1.png", Rect(0, 0, hoeSprite->getContentSize().width, hoeSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("Hoe_2.png", Rect(0, 0, hoeSprite->getContentSize().width, hoeSprite->getContentSize().height)));
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);

    // 动画结束后移除精灵
    auto finish = CallFunc::create([hoeSprite]() {
        hoeSprite->removeFromParent();
        });

    // 播放动画
    hoeSprite->runAction(Sequence::create(animate, finish, nullptr));

    // 检查点击位置是否有矿石，若有则移除矿石
    for (auto it = _mines.begin(); it != _mines.end(); ++it)
    {
        auto mine = *it;
        Rect boundingBox = mine->getBoundingBox();
        if (boundingBox.containsPoint(locationInMap))
        {
            mine->removeFromParent();  // 移除矿石
            _mines.erase(it);          // 从矿石列表中删除该矿石
            break;
        }
    }
}
