#include "rss_parser.hpp"
#include <boost/spirit/include/phoenix.hpp>
#include <fstream>

rss_grammar::rss_grammar() : rss_grammar::base_type(start) {

	using boost::phoenix::bind;
	using boost::spirit::qi::_val;
	using boost::spirit::qi::_1;
	using boost::spirit::qi::_a;
	using boost::spirit::standard_wide::char_;
	using boost::spirit::lit;
	using boost::spirit::int_;

	tag = open_tag(boost::spirit::qi::labels::_r1) >>
		tag_text >>
		close_tag(boost::spirit::qi::labels::_r1);

	//May not be valid xml.
	any_tag = &boost::spirit::lit(L"<") >> lit(L"<") >> !boost::spirit::lit(L"/") >> 
		(+((char_ - char_(L"/>")) >> *(lit(L"/") >> !lit(L">"))))  
		>> (L"/>" | L">" >> junk >> L"</" >> +(char_ - L">") >> L">");

	open_tag = &lit(L"<") >> L"<" >>
		end_tag(boost::spirit::qi::labels::_r1);

	close_tag = &lit(L"</") >> L"</" >>
		end_tag(boost::spirit::qi::labels::_r1);

	end_tag = boost::spirit::standard_wide::string(boost::spirit::qi::labels::_r1) >> 
		L">";

	junk = tag_text;
	tag_text = +(+(html_entity | (char_ - L'<')) | cdata);

	cdata = L"<![CDATA[" >> 
		*(
			html_entity | (char_ - lit(L"]]>"))
		) >> 
		lit(L"]]>");

	start = lit(L"<item>") >> *(
		junk |
		(tag(std::wstring(L"title"))[bind(&rss_item::title, _val) = _1]) |
		(tag(std::wstring(L"description"))[bind(&rss_item::description, _val) = _1]) |
		(tag(std::wstring(L"link"))[bind(&rss_item::link, _val) = _1]) |
		(tag(std::wstring(L"pubDate"))[bind(&rss_item::set_date, _val, _1)]) |
		any_tag
		) >>
		lit(L"</item>");	


        html_entity = (lit(L'&') >> lit(L'#') >> boost::spirit::int_[_a = _1] >> lit(L';')[_val = _a]);
}

namespace rss_parser {

	rss_grammar g;

	std::vector<rss_item> parse_xml(const std::string &xml_p) {

		std::vector<rss_item> ret;


		std::wstring xml;
		xml.assign(xml_p.begin(), xml_p.end());
		
		std::wstring::const_iterator st = xml.begin(),
						en = xml.end();

		do {
			rss_item tmp;
			if (boost::spirit::qi::parse(st, en, g, tmp)) {
				ret.push_back(tmp);
				std::wcout << tmp.description << std::endl;
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
