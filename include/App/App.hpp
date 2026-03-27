#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Map/Map.hpp"
#include "Character/Player.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    [[nodiscard]] State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End();

private:
    Map m_GameMap;
    std::shared_ptr<Player> m_Player;
    std::shared_ptr<BackgroundImage> m_MainBackground;

    State m_CurrentState = State::START;

private:

};

#endif
