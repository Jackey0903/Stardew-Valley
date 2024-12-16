// GameScene.cpp
#include "Scene/GameScene.h"
#include "MapLayer.h"
#include "Player/Player.h"
#include "ui/CocosGUI.h"  // ����Cocos2d��UIģ��

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

    _isPlanting = false; // ��ʼ��������ֲģʽ
    _npc = nullptr;

    currentMap = "Map/Map2/map2.tmx";

    auto mapLayer = MapLayer::create();
    mapLayer->setName("MapLayer");
    this->addChild(mapLayer, 0);
    mapLayer->loadMap(currentMap);

    auto tiledMap = mapLayer->getTMXTiledMap();

    // ���õ�ͼ��ê��Ϊ����
    tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f));

    // ��ȡ��Ļ�Ŀɼ������С
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // �����ͼ�����ű�����ʹ����������
    float scaleX = visibleSize.width / tiledMap->getContentSize().width * 2.4;
    float scaleY = visibleSize.height / tiledMap->getContentSize().height * 2.4;
    float scale = MAX(scaleX, scaleY); // ѡ��ϴ�����ű�����ȷ����ͼ��������
    tiledMap->setScale(scale);

    // ����ͼ������Ļ����
    tiledMap->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

    if (!tiledMap)
    {
        CCLOG("loaded ");
    }

    _player = Player::create();
    this->addChild(_player, 10); // ȷ������ڵ�ͼ֮��

    _player->setTiledMap(tiledMap);
    _player->setInitPositionMap(tiledMap->getPosition());

    // ��ʼ���¼������������̡���꣩
    initEventListeners();

    this->scheduleUpdate();
    
    return true;
}

void GameScene::onEnter()
{
    Scene::onEnter();
   
    if (!g_selectedMap.empty() && g_selectedMap != currentMap)
    {
        CCLOG("GameScene: �л����µ�ͼ: %s", g_selectedMap.c_str());
        currentMap = g_selectedMap;

        auto mapLayer = dynamic_cast<MapLayer*>(this->getChildByName("MapLayer"));
        if (mapLayer)
        {
            mapLayer->loadMap(currentMap);
            auto tiledMap = mapLayer->getTMXTiledMap();

            if (currentMap == "Map/Map1/map1.tmx")
            {
                // ���õ�ͼ��ê��Ϊ����
                tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f));

                // ��ȡ��Ļ�Ŀɼ������С
                auto visibleSize = Director::getInstance()->getVisibleSize();
                auto origin = Director::getInstance()->getVisibleOrigin();

                // �����ͼ�����ű�����ʹ����������
                float scaleX = visibleSize.width / tiledMap->getContentSize().width * 1.5;
                float scaleY = visibleSize.height / tiledMap->getContentSize().height * 1.5;
                float scale = MAX(scaleX, scaleY); // ѡ��ϴ�����ű�����ȷ����ͼ��������
                tiledMap->setScale(scale);

                // ����ͼ������Ļ����
                tiledMap->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
            }
            else if (currentMap == "Map/Map2/map2.tmx")
            {
                // ���õ�ͼ��ê��Ϊ����
                tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f));

                // ��ȡ��Ļ�Ŀɼ������С
                auto visibleSize = Director::getInstance()->getVisibleSize();
                auto origin = Director::getInstance()->getVisibleOrigin();

                // �����ͼ�����ű�����ʹ����������
                float scaleX = visibleSize.width / tiledMap->getContentSize().width * 2.4;
                float scaleY = visibleSize.height / tiledMap->getContentSize().height * 2.4;
                float scale = MAX(scaleX, scaleY); // ѡ��ϴ�����ű�����ȷ����ͼ��������
                tiledMap->setScale(scale);

                // ����ͼ������Ļ����
                tiledMap->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
            }

            else if (currentMap == "Map/Map3/map3.tmx")
            {
                // ���õ�ͼ��ê��Ϊ����
                tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f));

                // ��ȡ��Ļ�Ŀɼ������С
                auto visibleSize = Director::getInstance()->getVisibleSize();
                auto origin = Director::getInstance()->getVisibleOrigin();

                // �����ͼ�����ű�����ʹ����������
                float scaleX = visibleSize.width / tiledMap->getContentSize().width * 0.8;
                float scaleY = visibleSize.height / tiledMap->getContentSize().height * 0.8;
                float scale = MAX(scaleX, scaleY); // ѡ��ϴ�����ű�����ȷ����ͼ��������
                tiledMap->setScale(scale);

                // ����ͼ������Ļ����
                tiledMap->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

                // �ڵ�ͼ������в���NPC
                auto npcLayer = tiledMap->getObjectGroup("Npc"); // ��ȡ�����
                if (npcLayer)
                {
                    auto npcObjects = npcLayer->getObjects(); // ��ȡ���ж���

                    for (const auto& obj : npcObjects)
                    {// �� Value ת��Ϊ ValueMap
                        auto npcObject = obj.asValueMap();
                        // ����Ƿ�ΪNPC����
                        if (npcObject["name"].asString() == "Npc") // �������NPC��������Ϊ"NPC1"
                        {
                            Vec2 npcPosition = Vec2(npcObject["x"].asFloat(), npcObject["y"].asFloat());
                            _npc = Sprite::create("npc.png"); // �滻ΪNPC��ʵ��ͼƬ·��
                            _npc->setPosition(npcPosition);
                            tiledMap->addChild(_npc, 5);

                            // ���õ���¼�
                            auto npcListener = EventListenerTouchOneByOne::create();
                            npcListener->setSwallowTouches(true);
                            npcListener->onTouchBegan = [this](Touch* touch, Event* event) {
                                // �����λ���Ƿ���NPC����������
                                Rect boundingBox = _npc->getBoundingBox();
                                if (boundingBox.containsPoint(touch->getLocation()))
                                {
                                    showDialogue();  // ��ʾ�Ի���
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

                _player->setPosition(Vec2(60.000000, 50.000000)); // ������ҵĳ�ʼλ��

                _player->setTiledMap(tiledMap);
                _player->setInitPositionMap(tiledMap->getPosition());


            }
        }
        g_selectedMap.clear();
    }
}


// ����NPC����
void GameScene::createNPC()
{
    // �Ȼ�ȡMapLayer
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
    auto npc = Sprite::create("npc.png");  // �滻ΪNPC��ʵ��ͼƬ·��
    npc->setScale(3.0f);
    npc->setPosition(Vec2(300, 200));  // ����NPC�ڵ�ͼ�ϵ�λ��
    this->addChild(npc, 20);

    // ���õ���¼�
    auto npcListener = EventListenerTouchOneByOne::create();
    npcListener->setSwallowTouches(true);

    npcListener->onTouchBegan = [this, tiledMap](Touch* touch, Event* event) {
        auto locationInView = touch->getLocation();
        auto locationInMap = tiledMap->convertToNodeSpace(locationInView);

        Rect boundingBox = _npc->getBoundingBox();
        // boundingBox�����������tiledMap������ϵ����Ϊnpc��tiledMap���ӽڵ�
        if (boundingBox.containsPoint(locationInMap))
        {
            showDialogue();
            return true;
        }
        return false;
        };


    _eventDispatcher->addEventListenerWithSceneGraphPriority(npcListener, npc);
    // �洢NPCָ���Ա��������
    _npc = npc;
}

// ����NPCλ���Ա��������λ��
void GameScene::updateNPCPosition()
{
    if (_npc) {
        auto mapLayer = dynamic_cast<MapLayer*>(this->getChildByName("MapLayer"));
        auto tiledMap = mapLayer->getTMXTiledMap();
        Vec2 mapPosition = tiledMap->getPosition();
        // ����NPC����ڵ�ͼԭ��Ĺ̶�λ��
        _npc->setPosition(Vec2(300 + mapPosition.x, 200 + mapPosition.y));
    }
}

// ��ÿ֡�����е���
void GameScene::update(float delta)
{
    updateNPCPosition(); // ����NPCλ���Ը��汳��
}

// ��ʾ�Ի���
void GameScene::showDialogue()
{
    auto dialogueBox = ui::Text::create("��ã���ʿ��", "fonts/Marker Felt.ttf", 24);
    dialogueBox->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 4));
    this->addChild(dialogueBox, 20);

    // �ӳ�3����Ƴ��Ի���
    this->runAction(Sequence::create(DelayTime::create(3), RemoveSelf::create(), nullptr));
}

