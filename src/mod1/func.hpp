#pragma once

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/attributes/attribute.hpp>

namespace mod1 {

enum severity_level { debug, info, warning, error, critical };
std::ostream &operator<<(std::ostream &ost, severity_level l);
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(
    logger,                                                 // loggerの名称
    boost::log::sources::severity_logger_mt<severity_level> // loggerの型
    );

BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope",
                            boost::log::attributes::named_scope::value_type)

void init();
void func1(bool);
void func2();

} // namespace mod1
