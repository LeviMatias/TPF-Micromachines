

#ifndef MICROMACHINES_RACECAR_H
#define MICROMACHINES_RACECAR_H

#include "GameObject.h"
#include "../../common/InfoBlock.h"

struct key_directions {

};

class RaceCar : public GameObject {
    b2Vec2 accel;
    int health;
    int id;
    InfoBlock stats;

public:
    RaceCar(int carId, InfoBlock stats, b2Body* &newBody);

    //PRE x and y must be between [0-1]
    //POS accelerates the car in the specified direction
    b2Vec2 accelerate(b2Vec2 direction);

    //POS advances the car simulation one timestep
    void step();

    //POS the car takes damage and returns true if life went below 0
    bool takeDamage(int dmg);

    bool isDead();

    //PRE info block must contain the user key inputs to convert into accel direction
    //under the names key1, key2
    //POS interprets the keys and accelerate() in the given direction
    void drive(InfoBlock keys);
};


#endif //MICROMACHINES_RACECAR_H