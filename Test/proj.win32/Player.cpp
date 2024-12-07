#include "Player.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* Player::createScene()
{
    return Player::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


void Player::startWalkingAnimation(const std::string& direction)
{
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 2; ++i) {
        std::string frameName = "Walk_" + direction + "_" + std::to_string(i) + ".png";
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        frames.pushBack(frame);
    }

    _walkAnimation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto animate = Animate::create(_walkAnimation);
    _playerSprite->runAction(RepeatForever::create(animate));
}

void Player::stopWalkingAnimation()
{
    _playerSprite->stopAllActions(); // ֹͣ���ж���
    _playerSprite->setTexture("Stand_" + _currentDirection + ".png"); // ���ô���״̬
}

// �� onKeyPressed ������ _isMoving ��־
void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    _isMoving = true;

    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_A:
        if (!_isMovingLeft) {
            _isMovingLeft = true;
            _currentTexture = "Stand_Left.png"; // ���õ�ǰ����
            _playerSprite->setTexture(_currentTexture); // ���¾�������
            _currentDirection = "Left"; // ���µ�ǰ����
            startWalkingAnimation(_currentDirection); // ��ʼ����
            _isMovingRight = false; // ������
        }
        break;
    case EventKeyboard::KeyCode::KEY_D:
        if (!_isMovingRight) {
            _isMovingRight = true;
            _currentTexture = "Stand_Right.png"; // ���õ�ǰ����
            _playerSprite->setTexture(_currentTexture); // ���¾�������
            _currentDirection = "Right"; // ���µ�ǰ����
            startWalkingAnimation(_currentDirection); // ��ʼ����
            _isMovingLeft = false; // ������
        }
        break;
    case EventKeyboard::KeyCode::KEY_W:
        if (!_isMovingUp) {
            _isMovingUp = true;
            _currentTexture = "Stand_Up.png"; // ���õ�ǰ����
            _playerSprite->setTexture(_currentTexture); // ���¾�������
            _currentDirection = "Up"; // ���µ�ǰ����
            startWalkingAnimation(_currentDirection); // ��ʼ����
            _isMovingDown = false; // ������
        }
        break;
    case EventKeyboard::KeyCode::KEY_S:
        if (!_isMovingDown) {
            _isMovingDown = true;
            _currentTexture = "Stand_Down.png"; // ���õ�ǰ����
            _playerSprite->setTexture(_currentTexture); // ���¾�������
            _currentDirection = "Down"; // ���µ�ǰ����
            startWalkingAnimation(_currentDirection); // ��ʼ����
            _isMovingUp = false; // ������
        }
        break;
    default:
        break;
    }
}

// �� onKeyReleased ������ _isMoving ��־
void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_A:
        _isMovingLeft = false;
        break;
    case EventKeyboard::KeyCode::KEY_D:
        _isMovingRight = false;
        break;
    case EventKeyboard::KeyCode::KEY_W:
        _isMovingUp = false;
        break;
    case EventKeyboard::KeyCode::KEY_S:
        _isMovingDown = false;
        break;
    default:
        break;
    }

    // �����з�����ɿ�ʱ��ֹͣ�ƶ�
    if (!_isMovingLeft && !_isMovingRight && !_isMovingUp && !_isMovingDown) {
        _isMoving = false;
        if (_currentDirection != "") {
            stopWalkingAnimation(); // ֹͣ����
            _currentDirection = ""; // ��յ�ǰ����
        }
    }
}

