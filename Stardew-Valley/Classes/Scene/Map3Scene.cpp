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

    // 加载Map3，假设BaseMapScene的loadMap会将_tiledMap的anchorPoint设为(0,0)、position为(0,0)，并缩放地图
    // 比如缩放5倍
    loadMap("Map/Map3/map3.tmx", 5.0f);
    initPlayer();

    _npc = nullptr;

    return true;
}

void Map3Scene::onEnter()
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

                    _npc = Sprite::create("npc.png");
                    _npc->setScale(2.0f); // 增大NPC的缩放，使点击区域更大
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

void Map3Scene::showDialogue()
{
    CCLOG("in");
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto dialogueBox = ui::Text::create("你好，勇士！", "fonts/Marker Felt.ttf", 24);
    dialogueBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
    this->addChild(dialogueBox, 20);

    dialogueBox->runAction(Sequence::create(
        DelayTime::create(3),
        CallFunc::create([dialogueBox]() {
            dialogueBox->removeFromParent();
            }),
        nullptr));
}
