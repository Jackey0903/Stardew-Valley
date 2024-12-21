/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : Map2Scene.cpp
 * �ļ�����      : ����2������Ĺ���ʵ��
 * ����          : ���ƽܡ��ܽ�˶��������
 * ��������      : 2024/12/21
 * ���֤        : MIT License
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
    return Map2Scene::create();  // ʹ��C++11��CREATE_FUNC�괴��ʵ��
}

bool Map2Scene::init()
{
    if (!BaseMapScene::init())  // ���û����ʼ������
        return false;

    // ����Map2��ͼ����ʼ�����
    loadMap("Map/Map2/map2.tmx", 2.4f);
    initPlayer();

    // ���嶯���������
    cocos2d::Rect animalArea(1000, 1000, 300, 300);

    // ��������Ӷ��ﵽ��ͼ
    auto cow1 = Animal::create(animalArea, "Cow");  // C++11���ԣ�ʹ��auto�Զ��ƶ�����
    _tiledMap->addChild(cow1, 20);

    auto cow2 = Animal::create(animalArea, "Cow");
    _tiledMap->addChild(cow2, 20);

    auto sheep = Animal::create(animalArea, "Sheep");
    _tiledMap->addChild(sheep, 20);

    auto pig = Animal::create(animalArea, "Pig");
    _tiledMap->addChild(pig, 20);

    // ��ʼ����ֲģʽ��־
    _isPlanting = false;
    _isPlantingTree = false;

    // ��ʼ���¼������������̡���꣩
    initEventListeners();

    this->scheduleUpdate();  // ʹ��C++11�ĵ��Ⱥ�����ÿ֡����

    return true;
}

