/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Map2Scene.cpp
 * 文件功能      : 场景2派生类的功能实现
 * 作者          : 胡浩杰、曹津硕、胡正华
 * 更新日期      : 2024/12/21
 * 许可证        : MIT License
 ****************************************************************/

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
    return Map2Scene::create();  // 使用C++11的CREATE_FUNC宏创建实例
}

bool Map2Scene::init()
{
    if (!BaseMapScene::init())  // 调用基类初始化函数
        return false;

    // 加载Map2地图并初始化玩家
    loadMap("Map/Map2/map2.tmx", 2.4f);
    initPlayer();

    // 定义动物出现区域
    cocos2d::Rect animalArea(1000, 1000, 300, 300);

    // 创建并添加动物到地图
    auto cow1 = Animal::create(animalArea, "Cow");  // C++11特性：使用auto自动推断类型
    _tiledMap->addChild(cow1, 20);

    auto cow2 = Animal::create(animalArea, "Cow");
    _tiledMap->addChild(cow2, 20);

    auto sheep = Animal::create(animalArea, "Sheep");
    _tiledMap->addChild(sheep, 20);

    auto pig = Animal::create(animalArea, "Pig");
    _tiledMap->addChild(pig, 20);

    // 初始化种植模式标志
    _isPlanting = false;
    _isPlantingTree = false;

    // 初始化事件监听器（键盘、鼠标）
    initEventListeners();

    this->scheduleUpdate();  // 使用C++11的调度函数，每帧更新

    return true;
}

void Map2Scene::onEnter()
{
    BaseMapScene::onEnter();  // 调用基类的onEnter方法

    // 确保地图存在，查找并创建NPC
    if (_tiledMap)
    {
        CCLOG("Tiled map exists, proceeding to get NPC layer.");

        // 获取名为"Npc_Lewis"的对象层
        auto npcLayer = _tiledMap->getObjectGroup(u8"Npc_Lewis");
        if (npcLayer)
        {
            CCLOG("Npc layer found, proceeding to create NPCs.");

            auto npcObjects = npcLayer->getObjects();
            for (auto& obj : npcObjects)  // C++11特性：范围-based for循环
            {
                auto npcObject = obj.asValueMap();
                if (npcObject["name"].asString() == u8"Npc_Lewis")
                {
                    // 获取NPC在地图坐标系中的位置（Tiled左下为(0,0)）
                    float npcX = npcObject["x"].asFloat();
                    float npcY = npcObject["y"].asFloat();
                    Vec2 npcPosition(npcX, npcY);
                    CCLOG("Creating Npc_Lewis at position: (%.2f, %.2f)", npcPosition.x, npcPosition.y);

                    // 定义NPC对话内容
                    _npcLewisDialogues = {
                        {u8"Hey, young man, can you help me complete some tasks? I will pay you a fee.", "Lewis/Lewis_1.png"},
                        {u8"Can you help me repair my house?", "Lewis/Lewis_2.png"},
                        {u8"Can you help me collect some herbs that I want?", "Lewis/Lewis_3.png"},
                        {u8"Can you help me drive away wild beasts? These beasts will harm my livestock.", "Lewis/Lewis_4.png"}
                    };

                    // 定义NPC对话选项
                    LewisOption = {
                        {u8"Okay, I'll help you repair your house."},
                        {u8"Okay, I'll help you collect herbs. May I ask what herbs you need?"},
                        {u8"Okay, I'll help you drive away the wild beasts."}
                    };

                    // 创建Npc_Lewis实例并添加到地图
                    auto npcLewis = Npc::createWithPosition(npcPosition, u8"Lewis", _npcLewisDialogues, LewisOption);
                    if (npcLewis)
                    {
                        CCLOG("Npc_Lewis created successfully.");
                        npcLewis->setScale(0.2f);  // 设置NPC的缩放
                        _tiledMap->addChild(npcLewis, 35);
                    }
                    else
                    {
                        CCLOG("Failed to create Npc_Lewis instance.");
                        Director::getInstance()->end();  // C++11特性：使用智能指针管理Director实例
                    }
                }
                else
                {
                    CCLOG("Found NPC object but name does not match: %s", npcObject["name"].asString().c_str());
                }
            }
        }
        else
        {
            CCLOG("Npc layer not found in the tiled map.");
        }
    }
    else
    {
        CCLOG("Tiled map does not exist.");
    }
}

