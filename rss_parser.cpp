#include "rss_parser.hpp"
#include <boost/spirit/include/phoenix.hpp>
#include <fstream>

img_grammar::img_grammar() : img_grammar::base_type(start) {

        using boost::phoenix::bind;
        using boost::spirit::qi::_val;
        using boost::spirit::qi::_1;
        using boost::spirit::qi::_a;
        using boost::spirit::qi::_b;
        using boost::spirit::qi::_r1;
        using boost::spirit::qi::_r2;
        using boost::spirit::qi::_r3;
        using boost::spirit::ascii::char_;
        using boost::spirit::ascii::string;
        using boost::spirit::lit;

	start = find_tag(std::string("div"), std::string("id"), std::string("postcontent")) 
		>> find_name(std::string("img")) >> 
		parse_att(std::string("src"));

	
	//Move the iterator to the start (right after the name) of a tag with the given name.
	find_name = *(char_ - char_("<")) >> //Anything up until the start of a tag.
		(
			 lit("<") >> //Start of tag.
			(			//Below: What about <div> (No space)?
				string(_r1) >> -lit(" ") | //Either the tag name we are looking for.
				find_name(_r1) 		   //Recusive call with the remaining text.
			)
		);
	
	find_tag = find_name(_r1) >> find_att(_r2, _r3) >> *(char_ - char_(">")) >> lit (">");
	
	//Move the iterator to the beggining of a tag's content where that tag has an
	//attibute and value matching the inherited attribute of the rule.
	find_att = *(char_ - string(_r1)) >> string(_r1) >>	//Find the given attribute name.
		*boost::spirit::ascii::space >> //Disregard space.
		lit("=") >> 			//Find the equals sign.
		*boost::spirit::ascii::space >> //Disregard space.
		(char_("\"'")) >>		//Find the opening ' or "
		string(_r2) >>			//Check for the sought for value. 
		char_("\"'");			//Find the closing ' or "

	parse_att = *(char_ - string(_r1)) >> string(_r1) >> 
		*boost::spirit::ascii::space >> 
		lit("=") >> 
		*boost::spirit::ascii::space >> 
		(char_("\"'")) >>
		*(char_ - char_("\"'"))[_val += _1] >> 
		char_("\"'");
}

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
			html_entity | char_(L"&") | (char_ - char_(L"]"))
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

        html_entity = (lit('&') >> lit('#') >> boost::spirit::int_[_a = _1] >> lit(';')[_val = _a]);
}

namespace rss_parser {


	std::vector<rss_item> parse_xml(const std::string &xml_p) {

		rss_grammar g;
		std::vector<rss_item> ret;


		std::wstring xml;
		xml.assign(xml_p.begin(), xml_p.end());
		
		std::wstring::const_iterator st = xml.begin(),
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

	std::string parse_img(const std::string &xml) {

		img_grammar g;
		std::string::const_iterator st = xml.begin(),
						en = xml.end();

		do {
			std::string tmp;
			if (boost::spirit::qi::parse(st, en, g, tmp)) {
				return tmp;	
			} else {
				st++;
			}
		} while (st!=en);

		//Probably 403ed by Cloudflare for making too many requests.
		std::cout << xml << std::endl;
		return "";
	}
}
