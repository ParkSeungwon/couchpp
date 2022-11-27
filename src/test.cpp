#include<nlohmann/json.hpp>
#include<iostream>
#include"couch.h"
using namespace std;

int main()
{
	CouchConn pageDB;
	pageDB.connect("localhost:5984", "user", "password");
	auto r0 = pageDB.create_db("test5");
	cout << r0.dump(2) << endl;
	pageDB.set_db("dndd_page");
	//auto r = conn.select("z@zeta2374.com");
	auto r = pageDB.view
	( "num"
	, "num"
	, { { "startkey", "[1, 0]"}
		, { "endkey", "[4, 0]"}
		}
	);
//	json jv;
//	jv[0]["_id"] = "aaa";
//	jv[0]["val"] = "bbb";
//	jv[1]["_id"] = "bdf";
//	jv[1]["val"] = "한글";
//	auto r = conn.bulk(jv);
	cout << r.dump(2) << endl;
	cout << r["rows"].size() << endl;

	json selector;
	selector["num"] = {"$eq", 20};

	
	auto r2 = pageDB.find
	( json::parse(R"(
		{ "selector": {"num":{"$gt":20}}
		, "fields": ["num", "page", "title"]
		, "sort": ["num", "page"]
		, "limit": 10
		}
	)"));
	cout << r2.dump(2) << endl;
}

