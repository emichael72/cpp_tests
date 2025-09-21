#include "iter.h"
#include "shared_ptr.h"
#include "unique_ptr.h"
#include "container.h"
#include <iostream>
#include <vector>


typedef void (*DeleterFunc)(int *);

struct dummy_object {
  char arr[34]; // dummy : just to see what happens when x isa not the first
  int x;
};

// note: C style function as deleter
void my_delete(int *p) {
  std::cout << "my_delete called\n";
  delete p;
}

int test_shared_ptr() {

  std::cout << "\nTesting SharedPtr\n";

  try {

    SharedPtr<dummy_object> p1(new dummy_object{.x = 42});
    std::cout << "p1->x value: " << p1->x << "\n";

    SharedPtr<dummy_object> p2(std::move(p1)); // move p1 to p2
    std::cout << "p2->x value: " << p2->x << "\n";

    // modify p2
    ++p2->x;
    std::cout << "After p2 increment, p2->x = " << p2->x << "\n";

    SharedPtr<dummy_object> p3(new dummy_object{.x = 100});
    std::cout << "p3->x value: " << p3->x << "\n";

    p2 = p1 = p3;
    std::cout << "After reassignment:\n";
    std::cout << "p2->x = " << p2->x << "\n";
    std::cout << "p3->x = " << p3->x << "\n";

    return p2.get()->x;

  } catch (...) {
    std::cerr << "unknown exception\n";
    return -1;
  }
}

int test_unique_ptr() {

  std::cout << "\nTesting UniquePtr\n";

  try {
    // note: falling to default delete
    UniquePtr<int> p1(new int(42));
    int p2_val = 0;

    std::cout << "p1 value: " << *p1.get() << "\n";

    // note: move p1 to p2 (take ownership)
    UniquePtr<int> p2 = std::move(p1);

    // *p2 = (*p1)++; // run time error
    ++(*p2);

    if (p1.get() != nullptr) {
      throw std::runtime_error("p1 is not nullptr after move");
    }

    if (p2.get() != nullptr) {
      p2_val = *(p2.release());
    }

    std::cout << "p2 value: " << p2_val << "\n";

    // note: Using a C-style function as a custom deleter
    UniquePtr<int, DeleterFunc> p3(new int(++p2_val), my_delete);
    std::cout << "p3 value: " << *p3 << "\n";

    return p2_val;

  } catch (...) {
    std::cerr << "unknown exception\n";
    return -1;
  }
}

int test_iter() {

  int arr[3] = {10, 20, 30};

  SimpleIterator<int> begin(arr);
  SimpleIterator<int> end(arr + 3);

  for (auto it = begin; it != end; ++it) {
    std::cout << *it << " ";
  }
  std::cout << "\n";
  return 0;
}


int test_container()
{
    MiniContainer cont;
    cont.add({"a", 85});
    cont.add({"b", 92});
    cont.add({"c", 78});
    cont.add({"d", 95});
    cont.add({"s", 88});


    cont.print();

    std::sort(cont.begin(), cont.end());

    std::cout << "\nsorted:\n";
    cont.print();

    return 0;
}

int main() {

  // int ret = test_unique_ptr();
  // int ret = test_shared_ptr();
  // int ret = test_iter();
  //int ret = test_container();

  SimpleString s("hello world");
   for (size_t i = 0; i < s.size(); ++i) {
       std::cout << s[i];
   }
   std::cout << s.size() << "\n";   
   s[0] = 'Y'; 
   



  int ret = 0;
  return ret;
}