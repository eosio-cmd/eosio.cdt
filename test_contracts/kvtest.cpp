#include <eosio/eosio.hpp>

#define EXCEPTION 0

struct my_struct {
   eosio::name n1;
   eosio::name n2;
   std::string foo;
   std::string bar;

   auto primary_key() const { return eosio::make_key(n1); }
   auto foo_key() const { return eosio::make_key(foo); }
   auto bar_key() const { return eosio::make_key(bar); }

   bool operator==(const my_struct b) const {
      return n1 == b.n1 &&
             n2 == b.n2 &&
             foo == b.foo &&
             bar == b.bar;
   }
};

struct my_table : eosio::kv_table<my_struct> {
   eosio::kv_table<my_struct>::index primary_index{eosio::name{"primary"}, &my_struct::primary_key};

   my_table() {
      init(eosio::name{"kvtest"}, eosio::name{"table"}, &primary_index);
   }
};

class [[eosio::contract]] kvtest: public eosio::contract {
public:
   using contract::contract;

   [[eosio::action]]
   void test() {
      my_table t;
      my_struct s{
         .n1 = "bob"_n,
         .n2 = "alice"_n,
         .foo = "a",
         .bar = "b"
      };
      my_struct s2{
         .n1 = "alice"_n,
         .n2 = "bob"_n,
         .foo = "c",
         .bar = "d"
      };
      my_struct s3{
         .n1 = "john"_n,
         .n2 = "joe"_n,
         .foo = "e",
         .bar = "f"
      };
      my_struct s4{
         .n1 = "joe"_n,
         .n2 = "john"_n,
         .foo = "g",
         .bar = "h"
      };
      my_struct s5{
         .n1 = "billy"_n,
         .n2 = "vincent"_n,
         .foo = "i",
         .bar = "j"
      };

      t.upsert(s3);
      t.upsert(s);
      t.upsert(s4);
      t.upsert(s2);
      t.upsert(s5);

      auto begin_itr = t.primary_index.begin();
      auto end_itr = t.primary_index.end();

      // Find
      // ----
      auto itr = t.primary_index.find("bob"_n);
      auto val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.n1 == "bob"_n, "Got the wrong n1");
      eosio::check(val.n2 == "alice"_n, "Got the wrong n2");

      itr = t.primary_index.find("joe"_n);
      val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.n1 == "joe"_n, "Got the wrong n1");
      eosio::check(val.n2 == "john"_n, "Got the wrong n2");

      itr = t.primary_index.find("alice"_n);
      val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.n1 == "alice"_n, "Got the wrong n1");
      eosio::check(val.n2 == "bob"_n, "Got the wrong n2");

      itr = t.primary_index.find("john"_n);
      val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.n1 == "john"_n, "Got the wrong n1");
      eosio::check(val.n2 == "joe"_n, "Got the wrong n2");

      // operator++
      // ----------
      itr = t.primary_index.begin();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().n1 == "alice"_n, "Got the wrong beginning");
      itr++;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().n1 == "billy"_n, "Got the wrong value");
      itr++;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().n1 == "bob"_n, "Got the wrong value");
      itr++;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().n1 == "joe"_n, "Got the wrong value");
      itr++;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().n1 == "john"_n, "Got the wrong value");
      itr++;
      eosio::check(itr == end_itr, "Should be the end");

      // operator--
      // ----------
      itr--;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      itr--;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      itr--;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      itr--;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      itr--;
      eosio::check(itr == begin_itr, "Should be the beginning");

      // Range 
      // -----
      std::vector<my_struct> expected{s, s4, s3};
      auto vals = t.primary_index.range("bob"_n, "john"_n);
      eosio::check(vals == expected, "range did not return expected vector");

      expected = {s};
      vals = t.primary_index.range("bob"_n, "bob"_n);
      eosio::check(vals == expected, "range did not return expected vector");

#if EXCEPTION
      expected = {s4, s3, s2};
      vals = t.primary_index.range("joe"_n, "alice"_n);
      eosio::check(vals == expected, "range did not return expected vector");
#endif

      // Erase 
      // -----
      t.erase("joe"_n);
      itr = t.primary_index.find("joe"_n);
      eosio::check(itr == end_itr, "key was not properly deleted");

      expected = {s, s3};
      vals = t.primary_index.range("bob"_n, "john"_n);
      eosio::check(vals == expected, "range did not return expected vector");
   }
};
