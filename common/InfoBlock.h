

#ifndef MICROMACHINES_INFOBLOCK_H
#define MICROMACHINES_INFOBLOCK_H

#include "yaml-cpp/yaml.h"

class InfoBlock {
public:
    YAML::Node src_yaml;

    explicit InfoBlock(const std::string &init, bool fromFile);

    std::string getString(std::string key);

    int getInt(std::string key);

    float getFloat(std::string key);

    std::string srcString();

    bool exists(std::string key);

    InfoBlock getNestedInfo(std::string key);

    template <class T>
    std::list<T> getList(std::string key){
        //Implement me
        auto size = this->getInt("size");
        auto values = this->getNestedInfo("values");

        std::list<T> list;
        for (int i=0; i<size; i++){
            list.push_back(values.get<T>(std::to_string(i)));
        }
        
        return list;
    }

    template <class T>
    T get(std::string key){
        return this->src_yaml[key].as<T>();
    }
};


#endif //MICROMACHINES_INFOBLOCK_H
