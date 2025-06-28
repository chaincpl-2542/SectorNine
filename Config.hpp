#pragma once
#include <string>
#include <fstream>
#include <json.hpp>

struct Config
{
    int  mapWidth = 80;
    int  mapHeight = 25;
    int  doorChance = 30;
    int  playerHP = 12;
    int  playerSight = 5;
    int  enemyAmount = 1;

    bool loadFromFile(const std::string& path = "settings.json")
    {
        std::ifstream f(path);
        if (!f) return false;

        nlohmann::json j;
        f >> j;

        if (j.contains("map")) {
            mapWidth = j["map"].value("width", mapWidth);
            mapHeight = j["map"].value("height", mapHeight);
        }

        if (j.contains("door")) {
            doorChance = j["door"].value("chance", doorChance);
        }

        if (j.contains("player")) {
            playerHP = j["player"].value("hp", playerHP);
            playerSight = j["player"].value("sight", playerSight);
        }

        if (j.contains("enemy")) {
            enemyAmount = j["enemy"].value("amount", enemyAmount);
        }

        return true;
    }
};
