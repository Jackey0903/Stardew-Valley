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
//cocos2d 命名空间
USING_NS_CC;

Scene* StartMenu::createScene()
{
    auto scene = Scene::create();
    auto layer = StartMenu::create();
    scene->addChild(layer);
    return scene;
}

void StartMenu::exitGame(Ref* pSender)
{
    // 退出程序
    Director::getInstance()->end();
}
void StartMenu::startGame(Ref* pSender)
{

    cocos2d::Director::getInstance()->
        replaceScene(cocos2d::TransitionFade::create(SCENE_TRANSITION_DURATION,
            Player::createScene(), cocos2d::Color3B::WHITE));
}

bool StartMenu::init()
{
    if (!Scene::init())
    {
        return false;
    }
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	CCLOG("size = (%f,%f)", visibleSize.width, visibleSize.height);
	CCLOG("origin = (%f,%f)", origin.x, origin.y);
    // 创建背景图像
    _backgroundSprite = Sprite::create("StartMenu/BackGround.png");
    _title = Sprite::create("StartMenu/Title.png");
    // 获取背景的原始尺寸
    Size titleSize = _title->getContentSize();

    // 计算缩放比例：确保背景图像与屏幕尺寸一致
    float scaleX = visibleSize.width / titleSize.width;
    float scaleY = visibleSize.height / titleSize.height;
    float scale = std::max(scaleX, scaleY); // 保持宽高比，可能会裁剪

    // 应用缩放比例
    _title->setScale(scale / 3);
	//_title->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(_title, 1); // 添加在其他元素的下面
    // 设置背景图像的位置到屏幕中心
    _title->setPosition(Vec2(visibleSize.width / 2,visibleSize.height*0.75));

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
    this->addChild(_backgroundSprite, 0); // 添加在其他元素的下面

   constexpr float offSetX = 200;
    auto _exitButton = HoverButton::create(static_cast<std::string>("../Resources/StartMenu") + static_cast<std::string>("/Exit_") + static_cast<std::string>("D.png"),
        static_cast<std::string>("../Resources/StartMenu") + static_cast<std::string>("/Exit_") + static_cast<std::string>("L.png"),
        static_cast<std::string>("../Resources/StartMenu") + static_cast<std::string>("/Exit_") + static_cast<std::string>("L.png"));
    Size buttonSize = _exitButton->getContentSize();
	float scaleButton = visibleSize.width / buttonSize.width;
    _exitButton->setPosition(Vec2(visibleSize.width / 2 + offSetX ,visibleSize.height*0.3));
    _exitButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
                // 加载点击音效
                audioPlayer("../Resources/Music/ClickSoundEffect.mp3", false);
				this->exitGame(sender);
        }
    });
    _exitButton->setScale(scaleButton / 6);
    this->addChild(_exitButton,5);

    auto _startButton = HoverButton::create(static_cast<std::string>("../Resources/StartMenu") + static_cast<std::string>("/Start_") + static_cast<std::string>("D.png"),
            static_cast<std::string>("../Resources/StartMenu") + static_cast<std::string>("/Start_") + static_cast<std::string>("L.png"),
            static_cast<std::string>("../Resources/StartMenu") + static_cast<std::string>("/Start_") + static_cast<std::string>("L.png"));
    _startButton->setPosition(Vec2(visibleSize.width / 2 - offSetX, visibleSize.height * 0.3));
    _startButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
                // 加载点击音效
                audioPlayer("../Resources/Music/ClickSoundEffect.mp3", false);
                this->startGame(sender);
        }
    });
    _startButton->setScale(scaleButton / 6);
    this->addChild(_startButton, 5);

    return true;
}

void StartMenu::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