void Map2Scene::onEnter()
{
    BaseMapScene::onEnter();  // ���û����onEnter����

    // ȷ����ͼ���ڣ����Ҳ�����NPC
    if (_tiledMap)
    {
        CCLOG("Tiled map exists, proceeding to get NPC layer.");

        // ��ȡ��Ϊ"Npc_Lewis"�Ķ����
        auto npcLayer = _tiledMap->getObjectGroup(u8"Npc_Lewis");
        if (npcLayer)
        {
            CCLOG("Npc layer found, proceeding to create NPCs.");

            auto npcObjects = npcLayer->getObjects();
            for (auto& obj : npcObjects)  // C++11���ԣ���Χ-based forѭ��
            {
                auto npcObject = obj.asValueMap();
                if (npcObject["name"].asString() == u8"Npc_Lewis")
                {
                    // ��ȡNPC�ڵ�ͼ����ϵ�е�λ�ã�Tiled����Ϊ(0,0)��
                    float npcX = npcObject["x"].asFloat();
                    float npcY = npcObject["y"].asFloat();
                    Vec2 npcPosition(npcX, npcY);
                    CCLOG("Creating Npc_Lewis at position: (%.2f, %.2f)", npcPosition.x, npcPosition.y);

                    // ����NPC�Ի�����
                    _npcLewisDialogues = {
                        {u8"Hey, young man, can you help me complete some tasks? I will pay you a fee.", "Lewis/Lewis_1.png"},
                        {u8"Can you help me repair my house?", "Lewis/Lewis_2.png"},
                        {u8"Can you help me collect some herbs that I want?", "Lewis/Lewis_3.png"},
                        {u8"Can you help me drive away wild beasts? These beasts will harm my livestock.", "Lewis/Lewis_4.png"}
                    };

                    // ����NPC�Ի�ѡ��
                    LewisOption = {
                        {u8"Okay, I'll help you repair your house."},
                        {u8"Okay, I'll help you collect herbs. May I ask what herbs you need?"},
                        {u8"Okay, I'll help you drive away the wild beasts."}
                    };

                    // ����Npc_Lewisʵ������ӵ���ͼ
                    auto npcLewis = Npc::createWithPosition(npcPosition, u8"Lewis", _npcLewisDialogues, LewisOption);
                    if (npcLewis)
                    {
                        CCLOG("Npc_Lewis created successfully.");
                        npcLewis->setScale(0.2f);  // ����NPC������
                        _tiledMap->addChild(npcLewis, 35);
                    }
                    else
                    {
                        CCLOG("Failed to create Npc_Lewis instance.");
                        Director::getInstance()->end();  // C++11���ԣ�ʹ������ָ�����Directorʵ��
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
    // ��ʼ�������¼�������
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Map2Scene::onKeyPressed, this);    // C++11���ԣ�ʹ��lambda���ʽ�󶨻ص�
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Map2Scene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // ��ʼ������¼�������
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Map2Scene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Map2Scene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    // ���̰����¼������û������ֲģʽ
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
    // �����ͷ��¼����ر���ֲģʽ
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
    auto locationInMap = _tiledMap->convertToNodeSpace(locationInView);  // ����ͼ����ת��Ϊ��ͼ����

    // �������ֲ����ģʽ
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

    // �������ֲ��ģʽ
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

    // �������Ͼ��鲢��ӵ���Ҿ���
    auto fertilizerSprite = Sprite::create("Feiliao_1.png");
    fertilizerSprite->setScale(0.6f);
    fertilizerSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _player->getPlayerSprite()->addChild(fertilizerSprite, 40);
    fertilizerSprite->setPosition(Vec2(60, 100));  // ����ƫ��λ��

    // ��������֡�����Ŷ���
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrame::create("Feiliao_1.png", Rect(0, 0, fertilizerSprite->getContentSize().width, fertilizerSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("Feiliao_2.png", Rect(0, 0, fertilizerSprite->getContentSize().width, fertilizerSprite->getContentSize().height)));
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);
    auto finish = CallFunc::create([fertilizerSprite]() {
        fertilizerSprite->removeFromParent();  // ��������ʱ�Ƴ�����
        });
    fertilizerSprite->runAction(Sequence::create(animate, finish, nullptr));

    // ������������ﾫ��
    std::vector<std::string> cropImages = {
        "Plants/Carrot_1.png","Plants/Carrot_2.png","Plants/Carrot_3.png",
        "Plants/Pumpkin_1.png","Plants/Pumpkin_2.png","Plants/Pumpkin_3.png",
        "Plants/Lajiao_1.png","Plants/Lajiao_2.png","Plants/Lajiao_3.png"
    };

    static int cropIndex = 0;  // ��̬��������¼��ǰ��������
    auto crop = Sprite::create(cropImages[cropIndex]);
    crop->setScale(0.1f);
    crop->setPosition(locationInMap);
    _tiledMap->addChild(crop, 15);
    _crops.push_back(crop);
    CCLOG("Planted a crop (%s) at (%.2f,%.2f)", cropImages[cropIndex].c_str(), locationInMap.x, locationInMap.y);
    cropIndex = (cropIndex + 1) % cropImages.size();  // ѭ��ʹ������ͼƬ
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

    // �������Ӿ��鲢��ӵ���Ҿ���
    auto hoeSprite = Sprite::create("Hoe_1.png");
    hoeSprite->setScale(0.8f);
    hoeSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _player->getPlayerSprite()->addChild(hoeSprite, 40);
    hoeSprite->setPosition(Vec2(60, 120));  // ����ƫ��λ��

    // ��������֡�����Ŷ���
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrame::create("Hoe_1.png", Rect(0, 0, hoeSprite->getContentSize().width, hoeSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("Hoe_2.png", Rect(0, 0, hoeSprite->getContentSize().width, hoeSprite->getContentSize().height)));
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);
    auto finish = CallFunc::create([hoeSprite]() {
        hoeSprite->removeFromParent();  // ��������ʱ�Ƴ�����
        });
    hoeSprite->runAction(Sequence::create(animate, finish, nullptr));

    // ��鲢�Ƴ����ո������
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

    // �������羫�鲢��ӵ���Ҿ���
    auto kettleSprite = Sprite::create("Shuihu_1.png");
    kettleSprite->setScale(0.8f);
    kettleSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _player->getPlayerSprite()->addChild(kettleSprite, 40);
    kettleSprite->setPosition(Vec2(60, 100));  // ����ƫ��λ��

    // ��������֡�����Ŷ���
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrame::create("Shuihu_1.png", Rect(0, 0, kettleSprite->getContentSize().width, kettleSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("Shuihu_2.png", Rect(0, 0, kettleSprite->getContentSize().width, kettleSprite->getContentSize().height)));
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);
    auto finish = CallFunc::create([kettleSprite]() {
        kettleSprite->removeFromParent();  // ��������ʱ�Ƴ�����
        });
    kettleSprite->runAction(Sequence::create(animate, finish, nullptr));

    // ���������������
    std::vector<std::string> treeImages = {
        "Tree_1.png","Tree_2.png","Tree_3.png","Tree_4.png"
    };

    static int treeIndex = 0;  // ��̬��������¼��ǰ������
    auto tree = Sprite::create(treeImages[treeIndex]);
    tree->setScale(0.1f);
    tree->setPosition(locationInMap);
    _tiledMap->addChild(tree, 15);
    _trees.push_back(tree);
    CCLOG("Planted a tree (%s) at (%.2f,%.2f)", treeImages[treeIndex].c_str(), locationInMap.x, locationInMap.y);
    treeIndex = (treeIndex + 1) % treeImages.size();  // ѭ��ʹ����ͼƬ
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

    // �������Ӿ��鲢��ӵ���Ҿ���
    auto chopSprite = Sprite::create("chop1.png");
    chopSprite->setScale(5.0f);
    chopSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _player->getPlayerSprite()->addChild(chopSprite, 40);
    chopSprite->setPosition(Vec2(60, 120));  // ����ƫ��λ��

    // ��������֡�����Ŷ���
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrame::create("chop1.png", Rect(0, 0, chopSprite->getContentSize().width, chopSprite->getContentSize().height)));
    frames.pushBack(SpriteFrame::create("chop2.png", Rect(0, 0, chopSprite->getContentSize().width, chopSprite->getContentSize().height)));
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);
    auto finish = CallFunc::create([chopSprite]() {
        chopSprite->removeFromParent();  // ��������ʱ�Ƴ�����
        });
    chopSprite->runAction(Sequence::create(animate, finish, nullptr));

    // ��鲢�Ƴ�����������
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

