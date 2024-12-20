// Map1Scene.cpp
#include "Scene/Map1Scene.h"
#include "ui/CocosGUI.h"
#include "Player/Player.h"
#include "MapLayer.h"
#include "Object/Mine.h"
#include "Object/Npc.h"
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
    _isCollectingStone = false;
    // 初始化事件监听器
    initEventListeners();
    this->scheduleUpdate();
    return true;
}

void Map1Scene::addManualMines()
{
    // 定义矿石的位置（根据地图的坐标系）
    std::vector<Vec2> minePositions = {
        Vec2(80, 100),
        Vec2(90, 120),
        Vec2(120, 70),
        Vec2(150, 90),
        Vec2(200, 190),
        Vec2(170, 270),
        Vec2(200, 190),
        Vec2(275, 200),
        Vec2(250, 190),
        Vec2(200, 270),
        Vec2(250, 270),
        Vec2(150, 250),
    };

    for (const auto& pos : minePositions)
    {
        // 创建 Mine 实例
        std::vector<std::string> a = { "Mine_1.png" ,"Mine_2.png" ,"Mine_3.png" };
        auto mine = Mine::createWithPosition(pos, a[random() % a.size()]);
        if (mine)
        {
            mine->setScale(0.1f); // 根据需要调整缩放比例
            _tiledMap->addChild(mine, 30); // 添加到地图层，
            _mines.push_back(mine); // 添加到矿石列表
        }
        else
        {
            CCLOG("Failed to create mine at position: (%f, %f)", pos.x, pos.y);
        }
    }

    CCLOG("Manual mines added: %lu", _mines.size());
}

void Map1Scene::onEnter()
{
    BaseMapScene::onEnter();

    // 在Map1上查找npc对象层并创建NPC
    if (_tiledMap)
    {
        // 获取名为"Npc"的对象层
        auto npcLayer = _tiledMap->getObjectGroup("Npc_Demi");
        if (npcLayer)
        {
            auto npcObjects = npcLayer->getObjects();
            for (auto& obj : npcObjects)
            {
                auto npcObject = obj.asValueMap();
                if (npcObject["name"].asString() == "Npc_Demi")
                {
                    // 获取NPC在地图坐标系中的位置（Tiled左下为(0,0)）
                    float npcX = npcObject["x"].asFloat();
                    float npcY = npcObject["y"].asFloat();
                    Vec2 npcPosition(npcX, npcY);

                    _npcDemiDialogues = {
                   {"Hey,buddy!", ""},
                   {"My name is Demi,and I am a miner.", ""},
                   {"There are many minerals here that can be mined.", ""},
                   {"Do you want to join me?", ""}
                    };
                    DemiOption = { {"Great! I want to join you."},{"No,thank you.I plan to leave here."},{"Never mind,I’ll just mine it myself."} };
                    // 创建 NpcLeah 实例并添加到地图
                    auto npcDemi = Npc::createWithPosition(npcPosition, "Demi", _npcDemiDialogues, DemiOption);
                    if (npcDemi)
                    {
                        // 根据需要调整缩放或其他属性
                        npcDemi->setScale(0.1f); // 增大NPC的缩放，使点击区域更大
                        _tiledMap->addChild(npcDemi, 25);
                    }
                }
            }
        }
        addManualMines();
        initEventListeners();
        this->scheduleUpdate();
    }
}

void Map1Scene::initEventListeners()
{
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Map1Scene::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Map1Scene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Map1Scene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Map1Scene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

    if (keyCode == EventKeyboard::KeyCode::KEY_U)
    {
        _isCollectingStone = true;
    }
}

void Map1Scene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_U)
    {
        _isCollectingStone = false;
    }
}

void Map1Scene::onMouseDown(Event* event)
{
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);

    auto mouseButton = mouseEvent->getMouseButton();
    Vec2 locationInView(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    auto locationInMap = _tiledMap->convertToNodeSpace(locationInView);
    CCLOG("ok");

    if (_isCollectingStone)
    {
        if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT)
        {
            collectStoneAt(locationInMap);
        }
    }
}

void Map1Scene::collectStoneAt(const Vec2& locationInMap)
{
    Vec2 playerPositionInMap = _tiledMap->convertToNodeSpace(_player->getPosition());
    /*playerPositionInMap.x += 300.0f;
    playerPositionInMap.y += 150.0f;*/

    CCLOG("ok");
    // 创建斧子精灵
    auto hoeSprite = Sprite::create("Hoe_1.png");
    hoeSprite->setScale(1.0f);
    hoeSprite->setAnchorPoint(Vec2(0.5f, 0.5f));

    // 将斧子添加为_playerSprite的子节点
    _player->getPlayerSprite()->addChild(hoeSprite, 40);

    // 设置相对于玩家精灵的偏移位置（可根据需要调整）
    hoeSprite->setPosition(Vec2(60, 120)); // 例如在玩家右手边稍上方
    // 创建动画帧
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrame::create("Hoe_1.png", Rect(0, 0, hoeSprite->getContentSize().width, hoeSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("Hoe_2.png", Rect(0, 0, hoeSprite->getContentSize().width, hoeSprite->getContentSize().height)));
    CCLOG("%f %f", playerPositionInMap.x, playerPositionInMap.y);
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);

    auto finish = CallFunc::create([hoeSprite]() {
        hoeSprite->removeFromParent(); // 动画结束时移除
        });

    // 播放动画
    hoeSprite->runAction(Sequence::create(animate, finish, nullptr));

    for (auto it = _mines.begin(); it != _mines.end(); ++it)
    {
        auto mine = *it;
        Rect boundingBox = mine->getBoundingBox();
        if (boundingBox.containsPoint(locationInMap))
        {
            mine->removeFromParent();
            _mines.erase(it);

            break;
        }
    }
}
