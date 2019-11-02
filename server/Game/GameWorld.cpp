
#include <iostream>
#include "GameWorld.h"
#include "../../config/constants.h"

//local use namespace for local functions
namespace {
    b2Body* makeNewBody(b2World &world, b2BodyType bodyType, float32 x, float32 y){
        b2BodyDef myBodyDef;
        myBodyDef.type = bodyType;
        myBodyDef.angle = 0;
        myBodyDef.position.Set(x, y);
        return world.CreateBody(&myBodyDef);
    }

    void createAndAddFixture(Entity* obj, int hx, int hy, int density,
                             uint16 catBits, uint16 maskBits, bool isSensor){

        b2PolygonShape boxShape;
        boxShape.SetAsBox(hx, hy);
        b2FixtureDef boxFixtureDef;
        boxFixtureDef.shape = &boxShape;
        boxFixtureDef.density = density;
        boxFixtureDef.filter.categoryBits = catBits;
        boxFixtureDef.filter.maskBits = maskBits;
        boxFixtureDef.isSensor = isSensor;
        obj->attachFixture(boxFixtureDef);
    }
}

GameWorld::GameWorld() : world(b2Vec2(0,0)) {
    //CollisionListener cl;
    //world.SetContactListener(&cl);
}

void GameWorld::loadWorld(std::string worldName){
    // TODO load box2D world with the mapNumber given
    std::string raceYaml = "maps/" + worldName + ".yaml";
    return;
}

InfoBlock GameWorld::status(){
    // TODO: create a real infoblock with the new world
    InfoBlock ib;
    ib[PLAYERS_AMOUNT] = this->cars.size();
//    ib[PLAYERS_AMOUNT] = this->cars.size();
//    for (auto & car : cars){
//        std::string car_id = std::to_string(car.id);
//        //ib["P" + car_id] = car.stateAsInfoBlock();
//    }
//    ib[OBJECTS_AMOUNT] = 0; // here goes something like this->objects.size();
//    int cont = 0;
//    for (auto & obj : objects){
//        std::string obj_id = std::to_string(cont);
//        ib["O" + obj_id] = obj.stateAsInfoBlock();
//        cont++;
//    }
    return ib;
}

void GameWorld::processEvent(InfoBlock event){
    // TODO : process event in physic world
    std::cout << event.srcString() << std::endl;
    return;
}

void GameWorld::Step() {
    float32 timeStep = 1/60.0;//the length of time passed to simulate (seconds)
    for (auto & car : cars){
        car.step(timeStep);
    }

    int32 velocityIterations = 8;//how strongly to correct velocity
    int32 positionIterations = 3;//how strongly to correct position
    world.Step(timeStep, velocityIterations, positionIterations);
}


void GameWorld::createBackgroundObject() {
    b2Body* newBody = makeNewBody(world, b2_staticBody,0,0);
    this->background_objs.emplace_back(newBody);

    createAndAddFixture(&(this->background_objs.back()), 1, 1, 0, TILE, SENSOR, false);
}

int GameWorld::createCar(InfoBlock carStats) {
    b2Body* newBody = makeNewBody(world, b2_dynamicBody,0,0);
    int carId = cars.size();
    cars.emplace_back(carId, carStats, newBody);

    createAndAddFixture(&(cars.back()),2,1,1,PLAYER, PLAYER, false);
    createAndAddFixture(&(cars.back()),2/2,1/2,0,SENSOR, TILE, true);

    return carId;
}

RaceCar &GameWorld::getCar(int id) {
    return cars.at(id);
}