// on "init" you need to initialize your instance
bool Player::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // ������Ƭ��ͼ
    _tiledMap = TMXTiledMap::create("map1.tmx"); // �滻Ϊ��� .tmx �ļ�·��
    if (_tiledMap == nullptr) {
        problemLoading("map1.tmx");
        CCLOG("Error: _tiledMap is null!");
        return false;
    }
    this->addChild(_tiledMap, 0); // ����ͼ��ӵ�������




    // ��ȡ��ͼ��

    _wallLayer = _tiledMap->getLayer("wall"); // ��ȡǽ��ͼ��

    // �����Ҫ������ԶԵ�ͼ����н�һ���Ĳ���
    // �����޸�λ�á�͸���ȵ�


    // Ԥ��������ͼ���ļ�
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Left.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Right.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Up.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Down.plist");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //// ������ͼ����
    //_backgroundSprite = Sprite::create("Pierres General Store.png");
    //if (_backgroundSprite == nullptr)
    //{
    //    problemLoading("Pierres General Store.png");
    //}
    //else
    //{
    //    // ��ȡ����ԭʼ�ߴ�
    //    Size bgSize = _backgroundSprite->getContentSize();

    //    // ��ȡ�ɼ��ߴ�
    //    auto visibleSize = Director::getInstance()->getVisibleSize();

    //    // �������ű���
    //    float scaleX = visibleSize.width / bgSize.width;
    //    float scaleY = visibleSize.height / bgSize.height;
    //    float scale = std::max(scaleX, scaleY); // ���ֿ�߱ȣ����ܻ�ü�

    //    // Ӧ�����ű���
    //    _backgroundSprite->setScale(scale);

    //    // ���ñ���ͼ���λ�õ���Ļ����
    //    _backgroundSprite->setPosition(Vec2(visibleSize.width / 2,
    //        visibleSize.height / 2));

    //    // ��������ӵ�������
    //    this->addChild(_backgroundSprite, -1); // ���������Ԫ�ص�����
    //}

    // ����һ������
    _playerSprite = Sprite::create("Stand_Down.png"); // ���丳ֵ����Ա����
    if (_playerSprite == nullptr)
    {
        problemLoading("'player1.png'");
    }
    else
    {
        // ���þ����λ��
        _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // ��������Сһ��
        _playerSprite->setScale(0.5f); // ��Сһ��

        // ��������ӵ�������
        this->addChild(_playerSprite, 2);
    }

    // ���������¼�������
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // ���ø��»ص�
    this->schedule(CC_SCHEDULE_SELECTOR(Player::update));

    // ��ʼ������
    _walkAnimation = nullptr; // ��ʼ��Ϊ nullptr

    return true;
}

bool Player::isCollisionWithWall(const cocos2d::Vec2& nextPosition)
{
    // ��ȡǽ��ͼ��
    if (!_wallLayer)
    {
        CCLOG("Error: 'wall' layer not found in the tiled map.");
        return false; // ���û���ҵ�ǽ��ͼ�㣬ֱ�ӷ��� false
    }

    // ����Ļ����ת��Ϊ��Ƭ����
    Size tileSize = _tiledMap->getTileSize();
    Vec2 tileCoord(
        nextPosition.x / tileSize.width,   // ���� x �������Ƭλ��
        nextPosition.y / tileSize.height   // ���� y �������Ƭλ��
    );

    // ��ȡ��λ�õ���Ƭ GID
    int tileGID = _wallLayer->getTileGIDAt(tileCoord);

    // �������Ƭ�� GID ��Ϊ 0����ʾ��λ����ǽ��
    return (tileGID != 0); // �������ǽ�ڣ����� true
}



// ��� update ���������������ƶ�
void Player::update(float delta)
{
    if (_isMoving) {
        Vec2 loc = _playerSprite->getPosition();

        if (_isMovingLeft) {
            loc.x -= 200 * delta;
            //_backgroundSprite->setPositionX(_backgroundSprite->getPositionX() + 200 * delta); // ��������
        }
        if (_isMovingRight) {
            loc.x += 200 * delta;
            //_backgroundSprite->setPositionX(_backgroundSprite->getPositionX() - 200 * delta); // ��������
        }
        if (_isMovingUp) {
            loc.y += 200 * delta;
            //_backgroundSprite->setPositionY(_backgroundSprite->getPositionY() - 200 * delta); // ��������
        }
        if (_isMovingDown) {
            loc.y -= 200 * delta;
            //_backgroundSprite->setPositionY(_backgroundSprite->getPositionY() + 200 * delta); // ��������
        }


        // �߽��⣬���Ʊ����;����ڿɼ�������
        auto visibleSize = Director::getInstance()->getVisibleSize();
        loc.x = MAX(0, MIN(loc.x, visibleSize.width));
        loc.y = MAX(0, MIN(loc.y, visibleSize.height));

        //�����־���
        //CCLOG("Player initialized");  // ȷ���Ƿ�ɹ����� init()
        //CCLOG("Update called");       // ȷ�� update �Ƿ񱻶��ڵ���

        //_playerSprite->setPosition(loc);



        // �ж��Ƿ�����ǽ��
        if (!isCollisionWithWall(loc)) {
            _playerSprite->setPosition(loc);  // ���û����ײ���������λ��
            CCLOG("Player moved to (%f, %f)", loc.x, loc.y); // ���������Ϣ
        }
        else {
            CCLOG("Collision detected at (%f, %f)", loc.x, loc.y); // �����ײ��Ϣ
        }


    }
}

