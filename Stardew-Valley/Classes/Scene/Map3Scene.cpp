// Map3Scene.cpp
#include "Map3Scene.h"
#include "MapLayer.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* Map3Scene::createScene()
{
    return Map3Scene::create();
}

bool Map3Scene::init()
{
    if (!BaseMapScene::init())
        return false;

    // ����Map3������BaseMapScene��loadMap�Ὣ_tiledMap��anchorPoint��Ϊ(0,0)��positionΪ(0,0)�������ŵ�ͼ
    // ��������5��
    loadMap("Map/Map3/map3.tmx", 5.0f);
    initPlayer();

    _npc = nullptr;

    return true;
}

void Map3Scene::onEnter()
{
    BaseMapScene::onEnter();

    // ��Map3�ϲ���npc����㲢����NPC
    if (_tiledMap)
    {
        // ��ȡ��Ϊ"Npc"�Ķ����
        auto npcLayer = _tiledMap->getObjectGroup("Npc");
        if (npcLayer)
        {
            auto npcObjects = npcLayer->getObjects();
            for (auto& obj : npcObjects)
            {
                auto npcObject = obj.asValueMap();
                if (npcObject["name"].asString() == "Npc")
                {
                    // ��ȡNPC�ڵ�ͼ����ϵ�е�λ�ã�Tiled����Ϊ(0,0)��
                    float npcX = npcObject["x"].asFloat();
                    float npcY = npcObject["y"].asFloat();
                    Vec2 npcPosition(npcX, npcY);

                    _npc = Sprite::create("npc.png");
                    _npc->setScale(2.0f); // ����NPC�����ţ�ʹ����������
                    _npc->setPosition(npcPosition);
                    _tiledMap->addChild(_npc, 15);


                    // ���õ���¼�������
                    auto npcListener = EventListenerTouchOneByOne::create();
                    npcListener->setSwallowTouches(true);
                    npcListener->onTouchBegan = [this](Touch* touch, Event* event) {
                        // ����Ļ����ת��Ϊ_tiledMap����
                        auto locationInView = touch->getLocation();
                        auto locationInMap = _tiledMap->convertToNodeSpace(locationInView);
                        CCLOG("Touch at screen: (%.2f,%.2f)", locationInView.x, locationInView.y);
                        CCLOG("Touch in map coords: (%.2f,%.2f)", locationInMap.x, locationInMap.y);

                        Rect boundingBox = _npc->getBoundingBox();
                        CCLOG("NPC boundingBox: origin(%.2f,%.2f), size(%.2f,%.2f)",
                            boundingBox.origin.x, boundingBox.origin.y,
                            boundingBox.size.width, boundingBox.size.height);
                        // boundingBox��locationInMap����_tiledMap����ϵ��
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

void Map3Scene::showDialogue()
{
    CCLOG("in");
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto dialogueBox = ui::Text::create("��ã���ʿ��", "fonts/Marker Felt.ttf", 24);
    dialogueBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
    this->addChild(dialogueBox, 20);

    dialogueBox->runAction(Sequence::create(
        DelayTime::create(3),
        CallFunc::create([dialogueBox]() {
            dialogueBox->removeFromParent();
            }),
        nullptr));
}
