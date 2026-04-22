#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include "entity.hpp"
#include "player.hpp"

typedef enum{
    AID,
    OBSTACLE
}Type;

typedef enum{
    HEALTH,
    SHIELD,
    SMALL,
    BIG
}EnemyType;

class Enemy : public Entity{
    private:
        textureAnimation animation[4];
        EnemyType type;
        double current_time, attack_time, cooldown;
        bool active;
    public:
        Enemy();
        void update();
        void animate();
        Rectangle get_collision_rec();
        void attack(Player &player);
        void reset(Type reset_type);
        EnemyType get_type() { return type; }
        bool is_inactive();
        void activate();
        ~Enemy();
};

#endif