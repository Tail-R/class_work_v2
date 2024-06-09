#include "unique_id_resolver.hpp"

UniqueIDResolver::UniqueIDResolver() {
    occupied_id_set_ = std::make_unique<std::vector<unsigned int>>();
}

unsigned int UniqueIDResolver::generate_available_id() {
    unsigned int new_id = previously_generated_id++;

    occupied_id_set_->push_back(new_id);

    return new_id;
}

void UniqueIDResolver::discard_id(unsigned int outdated_id) {
    std::erase_if(*occupied_id_set_, [=](unsigned int id) {
        return id == outdated_id;
    });
}