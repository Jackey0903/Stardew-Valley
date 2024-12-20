// GameScene.cpp
#include "Scene/GameScene.h"
#include "MapLayer.h"
#include "Player/Player.h"
#include "ui/CocosGUI.h"  // 导入Cocos2d的UI模块

USING_NS_CC;

extern std::string g_selectedMap;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
        return false;

    _isPlanting = false; // 初始不处于种植模式
    _npc = nullptr;

    currentMap = "Map/Map2/map2.tmx";

    auto mapLayer = MapLayer::create();
    mapLayer->setName("MapLayer");
    this->addChild(mapLayer, 0);
    //mapLayer->loadMap(currentMap);

    auto tiledMap = mapLayer->getTMXTiledMap();

    // 设置地图的锚点为中心
    tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f));

    // 获取屏幕的可见区域大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 计算地图的缩放比例，使其铺满窗口
    float scaleX = visibleSize.width / tiledMap->getContentSize().width * 2.4;
    float scaleY = visibleSize.height / tiledMap->getContentSize().height * 2.4;
    float scale = MAX(scaleX, scaleY); // 选择较大的缩放比例，确保地图铺满窗口
    tiledMap->setScale(scale);

    // 将地图置于屏幕中心
    tiledMap->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

    if (!tiledMap)
    {
        CCLOG("loaded ");
    }

    _player = Player::create();
    this->addChild(_player, 10); // 确保玩家在地图之上

    _player->setTiledMap(tiledMap);
    _player->setInitPositionMap(tiledMap->getPosition());

    // 初始化事件监听器（键盘、鼠标）
    initEventListeners();

    this->scheduleUpdate();
    
    return true;
}

void GameScene::onEnter()
{ 
    Scene::onEnter();
   
    if (!g_selectedMap.empty() && g_selectedMap != currentMap)
    {
        CCLOG("GameScene: 切换到新地图: %s", g_selectedMap.c_str());
        currentMap = g_selectedMap;

        auto mapLayer = dynamic_cast<MapLayer*>(this->getChildByName("MapLayer"));
        if (mapLayer)
        {
            //mapLayer->loadMap(currentMap);
            auto tiledMap = mapLayer->getTMXTiledMap();

            if (currentMap == "Map/Map1/map1.tmx")
            {
                // 设置地图的锚点为中心
                tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f));

                // 获取屏幕的可见区域大小
                auto visibleSize = Director::getInstance()->getVisibleSize();
                auto origin = Director::getInstance()->getVisibleOrigin();

                // 计算地图的缩放比例，使其铺满窗口
                float scaleX = visibleSize.width / tiledMap->getContentSize().width * 1.5;
                float scaleY = visibleSize.height / tiledMap->getContentSize().height * 1.5;
                float scale = MAX(scaleX, scaleY); // 选择较大的缩放比例，确保地图铺满窗口
                tiledMap->setScale(scale);

                // 将地图置于屏幕中心
                tiledMap->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
            }
            else if (currentMap == "Map/Map2/map2.tmx")
            {
                // 设置地图的锚点为中心
                tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f));

                // 获取屏幕的可见区域大小
                auto visibleSize = Director::getInstance()->getVisibleSize();
                auto origin = Director::getInstance()->getVisibleOrigin();

                // 计算地图的缩放比例，使其铺满窗口
                float scaleX = visibleSize.width / tiledMap->getContentSize().width * 2.4;
                float scaleY = visibleSize.height / tiledMap->getContentSize().height * 2.4;
                float scale = MAX(scaleX, scaleY); // 选择较大的缩放比例，确保地图铺满窗口
                tiledMap->setScale(scale);

                // 将地图置于屏幕中心
                tiledMap->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
            }

            else if (currentMap == "Map/Map3/map3.tmx")
            {
                // 设置地图的锚点为中心
                tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f));

                // 获取屏幕的可见区域大小
                auto visibleSize = Director::getInstance()->getVisibleSize();
                auto origin = Director::getInstance()->getVisibleOrigin();

                // 计算地图的缩放比例，使其铺满窗口
                float scaleX = visibleSize.width / tiledMap->getContentSize().width * 0.8;
                float scaleY = visibleSize.height / tiledMap->getContentSize().height * 0.8;
                float scale = MAX(scaleX, scaleY); // 选择较大的缩放比例，确保地图铺满窗口
                tiledMap->setScale(scale);

                // 将地图置于屏幕中心
                tiledMap->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

                // 在地图对象层中查找NPC
                auto npcLayer = tiledMap->getObjectGroup("Npc"); // 获取对象层
                if (npcLayer)
                {
                    auto npcObjects = npcLayer->getObjects(); // 获取所有对象

                    for (const auto& obj : npcObjects)
                    {// 将 Value 转换为 ValueMap
                        auto npcObject = obj.asValueMap();
                        // 检查是否为NPC对象
                        if (npcObject["name"].asString() == "Npc") // 假设你的NPC对象名称为"NPC1"
                        {
                            Vec2 npcPosition = Vec2(npcObject["x"].asFloat(), npcObject["y"].asFloat());
                            _npc = Sprite::create("npc.png"); // 替换为NPC的实际图片路径
                            _npc->setPosition(npcPosition);
                            tiledMap->addChild(_npc, 5);

                            // 设置点击事件
                            auto npcListener = EventListenerTouchOneByOne::create();
                            npcListener->setSwallowTouches(true);
                            npcListener->onTouchBegan = [this](Touch* touch, Event* event) {
                                // 检查点击位置是否在NPC精灵区域内
                                Rect boundingBox = _npc->getBoundingBox();
                                if (boundingBox.containsPoint(touch->getLocation()))
                                {
                                    showDialogue();  // 显示对话框
                                    return true;
                                }
                                return false;
                                };
                            _eventDispatcher->addEventListenerWithSceneGraphPriority(npcListener, _npc);
                        }
                    }
                }
            }
           
            if (_player && tiledMap)
            {

                _player->setPosition(Vec2(60.000000, 50.000000)); // 设置玩家的初始位置

                _player->setTiledMap(tiledMap);
                _player->setInitPositionMap(tiledMap->getPosition());


            }
        }
        g_selectedMap.clear();
    }
}


