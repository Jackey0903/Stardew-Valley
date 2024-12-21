

#include "SettingsScene.h"
#include "Button/HoverButton.h"
#include "Scene/StartMenu.h"
#include "proj.win32/AudioPlayer.h"
#include "proj.win32/Constant.h"

  // 命名空间
using cocos2d::Scene;
using cocos2d::Sprite;
using cocos2d::Label;
using cocos2d::Vec2;

// 音频引擎外部变量设置
extern int g_backgroundMusicSign;
extern int g_soundEffectSign;
extern float g_backgroundMusicVolumn;
extern float g_soundEffectVolumn;



// 创建场景
Scene* SettingsScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SettingsScene::create();
    scene->addChild(layer);
    return scene;
}

// 初始化场景
bool SettingsScene::init()
{
    // 创建场景
    if (!Scene::init()) {
        return false;
    }

    // 加载音乐
    audioPlayer("../Resources/Music/Setting.mp3", true);

    // 加载背景
    const auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();
    const auto background = Sprite::create("../Resources/SettingScene/SettingScene.png");
    background->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
    Size bgSize = background->getContentSize();
	background->setScale(screenSize.width / bgSize.width, screenSize.height / bgSize.height);
    this->addChild(background);

    // 创建滑动条
    auto backgroundMusicVolumnSlider = cocos2d::ui::Slider::create();
    backgroundMusicVolumnSlider->loadBarTexture("../Resources/SettingScene/SliderBackground.png");
    backgroundMusicVolumnSlider->loadSlidBallTextures("../Resources/SettingScene/SliderNodeNormal.png",
        "../Resources/SettingScene/SliderNodePress.png",
        "../Resources/SettingScene/SliderNodePress.png");
    backgroundMusicVolumnSlider->loadProgressBarTexture("../Resources/SettingScene/SliderBar.png");
    backgroundMusicVolumnSlider->setPosition(cocos2d::Vec2(screenSize.width / 2 + SETTINGS_SCENE_SLIDER_OFFSET_X, screenSize.height / 2 + SETTINGS_SCENE_BGMSLIDER_OFFSET_Y));
    backgroundMusicVolumnSlider->setPercent(g_backgroundMusicVolumn * 100);
    auto effectVolumnslider = cocos2d::ui::Slider::create();
    effectVolumnslider->loadBarTexture("../Resources/SettingScene/SliderBackground.png");
    effectVolumnslider->loadSlidBallTextures("../Resources/SettingScene/SliderNodeNormal.png",
        "../Resources/SettingScene/SliderNodePress.png",
        "../Resources/SettingScene/SliderNodePress.png");
    effectVolumnslider->loadProgressBarTexture("../Resources/SettingScene/SliderBar.png");
    effectVolumnslider->setPosition(cocos2d::Vec2(screenSize.width / 2 + SETTINGS_SCENE_SLIDER_OFFSET_X, screenSize.height / 2 + SETTINGS_SCENE_EFFSLIDER_OFFSET_Y));
    effectVolumnslider->setPercent(g_soundEffectVolumn * 100);

    // 为滑动条添加事件处理器
    backgroundMusicVolumnSlider->addEventListener([=](Ref* sender, cocos2d::ui::Slider::EventType type) {
        if (type == cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN) {
            // 加载点击音效
            audioPlayer("../Resources/Music/ClickSoundEffect.mp3", false);
        }
        if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            const cocos2d::ui::Slider* backgroundMusicVolumnSlider = dynamic_cast<cocos2d::ui::Slider*>(sender);
            const float percent = backgroundMusicVolumnSlider->getPercent();
            g_backgroundMusicVolumn = percent / 100.0f;
            cocos2d::experimental::AudioEngine::setVolume(g_backgroundMusicSign, g_backgroundMusicVolumn);
        }
        });
    effectVolumnslider->addEventListener([=](Ref* sender, cocos2d::ui::Slider::EventType type) {
        if (type == cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN) {
            // 加载点击音效
            audioPlayer("../Resources/Music/ClickSoundEffect.mp3", false);
        }
        if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            const cocos2d::ui::Slider* effectVolumnslider = dynamic_cast<cocos2d::ui::Slider*>(sender);
            const float percent = effectVolumnslider->getPercent();
            g_soundEffectVolumn = percent / 100.0f;
            cocos2d::experimental::AudioEngine::setVolume(g_soundEffectSign, g_soundEffectVolumn);
        }
        });

    // 将滑动条添加至场景
    this->addChild(backgroundMusicVolumnSlider);
    this->addChild(effectVolumnslider);

    // 创建按钮
    auto returnMenuButton = HoverButton::create("../Resources/SettingScene/Back_D.png",
        "../Resources/SettingScene/Back_L.png",
        "../Resources/SettingScene/Back_L.png");
	returnMenuButton->setScale(0.3f);
    returnMenuButton->setPosition(Vec2(screenSize.width / 2 + SETTINGS_SCENE_RETURN_MENU_BUTTON_OFFSET_X, screenSize.height / 2 + SETTINGS_SCENE_RETURN_MENU_BUTTON_OFFSET_Y));
    returnMenuButton->addTouchEventListener([](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            // 加载点击音效
            audioPlayer("../Resources/Music/Back.mp3", false);

            cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(SCENE_TRANSITION_DURATION, StartMenu::createScene(), cocos2d::Color3B::WHITE));
        }
        });
    this->addChild(returnMenuButton);

    

    return true;
}