void Map2Scene::initEventListeners()
{
    // 初始化键盘事件监听器
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Map2Scene::onKeyPressed, this);    // C++11特性：使用lambda表达式绑定回调
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Map2Scene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // 初始化鼠标事件监听器
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Map2Scene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Map2Scene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    // 键盘按下事件，启用或禁用种植模式
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
    // 键盘释放事件，关闭种植模式
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
    auto locationInMap = _tiledMap->convertToNodeSpace(locationInView);  // 将视图坐标转换为地图坐标

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

    // 创建肥料精灵并添加到玩家精灵
    auto fertilizerSprite = Sprite::create("Feiliao_1.png");
    fertilizerSprite->setScale(0.6f);
    fertilizerSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _player->getPlayerSprite()->addChild(fertilizerSprite, 40);
    fertilizerSprite->setPosition(Vec2(60, 100));  // 设置偏移位置

    // 创建动画帧并播放动画
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrame::create("Feiliao_1.png", Rect(0, 0, fertilizerSprite->getContentSize().width, fertilizerSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("Feiliao_2.png", Rect(0, 0, fertilizerSprite->getContentSize().width, fertilizerSprite->getContentSize().height)));
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);
    auto finish = CallFunc::create([fertilizerSprite]() {
        fertilizerSprite->removeFromParent();  // 动画结束时移除精灵
        });
    fertilizerSprite->runAction(Sequence::create(animate, finish, nullptr));

    // 创建并添加作物精灵
    std::vector<std::string> cropImages = {
        "Plants/Carrot_1.png","Plants/Carrot_2.png","Plants/Carrot_3.png",
        "Plants/Pumpkin_1.png","Plants/Pumpkin_2.png","Plants/Pumpkin_3.png",
        "Plants/Lajiao_1.png","Plants/Lajiao_2.png","Plants/Lajiao_3.png"
    };

    static int cropIndex = 0;  // 静态变量，记录当前作物索引
    auto crop = Sprite::create(cropImages[cropIndex]);
    crop->setScale(0.1f);
    crop->setPosition(locationInMap);
    _tiledMap->addChild(crop, 15);
    _crops.push_back(crop);
    CCLOG("Planted a crop (%s) at (%.2f,%.2f)", cropImages[cropIndex].c_str(), locationInMap.x, locationInMap.y);
    cropIndex = (cropIndex + 1) % cropImages.size();  // 循环使用作物图片
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

    // 创建斧子精灵并添加到玩家精灵
    auto hoeSprite = Sprite::create("Hoe_1.png");
    hoeSprite->setScale(0.8f);
    hoeSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _player->getPlayerSprite()->addChild(hoeSprite, 40);
    hoeSprite->setPosition(Vec2(60, 120));  // 设置偏移位置

    // 创建动画帧并播放动画
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrame::create("Hoe_1.png", Rect(0, 0, hoeSprite->getContentSize().width, hoeSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("Hoe_2.png", Rect(0, 0, hoeSprite->getContentSize().width, hoeSprite->getContentSize().height)));
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);
    auto finish = CallFunc::create([hoeSprite]() {
        hoeSprite->removeFromParent();  // 动画结束时移除斧子
        });
    hoeSprite->runAction(Sequence::create(animate, finish, nullptr));

    // 检查并移除被收割的作物
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

    // 创建树苗精灵并添加到玩家精灵
    auto kettleSprite = Sprite::create("Shuihu_1.png");
    kettleSprite->setScale(0.8f);
    kettleSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _player->getPlayerSprite()->addChild(kettleSprite, 40);
    kettleSprite->setPosition(Vec2(60, 100));  // 设置偏移位置

    // 创建动画帧并播放动画
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrame::create("Shuihu_1.png", Rect(0, 0, kettleSprite->getContentSize().width, kettleSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("Shuihu_2.png", Rect(0, 0, kettleSprite->getContentSize().width, kettleSprite->getContentSize().height)));
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);
    auto finish = CallFunc::create([kettleSprite]() {
        kettleSprite->removeFromParent();  // 动画结束时移除精灵
        });
    kettleSprite->runAction(Sequence::create(animate, finish, nullptr));

    // 创建并添加树精灵
    std::vector<std::string> treeImages = {
        "Tree_1.png","Tree_2.png","Tree_3.png","Tree_4.png"
    };

    static int treeIndex = 0;  // 静态变量，记录当前树索引
    auto tree = Sprite::create(treeImages[treeIndex]);
    tree->setScale(0.1f);
    tree->setPosition(locationInMap);
    _tiledMap->addChild(tree, 15);
    _trees.push_back(tree);
    CCLOG("Planted a tree (%s) at (%.2f,%.2f)", treeImages[treeIndex].c_str(), locationInMap.x, locationInMap.y);
    treeIndex = (treeIndex + 1) % treeImages.size();  // 循环使用树图片
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

    // 创建斧子精灵并添加到玩家精灵
    auto chopSprite = Sprite::create("chop1.png");
    chopSprite->setScale(5.0f);
    chopSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _player->getPlayerSprite()->addChild(chopSprite, 40);
    chopSprite->setPosition(Vec2(60, 120));  // 设置偏移位置

    // 创建动画帧并播放动画
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrame::create("chop1.png", Rect(0, 0, chopSprite->getContentSize().width, chopSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("chop2.png", Rect(0, 0, chopSprite->getContentSize().width, chopSprite->getContentSize().height)));
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);
    auto finish = CallFunc::create([chopSprite]() {
        chopSprite->removeFromParent();  // 动画结束时移除斧子
        });
    chopSprite->runAction(Sequence::create(animate, finish, nullptr));

    // 检查并移除被砍伐的树
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

