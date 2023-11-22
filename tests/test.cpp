#include "gmock/gmock.h"
#include <brigadier.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <limits>
#include <rapidcheck/Random.h>
#include <rapidcheck/gen/Arbitrary.h>
#include <rapidcheck/gtest.h>
#include <string>
#include <vector>

class Player {
public:
    Player() = delete;
    Player(int id):
        _id(id)
    {
    }
    MOCK_METHOD(void, addScore, (std::string scoreboard, int score), ());
    MOCK_METHOD(void, removeScore, (std::string scoreboard, int score), ());
    bool isOp() { return true; }

    int _id;
};

class World {
public:
    static World *getInstance()
    {
        static World world;
        return &world;
    }

    std::vector<std::shared_ptr<Player>> getEntities() { return _entities; }
    void add(std::shared_ptr<Player> player) { _entities.push_back(player); }

private:
    std::vector<std::shared_ptr<Player>> _entities;
};

class EntitySelector {
public:
    EntitySelector(int id):
        _id(id)
    {
    }

    std::vector<std::shared_ptr<Player>> getEntities() const { return World::getInstance()->getEntities(); }

    int _id;
};

std::ostream &operator<<(std::ostream &os, const EntitySelector &selector)
{
    os << "EntitySelector(" << selector._id << ")";
    return os;
}

struct EntitySelectorParser : public brigadier::Parser {
    using type = EntitySelector;

    static EntitySelector parse(brigadier::Reader &reader)
    {
        auto id = reader.readInt();
        return EntitySelector(id);
    }
};

TEST(UseTest, useTest)
{
    using brigadier::CommandNodeBuilder;

    brigadier::Registry dispatcher;
    World *world = World::getInstance();
    world->add(std::make_shared<Player>(0));
    world->add(std::make_shared<Player>(1));
    auto players = world->getEntities();

    // clang-format off
    dispatcher.add(CommandNodeBuilder("scoreboard")
        .withPermission([](brigadier::TypeHolder source) {
            if (source.is<Player>()) {
                if (source.getAs<Player>()->isOp())
                    return true;
                return false;
            }
            return true;
        })
        .add(CommandNodeBuilder("player")
            .add(CommandNodeBuilder("add")
                .expectArg<EntitySelectorParser>("player")
                .expectArg<brigadier::StringParser>("scoreboard")
                .expectArg<brigadier::NumberParser<int>>("score")
                .execute([](brigadier::TypeHolder source, EntitySelector entities, std::string scoreboard, int score) {

                    for (auto &entity : entities.getEntities()) {
                        std::cout << "calling addScore" << std::endl;
                        entity->addScore(scoreboard, score);
                    }
                })
            )
            .add(CommandNodeBuilder("remove")
                .expectArg<EntitySelectorParser>("player")
                .expectArg<brigadier::StringParser>("scoreboard")
                .expectArg<brigadier::NumberParser<int>>("score")
                .execute([](brigadier::TypeHolder source, EntitySelector entities, std::string scoreboard, int score) {
                    for (auto &entity : entities.getEntities()) {
                        std::cout << "calling removeScore" << std::endl;
                        entity->removeScore(scoreboard, score);
                    }
                })
            )
        )
    );
    // clang-format on

    auto add1 = brigadier::StringReader("scoreboard player add 1 test 1");
    auto remove3 = brigadier::StringReader("scoreboard player remove 2 test 3");

    dispatcher.parse(add1);
    dispatcher.parse(remove3);

    EXPECT_CALL(*players[0], addScore("test", 1)).Times(testing::AtLeast(1));
    EXPECT_CALL(*players[1], removeScore("test", 3)).Times(testing::AtLeast(1));
}
