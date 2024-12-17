// Map3Scene.cpp
#include "Map3Scene.h"
#include "MapLayer.h"
#include "Object/Animal.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* Map3Scene::createScene()
{
    return Map3Scene::create();
}

bool Map3Scene::init()
{
    if (!BaseMapScene::init())
        return false;

    // ����Map3������BaseMapScene��loadMap�Ὣ_tiledMap��anchorPoint��Ϊ(0,0)��positionΪ(0,0)�������ŵ�ͼ
    // ��������5��
    loadMap("Map/Map3/map3.tmx", 5.0f);
    initPlayer();


	cocos2d::Rect animalArea(50, 0, 100, 100);

    auto cat = Animal::create(animalArea, "Cat");
    _tiledMap->addChild(cat, 20);
    cat->setScale(0.5f);

    _npc = nullptr;

    return true;
}

void Map3Scene::onEnter()
{
    BaseMapScene::onEnter();

    // ��Map3�ϲ���npc����㲢����NPC
    if (_tiledMap)
    {
        // ��ȡ��Ϊ"Npc"�Ķ����
        auto npcLayer = _tiledMap->getObjectGroup("Npc_Leah");
        if (npcLayer)
        {
            auto npcObjects = npcLayer->getObjects();
            for (auto& obj : npcObjects)
            {
                auto npcObject = obj.asValueMap();
                if (npcObject["name"].asString() == "Npc_Leah")
                {
                    // ��ȡNPC�ڵ�ͼ����ϵ�е�λ�ã�Tiled����Ϊ(0,0)��
                    float npcX = npcObject["x"].asFloat();
                    float npcY = npcObject["y"].asFloat();
                    Vec2 npcPosition(npcX, npcY);
                    CCLOG("ok");
                    _npc = Sprite::create("Leah.png");
                    _npc->setScale(0.1f); // ����NPC�����ţ�ʹ����������
                    _npc->setPosition(npcPosition);
                    _tiledMap->addChild(_npc, 25);


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

void Map3Scene::showDialogue()
{
    struct DialogueLine {
        std::string text;
        std::string faceImage;
    };

    std::vector<DialogueLine> npcDialogues = {
        {"Hello, Jackey", "Leah_1.png"},
        {"You look lonely", "Leah_2.png"},
        {"Here is the thing, there is something I have always wanted to say to you", "Leah_3.png"},
        {"Would you like to be my lover?", "Leah_4.png"}
    };

    if (!_dialogueLayer)
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();

        _dialogueLayer = LayerColor::create(Color4B(0, 0, 0, 180),
            visibleSize.width,
            visibleSize.height / 2);
        _dialogueLayer->setPosition(origin.x, origin.y);
        this->addChild(_dialogueLayer, 20);

        _npcFace = Sprite::create(npcDialogues[_dialogueIndex].faceImage);
        _npcFace->setScale(0.5f);
        _npcFace->setAnchorPoint(Vec2(0, 1));
        _npcFace->setPosition(Vec2(10, visibleSize.height / 2 - 10));
        _dialogueLayer->addChild(_npcFace, 1);

        _dialogueLabel = ui::Text::create("", "Arial", 24);
        _dialogueLabel->setAnchorPoint(Vec2(0, 1));
        _dialogueLabel->setPosition(Vec2(400, visibleSize.height / 2 - 160)); // �޸�Ϊ�ұ�λ��
        _dialogueLabel->setContentSize(Size(
            visibleSize.width + 200, // �ʵ��������
            visibleSize.height / 2 + 100)); // �ʵ������߶�
        _dialogueLabel->setColor(Color3B::RED);
        _dialogueLayer->addChild(_dialogueLabel, 1);

        _dialogueIndex = 0;
        _dialogueLabel->setString(npcDialogues[_dialogueIndex].text);

        auto dialogListener = EventListenerTouchOneByOne::create();
        dialogListener->setSwallowTouches(true);
        dialogListener->onTouchBegan = [this](Touch* touch, Event* event) {
            auto target = event->getCurrentTarget();
            Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());

            Size size = _dialogueLayer->getContentSize();
            Rect rect(0, 0, size.width, size.height);
            if (rect.containsPoint(locationInNode)) {
                return true;
            }
            return false;
            };

        dialogListener->onTouchEnded = [this, npcDialogues](Touch* touch, Event* event) mutable {
            _dialogueIndex++;
            if (_dialogueIndex < (int)npcDialogues.size()) {
                // ��һ��Ի�
                _dialogueLabel->setString(npcDialogues[_dialogueIndex].text);
                _npcFace->setTexture(npcDialogues[_dialogueIndex].faceImage);
            }
            else {
                // �Ի���������ʾѡ��
                showOptions();
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(dialogListener, _dialogueLayer);
    }
}

void Map3Scene::showOptions()
{
    _dialogueLabel->setString(""); // ����ı�
    _npcFace->setTexture("Leah_4.png"); // ��ͷ����ΪĬ�ϻ򱣳����ͷ��

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto option1Label = Label::createWithSystemFont("God, I can do it.", "Arial", 24);
    auto option2Label = Label::createWithSystemFont("I love you.", "Arial", 24);
    auto option3Label = Label::createWithSystemFont("Sorry,but I have something more important.", "Arial", 24);

    auto option1 = MenuItemLabel::create(option1Label, [this](Ref* sender) {
        CCLOG("ѡ���� '�õ�'");
        // ����ѡ��ִ�ж�Ӧ�߼�...
        closeDialogue();
        });

    auto option2 = MenuItemLabel::create(option2Label, [this](Ref* sender) {
        CCLOG("ѡ���� '����Ҫ'");
        // ����ѡ��ִ�ж�Ӧ�߼�...
        closeDialogue();
        });

    auto option3 = MenuItemLabel::create(option3Label, [this](Ref* sender) {
        CCLOG("ѡ���� '�ټ�'");
        // ����ѡ��ִ�ж�Ӧ�߼�...
        closeDialogue();
        });

    auto menu = Menu::create(option1, option2, option3, nullptr);
    menu->alignItemsVerticallyWithPadding(20);
    menu->setPosition(visibleSize.width * 0.7f, visibleSize.height / 4);
    _dialogueLayer->addChild(menu, 2);
}

void Map3Scene::closeDialogue()
{
    if (_dialogueLayer)
    {
        _dialogueLayer->removeFromParent();
        _dialogueLayer = nullptr;
        _npcFace = nullptr;
        _dialogueLabel = nullptr;
        _dialogueIndex = 0;
    }
}