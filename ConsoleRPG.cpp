#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <vector>

namespace GameSettings
{
    constexpr int width{ 10 };
    constexpr int height{ 10 };
}

struct Point
{
    int x{};
    int y{};
};

class Entity
{
private:
    static inline std::vector<Entity*> entities{};

    Point mPosition{};
    char mSymbol{};

public:
    Entity(const Point& position, char symbol)
        : mPosition{ position }, mSymbol{ symbol }
    {
        entities.push_back(this);
    }

    ~Entity()
    {
        std::erase(entities, this); // recommendation from ChatGPT
    }

    const Point& getPosition() const { return mPosition; }
    char getSymbol() const { return mSymbol; }

    static const std::vector<Entity*>& getEntities() { return entities; }
};

class Map
{
private:
    std::array<std::array<Entity*, GameSettings::width>, GameSettings::height> mEntityMap{};

    static bool isPointInBounds(const Point& point)
    {
        return (point.x >= 0 && point.x < GameSettings::width) &&
               (point.y >= 0 && point.y < GameSettings::height);
    }

    Entity*& getEntityAtPosition(const Point& point)
    {
        return mEntityMap[static_cast<std::size_t>(point.y)][static_cast<std::size_t>(point.x)];
    }

    void updateEntityMap()
    {
        for (auto& row : mEntityMap)
            for (Entity*& entity : row)
                entity = nullptr;

        for (Entity* entity : Entity::getEntities())
        {
            if (isPointInBounds(entity->getPosition()))
                getEntityAtPosition(entity->getPosition()) = entity;
        }
    }

public:
    Map()
    {
        updateEntityMap();
    }

    void display()
    {
        updateEntityMap();
        std::cout << *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const Map& map);
};

std::ostream& operator<<(std::ostream& out, const Map& map)
{
    for (const auto& row : map.mEntityMap)
    {
        for (Entity* entity : row)
        {
            if (entity)
                out << entity->getSymbol() << " ";
            else
                out << ". ";
        }

        out << '\n';
    }

    return out;
}

int main()
{
    Map map{};

    Entity entity{ Point{ 5, 5 }, 'E' };

    {
        Entity entity2{ Point{ 1, 1 }, 'E' };
    }

    map.display();

    return 0;
}
