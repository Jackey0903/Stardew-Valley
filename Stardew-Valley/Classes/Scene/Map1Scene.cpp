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
    loadMap("Map/Map1/map1.tmx", 1.5f);
    initPlayer();

    return true;
}

void Map1Scene::onEnter()
{
    BaseMapScene::onEnter();
    // ���ض��߼�
}
