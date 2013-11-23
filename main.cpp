#include <boost/spirit/include/qi.hpp>
#include <iostream>
#include <fstream>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi_repeat.hpp>

#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/fusion/functional/generation/make_fused.hpp>
#include <boost/fusion/include/make_fused.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <ctime>

#include "rss_gui.hpp"
#include <QApplication>
struct item {

	std::string title, description, link;
	long date;

	void set_date(const std::string &input) {
		
                static const char *format("%d %h %Y %H:%M:%S %Z");
		//Mon, 18 Nov 2013 09:46:11 GMT

                struct tm tm;
                memset(&tm, 0, sizeof(tm));

                strptime(input.c_str() + 5, format, &tm);
		date = (long)mktime(&tm);
	}
};

struct rss_grammar : boost::spirit::qi::grammar<std::string::const_iterator, item()> {

rss_grammar() : rss_grammar::base_type(start) {

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
			(tag(std::string("title"))[bind(&item::title, _val) = _1]) |
			(tag(std::string("description"))[bind(&item::description, _val) = _1]) |
			(tag(std::string("link"))[bind(&item::link, _val) = _1]) |
			(tag(std::string("pubDate"))[bind(&item::set_date, _val, _1)]) |
			any_tag
			) >>
			lit("</item>");	
			//close_tag(std::string("item"));
				;
	//			*/

}

	boost::spirit::qi::rule<std::string::const_iterator, std::string()> tag_text, cdata;
	boost::spirit::qi::rule<std::string::const_iterator, item()> start;
	boost::spirit::qi::rule<std::string::const_iterator, boost::spirit::unused_type()>junk, any_tag;
	boost::spirit::qi::rule<std::string::const_iterator, boost::spirit::unused_type(std::string)>open_tag, close_tag, end_tag;

	boost::spirit::qi::rule<std::string::const_iterator, std::string(std::string)> tag;

};
int main(int argc, char **argv) {


	QApplication app(argc, argv);
	rss_gui window;

	const static std::string filename("rss.xml");
	std::ifstream ifs(filename.c_str());

	std::string tmp;

	std::string xml;
	while(std::getline(ifs, tmp)) {
		xml +=  tmp;
	}

	rss_grammar g;

	//xml = "<item>     \t<title>hello</title> <media:thumbnail width=\"144\" height=\"81\" url=\"http://news.bbcimg.co.uk/media/images/71172000/jpg/_71172468_71171667.jpg\"/></item>";
	//xml = "<item><title>  <![CDATA[ sf&>&<&>&<&& ]]> </title></item>";

	std::string::const_iterator st = xml.begin(),
					en = xml.end();

	std::cout << xml <<std::endl << std::endl;
	std::cout << "##############################" << std::endl;
	do {
		item tmp;
		if (boost::spirit::qi::parse(st, en, g, tmp)) {

			window.add_item(tmp.title);

			std::cout << "Match: #" << tmp.title << "#" << std::endl;
			std::cout << tmp.description << "#" << std::endl;
			std::cout << tmp.link << "#" << std::endl;
			std::cout << tmp.date << std::endl << std::endl;
//			std::cout << "Remaining: #" << std::string(st, en) << "#" << std::endl << std::endl;
		} else {
//			std::cout << "No match: " << /*std::string(st, en)<<*/ std::endl;
			st++;
		}
	} while (st!=en);

	window.resize(500, 500);
	window.show();
	return app.exec();

}
