CS144 lab的记录

## lab0

connection: close 不采用非持续连接，

shutdown: 关闭连接（两条连接，可关闭一个）



可靠连接实验

记第一次提交，我的思路似乎有问题： 比如容量是2， 写cat, 我的意愿是写入ca后，t之后（read之后）也会传过去，但是根据测试用例不是的，所以有测试用例没有通过，代码也比较复杂。因为实际传输中，write方会知道还有多少剩余容量可以传输，所以如果传的比剩余容量还多的话就可以丢掉。

思路是维护一个队列，写入时要考虑剩余的容量，读取时要考虑有多少已经写入的。（当然用string也是可以的）


## 代码规范
The lab assignments will be done in a contemporary C++ style that uses recent (2011)
features to program as safely as possible. This might be different from how you have been
asked to write C++ in the past. For references to this style, please see the C++ Core
Guidelines (http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).
The basic idea is to make sure that every object is designed to have the smallest possible
public interface, has a lot of internal safety checks and is hard to use improperly, and knows
how to clean up after itself. We want to avoid “paired” operations (e.g. malloc/free, or
new/delete), where it might be possible for the second half of the pair not to happen (e.g., if a CS144: Introduction to Computer Networking Fall 2020 function returns early or throws an exception). Instead, operations happen in the constructor to an object, and the opposite operation happens in the destructor. This style is called “Resource acquisition is initialization,” or RAII.
In particular, we would like you to:
- Use the language documentation at https://en.cppreference.com as a resource.
- Never use malloc() or free().
- Never use new or delete.
- Essentially never use raw pointers (*), and use “smart” pointers (unique ptr or
shared ptr) only when necessary. (You will not need to use these in CS144.)
- Avoid templates, threads, locks, and virtual functions. (You will not need to use these
in CS144.)
- Avoid C-style strings (char *str) or string functions (strlen(), strcpy()). These
are pretty error-prone. Use a std::string instead.
- Never use C-style casts (e.g., (FILE *)x). Use a C++ static cast if you have to (you
generally will not need this in CS144).
- Prefer passing function arguments by const reference (e.g.: const Address & address).
- Make every variable const unless it needs to be mutated.
- Make every method const unless it needs to mutate the object.
- Avoid global variables, and give every variable the smallest scope possible.
- Before handing in an assignment, please run make format to normalize the coding
style.