// 创建NPC精灵
void GameScene::createNPC()
{
    // 先获取MapLayer
    auto mapLayer = dynamic_cast<MapLayer*>(this->getChildByName("MapLayer"));
    if (!mapLayer) {
        CCLOG("MapLayer not found!");
        return;
    }

    auto tiledMap = mapLayer->getTMXTiledMap();
    if (!tiledMap) {
        CCLOG("TMXTiledMap not found!");
        return;
    }
    auto npc = Sprite::create("npc.png");  // 替换为NPC的实际图片路径
    npc->setScale(3.0f);
    npc->setPosition(Vec2(300, 200));  // 设置NPC在地图上的位置
    this->addChild(npc, 20);

    // 设置点击事件
    auto npcListener = EventListenerTouchOneByOne::create();
    npcListener->setSwallowTouches(true);

    npcListener->onTouchBegan = [this, tiledMap](Touch* touch, Event* event) {
        auto locationInView = touch->getLocation();
        auto locationInMap = tiledMap->convertToNodeSpace(locationInView);

        Rect boundingBox = _npc->getBoundingBox();
        // boundingBox现在是相对于tiledMap的坐标系，因为npc是tiledMap的子节点
        if (boundingBox.containsPoint(locationInMap))
        {
            showDialogue();
            return true;
        }
        return false;
        };


    _eventDispatcher->addEventListenerWithSceneGraphPriority(npcListener, npc);
    // 存储NPC指针以便后续访问
    _npc = npc;
}

// 更新NPC位置以保持其相对位置
void GameScene::updateNPCPosition()
{
    if (_npc) {
        auto mapLayer = dynamic_cast<MapLayer*>(this->getChildByName("MapLayer"));
        auto tiledMap = mapLayer->getTMXTiledMap();
        Vec2 mapPosition = tiledMap->getPosition();
        // 保持NPC相对于地图原点的固定位置
        _npc->setPosition(Vec2(300 + mapPosition.x, 200 + mapPosition.y));
    }
}

