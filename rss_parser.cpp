#include "rss_parser.hpp"
#include <boost/spirit/include/phoenix.hpp>
#include <fstream>

rss_grammar::rss_grammar() : rss_grammar::base_type(start) {

	using boost::phoenix::bind;
	using boost::spirit::qi::_val;
	using boost::spirit::qi::_1;
	using boost::spirit::ascii::char_;
	using boost::spirit::lit;

	tag = open_tag(boost::spirit::qi::labels::_r1) >>
		tag_text >>
		close_tag(boost::spirit::qi::labels::_r1);

	//May not be valid xml.
	any_tag = &boost::spirit::lit("<") >> lit("<") >> !boost::spirit::lit("/") >> 
		(+((char_ - char_("/>")) >> *(lit("/") >> !lit(">"))))  
		>> ("/>" | ">" >> junk >> "</" >> +(char_ - ">") >> ">");

	open_tag = &lit("<") >> "<" >>
		end_tag(boost::spirit::qi::labels::_r1);

	close_tag = &lit("</") >> "</" >>
		end_tag(boost::spirit::qi::labels::_r1);

	end_tag = boost::spirit::ascii::string(boost::spirit::qi::labels::_r1) >> 
		">";

	junk = tag_text;
	tag_text = +(+(boost::spirit::ascii::char_ - '<') | cdata);

	cdata = "<![CDATA[" >> *(char_ - lit("]]>")) >> lit("]]>");

	start = lit("<item>") >> *(
		junk |
		(tag(std::string("title"))[bind(&rss_item::title, _val) = _1]) |
		(tag(std::string("description"))[bind(&rss_item::description, _val) = _1]) |
		(tag(std::string("link"))[bind(&rss_item::link, _val) = _1]) |
		(tag(std::string("pubDate"))[bind(&rss_item::set_date, _val, _1)]) |
		any_tag
		) >>
		lit("</item>");	
}

namespace rss_parser {

	rss_grammar g;

	std::vector<rss_item> parse_xml(const std::string &xml) {

		std::vector<rss_item> ret;

		std::string::const_iterator st = xml.begin(),
						en = xml.end();

		do {
			rss_item tmp;
			if (boost::spirit::qi::parse(st, en, g, tmp)) {
				ret.push_back(tmp);
			} else {
				st++;
			}
		} while (st!=en);

		return ret;
	}

	std::vector<rss_item> parse_file(const char *filename) {

		std::ifstream ifs(filename);

		std::string tmp;

		std::string xml;
		while(std::getline(ifs, tmp))
			xml += tmp;

		return parse_xml(xml);
	}
}
