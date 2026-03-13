#include "Map/Map.hpp"
#include "Util/Renderer.hpp"

Map::Map() 
    : m_Wall(RESOURCE_DIR "/Image/Road/wall_b.bmp"),   // 對應你的實際路徑
      m_Floor(RESOURCE_DIR "/Image/Road/road.bmp") // 對應你的實際路徑
{}

void Map::Draw() {
    static Util::Renderer renderer;

    for (int i = 0; i < 11; ++i) {     // 列 (Y軸)
        for (int j = 0; j < 11; ++j) { // 行 (X軸)
            
            // 計算每個格子在螢幕上的座標
            float posX = j * m_TileSize;
            float posY = i * m_TileSize;

            if (m_MapData[i][j] == 1) {
                m_Wall.SetPosition({posX, posY});
                m_Wall.Draw();
            } else {
                m_Floor.SetPosition({posX, posY});
                m_Floor.Draw();
            }
        }
    }
    renderer.Update();
}