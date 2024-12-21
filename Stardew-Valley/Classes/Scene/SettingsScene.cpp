/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : SettingsScene.cpp
 * 文件功能      : 设置场景类实现
 * 作者          : 曹津硕
 * 更新日期      : 2024/12/20
 * 许可证        : MIT License
 ****************************************************************/

#include "SettingsScene.h"
#include "Button/HoverButton.h"
#include "Scene/StartMenu.h"
#include "proj.win32/AudioPlayer.h"
#include "proj.win32/Constant.h"

 // 使用Cocos2d命名空间中的特定类，简化代码书写
using cocos2d::Scene;
using cocos2d::Sprite;
using cocos2d::Label;
using cocos2d::Vec2;

// 音频引擎外部变量声明
extern int g_backgroundMusicSign;
extern int g_soundEffectSign;
extern float g_backgroundMusicVolumn;
extern float g_soundEffectVolumn;

// 创建场景的静态方法
Scene* SettingsScene::createScene()
{
    // 使用C++11的`auto`关键字自动推断类型
    auto scene = Scene::create(); // C++11特性：auto关键字
    auto layer = SettingsScene::create(); // C++11特性：auto关键字
    scene->addChild(layer);
    return scene;
}

// 初始化场景
bool SettingsScene::init()
{
    // 调用基类的初始化方法，若失败则返回false
    if (!Scene::init()) {
        return false;
    }

    // 加载背景音乐，循环播放
    audioPlayer("../Resources/Music/Setting.mp3", true);

    // 加载并设置背景图片
    const auto screenSize = cocos2d::Director::getInstance()->getVisibleSize(); // C++11特性：auto关键字
    const auto background = Sprite::create("../Resources/SettingScene/SettingScene.png"); // C++11特性：auto关键字
    background->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
    Size bgSize = background->getContentSize();
    // 适应屏幕大小
    background->setScale(screenSize.width / bgSize.width, screenSize.height / bgSize.height);
    this->addChild(background);

    // 创建背景音乐音量滑动条
    auto backgroundMusicVolumnSlider = cocos2d::ui::Slider::create(); // C++11特性：auto关键字
    backgroundMusicVolumnSlider->loadBarTexture("../Resources/SettingScene/SliderBackground.png");
    backgroundMusicVolumnSlider->loadSlidBallTextures("../Resources/SettingScene/SliderNodeNormal.png",
        "../Resources/SettingScene/SliderNodePress.png",
        "../Resources/SettingScene/SliderNodePress.png");
    backgroundMusicVolumnSlider->loadProgressBarTexture("../Resources/SettingScene/SliderBar.png");
    backgroundMusicVolumnSlider->setPosition(cocos2d::Vec2(screenSize.width / 2 + SETTINGS_SCENE_SLIDER_OFFSET_X, screenSize.height / 2 + SETTINGS_SCENE_BGMSLIDER_OFFSET_Y));
    backgroundMusicVolumnSlider->setPercent(g_backgroundMusicVolumn * 100);

    // 创建音效音量滑动条
    auto effectVolumnslider = cocos2d::ui::Slider::create(); // C++11特性：auto关键字
    effectVolumnslider->loadBarTexture("../Resources/SettingScene/SliderBackground.png");
    effectVolumnslider->loadSlidBallTextures("../Resources/SettingScene/SliderNodeNormal.png",
        "../Resources/SettingScene/SliderNodePress.png",
        "../Resources/SettingScene/SliderNodePress.png");
    effectVolumnslider->loadProgressBarTexture("../Resources/SettingScene/SliderBar.png");
    effectVolumnslider->setPosition(cocos2d::Vec2(screenSize.width / 2 + SETTINGS_SCENE_SLIDER_OFFSET_X, screenSize.height / 2 + SETTINGS_SCENE_EFFSLIDER_OFFSET_Y));
    effectVolumnslider->setPercent(g_soundEffectVolumn * 100);

    // 为背景音乐音量滑动条添加事件监听器
    backgroundMusicVolumnSlider->addEventListener([=](Ref* sender, cocos2d::ui::Slider::EventType type) { // C++11特性：Lambda表达式
        if (type == cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN) {
            // 播放点击音效
            audioPlayer("../Resources/Music/ClickSoundEffect.mp3", false);
        }
        if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            const cocos2d::ui::Slider* slider = dynamic_cast<cocos2d::ui::Slider*>(sender); // C++11特性：dynamic_cast
            if (slider) {
                float percent = slider->getPercent();
                g_backgroundMusicVolumn = percent / 100.0f;
                // 设置背景音乐音量
                cocos2d::experimental::AudioEngine::setVolume(g_backgroundMusicSign, g_backgroundMusicVolumn);
            }
        }
        });

    // 为音效音量滑动条添加事件监听器
    effectVolumnslider->addEventListener([=](Ref* sender, cocos2d::ui::Slider::EventType type) { // C++11特性：Lambda表达式
        if (type == cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN) {
            // 播放点击音效
            audioPlayer("../Resources/Music/ClickSoundEffect.mp3", false);
        }
        if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            const cocos2d::ui::Slider* slider = dynamic_cast<cocos2d::ui::Slider*>(sender); // C++11特性：dynamic_cast
            if (slider) {
                float percent = slider->getPercent();
                g_soundEffectVolumn = percent / 100.0f;
                // 设置音效音量
                cocos2d::experimental::AudioEngine::setVolume(g_soundEffectSign, g_soundEffectVolumn);
            }
        }
        });

    // 将滑动条添加到场景中
    this->addChild(backgroundMusicVolumnSlider);
    this->addChild(effectVolumnslider);

    // 创建返回菜单按钮
    auto returnMenuButton = HoverButton::create("../Resources/SettingScene/Back_D.png",
        "../Resources/SettingScene/Back_L.png",
        "../Resources/SettingScene/Back_L.png"); // C++11特性：auto关键字
    returnMenuButton->setScale(0.3f);
    returnMenuButton->setPosition(Vec2(screenSize.width / 2 + SETTINGS_SCENE_RETURN_MENU_BUTTON_OFFSET_X, screenSize.height / 2 + SETTINGS_SCENE_RETURN_MENU_BUTTON_OFFSET_Y));
    returnMenuButton->addTouchEventListener([](Ref* sender, cocos2d::ui::Widget::TouchEventType type) { // C++11特性：Lambda表达式
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            // 播放点击音效
            audioPlayer("../Resources/Music/Back.mp3", false);

            // 切换到主菜单场景，使用C++11的统一初始化列表
            cocos2d::Director::getInstance()->replaceScene(
                cocos2d::TransitionFade::create(SCENE_TRANSITION_DURATION, StartMenu::createScene(), cocos2d::Color3B::WHITE)
            ); // C++11特性：统一初始化列表
        }
        });
    this->addChild(returnMenuButton);

    return true;
}
