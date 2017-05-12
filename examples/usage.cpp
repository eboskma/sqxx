
#include "sqxx.hpp"
#include <iostream>

int main() {
    // opening a database. Could use a file name instead of :memory:
    sqxx::connection conn(":memory:", sqxx::OPEN_CREATE|sqxx::OPEN_READWRITE);

    // simple statement execution
    conn.query("create table items (id integer, value integer)");
    conn.query("insert into items (id, value) values (1, 11), (2, 22), (3, 33)");

	 // getting results
	 sqxx::statement s1 = conn.query("select * from items where id = 1");
	 std::cout << "id=" << s1.val<int>(0) << " value=" << s1.val<int>(1) << std::endl;

    // getting multiple result rows, manual iteration
    sqxx::statement s2 = conn.query("select * from items");
	 while (!s2.done()) {
		 std::cout << "id=" << s2.val<int>(0) << " value=" << s2.val<int>(1) << std::endl;
		 s2.next_row();
	 }

    // iterating over multiple result rows
    sqxx::statement s3 = conn.query("select * from items where id <= 2");
    for (auto rownum : s3) {
		 std::cout << "row #" << rownum << ": id=" << s3.val<int>(0) << " value=" << s3.val<int>(1) << std::endl;
    }

    // Prepared statements
    sqxx::statement s4 = conn.prepare("select value from items where id = ?");

	 // ... binding parameters
    s4.bind(0, 2);
    s4.query();
    std::cout << "value=" << s4.val<int>(0) << std::endl;

	 // ... reset query and bindings, bind new parameters
	 s4.reset();
	 s4.clear_bindings();
    s4.bind(0, "3");
    s4.query();
    std::cout << "value=" << s4.val<int>(0) << std::endl;

    // ... resolving ambiguities by explicit types
	 s4.reset();
	 s4.clear_bindings();
    s4.bind<int64_t>(0, 2U);
    s4.query();
    std::cout << "value=" << s4.val<int>(0) << std::endl;
}

