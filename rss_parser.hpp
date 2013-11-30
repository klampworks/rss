#ifndef RSS_PARSER_HPP
#define RSS_PARSER_HPP

#include <boost/spirit/include/qi.hpp>
#include "rss_item.hpp"
#include <map>

struct img_grammar : boost::spirit::qi::grammar<std::string::const_iterator, std::string()> {

	img_grammar();
    boost::spirit::qi::rule<std::string::const_iterator, boost::spirit::unused_type(std::string, std::string, std::string)> find_tag;
	
        boost::spirit::qi::rule<std::string::const_iterator, std::string()> start;
        boost::spirit::qi::rule<std::string::const_iterator, boost::spirit::unused_type(std::string)> find_name;
        boost::spirit::qi::rule<std::string::const_iterator, boost::spirit::locals<std::string>, boost::spirit::qi::unused_type(std::string, std::string)> find_att;

        boost::spirit::qi::rule<std::string::const_iterator, std::string(std::string)> parse_att;
};

struct rss_grammar : boost::spirit::qi::grammar<std::wstring::const_iterator, rss_item()> {

	rss_grammar();

	boost::spirit::qi::rule<std::wstring::const_iterator, std::wstring()> tag_text, cdata;
	boost::spirit::qi::rule<std::wstring::const_iterator, rss_item()> start;
	boost::spirit::qi::rule<std::wstring::const_iterator, boost::spirit::unused_type()>junk, any_tag;
	boost::spirit::qi::rule<std::wstring::const_iterator, 
		boost::spirit::unused_type(std::wstring)>open_tag, close_tag, end_tag;

	boost::spirit::qi::rule<std::wstring::const_iterator, std::wstring(std::wstring)> tag;
        boost::spirit::qi::rule<std::wstring::const_iterator, boost::spirit::qi::locals<int>, wchar_t()> html_entity;
};

namespace rss_parser {

	std::map<unsigned, rss_item> parse_file(const char *);
	std::map<unsigned, rss_item> parse_xml(const std::string &xml);
	std::string parse_img(const std::string &xml);
}
#endif
