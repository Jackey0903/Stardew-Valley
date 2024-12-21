/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : StartMenu.cpp
 * 文件功能      : StartMenuScene的功能实现
 * 作者          : 曹津硕
 * 更新日期      : 2024/12/04
 * 许可证        : MIT License
 ****************************************************************/

#include "StartMenu.h"
#include "SimpleAudioEngine.h"
#include "Player/Player.h"
#include "proj.win32/Constant.h"
#include "Button/HoverButton.h"
#include "proj.win32/AudioPlayer.h"
#include "Scene/GameScene.h"
#include "Scene/Map2Scene.h"
#include "Scene/Map3Scene.h"
#include "Scene/SettingsScene.h"

 // 使用Cocos2d命名空间
USING_NS_CC;

// 创建场景的静态方法
Scene* StartMenu::createScene()
{
    // 使用C++11的`auto`关键字自动推断类型
    auto scene = Scene::create(); // C++11特性：auto关键字
    auto layer = StartMenu::create(); // C++11特性：auto关键字
    scene->addChild(layer);
    return scene;
}

// 退出游戏的回调函数
void StartMenu::exitGame(Ref* pSender)
{
    // 退出程序
    Director::getInstance()->end();
}

// 开始游戏的回调函数
void StartMenu::startGame(Ref* pSender)
{
    // 使用C++11的`auto`关键字自动推断类型
    cocos2d::Director::getInstance()->
        replaceScene(cocos2d::TransitionFade::create(SCENE_TRANSITION_DURATION,
            Map3Scene::createScene(), cocos2d::Color3B::WHITE)); // C++11特性：统一初始化列表
}

// 打开设置界面的回调函数
void StartMenu::settings(Ref* pSender)
{
    cocos2d::Director::getInstance()->
        replaceScene(cocos2d::TransitionFade::create(SCENE_TRANSITION_DURATION,
            SettingsScene::createScene(), cocos2d::Color3B::WHITE)); // C++11特性：统一初始化列表
}

// 初始化场景
bool StartMenu::init()
{
    // 调用基类的初始化方法，若失败则返回false
    if (!Scene::init())
    {
        return false;
    }

    // 获取可视区域的大小和原点
    const auto visibleSize = Director::getInstance()->getVisibleSize(); // C++11特性：auto关键字
    const Vec2 origin = Director::getInstance()->getVisibleOrigin(); // C++11特性：auto关键字

    // 输出屏幕尺寸和原点信息到日志
    CCLOG("size = (%f,%f)", visibleSize.width, visibleSize.height);
    CCLOG("origin = (%f,%f)", origin.x, origin.y);

    // 创建背景图像
    _backgroundSprite = Sprite::create("StartMenu/BackGround.png");
    _title = Sprite::create("StartMenu/Title.png");

    // 获取标题的原始尺寸
    Size titleSize = _title->getContentSize();

    // 计算缩放比例：确保标题图像与屏幕尺寸一致
    float scaleX = visibleSize.width / titleSize.width;
    float scaleY = visibleSize.height / titleSize.height;
    float scale = std::max(scaleX, scaleY); // 保持宽高比，可能会裁剪

    // 应用缩放比例
    _title->setScale(scale / 3);
    this->addChild(_title, 1); // 添加在其他元素的上方

    // 设置标题图像的位置到屏幕上方中心
    _title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.75));

    // 获取背景的原始尺寸
    Size bgSize = _backgroundSprite->getContentSize();

    // 计算缩放比例：确保背景图像与屏幕尺寸一致
    float scaleXBg = visibleSize.width / bgSize.width;
    float scaleYBg = visibleSize.height / bgSize.height;
    float scaleBg = std::max(scaleXBg, scaleYBg); // 保持宽高比，可能会裁剪

    // 应用缩放比例
    _backgroundSprite->setScale(scaleBg);

    // 设置背景图像的位置到屏幕中心
    _backgroundSprite->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height / 2));

    // 将背景添加到场景中
    this->addChild(_backgroundSprite, 0); // 添加在其他元素的下方

    // 定义按钮的偏移量
    constexpr float offSetX = 200;

    // 创建退出按钮
    auto _exitButton = HoverButton::create(
        "../Resources/StartMenu/Exit_D.png", // 默认状态图片
        "../Resources/StartMenu/Exit_L.png", // 选中状态图片
        "../Resources/StartMenu/Exit_L.png"  // 按下状态图片
    ); // C++11特性：`auto`关键字和统一初始化
    Size buttonSize = _exitButton->getContentSize();
    float scaleButton = visibleSize.width / buttonSize.width;
    _exitButton->setPosition(Vec2(visibleSize.width / 2 + offSetX, visibleSize.height * 0.3));
    _exitButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) { // C++11特性：Lambda表达式
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            // 播放点击音效
            audioPlayer("../Resources/Music/ClickSoundEffect.mp3", false);
            this->exitGame(sender);
        }
        });
    _exitButton->setScale(scaleButton / 6);
    this->addChild(_exitButton, 5);

    // 创建开始游戏按钮
    auto _startButton = HoverButton::create(
        "../Resources/StartMenu/Start_D.png", // 默认状态图片
        "../Resources/StartMenu/Start_L.png", // 选中状态图片
        "../Resources/StartMenu/Start_L.png"  // 按下状态图片
    );
    _startButton->setPosition(Vec2(visibleSize.width / 2 - offSetX, visibleSize.height * 0.3));
    _startButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) { // C++11特性：Lambda表达式
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            // 播放点击音效
            audioPlayer("../Resources/Music/ClickSoundEffect.mp3", false);
            this->startGame(sender);
        }
        });
    _startButton->setScale(scaleButton / 6);
    this->addChild(_startButton, 5);

    // 创建设置按钮
    auto _settingButton = HoverButton::create(
        "../Resources/SettingScene/Setting_D.png", // 默认状态图片
        "../Resources/SettingScene/Setting_L.png", // 选中状态图片
        "../Resources/SettingScene/Setting_L.png"  // 按下状态图片
    );
    _settingButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) { // C++11特性：Lambda表达式
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            // 播放点击音效
            audioPlayer("../Resources/Music/ClickSoundEffect.mp3", false);
            this->settings(sender);
        }
        });
    Size settingButtonSize = _settingButton->getContentSize();
    _settingButton->setScale(scaleButton / SETTING_BUTTON_SCALE);
    Size realSettingSize = settingButtonSize * scaleButton / SETTING_BUTTON_SCALE;

    // 设置设置按钮的位置
    _settingButton->setPosition(Vec2(visibleSize.width - realSettingSize.width + SETTING_BUTTON_OFFSET, realSettingSize.height - SETTING_BUTTON_OFFSET));
    this->addChild(_settingButton, 5);

    // 播放背景音乐，循环播放
    audioPlayer("../Resources/Music/Stardew Valley.mp3", true);

    return true;
}

// 关闭菜单的回调函数（未使用）
void StartMenu::menuCloseCallback(Ref* pSender)
{
    // 关闭Cocos2d-x游戏场景并退出应用程序
    Director::getInstance()->end();

    /* 若存在原生iOS屏幕而不希望退出应用程序，则不要使用Director::getInstance()->end()，
       而是触发在RootViewController.mm中创建的自定义事件，如下所示：

    EventCustom customEndEvent("game_scene_close_event");
    _eventDispatcher->dispatchEvent(&customEndEvent);
    */
}
