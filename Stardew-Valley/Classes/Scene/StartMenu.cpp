/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : StartMenu.cpp
 * �ļ�����      : StartMenuScene�Ĺ���ʵ��
 * ����          : �ܽ�˶
 * ��������      : 2024/12/04
 * ���֤        : MIT License
 ****************************************************************/
#include "StartMenu.h"
#include "SimpleAudioEngine.h"
#include "Player/Player.h"
#include "proj.win32/Constant.h"
#include "Button/HoverButton.h"
#include "proj.win32/AudioPlayer.h"
//cocos2d �����ռ�
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
    // �˳�����
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
    // ��������ͼ��
    _backgroundSprite = Sprite::create("StartMenu/BackGround.png");
    _title = Sprite::create("StartMenu/Title.png");
    // ��ȡ������ԭʼ�ߴ�
    Size titleSize = _title->getContentSize();

    // �������ű�����ȷ������ͼ������Ļ�ߴ�һ��
    float scaleX = visibleSize.width / titleSize.width;
    float scaleY = visibleSize.height / titleSize.height;
    float scale = std::max(scaleX, scaleY); // ���ֿ�߱ȣ����ܻ�ü�

    // Ӧ�����ű���
    _title->setScale(scale / 3);
	//_title->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(_title, 1); // ���������Ԫ�ص�����
    // ���ñ���ͼ���λ�õ���Ļ����
    _title->setPosition(Vec2(visibleSize.width / 2,visibleSize.height*0.75));

    // ��ȡ������ԭʼ�ߴ�
    Size bgSize = _backgroundSprite->getContentSize();

    // �������ű�����ȷ������ͼ������Ļ�ߴ�һ��
    float scaleXBg = visibleSize.width / bgSize.width;
    float scaleYBg = visibleSize.height / bgSize.height;
    float scaleBg = std::max(scaleXBg, scaleYBg); // ���ֿ�߱ȣ����ܻ�ü�

    // Ӧ�����ű���
    _backgroundSprite->setScale(scaleBg);

    // ���ñ���ͼ���λ�õ���Ļ����
    _backgroundSprite->setPosition(Vec2(visibleSize.width / 2,
    visibleSize.height / 2));

    // ��������ӵ�������
    this->addChild(_backgroundSprite, 0); // ���������Ԫ�ص�����

   constexpr float offSetX = 200;
    auto _exitButton = HoverButton::create(static_cast<std::string>("../Resources/StartMenu") + static_cast<std::string>("/Exit_") + static_cast<std::string>("D.png"),
        static_cast<std::string>("../Resources/StartMenu") + static_cast<std::string>("/Exit_") + static_cast<std::string>("L.png"),
        static_cast<std::string>("../Resources/StartMenu") + static_cast<std::string>("/Exit_") + static_cast<std::string>("L.png"));
    Size buttonSize = _exitButton->getContentSize();
	float scaleButton = visibleSize.width / buttonSize.width;
    _exitButton->setPosition(Vec2(visibleSize.width / 2 + offSetX ,visibleSize.height*0.3));
    _exitButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
                // ���ص����Ч
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
                // ���ص����Ч
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
