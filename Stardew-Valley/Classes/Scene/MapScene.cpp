/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : MapScene.cpp
 * �ļ�����      : ��ͼ������ʵ��
 * ����          : 
 * ��������      : 2024/12/07
 * ���֤        : MIT License
 ****************************************************************/
#include "MapScene.h"
#include "cocos2d.h"
#include "Map1Scene.h"
#include "Map2Scene.h"
#include "Map3Scene.h"
extern std::string g_selectedMap; // ����ȫ�ֱ���

USING_NS_CC;

Scene* MapScene::createScene()
{
    return MapScene::create();
}

bool MapScene::init()
{
    if (!Scene::init())
        return false;

    mapNames.push_back("Map/Map1/map1.tmx");
    mapNames.push_back("Map/Map2/map2.tmx");
    mapNames.push_back("Map/Map3/map3.tmx");
    currentMapIndex = 0;

    mapMenu = Menu::create();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    mapMenu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    for (size_t i = 0; i < mapNames.size(); ++i)
    {
        std::string labelText = "Map " + std::to_string(i + 1);
        auto label = Label::createWithSystemFont(labelText, "Arial", 24);
        auto item = MenuItemLabel::create(label, [=](Ref* sender) {
            currentMapIndex = i;
            onMapItemClicked(sender);
            });
        mapMenu->addChild(item);
    }

    mapMenu->alignItemsVerticallyWithPadding(20);
    this->addChild(mapMenu, 1);

    // ��ӷ����߼������� M ����
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event) {
        if (code == EventKeyboard::KeyCode::KEY_M)
        {
            CCLOG("MapScene: ��������Ϸ����");
            Director::getInstance()->popScene();
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void MapScene::onMapItemClicked(Ref* sender)
{
    CCLOG("�л�����ͼ��%s", mapNames[currentMapIndex].c_str());
    g_selectedMap = mapNames[currentMapIndex]; // ��¼ѡ�еĵ�ͼ����

    // ����g_selectedMapֵ�����������ĸ�����
    if (g_selectedMap == "Map/Map1/map1.tmx")
    {
        Director::getInstance()->replaceScene(Map1Scene::createScene());
    }
    else if (g_selectedMap == "Map/Map2/map2.tmx")
    {
        Director::getInstance()->replaceScene(Map2Scene::createScene());
    }
    else if (g_selectedMap == "Map/Map3/map3.tmx")
    {
        Director::getInstance()->replaceScene(Map3Scene::createScene());
    }
}

