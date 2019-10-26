
#ifndef MICROMACHINES_GAMEWORLD_H
#define MICROMACHINES_GAMEWORLD_H

#include "Box2D/Box2D.h"
#include <vector>
#include "GameObject.h"
#include "RaceCar.h"
#include "CollisionDetector.h"


class GameWorld {
    b2World world;
    //CollisionDetector cd;
    std::vector<RaceCar> cars;

    //objects which are a static part of the map background
    std::vector<GameObject> background_objs;

    //Dynamic Instances are those which come and go from the field, (ie items)
    //std::vector<GameObject> dynamic_objs;

public:

    //POS creates a world with no gravity for top-down action
    GameWorld();


      int createCar(std::string &carStats);

      RaceCar& getCar(int id);
    /*
      &GameObject createDynamicObject(yaml &stats?);

     */

    //POS advances the simulation and all instances within one timestep
    void Step();

    void createBackgroundObject(); //yaml &stats?
};


#endif //MICROMACHINES_GAMEWORLD_H
