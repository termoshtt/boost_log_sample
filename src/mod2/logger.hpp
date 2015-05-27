#pragma once

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/attributes/attribute.hpp>

namespace mod2 {

namespace logging = boost::log;

namespace log {
namespace attrs = logging::attributes;
namespace src = logging::sources;
}

using severity_level = logging::trivial::severity_level;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(
    logger, log::src::severity_logger_mt<severity_level>);
void init();

} // namespace mod1
