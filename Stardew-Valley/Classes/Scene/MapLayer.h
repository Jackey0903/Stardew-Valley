/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : MapLayer.h
 * �ļ�����      : ��ͼ����ͷ�ļ�
 * ����          : 
 * ��������      : 2024/12/07
 * ���֤        : MIT License
 ****************************************************************/
#ifndef __MAP_LAYER_H__
#define __MAP_LAYER_H__

#include "cocos2d.h"

class MapLayer : public cocos2d::Layer
{
public:
    virtual bool init();

    // ���ص�ͼ
    void loadMap(const std::string& mapFile);

    // ��ȡǽ�����
    std::vector<cocos2d::Rect> getWallRects() const { return _wallRects; }

    // ��ȡ��ǰ��ͼ
    cocos2d::TMXTiledMap* getTMXTiledMap() const { return currentMap; }

    CREATE_FUNC(MapLayer);

private:
    cocos2d::TMXTiledMap* currentMap;
    std::string currentMapName;
    std::vector<cocos2d::Rect> _wallRects;
};

#endif // __MAP_LAYER_H__
