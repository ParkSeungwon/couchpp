#include"couch.h"
using namespace std;

json text2json(string s)
{
	return json::parse(s);
}

string json2text(const json &jv) {
	return jv.dump(2);
}

void CouchConn::connect(string host, string id, string pass)
{
	host_ = host; id_ = id; pass_ = pass;
}

void CouchConn::set_db(string db)
{
	db_ = db;
}

json CouchConn::create_db(string db)
{
	cpr::Response r = cpr::Put
	( cpr::Url{host_ + '/' + db}
	, cpr::Authentication{id_, pass_}
	);
	return json::parse(r.text);
}

std::string CouchConn::db_url() 
{ 
	return host_ + '/' + db_; 
}

json CouchConn::insert(string doc, const json &jv)
{
	cpr::Response r = cpr::Put
		( cpr::Url{db_url() + '/' + doc}
		, cpr::Body{json2text(jv)}
		, cpr::Authentication{id_, pass_}
		);
	return text2json(r.text);
}

json CouchConn::select(string doc)
{
	cpr::Response r = cpr::Get
		( cpr::Url{db_url() + '/' + doc}
		, cpr::Authentication{id_, pass_}
		);
	return text2json(r.text);
}

json CouchConn::view(string design, string view, cpr::Parameters params)
{
	auto r = cpr::Get
		( cpr::Url{db_url() + "/_design/" + design + "/_view/" + view}
		, cpr::Authentication{id_, pass_}
		, params
		);
	return text2json(r.text);
}

json CouchConn::find(json body)
{
	auto r = cpr::Post
		( cpr::Url{db_url() + "/_find"}
		, cpr::Authentication{id_, pass_}
		, cpr::Body{body.dump()}
		, cpr::Header{{"Content-Type", "application/json"}}
		);
	return text2json(r.text);
}

json CouchConn::bulk(json jv)
{
	json j;
	j["docs"] = jv;
	auto r = cpr::Post
	( cpr::Url{db_url() + "/_bulk_docs"}
	, cpr::Authentication{id_, pass_}
	, cpr::Header{{"Content-Type", "application/json"}}
	, cpr::Body{json2text(j)}
	);
	return text2json(r.text);
}

json CouchConn::attach(string doc, string full_path, string rev)
{
	string file = full_path.substr(full_path.rfind('/') + 1);
	ifstream f{full_path};
	string contents;
	for(char c; f >> noskipws >> c; ) contents += c;
	if(rev == "") rev = get_rev(doc);
	auto r = cpr::Put
		( cpr::Url{db_url() + '/' + doc + '/' + file}
		, cpr::Authentication{id_, pass_}
		, cpr::Parameters{{"rev", rev}}
		, cpr::Body{contents}
			//, cpr::Header{{"Content-Type", "image/png"}}
		);
	return text2json(r.text);
}

string CouchConn::download(string doc, string file)
{
	auto r = cpr::Get
		( cpr::Url{db_url() + '/' + doc + '/' + file}
		, cpr::Authentication{id_, pass_}
		);
	return r.text;
}

string CouchConn::get_rev(string doc)
{
	auto r = cpr::Get(cpr::Url{db_url() + '/' + doc}, cpr::Authentication{id_, pass_});
	auto jv = text2json(r.text);
	return jv["_rev"].get<std::string>();
}

json CouchConn::update(string doc, const json &jv, string rev)
{
	if(rev == "") rev = get_rev(doc);
	cpr::Response r = cpr::Put
		( cpr::Url{db_url() + '/' + doc}
		, cpr::Authentication{id_, pass_}
		, cpr::Body{json2text(jv)}
		, cpr::Parameters{{"rev", rev}}
		);
	return text2json(r.text);
}

json CouchConn::del(string doc, string rev)
{
	if(rev == "") rev = get_rev(doc);
	cpr::Response r = cpr::Delete
		( cpr::Url{db_url() + '/' + doc}
		, cpr::Authentication{id_, pass_}
		, cpr::Parameters{{"rev", rev}}
		);
	return text2json(r.text);
}

json CouchConn::remove_file(string doc, string file, string rev)
{
	if(rev == "") rev = get_rev(doc);
	auto r = cpr::Delete
		( cpr::Url{db_url() + '/'+ doc + '/' + file}
		, cpr::Authentication{id_, pass_}
		, cpr::Parameters{{"rev", rev}}
		);
	return text2json(r.text);
}
