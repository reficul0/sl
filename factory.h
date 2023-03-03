#pragma once

#include <memory>

namespace sl {

class i_factory {
public:
    virtual ~i_factory() = default;
    virtual std::shared_ptr<void> create() = 0;
};

} // namespace sl
