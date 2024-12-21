// Map2Scene.cpp
#include "Map2Scene.h"
#include "MapLayer.h"
#include "Player/Player.h"
#include "ui/CocosGUI.h"
#include "BackpackScene.h"
#include "Object/Animal.h"
#include "Object/Npc.h"
USING_NS_CC;

Scene* Map2Scene::createScene()
{
    return Map2Scene::create();
}

bool Map2Scene::init()
{
    if (!BaseMapScene::init())
        return false;

    // 加载Map2并初始化玩家
    loadMap("Map/Map2/map2.tmx", 2.4f);
    initPlayer();

    // 在Map2Scene.cpp的onEnter或者init中，当_tiledMap已经存在后：
    cocos2d::Rect animalArea(1000, 1000, 300, 300);

    auto cow1 = Animal::create(animalArea,"Cow");
    _tiledMap->addChild(cow1, 20);

    auto cow2 = Animal::create(animalArea, "Cow");
    _tiledMap->addChild(cow2, 20);

	auto sheep = Animal::create(animalArea, "Sheep");
	_tiledMap->addChild(sheep, 20);

    auto pig = Animal::create(animalArea, "Pig");
    _tiledMap->addChild(pig, 20);


    _isPlanting = false;
    _isPlantingTree = false;

    // 初始化事件监听器（键盘、鼠标）
    initEventListeners();

    this->scheduleUpdate();

    return true;
}

void Map2Scene::onEnter()
{
    BaseMapScene::onEnter();
    // 无额外逻辑

    // 在Map2上查找npc对象层并创建NPC
    if (_tiledMap)
    {
        CCLOG("Tiled map exists, proceeding to get NPC layer.");

        // 获取名为"Npc"的对象层
        auto npcLayer = _tiledMap->getObjectGroup(u8"Npc_Lewis");

        if (npcLayer)
        {
            CCLOG("Npc layer found, proceeding to create NPCs.");

            auto npcObjects = npcLayer->getObjects();
            for (auto& obj : npcObjects)
            {
                auto npcObject = obj.asValueMap();
                if (npcObject["name"].asString() == u8"Npc_Lewis")
                {
                    // 获取NPC在地图坐标系中的位置（Tiled左下为(0,0)）
                    float npcX = npcObject["x"].asFloat();
                    float npcY = npcObject["y"].asFloat();
                    Vec2 npcPosition(npcX, npcY);
                    CCLOG("Creating Npc_Lewis at position: (%.2f, %.2f)", npcPosition.x, npcPosition.y);

                    _npcLewisDialogues = {
                        {u8"Hey, young man, can you help me complete some tasks?I will pay you a fee.", "Lewis/Lewis_1.png"},
                        {u8"Can you help me repair my house?", "Lewis/Lewis_2.png"},
                        {u8"Can you help me collect some herbs that I want?", "Lewis/Lewis_3.png"},
                        {u8"Can you help me drive away wild beasts? These beasts will harm my livestock.", "Lewis/Lewis_4.png"}
                    };
                    LewisOption = { {u8"Okay, I'll help you repair your house."},{u8"Okay, I'll help you collect herbs. May I ask what herbs you need?"},{u8"Okay, I'll help you drive away the wild beasts."} };

                    // 创建 NpcLewis 实例并添加到地图
                    auto npcLewis = Npc::createWithPosition(npcPosition, u8"Lewis", _npcLewisDialogues, LewisOption);
                    if (npcLewis)
                    {
                        CCLOG("Npc_Lewis created successfully.");
                        // 根据需要调整缩放或其他属性
                        npcLewis->setScale(0.2f); // 增大NPC的缩放，使点击区域更大
                        _tiledMap->addChild(npcLewis, 35);
                    }
                    else {
                        CCLOG("Failed to create Npc_Lewis instance.");
                        Director::getInstance()->end();
                    }
                }
                else {
                    CCLOG("Found NPC object but name does not match: %s", npcObject["name"].asString().c_str());
                }
            }
        }
        else {
            CCLOG("Npc layer not found in the tiled map.");
        }
    }
    else {
        CCLOG("Tiled map does not exist.");
    }
}

void Map2Scene::initEventListeners()
{
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Map2Scene::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Map2Scene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Map2Scene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Map2Scene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_P)
    {
        _isPlanting = true;
        CCLOG("Planting mode ON");
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_T)
    {
        _isPlantingTree = true;
        CCLOG("Planting tree mode ON");
    }
}