// 在每帧更新中调用
void GameScene::update(float delta)
{
    updateNPCPosition(); // 更新NPC位置以跟随背景
}

// 显示对话框
void GameScene::showDialogue()
{
    auto dialogueBox = ui::Text::create("你好，勇士！", "fonts/Marker Felt.ttf", 24);
    dialogueBox->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 4));
    this->addChild(dialogueBox, 20);

    // 延迟3秒后移除对话框
    this->runAction(Sequence::create(DelayTime::create(3), RemoveSelf::create(), nullptr));
}

// 初始化事件监听器
void GameScene::initEventListeners()
{
    // 键盘事件
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // 鼠标事件（用于种植作物）
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_P)
    {
        _isPlanting = true;
        CCLOG("Planting mode ON");
    }
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_P)
    {
        _isPlanting = false;
        CCLOG("Planting mode OFF");
    }
}

void GameScene::plantCropAt(const Vec2& locationInMap, cocos2d::TMXTiledMap* tiledMap)
{
    // 定义九个作物的图像文件名
    std::vector<std::string> cropImages = {
        "Carrot_1.png", "Carrot_2.png", "Carrot_3.png",
        "Pumpkin_1.png", "Pumpkin_2.png", "Pumpkin_3.png",
        "Lajiao_1.png", "Lajiao_2.png", "Lajiao_3.png"
    };

    // 使用计数器来循环选择作物
    static int cropIndex = 0;

    // 创建新的作物Sprite
    auto crop = Sprite::create(cropImages[cropIndex]);
    crop->setScale(0.1f);
    crop->setPosition(locationInMap);

    // 将作物添加到tiledMap上，保证它与地图的缩放和位置同步
    tiledMap->addChild(crop, 15);

    // 将作物添加到 _crops 容器中
    _crops.push_back(crop);

    // 输出日志，显示种植作物的位置
    CCLOG("Planted a crop (%s) at (%.2f,%.2f)", cropImages[cropIndex].c_str(), locationInMap.x, locationInMap.y);

    // 更新计数器，确保在0到8之间循环
    cropIndex = (cropIndex + 1) % cropImages.size();
}

void GameScene::harvestCropAt(const Vec2& locationInMap)
{
    // 遍历_crops，检查点击位置是否在某个作物的boundingBox中
    for (auto it = _crops.begin(); it != _crops.end(); ++it)
    {
        auto crop = *it;
        Rect boundingBox = crop->getBoundingBox();
        if (boundingBox.containsPoint(locationInMap))
        {
            // 找到要收获的作物
            crop->removeFromParent();
            _crops.erase(it);
            CCLOG("Harvested a crop at (%.2f,%.2f)", locationInMap.x, locationInMap.y);
            break;
        }
    }
}

void GameScene::onMouseDown(Event* event)
{
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
    auto mouseButton = mouseEvent->getMouseButton();
    if (!mouseEvent) return;

    if (_isPlanting)
    {
        auto locationInView = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
        auto mapLayer = this->getChildByName<MapLayer*>("MapLayer");
        if (mapLayer)
        {
            auto tiledMap = dynamic_cast<MapLayer*>(mapLayer)->getTMXTiledMap();
            if (!tiledMap) return;

            // 将屏幕坐标转换为地图坐标
            // 因为tiledMap经过缩放和居中，因此使用tiledMap->convertToNodeSpace来获取相对于tiledMap的坐标
            auto locationInMap = tiledMap->convertToNodeSpace(locationInView);

            if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT)
            {
                // 左键种植
                plantCropAt(locationInMap, tiledMap);
            }
            else if (mouseButton == EventMouse::MouseButton::BUTTON_RIGHT)
            {
                // 右键收获
                harvestCropAt(locationInMap);
            }
        }
    }
}

