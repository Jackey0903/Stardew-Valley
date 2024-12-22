/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : MapScene.cpp
 * 文件功能      : 地图场景类实现
 * 作者          : 胡浩杰，曹津硕
 * 更新日期      : 2024/12/22
 * 许可证        : MIT License
 ****************************************************************/

#include "MapScene.h"
#include "cocos2d.h"
#include "Map1Scene.h"
#include "Map2Scene.h"
#include "Map3Scene.h"
#include "proj.win32/AudioPlayer.h"
#include "Button/HoverButton.h"

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
    auto _MapScene = Sprite::create("../Resources/MapScene/MapScene.png");
    Size bgSize = _MapScene->getContentSize();
    // 创建菜单用于显示地图选项
    mapMenu = Menu::create();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    // 设置菜单位置为屏幕中心
    mapMenu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    _MapScene->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    float scale = visibleSize.width / bgSize.width;
    _MapScene->setScale(scale);

    this->addChild(_MapScene, 0);




    // 创建map2选择戏按钮
    auto _map2 = HoverButton::create(
        "../Resources/MapScene/Map2_D.png", // 默认状态图片
        "../Resources/MapScene/Map2_L.png", // 选中状态图片
        "../Resources/MapScene/Map2_L.png"  // 按下状态图片
    );
    _map2->setPosition(Vec2(visibleSize.width / 2 + MAP2_BUTTON_OFFSET_X, visibleSize.height + MAP2_BUTTON_OFFSET_Y));
    _map2->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) { // C++11特性：Lambda表达式
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            // 播放点击音效
            audioPlayer("../Resources/Music/ClickSoundEffect.mp3", false);
            Director::getInstance()->replaceScene(Map2Scene::createScene());
        }
        });
    _map2->setScale(scale);
    this->addChild(_map2, 5);


    auto _map3 = HoverButton::create(
        "../Resources/MapScene/Map3_D.png", // 默认状态图片
        "../Resources/MapScene/Map3_L.png", // 选中状态图片
        "../Resources/MapScene/Map3_L.png"  // 按下状态图片
    );
    _map3->setPosition(Vec2(visibleSize.width / 2 + MAP3_BUTTON_OFFSET_X, visibleSize.height + MAP3_BUTTON_OFFSET_Y));
    _map3->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) { // C++11特性：Lambda表达式
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            // 播放点击音效
            audioPlayer("../Resources/Music/ClickSoundEffect.mp3", false);
            Director::getInstance()->replaceScene(Map3Scene::createScene());
        }
        });
    _map3->setScale(scale);
    this->addChild(_map3, 5);

    auto _map1 = HoverButton::create(
        "../Resources/MapScene/Map1_D.png", // 默认状态图片
        "../Resources/MapScene/Map1_L.png", // 选中状态图片
        "../Resources/MapScene/Map1_L.png"  // 按下状态图片
    );
    _map1->setPosition(Vec2(visibleSize.width / 2 + MAP1_BUTTON_OFFSET_X, visibleSize.height + MAP1_BUTTON_OFFSET_Y));
    _map1->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) { // C++11特性：Lambda表达式
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            // 播放点击音效
            audioPlayer("../Resources/Music/ClickSoundEffect.mp3", false);
            Director::getInstance()->replaceScene(Map1Scene::createScene());
        }
        });
    _map1->setScale(scale);
    this->addChild(_map1, 5);


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

    return true;
}