void Map2Scene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_P)
    {
        _isPlanting = false;
        CCLOG("Planting mode OFF");
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_T)
    {
        _isPlantingTree = false;
        CCLOG("Planting tree mode OFF");
    }
}

void Map2Scene::onMouseDown(Event* event)
{
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
    if (!mouseEvent) return;

    if (!_isPlanting && !_isPlantingTree) return;

    if (!_tiledMap) return;
    auto mouseButton = mouseEvent->getMouseButton();
    Vec2 locationInView(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    auto locationInMap = _tiledMap->convertToNodeSpace(locationInView);

    // 如果在种植作物模式
    if (_isPlanting)
    {
        if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT)
        {
            plantCropAt(locationInMap);
        }
        else if (mouseButton == EventMouse::MouseButton::BUTTON_RIGHT)
        {
            harvestCropAt(locationInMap);
        }
    }

    // 如果在种植树模式
    if (_isPlantingTree)
    {
        if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT)
        {
            plantTreeAt(locationInMap);
        }
        else if (mouseButton == EventMouse::MouseButton::BUTTON_RIGHT)
        {
            fellTreeAt(locationInMap);
        }
    }
}

void Map2Scene::plantCropAt(const Vec2& locationInMap)
{
    Vec2 playerPositionInMap = _tiledMap->convertToNodeSpace(_player->getPosition());
	playerPositionInMap.x += 300.0f;
	playerPositionInMap.y += 150.0f;
    if (locationInMap.distance(playerPositionInMap) > 100.0f)
    {
        CCLOG("Too far to plant a crop.");
        return;
    }

    auto fertilizerSprite = Sprite::create("Feiliao_1.png");
    fertilizerSprite->setScale(0.6f);
    fertilizerSprite->setAnchorPoint(Vec2(0.5f, 0.5f));

    // 将添加为_playerSprite的子节点
    _player->getPlayerSprite()->addChild(fertilizerSprite, 40);


    // 设置相对于玩家精灵的偏移位置（可根据需要调整）
    fertilizerSprite->setPosition(Vec2(60, 100)); // 例如在玩家右手边稍上方
    // 创建动画帧
    Vector<SpriteFrame*> frames;

    frames.pushBack(SpriteFrame::create("Feiliao_1.png", Rect(0, 0, fertilizerSprite->getContentSize().width, fertilizerSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("Feiliao_2.png", Rect(0, 0, fertilizerSprite->getContentSize().width, fertilizerSprite->getContentSize().height)));
    CCLOG("%f %f", playerPositionInMap.x, playerPositionInMap.y);
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);

    auto finish = CallFunc::create([fertilizerSprite]() {
        fertilizerSprite->removeFromParent(); // 动画结束时移除斧子
        });

    // 播放动画
    fertilizerSprite->runAction(Sequence::create(animate, finish, nullptr));

    std::vector<std::string> cropImages = {
        "Carrot_1.png","Carrot_2.png","Carrot_3.png",
        "Pumpkin_1.png","Pumpkin_2.png","Pumpkin_3.png",
        "Lajiao_1.png","Lajiao_2.png","Lajiao_3.png"
    };

    static int cropIndex = 0;
    auto crop = Sprite::create(cropImages[cropIndex]);
    crop->setScale(0.1f);
    crop->setPosition(locationInMap);
    _tiledMap->addChild(crop, 15);
    _crops.push_back(crop);

    CCLOG("Planted a crop (%s) at (%.2f,%.2f)", cropImages[cropIndex].c_str(), locationInMap.x, locationInMap.y);
    cropIndex = (cropIndex + 1) % cropImages.size();
}

void Map2Scene::harvestCropAt(const Vec2& locationInMap)
{
    Vec2 playerPositionInMap = _tiledMap->convertToNodeSpace(_player->getPosition());
    playerPositionInMap.x += 300.0f;
    playerPositionInMap.y += 150.0f;
    if (locationInMap.distance(playerPositionInMap) > 100.0f)
    {
        CCLOG("Too far to harvest a crop.");
        return;
    }

    // 创建斧子精灵
    auto hoeSprite = Sprite::create("Hoe_1.png");
    hoeSprite->setScale(0.8f);
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
        hoeSprite->removeFromParent(); // 动画结束时移除斧子
        });

    // 播放动画
    hoeSprite->runAction(Sequence::create(animate, finish, nullptr));

    for (auto it = _crops.begin(); it != _crops.end(); ++it)
    {
        auto crop = *it;
        Rect boundingBox = crop->getBoundingBox();
        if (boundingBox.containsPoint(locationInMap))
        {
            crop->removeFromParent();
            _crops.erase(it);
            CCLOG("Harvested a crop at (%.2f,%.2f)", locationInMap.x, locationInMap.y);
            break;
        }
    }
}

