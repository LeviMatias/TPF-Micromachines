

#ifndef MICROMACHINES_GAMEOBJECT_H
#define MICROMACHINES_GAMEOBJECT_H

#include "Box2D/Box2D.h"

//probably make this one a virtual class
class GameObject {
    bool enabled;
    int y_size;
    int x_size;
    //std::string stats;

protected:
    b2Body* body;
public:

    //only supports making objects with a square bounding box
    GameObject(b2Body* &newbody);

    virtual void resolveCollision(b2Fixture &collidedWith);

    b2Vec2 getPosition();

    b2Vec2 getSize();

    float32 getAngle();

    void attachFixture(b2FixtureDef &boxFixtureDef);
};


#endif //MICROMACHINES_GAMEOBJECT_H