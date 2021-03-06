#include "rinput.hpp"
#include "window.hpp"
#include "transition.hpp"
#include <sstream>
#include "main.hpp"
#include "application.hpp"

QMap<QString, RInput::action> RInput::actions;

RInput::RInput(QString name) :
		Ressource::Ressource(name)
{
	initActions();
        speedX = 0;
        speedY = 0;
        first  = true;
        _canFire = 0;
        isCharging = false;
}

bool RInput::do_catch(QMap<QString, QString>& properties, const sf::Input& input)
{
    RNetwork &network = RNetwork::instance();
    InputManager &im = InputManager::instance();
    float sX, sY;
    sX = speedX;
    sY = speedY;

    if (input.IsKeyDown(im.getKey(InputManager::MOVE_DOWN)))
        speedY = 1;
    if (input.IsKeyDown(im.getKey(InputManager::MOVE_UP)))
        speedY = -1;
    if (((input.IsKeyDown(im.getKey(InputManager::MOVE_UP))) &&
         (input.IsKeyDown(im.getKey(InputManager::MOVE_DOWN)))) ||
        (!(input.IsKeyDown(im.getKey(InputManager::MOVE_UP))) &&
         !(input.IsKeyDown(im.getKey(InputManager::MOVE_DOWN)))))
        speedY = 0;

    if (input.IsKeyDown(im.getKey(InputManager::MOVE_LEFT)))
        speedX = -1;
    if (input.IsKeyDown(im.getKey(InputManager::MOVE_RIGHT)))
        speedX = 1;
    if (((input.IsKeyDown(im.getKey(InputManager::MOVE_RIGHT))) &&
         (input.IsKeyDown(im.getKey(InputManager::MOVE_LEFT)))) ||
        (!(input.IsKeyDown(im.getKey(InputManager::MOVE_RIGHT))) &&
         !(input.IsKeyDown(im.getKey(InputManager::MOVE_LEFT)))))
        speedX = 0;
    if (first)
        network.send("W");
    first  = false;
    std::string packet = "\x51";
    packet = Formater::pack(packet, speedX);
    packet = Formater::pack(packet, speedY);
    if (speedX != sX || speedY != sY)
        network.send(packet);

    float currentTime = Application::instance().getTheTime();

    if (input.IsKeyDown(im.getKey(InputManager::USE_WEAPON_1)) && currentTime - _canFire > 0.2)
    {
        _canFire = currentTime;
        network.send("\x10");
    }
    else if (!input.IsKeyDown(im.getKey(InputManager::USE_WEAPON_1)))
        _canFire = 0;

    if (input.IsKeyDown(im.getKey(InputManager::USE_WEAPON_2)) && !isCharging)
    {
        isCharging = true;
        network.send("\x04");
    }

    if (!input.IsKeyDown(im.getKey(InputManager::USE_WEAPON_2)) && isCharging)
    {
        isCharging = false;
        network.send("\x10");
    }
    return true;
}


void RInput::initActions()
{
        if (!actions.count("catch"))
                actions["catch"] = &RInput::do_catch;
}
