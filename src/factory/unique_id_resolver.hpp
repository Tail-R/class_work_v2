#pragma once

#include "../shared.hpp"

class UniqueIDResolver {
public:
    UniqueIDResolver();

    unsigned int generate_available_id();
    void discard_id(unsigned int id);

private:
    unsigned int previously_generated_id{1};
    std::unique_ptr<std::vector<unsigned int>> occupied_id_set_;
};