// Map2Scene.cpp
#include "Map2Scene.h"
#include "MapLayer.h"
#include "Player/Player.h"
#include "ui/CocosGUI.h"
#include "BackpackScene.h"
#include "Object/Animal.h"

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
    auto animal = Animal::create(animalArea);
    _tiledMap->addChild(animal, 20);


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

