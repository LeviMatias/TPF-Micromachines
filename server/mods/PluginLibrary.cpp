#include "PluginLibrary.h"
#include <string>

PluginLibrary::PluginLibrary(std::string& path) {
    // Loadgin all plugins .so in plugins foldier
    if (auto dir = opendir(path.c_str())) {
        while (auto f = readdir(dir)) {
            if (f->d_name[0] == '.') continue;
            std::string fileName = f->d_name;
            if (fileName.find(".so") != std::string::npos){
                fileName =  path + "/" + fileName;
                auto plugin = new PluginLoader(fileName.c_str());
                this->plugins.push_back(plugin);
            }
        }
        closedir(dir);
    }
}

void PluginLibrary::runPlugins(){
    std::vector<int> cars = {12,23,3,2};
    std::cout << "run plugins\n";
    for (size_t i=0; i< plugins.size(); ++i){
        plugins[i]->plugin->modifyCars(cars);
        plugins[i]->plugin->modifyWorld(cars);
    }
}

PluginLibrary::~PluginLibrary() {
    for (auto it = plugins.begin(); it != plugins.end(); ++it){
        delete(*it);
    }
}