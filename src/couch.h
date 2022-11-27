#include<cpr/cpr.h>
#include<nlohmann/json.hpp>
using json = nlohmann::json;

json text2json(std::string s);
std::string json2text(const json &jv);

class CouchConn 
{
public:
	void connect(std::string host, std::string id, std::string pass);
	void set_db(std::string db);
	json create_db(std::string db);
	json insert(std::string doc, const json &jv);
	json update(std::string doc, const json &jv, std::string rev = "");
	json del(std::string doc, std::string rev = "");
	json select(std::string doc);//can use _all_docs
	json view(std::string design, std::string view, cpr::Parameters params);
	json bulk(json jv);
	json attach(std::string doc, std::string fullpath, std::string rev = "");
	std::string download(std::string doc, std::string file);
	json remove_file(std::string doc, std::string file, std::string rev= "");
	json find(json body);
protected:
	std::string id_, pass_, db_, host_;
	std::string db_url(), get_rev(std::string doc);

};
