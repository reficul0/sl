#pragma once

#include <system_error>
#include <string_view>

#include <boost/container/flat_map.hpp>

namespace error_code {

enum class sl_error_codes {
    /* =0 is reserved as success code */
    unknown_error = 1,
    entity_already_registered,
    invalid_entity_factory,
    invalid_instance,
    entity_is_not_registered,
};

struct sl_error_codes_category : std::error_category
{
    [[nodiscard]] const char* name() const noexcept override {
        return "service_locator";
    }
    [[nodiscard]] std::string message(int ec) const override {
        static boost::container::flat_map<sl_error_codes, std::string_view> descr_by_ec {
            {sl_error_codes::entity_already_registered, "entity already registered" }
        };
        auto ec_descr_iter = descr_by_ec.find(static_cast<sl_error_codes>(ec));
        return static_cast<std::string>(
                ec_descr_iter != descr_by_ec.end()
                      ? ec_descr_iter->second
                      : "error code has no description");
    }
};

std::error_code make_error_code(sl_error_codes ec) {
    static const sl_error_codes_category ec_category {};
    return { static_cast<int>(ec), ec_category };
}

} // namespace error_code


namespace std
{
    template <>
    struct is_error_code_enum<error_code::sl_error_codes> : std::true_type {};
}