void Map2Scene::plantTreeAt(const Vec2& locationInMap)
{
    Vec2 playerPositionInMap = _tiledMap->convertToNodeSpace(_player->getPosition());
    playerPositionInMap.x += 300.0f;
    playerPositionInMap.y += 150.0f;
    if (locationInMap.distance(playerPositionInMap) > 100.0f)
    {
        CCLOG("Too far to plant a tree.");
        return;
    }
    auto kettleSprite = Sprite::create("Shuihu_1.png");
    kettleSprite->setScale(0.8f);
    kettleSprite->setAnchorPoint(Vec2(0.5f, 0.5f));

    // 将添加为_playerSprite的子节点
    _player->getPlayerSprite()->addChild(kettleSprite, 40);


    // 设置相对于玩家精灵的偏移位置（可根据需要调整）
    kettleSprite->setPosition(Vec2(60, 100)); // 例如在玩家右手边稍上方
    // 创建动画帧
    Vector<SpriteFrame*> frames;
    // chop1.png, chop2.png 应该存在
    frames.pushBack(SpriteFrame::create("Shuihu_1.png", Rect(0, 0, kettleSprite->getContentSize().width, kettleSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("Shuihu_2.png", Rect(0, 0, kettleSprite->getContentSize().width, kettleSprite->getContentSize().height)));
    CCLOG("%f %f", playerPositionInMap.x, playerPositionInMap.y);
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);

    auto finish = CallFunc::create([kettleSprite]() {
        kettleSprite->removeFromParent(); // 动画结束时移除斧子
        });

    // 播放动画
    kettleSprite->runAction(Sequence::create(animate, finish, nullptr));
    std::vector<std::string> treeImages = {
        "Tree_1.png","Tree_2.png","Tree_3.png","Tree_4.png"
    };

    static int treeIndex = 0;
    auto tree = Sprite::create(treeImages[treeIndex]);
    tree->setScale(0.1f);
    tree->setPosition(locationInMap);
    _tiledMap->addChild(tree, 15);
    _trees.push_back(tree);

    CCLOG("Planted a tree (%s) at (%.2f,%.2f)", treeImages[treeIndex].c_str(), locationInMap.x, locationInMap.y);
    treeIndex = (treeIndex + 1) % treeImages.size();
}

void Map2Scene::fellTreeAt(const Vec2& locationInMap)
{
    Vec2 playerPositionInMap = _tiledMap->convertToNodeSpace(_player->getPosition());
    playerPositionInMap.x += 300.0f;
    playerPositionInMap.y += 150.0f;
    if (locationInMap.distance(playerPositionInMap) > 100.0f)
    {
        CCLOG("Too far to fell a tree.");
        return;
    }

    // 创建斧子精灵
	auto chopSprite = Sprite::create("chop1.png");
	chopSprite->setScale(5.0f);
    chopSprite->setAnchorPoint(Vec2(0.5f, 0.5f));

    // 将斧子添加为_playerSprite的子节点
    _player->getPlayerSprite()->addChild(chopSprite, 40);

    // 设置相对于玩家精灵的偏移位置（可根据需要调整）
    chopSprite->setPosition(Vec2(60, 120)); // 例如在玩家右手边稍上方
    // 创建动画帧
    Vector<SpriteFrame*> frames;
    // chop1.png, chop2.png 应该存在
    frames.pushBack(SpriteFrame::create("chop1.png", Rect(0, 0,chopSprite->getContentSize().width, chopSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("chop2.png", Rect(0, 0, chopSprite->getContentSize().width, chopSprite->getContentSize().height)));
    CCLOG("%f %f",playerPositionInMap.x, playerPositionInMap.y);
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);

    auto finish = CallFunc::create([chopSprite]() {
        chopSprite->removeFromParent(); // 动画结束时移除斧子
        });

    // 播放动画
    chopSprite->runAction(Sequence::create(animate, finish, nullptr));

    // 砍树逻辑
    for (auto it = _trees.begin(); it != _trees.end(); ++it)
    {
        auto tree = *it;
        Rect boundingBox = tree->getBoundingBox();
        if (boundingBox.containsPoint(locationInMap))
        {
            tree->removeFromParent();
            _trees.erase(it);
            CCLOG("Felled a tree at (%.2f,%.2f)", locationInMap.x, locationInMap.y);
            break;
        }
    }
}

