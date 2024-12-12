/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : MapLayer.cpp
 * �ļ�����      : ��ͼ����ʵ��
 * ����          : 
 * ��������      : 2024/12/07
 * ���֤        : MIT License
 ****************************************************************/
#include "MapLayer.h"
#include "cocos2d.h"

USING_NS_CC;

bool MapLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    currentMap = nullptr;
    currentMapName = "";
    _wallRects.clear();

    return true;
}

void MapLayer::loadMap(const std::string& mapFile)
{
    CCLOG("���Լ��ص�ͼ: %s", mapFile.c_str());

    // �Ƴ���ǰ��ͼ
    if (currentMap)
    {
        this->removeChild(currentMap, true);
        currentMap = nullptr;
        CCLOG("�Ƴ��ɵ�ͼ");
    }

    // �����µ�ͼ
    currentMap = TMXTiledMap::create(mapFile);

    if (currentMap)
    {
        this->addChild(currentMap, 0);

        currentMapName = mapFile;
        CCLOG("��ͼ���سɹ�: %s", mapFile.c_str());

        // �ռ�ǽ�����
        auto wallLayer = currentMap->getLayer("wal"); // ȷ�� "Paths" ��ǽ�����ڵ�ͼ������
        if (wallLayer)
        {
            _wallRects.clear();
            for (int x = 0; x < wallLayer->getLayerSize().width; ++x)
            {
                for (int y = 0; y < wallLayer->getLayerSize().height; ++y)
                {
                    auto tileGID = wallLayer->getTileGIDAt(Vec2(x, y));
                    if (tileGID != 0) // ���� GID Ϊ 0 ��ʾ��Ч��Ƭ
                    {
                        Vec2 tilePos = wallLayer->getPositionAt(Vec2(x, y));
                        Size tileSize = currentMap->getTileSize();
                        Rect tileRect = Rect(tilePos.x, tilePos.y, tileSize.width, tileSize.height);
                        _wallRects.push_back(tileRect);
                    }
                }
            }
            CCLOG("�ռ��� %zu ��ǽ����Ρ�", _wallRects.size());
        }
        else
        {
            CCLOG("δ�ҵ�ǽ��ͼ�㡣");
        }
    }
    else
    {
        CCLOG("��ͼ����ʧ��: %s", mapFile.c_str());
    }
}
