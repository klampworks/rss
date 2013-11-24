#ifndef RSS_PARSER_HPP
#define RSS_PARSER_HPP

#include <boost/spirit/include/qi.hpp>
#include "rss_item.hpp"

struct rss_grammar : boost::spirit::qi::grammar<std::string::const_iterator, rss_item()> {

	rss_grammar();

	boost::spirit::qi::rule<std::string::const_iterator, std::string()> tag_text, cdata;
	boost::spirit::qi::rule<std::string::const_iterator, rss_item()> start;
	boost::spirit::qi::rule<std::string::const_iterator, boost::spirit::unused_type()>junk, any_tag;
	boost::spirit::qi::rule<std::string::const_iterator, 
		boost::spirit::unused_type(std::string)>open_tag, close_tag, end_tag;

	boost::spirit::qi::rule<std::string::const_iterator, std::string(std::string)> tag;
};

namespace rss_parser {

	std::vector<rss_item> parse_file(const char *);
	std::vector<rss_item> parse_xml(const std::string &xml);
}
#endif