// ��ʼ���¼�������
void GameScene::initEventListeners()
{
    // �����¼�
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // ����¼���������ֲ���
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
    // ����Ÿ������ͼ���ļ���
    std::vector<std::string> cropImages = {
        "Carrot_1.png", "Carrot_2.png", "Carrot_3.png",
        "Pumpkin_1.png", "Pumpkin_2.png", "Pumpkin_3.png",
        "Lajiao_1.png", "Lajiao_2.png", "Lajiao_3.png"
    };

    // ʹ�ü�������ѭ��ѡ������
    static int cropIndex = 0;

    // �����µ�����Sprite
    auto crop = Sprite::create(cropImages[cropIndex]);
    crop->setScale(0.1f);
    crop->setPosition(locationInMap);

    // ��������ӵ�tiledMap�ϣ���֤�����ͼ�����ź�λ��ͬ��
    tiledMap->addChild(crop, 15);

    // ��������ӵ� _crops ������
    _crops.push_back(crop);

    // �����־����ʾ��ֲ�����λ��
    CCLOG("Planted a crop (%s) at (%.2f,%.2f)", cropImages[cropIndex].c_str(), locationInMap.x, locationInMap.y);

    // ���¼�������ȷ����0��8֮��ѭ��
    cropIndex = (cropIndex + 1) % cropImages.size();
}

void GameScene::harvestCropAt(const Vec2& locationInMap)
{
    // ����_crops�������λ���Ƿ���ĳ�������boundingBox��
    for (auto it = _crops.begin(); it != _crops.end(); ++it)
    {
        auto crop = *it;
        Rect boundingBox = crop->getBoundingBox();
        if (boundingBox.containsPoint(locationInMap))
        {
            // �ҵ�Ҫ�ջ������
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

            // ����Ļ����ת��Ϊ��ͼ����
            // ��ΪtiledMap�������ź;��У����ʹ��tiledMap->convertToNodeSpace����ȡ�����tiledMap������
            auto locationInMap = tiledMap->convertToNodeSpace(locationInView);

            if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT)
            {
                // �����ֲ
                plantCropAt(locationInMap, tiledMap);
            }
            else if (mouseButton == EventMouse::MouseButton::BUTTON_RIGHT)
            {
                // �Ҽ��ջ�
                harvestCropAt(locationInMap);
            }
        }
    }
}

