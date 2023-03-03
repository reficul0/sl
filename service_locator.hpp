#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include <boost/uuid/uuid.hpp>
#include <boost/noncopyable.hpp>

#include "factory.h"
#include "sl_error_codes.hpp"

class service_locator : boost::noncopyable {
    service_locator();
public:
	~service_locator() noexcept;

	void clear() noexcept;

	std::error_code register_instance(boost::uuids::uuid uuid, std::shared_ptr<void> instance) noexcept;
    std::error_code register_factory(const boost::uuids::uuid& uuid, std::shared_ptr<sl::i_factory> creator) noexcept;
    std::error_code resolve(const boost::uuids::uuid&, std::shared_ptr<void>&) noexcept;

    static std::error_code make_service_locator(std::shared_ptr<service_locator>&) noexcept;

private:
    class impl;
    std::unique_ptr<impl> impl_;
};