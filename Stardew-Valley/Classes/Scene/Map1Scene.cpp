// Map1Scene.cpp
#include "Map1Scene.h"

USING_NS_CC;

Scene* Map1Scene::createScene()
{
    return Map1Scene::create();
}

bool Map1Scene::init()
{
    if (!BaseMapScene::init())
        return false;

    // ���õ�ͼ�ļ�������
    loadMap("Map/Map1/map1.tmx", 5.0f);
    initPlayer();

    return true;
}

void Map1Scene::onEnter()
{
    BaseMapScene::onEnter();

    // ��Map3�ϲ���npc����㲢����NPC
    if (_tiledMap)
    {
        // ��ȡ��Ϊ"Npc"�Ķ����
        auto npcLayer = _tiledMap->getObjectGroup("Npc");
        if (npcLayer)
        {
            auto npcObjects = npcLayer->getObjects();
            for (auto& obj : npcObjects)
            {
                auto npcObject = obj.asValueMap();
                if (npcObject["name"].asString() == "Npc")
                {
                    // ��ȡNPC�ڵ�ͼ����ϵ�е�λ�ã�Tiled����Ϊ(0,0)��
                    float npcX = npcObject["x"].asFloat();
                    float npcY = npcObject["y"].asFloat();
                    Vec2 npcPosition(npcX, npcY);

                    _npc = Sprite::create("Bouncer.png");
                    _npc->setScale(1.0f); // ����NPC�����ţ�ʹ����������
                    _npc->setPosition(npcPosition);
                    _tiledMap->addChild(_npc, 15);


                    // ���õ���¼�������
                    auto npcListener = EventListenerTouchOneByOne::create();
                    npcListener->setSwallowTouches(true);
                    npcListener->onTouchBegan = [this](Touch* touch, Event* event) {
                        // ����Ļ����ת��Ϊ_tiledMap����
                        auto locationInView = touch->getLocation();
                        auto locationInMap = _tiledMap->convertToNodeSpace(locationInView);
                        CCLOG("Touch at screen: (%.2f,%.2f)", locationInView.x, locationInView.y);
                        CCLOG("Touch in map coords: (%.2f,%.2f)", locationInMap.x, locationInMap.y);

                        Rect boundingBox = _npc->getBoundingBox();
                        CCLOG("NPC boundingBox: origin(%.2f,%.2f), size(%.2f,%.2f)",
                            boundingBox.origin.x, boundingBox.origin.y,
                            boundingBox.size.width, boundingBox.size.height);
                        // boundingBox��locationInMap����_tiledMap����ϵ��
                        if (boundingBox.containsPoint(locationInMap))
                        {
                            showDialogue();
                            return true;
                        }
                        return false;
                        };
                    _eventDispatcher->addEventListenerWithSceneGraphPriority(npcListener, _npc);
                }
            }
        }
    }
}

void Map1Scene::showDialogue()
{
    // �Ի�����
    static std::vector<std::string> npcDialogues = {
        "Hey��man��",
        "Wish u can do it",
        "Need Help��",
        "Ok��bye��"
    };
    static int currentIndex = 0; // ��ǰ�Ի�����

    // ����Ի����Ѿ����ڲ�����ʾ�У��������Լ�����һ��
    // Ϊ�ˣ����Խ��Ի�����ΪMap3Scene�ĳ�Ա�������ú�������ɷ���
    // ����ֻ�ǵ�һ�δ���������Map3Scene.h��������
    // LayerColor* _dialogueLayer;
    // Label* _dialogueLabel;
    // Sprite* _npcFace;
    // int _dialogueIndex;
    // npcDialoguesΪstatic����Ҳ�У�����Ϊ��Ա������

    // ����Ի��򲻴��ڣ������Ի���UI
    if (!_dialogueLayer)
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();

        // ���������ߵĶԻ��򱳾����°�����
        _dialogueLayer = LayerColor::create(Color4B(0, 0, 0, 180),
            visibleSize.width,
            visibleSize.height / 2);
        _dialogueLayer->setPosition(origin.x, origin.y);
        this->addChild(_dialogueLayer, 20);

        // �ڶԻ�������NPCͷ��
        _npcFace = Sprite::create("npc_face.png"); // �滻Ϊ����NPCͷ��ͼƬ
        _npcFace->setScale(2.0f); // �Ŵ�ͷ��
        _npcFace->setAnchorPoint(Vec2(0, 1));
        _npcFace->setPosition(Vec2(10, visibleSize.height / 2 - 10));
        _dialogueLayer->addChild(_npcFace, 1);

        // �ڶԻ����Ҳ�ŶԻ��ı�Label
        _dialogueLabel = cocos2d::ui::Text::create("", "Arial", 24);
        _dialogueLabel->setAnchorPoint(Vec2(0, 1));
        _dialogueLabel->setPosition(Vec2(_npcFace->getContentSize().width * 2 + 30,
            visibleSize.height / 2 - 10));
        _dialogueLabel->setContentSize(Size(
            visibleSize.width - (_npcFace->getContentSize().width * 2 + 50),
            visibleSize.height / 2 - 20));
        // ��Label���ö�����ʾ��dimensionsʹ���Զ�����
        _dialogueLabel->setColor(Color3B::WHITE);
        _dialogueLayer->addChild(_dialogueLabel, 1);

        // ��ʼ���Ի�����
        _dialogueIndex = 0;
        _dialogueLabel->setString(npcDialogues[_dialogueIndex]);

        // Ϊ�Ի�����ӵ���¼��������Ի�������ɵ㣩
        auto dialogListener = EventListenerTouchOneByOne::create();
        dialogListener->setSwallowTouches(true);
        dialogListener->onTouchBegan = [this](Touch* touch, Event* event) {
            auto target = event->getCurrentTarget();
            Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());

            // ��ȡ�Ի����С���жϵ���Ƿ����䷶Χ��
            Size size = _dialogueLayer->getContentSize();
            Rect rect(0, 0, size.width, size.height);
            if (rect.containsPoint(locationInNode)) {
                return true; // �ڶԻ��������ڣ����Ĵ���
            }
            return false;
            };

        dialogListener->onTouchEnded = [this](Touch* touch, Event* event) {
            // ����Ի������ʾ��һ��Ի�
            _dialogueIndex++;
            if (_dialogueIndex < (int)npcDialogues.size()) {
                _dialogueLabel->setString(npcDialogues[_dialogueIndex]);
            }
            else {
                // �Ի��������Ƴ��Ի���
                _dialogueLayer->removeFromParent();
                _dialogueLayer = nullptr;
                _npcFace = nullptr;
                _dialogueLabel = nullptr;
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(dialogListener, _dialogueLayer);
    }
    else
    {
        // ����Ի����Ѿ����ڣ��������߼��²���Ҫ���⴦��
        // ��Ϊ����߼���onTouchEnded����
    }
}