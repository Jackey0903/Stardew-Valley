/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : MapScene.cpp
 * 文件功能      : 地图场景类实现
 * 作者          : 
 * 更新日期      : 2024/12/07
 * 许可证        : MIT License
 ****************************************************************/
#include "MapScene.h"
#include "cocos2d.h"
#include "Map1Scene.h"
#include "Map2Scene.h"
#include "Map3Scene.h"
extern std::string g_selectedMap; // 声明全局变量

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

    // 添加返回逻辑：按下 M 返回
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event) {
        if (code == EventKeyboard::KeyCode::KEY_M)
        {
            CCLOG("MapScene: 返回主游戏场景");
            Director::getInstance()->popScene();
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void MapScene::onMapItemClicked(Ref* sender)
{
    CCLOG("切换到地图：%s", mapNames[currentMapIndex].c_str());
    g_selectedMap = mapNames[currentMapIndex]; // 记录选中的地图名称

    // 根据g_selectedMap值来决定加载哪个场景
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

