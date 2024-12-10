/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : Player.cpp
 * �ļ�����      : �����Ĺ���ʵ��
 * ����          : ���ƽܣ����������ܽ�˶
 * ��������      : 2024/12/07
 * ���֤        : MIT License
 ****************************************************************/
#include "Player.h"
#include "proj.win32/AudioPlayer.h"

USING_NS_CC;

Scene* Player::createScene()
{
    return Player::create();
}
bool Player::isCollidingWithWall(const cocos2d::Rect& playerRect)
{
    for (const auto& wallRect : _wallRects)
    {
        if (playerRect.intersectsRect(wallRect))
        {
            return true; // ������ײ
        }
    }
    return false; // ����ײ
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
        case EventKeyboard::KeyCode::KEY_B: // ��Ӷ� B ������Ӧ
            openBackpack(); // �򿪱���
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
        case EventKeyboard::KeyCode::KEY_B: // ��Ӷ� B ������Ӧ
            _isInBackpackScene = false; // �ͷű�־
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

    // ��ʼ������
    _backpack = new Backpack();
    _backpack->addItem(new Item("Potion", "sword.png", 10));
    _backpack->addItem(new Item("Sword", "sword.png", 1));
    _backpack->addItem(new Item("Shield", "sword.png", 1));

    // ������Ƭ��ͼ
    _tiledMap = TMXTiledMap::create("map2-1.tmx"); // �滻Ϊ��� .tmx �ļ�·��
    this->addChild(_tiledMap, 0); // ����ͼ��ӵ�������

 
	_tiledMap->setScale(3.0f); // ���ŵ�ͼ����ѡ��
    
    // ��ȡ��ͼ��
    //auto layer = _tiledMap->getLayer("grass"); // �� "LayerName" �滻Ϊ���ͼ��ʵ�ʲ�����

    // �����Ҫ������ԶԵ�ͼ����н�һ���Ĳ���
    // �����޸�λ�á�͸���ȵ�
    //layer->setPosition(Vec2(0, 0));

    // Ԥ��������ͼ���ļ�
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Left.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Right.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Up.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Down.plist");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ����һ������
    _playerSprite = Sprite::create("Stand_Down.png"); // ���丳ֵ����Ա����
    // ���þ����λ��
    _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
      
    // ��������Сһ��
    _playerSprite->setScale(0.5f); // ��Сһ��

    // ��������ӵ�������
    this->addChild(_playerSprite, 2);
	_playerSprite->setPosition(Vec2(visibleSize.width / 2-30, visibleSize.height / 2));
    // ���������¼�������
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // ���ø��»ص�
    this->schedule(CC_SCHEDULE_SELECTOR(Player::update));

    // ��ʼ������
    _walkAnimation = nullptr; // ��ʼ��Ϊ nullptr



    auto wallLayer = _tiledMap->getLayer("wall"); // ȷ����������ȷ
    if (wallLayer)
    {
        // ����������Ƭ
        for (int x = 0; x < wallLayer->getLayerSize().width; ++x)
        {
            for (int y = 0; y < wallLayer->getLayerSize().height; ++y)
            {
                auto tileGID = wallLayer->getTileGIDAt(Vec2(x, y));
                if (tileGID != 0) // ���� GID Ϊ 0 ��ʾ��Ч��Ƭ
                {
                    // ��ȡ��Ƭλ��
                    Vec2 tilePos = wallLayer->getPositionAt(Vec2(x, y));
                    // ��ȡ��Ƭ��С
                    Size tileSize = _tiledMap->getTileSize();
                    // ������Ƭ����
                    Rect tileRect = Rect(tilePos.x, tilePos.y, tileSize.width, tileSize.height);
                    _wallRects.push_back(tileRect);
                }
            }
        }
        CCLOG("Collected %zu wall rectangles.", _wallRects.size());
    }
    else
    {
        CCLOG("Wall layer is null");
    }




    return true;
}

// ��� update ���������������ƶ�
void Player::update(float delta)
{
    if (_isMoving) {
        Vec2 loc = _playerSprite->getPosition();
		Vec2 loc2 = _playerSprite->getPosition();
        if (_isMovingLeft) {
            loc.x -= 200 * delta;
            //_tiledMap->setPositionX(_tiledMap->getPositionX() + 200 * delta); // ��������
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
        _playerSprite->setPosition(loc);
        // ������ҵľ���
        Rect playerRect = _playerSprite->getBoundingBox();

        // �����ǽ�ڵ���ײ
        if (isCollidingWithWall(playerRect))
        {
            _playerSprite->setPosition(loc2);
            return;
        }

        Backpack* backpack = new Backpack();
        backpack->addItem(new Item("С������", "player.png", 10));
        backpack->addItem(new Item("��ͷ", "tool.png", 1));

        // �г������е���Ʒ
        backpack->listItems();



        _playerSprite->setPosition(loc);
    }
}

void Player::openBackpack() {
    auto scene = BackpackScene::createScene(); // ���ݱ���ʵ��
    Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));
}

