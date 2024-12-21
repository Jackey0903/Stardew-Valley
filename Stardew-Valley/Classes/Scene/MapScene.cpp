/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : MapScene.cpp
 * 文件功能      : 地图场景类实现
 * 作者          : 胡浩杰
 * 更新日期      : 2024/12/07
 * 许可证        : MIT License
 ****************************************************************/

#include "MapScene.h"
#include "cocos2d.h"
#include "Map1Scene.h"
#include "Map2Scene.h"
#include "Map3Scene.h"

 // 声明全局变量，用于记录选中的地图名称
extern std::string g_selectedMap;

USING_NS_CC;

// 创建场景的静态方法
Scene* MapScene::createScene()
{
    return MapScene::create();
}

// 初始化地图场景
bool MapScene::init()
{
    // 调用基类的初始化方法，若失败则返回false
    if (!Scene::init())
        return false;

    // 初始化地图名称列表
    mapNames.push_back("Map/Map1/map1.tmx");
    mapNames.push_back("Map/Map2/map2.tmx");
    mapNames.push_back("Map/Map3/map3.tmx");
    currentMapIndex = 0;

    // 创建菜单用于显示地图选项
    mapMenu = Menu::create();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    // 设置菜单位置为屏幕中心
    mapMenu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    // 使用传统的for循环创建菜单项
    for (size_t i = 0; i < mapNames.size(); ++i)
    {
        // 使用C++11的std::to_string将索引转换为字符串
        std::string labelText = "Map " + std::to_string(i + 1); // C++11特性：std::to_string

        // 创建标签
        auto label = Label::createWithSystemFont(labelText, "Arial", 24);

        // 创建菜单项，并绑定回调函数
        auto item = MenuItemLabel::create(label, [=](Ref* sender) { // C++11特性：Lambda表达式
            currentMapIndex = i;
            onMapItemClicked(sender);
            });

        // 添加菜单项到菜单中
        mapMenu->addChild(item);
    }

    // 垂直排列菜单项，并设置间距
    mapMenu->alignItemsVerticallyWithPadding(20);
    // 将菜单添加到场景中
    this->addChild(mapMenu, 1);

    // 添加键盘事件监听器，用于处理返回逻辑
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event) { // C++11特性：Lambda表达式
        if (code == EventKeyboard::KeyCode::KEY_M)
        {
            CCLOG("MapScene: 返回主游戏场景");
            Director::getInstance()->popScene(); // 返回上一个场景
        }
        };
    // 将事件监听器添加到事件分发器中
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

// 处理地图菜单项点击事件
void MapScene::onMapItemClicked(Ref* sender)
{
    CCLOG("切换到地图：%s", mapNames[currentMapIndex].c_str());
    g_selectedMap = mapNames[currentMapIndex]; // 记录选中的地图名称

    // 根据选中的地图名称决定加载哪个地图场景
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
