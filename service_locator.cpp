#include "service_locator.hpp"
#include <optional>
#include <boost/functional/hash.hpp>

// todo: contract-based framework
// todo: logger
// todo: tests

class service_locator::impl {
public:
    void clear() {
        registry_.clear();
    }

    std::error_code register_instance(boost::uuids::uuid uuid, std::shared_ptr<void> instance) {
        auto instance_info_iter = registry_.find(uuid);

        if(instance_info_iter == registry_.end()) {
            registry_.emplace_hint(instance_info_iter, uuid,
                        entity_registration_info{std::move(instance)});
            return std::make_error_code(std::errc{0});
        }

        if (instance_info_iter->second.instance.has_value()) {
            return make_error_code(error_code::sl_error_codes::entity_already_registered);
        }

        instance_info_iter->second.instance = std::move(instance);

        return std::make_error_code(std::errc{0});
    }

    std::error_code register_factory(const boost::uuids::uuid& uuid, std::shared_ptr<sl::i_factory> factory) {
        auto instance_info_iter = registry_.find(uuid);

        if(instance_info_iter == registry_.end()) {
            instance_info_iter = registry_.emplace_hint(instance_info_iter, uuid,
                                                        entity_registration_info{std::nullopt, std::move(factory)});
            return std::make_error_code(std::errc{0});
        }

        if (instance_info_iter->second.factory.has_value()) {
            return make_error_code(error_code::sl_error_codes::entity_already_registered);
        }

        instance_info_iter->second.factory = std::move(factory);

        return std::make_error_code(std::errc{0});
    }

    std::error_code resolve(const boost::uuids::uuid& uuid, std::shared_ptr<void>& result) {
        auto entity_info = registry_.find(uuid);
        if (entity_info == registry_.end()) {
            return make_error_code(error_code::sl_error_codes::entity_is_not_registered);
        }

        entity_registration_info& registration_info = entity_info->second;
        if(registration_info.instance.has_value()) {
            result = *registration_info.instance;
            return make_error_code(std::errc{0});
        }

        if (registration_info.factory.has_value()) {
            // It is impossible situation in current sl design but let's process it just in case.
            if(auto factory_ptr = *registration_info.factory) {
                auto new_instance = factory_ptr->create();
                register_instance(uuid, new_instance);
                result = new_instance;
                return make_error_code(std::errc{0});
            }
        }

        // It is impossible situation in current sl design but let's process it just in case.
        return make_error_code(error_code::sl_error_codes::entity_is_not_registered);
    }

private:
    template<typename T>
    using uuid_unordered_map_t = std::unordered_map<boost::uuids::uuid, T, boost::hash<boost::uuids::uuid>>;
    struct entity_registration_info {
        std::optional<std::shared_ptr<void>> instance;
        std::optional<std::shared_ptr<sl::i_factory>> factory;

    };

    uuid_unordered_map_t<entity_registration_info> registry_;
};

service_locator::~service_locator() try {
	clear();
} catch(...) {
    // TODO: log error
}

void service_locator::clear() {
}


#include <type_traits>

std::error_code service_locator::register_instance(const boost::uuids::uuid& uuid, std::shared_ptr<void> instance) {
//    const size_t hash = typeid(T).hash_code();
//    if (instances_.find(hash) == instances_.end())
//        instances_.emplace(hash, std::shared_ptr<void>(instance));
}

std::error_code service_locator::register_factory(const boost::uuids::uuid& uuid, std::shared_ptr<sl::i_factory> factory) {
//    const size_t hash = typeid(T).hash_code();
//	if (factories_.find(hash) == factories_.end())
//        factories_.emplace(hash, creator);
}

std::error_code service_locator::resolve(const boost::uuids::uuid&, std::shared_ptr<void>&) {


//    const size_t hash = typeid(T).hash_code();
//    auto itr1 = instances_.find(hash);
//    if (itr1 != instances_.end())
//        return std::static_pointer_cast<T>(itr1->second);
//
//    auto itr2 = factories_.find(hash);
//    if (itr2 != factories_.end())
//        return std::static_pointer_cast<T>(itr2->second());

    return nullptr;
}

std::error_code service_locator::make_service_locator(std::shared_ptr<service_locator>& sl) try {
    struct make_shared_enabler : public service_locator {};
    sl = std::make_shared<make_shared_enabler>();
    return std::make_error_code(std::errc{0});
} catch(...) {
    return make_error_code(error_code::sl_error_codes::unknown_error);
}
