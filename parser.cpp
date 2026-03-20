#include "parser.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

DirectionVector string_to_dir(str::string s){
    if(s == "UP")return DirectionVector::UP;
    if(s == "DOWN")return DirectionVector::DOWN;
    if(s == "LEFT")return DirectionVector::LEFT;
    if(s == "RIGHT")return DirectionVector::RIGHT;
    return DirectionVector::ZERO;

}

void load_config_from_json(const std::string& filename){
    std::ifstream file(filename);
    if(!file.is_open)return;
    json data;
    file>>data;

    for (auto& [name, val] : data.items()){
        TileType type = string_tile.at(name);

        TileProperties props;
        props.updateFrequency = val["updateFrequency"];
        props.chemProperties.reactivity = val["reactivity"];

        props.moveProperties.isPositionStable = val["physics"]["stable"];
        props.moveProperties.density = val["physics"]["density"];

        for(auto& l : val["physics"]["layers"]){
            MoveLogicProbabilityLayer layer;
            for(std::string d : l["choices"]) layer.choices.push_back(string_to_dir(d));
            for(float p : l["probs"]) layer.probs.push_back(p);
            props.moveProperties.logicLayers.push_back(layer);
        }
        tile_properties[type] = props;
    }
}