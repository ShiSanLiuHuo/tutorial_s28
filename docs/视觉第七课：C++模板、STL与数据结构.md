# 视觉第七课：C\+\+模板、STL与数据结构

**模板（Template）**：让同一套代码适用于不同类型。  

**STL（标准模板库）**：提供现成的容器、迭代器、算法等工具。  

**数据结构**：研究数据怎么组织，才能让操作更快、更方便。



**这是今天课堂上的练习**：https://github.com/broruan/cpp_stl_training



# **第一部分：C\+\+ 模板**



## **1\. 为什么要有模板？**



假设我们要写一个函数，返回两个数字里较大的那个。



```C++
int myMax(int a, int b) {
    return a > b ? a : b;
}
```



现在要比较 `double` 呢？再写一个：



```C++
double myMax(double a, double b) {
    return a > b ? a : b;
}
```



再来个 `long long`？继续写。



其实逻辑完全一样，只有类型不同。模板就是用来解决这种重复工作的。



## **2\. 函数模板：把类型当成参数**



```C++
#include <iostream>

template <typename T>
T myMax(T a, T b) {
    return a > b ? a : b;
}

int main() {
    std::cout << myMax(3, 8) << '\n';        // T 推导为 int
    std::cout << myMax(2.5, 1.8) << '\n';    // T 推导为 double
    std::cout << myMax<char>('a', 'z') << '\n';
}
```



`template <typename T>` 可以理解为：**这里先留一个类型占位符 T，等你调用时再确定它是什么。**



`typename` 这里换成 `class` 也可以：



```C++
template <class T>
T add(T a, T b) {
    return a + b;
}
```



在这种场景下，`typename` 和 `class` 没有功能上的区别。一般选自己觉得好读的那个即可。



### **类型推导不是万能的**



下面这段可能会报错：



```C++
// myMax(3, 2.5);  // 一个是 int，一个是 double，T 推导发生冲突
```



可以明确指定类型：



```C++
myMax<double>(3, 2.5);
```



也可以设计两个类型参数：



```C++
#include <type_traits>

template <typename A, typename B>
auto myMax2(A a, B b) -> std::common_type_t<A, B> {
    return a > b ? a : b;
}
```

这里 `std::common_type_t<A, B>` 用来找一个适合这两种类型的公共类型。`auto`指向的是a和b的公共数据类型。



也可以这样写：

```C++
#include <type_traits>

template <typename A, typename B>
auto myMax2(A a, B b){
    return a > b ? a : b;
}
```

这就是`auto`自动由return推导数据类型，比较常见。



### **模板不是“所有类型都支持”**



`myMax()` 内部用到了 `>`，所以传进去的类型必须能比较大小。如果类型根本不支持 `>`，编译器就会报错。



模板只是让代码能针对不同类型生成相应版本，**并不代表任何类型都能无条件套进去**。



## **3\. 类模板**



函数能把类型参数化，类也可以。我们自己写一个最简单的“盒子”试试。



```C++
#include <iostream>
#include <string>

template <typename T>
class Box {
private:
    T value_;

public:
    explicit Box(T value) : value_(value) {}

    const T& get() const {
        return value_;
    }

    void set(const T& value) {
        value_ = value;
    }
};

int main() {
    Box<int> a(10);
    Box<std::string> b("hello");

    std::cout << a.get() << '\n';
    std::cout << b.get() << '\n';
}
```



这里的：



```C++
Box<int> a(10);
Box<std::string> b("hello");
```



就是让编译器分别生成适用于 `int` 和 `std::string` 的类。



`std::vector<int>`、`std::vector<double>` 原理上也差不多。`vector` 是一个类模板，而 `<int>` 是它的类型实参。



## **4\. 模板可以有多个参数，也可以传常量**



先看多个类型参数：



```C++
template <typename K, typename V>
struct Pair {
    K key;
    V value;
};

Pair<int, double> item{1, 95.5};
```



接着看**非类型模板参数**：



```C++
#include <array>

std::array<int, 5> nums{1, 2, 3, 4, 5};
```



`std::array` 的模板原型可以粗略理解为：



```C++
template <typename T, std::size_t N>
struct ArrayLike {
    T data[N];
};
```



注意这里 `N` 不是类型，而是一个编译期常量。所以 `std::array<int, 5>` 的长度 `5` 是类型的一部分，和 `std::array<int, 6>` 不是同一个类型。



## **5\.\* 模板特化：有些类型想单独处理**



比如大多数类型直接输出即可，但 `bool` 想输出中文“是/否”。



```C++
#include <iostream>

template <typename T>
struct Printer {
    static void print(const T& value) {
        std::cout << value << '\n';
    }
};

// 对 bool 做全特化
template <>
struct Printer<bool> {
    static void print(bool value) {
        std::cout << (value ? "是" : "否") << '\n';
    }
};

int main() {
    Printer<int>::print(42);
    Printer<bool>::print(true);
}
```



理解成“通用版本管大部分情况，特殊类型单独开小灶”就行。



类模板还支持**偏特化**，比如单独处理所有指针类型：



```C++
template <typename T>
struct TypeInfo {
    static constexpr bool isPointer = false;
};

template <typename T>
struct TypeInfo<T*> {
    static constexpr bool isPointer = true;
};

static_assert(!TypeInfo<int>::isPointer);
static_assert(TypeInfo<int*>::isPointer);
```



这类技巧经常出现在标准库、类型萃取和泛型框架里。只用看懂在做什么即可。

> static\_assert\(\)编译期检查条件是否满足
> 
> 







## **6\. 可变参数模板：参数数量也可以不固定**



```C++
#include <iostream>

template <typename... Args>
void printAll(const Args&... args) {
    ((std::cout << args << ' '), ...);  // C++17 折叠表达式
    std::cout << '\n';
}

int main() {
    printAll(1, 2.5, "hello", 'X');
}
```



- `typename... Args`：表示接收一组类型。

- `args...`：表示接收一组对应的实参。

- `((表达式), ...)`：把同一个操作依次应用到每个参数上。

如果你以后写日志工具、通用包装器，很可能用到它。



## **7\. 模板代码为什么经常写在头文件里？**



普通函数可以把声明放 \`\.h\`，实现放 \`\.cpp\`。但模板通常需要在使用它的地方**看到完整定义**，这样编译器才能为具体类型实例化代码。

所以模板类、模板函数的实现经常直接写在 `.hpp` 里，或者写在被头文件包含的 `.tpp` 中。（一般都是在 

`.hpp`中的）

# **第二部分：STL，平时写 C\+\+ 最常用的一套工具**

## **8\. STL 到底包含什么？**



我们一般说 STL，主要会接触这几类：



|部分|干什么的|例子|
|---|---|---|
|容器|存数据|`vector`、`list`、`map`、`set`|
|迭代器|按统一方式访问元素|`begin()`、`end()`|
|算法|对区间里的数据进行处理|`sort`、`find`、`lower_bound`|
|容器适配器|包装出特定的使用方式|`stack`、`queue`、`priority_queue`|





## **9\.\`vector\`：绝大多数时候先想到它**



`std::vector` 是**动态数组**，元素在内存中连续存放。它会根据需要扩容，比普通数组用起来省心。



```C++
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{10, 20, 30};

    v.push_back(40);       // 尾部追加
    v[1] = 25;             // 修改第二个元素

    std::cout << v.size() << '\n';   // 元素数量：4
    std::cout << v.front() << '\n';  // 第一个元素
    std::cout << v.back() << '\n';   // 最后一个元素

    for (int x : v) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}
```



### **\`\[\]\` 和 \`\.at\(\)\` 有什么区别？**



```C++
v[100];     // 越界访问：未定义行为，不能指望程序一定报错
v.at(100);  // 越界时抛出 std::out_of_range
```



写题目时大家经常用 `[]`，但前提是自己能保证索引合法。



### **\`reserve\(\)\` 和 \`resize\(\)\` **



```C++
std::vector<int> v;
v.reserve(100);  // 预留至少 100 个元素的容量，size 还是 0
v.resize(10);   // 真正调整成 10 个元素，新增 int 值初始化为 0
```



`reserve()` 是预留空位，但是没有数据，`resize()` 是初始化为0。



#### **\`push\_back\(\)\` 和 \`emplace\_back\(\)\`**



`push_back(x)` 把已有对象追加进去；`emplace_back(args...)` 把参数传给元素的构造函数，在容器尾部直接构造对象。



`emplace_back()` **\*\*不保证一定比 \`push\_back\(\)\` 快\*\***。对于 \`int\` 这种简单类型，没必要纠结；需要就地构造对象时它更方便。



#### **这里补充一个知识（关于\&（左值）和\&\&（右值））**



`&`是左值引用：

```C++
int a = 10;
int c = 30;
int& b = a;

a = 20;
cout << b;

b = c;
cout << a;

```

右值引用用：int\&\& b = std::move\(a\);







#### **\`vector\` 常见的坑**



当 `vector` 扩容时，可能会换一块更大的内存。原来保存的元素指针、引用和迭代器就可能全部失效。



```C++
std::vector<int> v{1, 2, 3};
int* p = &v[0];
v.push_back(4);
// 不要再使用 p：如果发生扩容，p 已经失效
```



## **11\.\`string\`**



```C++
#include <iostream>
#include <string>

int main() {
    std::string s = "hello";
    s += " world";

    std::cout << s.size() << '\n';         // 11
    std::cout << s.substr(0, 5) << '\n';   // hello
    std::cout << s.find("world") << '\n';  // 6
}
```



`find()` 找不到时返回 `std::string::npos`，判断时直接和它比较，不要写成和 \-1 比：



```C++
if (s.find("abc") == std::string::npos) {
    // 没找到
}
```



`std::string` 存的是字节。处理 UTF\-8 中文时，`size()` 返回的是字节数，不是汉字个数。



## **12\.\`array\`、\`deque\`、\`list\`**



#### **\`std::array\`：固定长度数组**



```C++
#include <array>
std::array<int, 3> a{10, 20, 30};
```



长度在编译时确定，元素连续存放，适合大小固定的数据。



#### **\`std::deque\`：双端队列**



```C++
#include <deque>
std::deque<int> d;
d.push_front(1);
d.push_back(2);
d.pop_front();
```



两端插入、删除都很快，也**支持下标访问**。但它的内存不是整块连续的，不能当成 \`vector\` 那样用**指针**遍历整段数据。



#### **\`std::list\`：双向链表**

链表相关基础：https://blog\.csdn\.net/Edward\_Asia/article/details/120876314

```C++
#include <iterator>
#include <list>
std::list<int> values{1, 3};
auto it = std::next(values.begin());
values.insert(it, 2);  // 1, 2, 3
// 可以自己试一下这个是插入到前面还是后面的
```



已知位置时，插入和删除是 O\(1\)。但要找到这个位置得从头遍历，是 O\(n\)，而且不支持 `values[2]`。



链表插入快不代表该优先选它。`vector` 内存连续，对缓存友好，多数场景反而更快。



## **13\.\`stack\`、\`queue\`、\`priority\_queue\`**



这三个都是容器适配器，只开放有限的几个操作。



### **栈\`stack\`：后进先出（LIFO）**



```C++
#include <stack>
std::stack<int> st;
st.push(10);
st.push(20);
int x = st.top();  // 20
st.pop();          // 删除栈顶，pop() 不返回元素
```



### **队列\`queue\`：先进先出（FIFO）**



```C++
#include <queue>
std::queue<int> q;
q.push(10);
q.push(20);
int x = q.front();  // 10
q.pop();
```



### **\*优先队列\`priority\_queue\`**



默认是大根堆，`top()` 取到的是当前最大的元素。



```C++
#include <functional>
#include <queue>
#include <vector>

std::priority_queue<int> big;
big.push(3);
big.push(10);
big.push(5);
// big.top() == 10

std::priority_queue<int, std::vector<int>, std::greater<int>> small;
small.push(3);
small.push(10);
small.push(5);
// small.top() == 3，小根堆
```



它不会把所有元素排好序，只维护堆的结构。`top()` 是 O\(1\)，`push()` 和 `pop()` 是 O\(log n\)。

std::greater\<int\>是一个比较器，greater\(a, b\)，若a\>b则返回true（前一个大于后一个）。



## **14\.\`set\`、\`map\`**



`set` 存不重复的键，`map` 存键值对，键不能重复。两者都按键有序。



```C++
#include <iostream>
#include <map>
#include <set>
#include <string>

int main() {
    std::set<int> s{5, 1, 5, 3};
    // 遍历结果：1 3 5

    std::map<std::string, int> score;
    score["Bob"] = 85;
    score["Alice"] = 90;

    for (const auto& [name, value] : score) {
        std::cout << name << ": " << value << '\n';
    }
}
```



标准库一般用平衡搜索树实现，查找、插入、删除都是 O\(log n\)。



#### **\`map\[key\]\` 会插入不存在的键**



> 调用了就会插入
> 
> 



```C++
std::map<std::string, int> score;
int x = score["Nobody"];  // 原来没有这个键，现在插入了 {"Nobody", 0}
```



只想判断键在不在，用 `find()`：



```C++
if (score.find("Nobody") != score.end()) {
    // 找到了
}
```



C\+\+20 有 `contains()`，本教程以 C\+\+17 为主，所以用 `find()`。



## **15\.\`unordered\_map\`、\`unordered\_set\`**



底层是哈希表，不维护键的顺序，平均查找、插入、删除都是 O\(1\)。



```C++
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    std::unordered_map<std::string, int> count;

    count["apple"]++;
    count["banana"]++;
    count["apple"]++;

    std::cout << count["apple"] << '\n'; // 2
}
```



两点要记住：



1. 平均 O\(1\)，不是每次都 O\(1\)。哈希冲突很多时，最坏会退化到 O\(n\)。

2. 遍历顺序没有保证，不按插入顺序，也不按字母顺序。

需要有序或范围查询，用 `map`；只是判断存不存在、统计次数，一般先用 `unordered_map`。



### **容器速查表**



|容器|底层|访问 / 查找|插入 / 删除|典型用途|
|---|---|---|---|---|
|`vector`|动态数组|下标 O\(1\)|尾部均摊 O\(1\)，中间 O\(n\)|通用顺序表|
|`array`|固定数组|下标 O\(1\)|不能改变长度|固定大小数据|
|`deque`|分段连续存储|下标 O\(1\)|两端 O\(1\)|双端操作|
|`list`|双向链表|按位置查找 O\(n\)|已知位置 O\(1\)|频繁在已知位置插删|
|`set` / `map`|平衡树|O\(log n\)|O\(log n\)|需要有序|
|`unordered_set` / `unordered_map`|哈希表|平均 O\(1\)|平均 O\(1\)|快速查找、计数|
|`stack`|容器适配器|只能访问栈顶|栈顶 O\(1\)|括号匹配、回溯|
|`queue`|容器适配器|只能访问队首|入队出队 O\(1\)|BFS、任务队列|
|`priority_queue`|堆|取堆顶 O\(1\)|O\(log n\)|优先级调度|



> 哈希表标的是平均复杂度，`vector` 尾插标的是均摊复杂度。
> 
> 



## **16\. 迭代器**



迭代器可以看作指向容器中某个位置的东西，用法和指针很像，但不一定就是指针。



```C++
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{10, 20, 30};

    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << ' ';
    }
}
```



- `begin()` 指向第一个元素。

- `end()` 指向最后一个元素的下一个位置，不能解引用。

- `*it` 取当前元素，`++it` 移到下一个。

区间用左闭右开的 `[begin, end)`，这样空区间好表示，元素个数就是 `end - begin`，相邻的两个区间也能直接接起来。



### **迭代器的能力不同**



`vector` 的迭代器支持随机访问，可以写 `v.begin() + 2`。`list` 的迭代器只能一步步移动，不能 `it + 2`，要用 `std::next(it, 2)`，内部还是逐个节点往前走。iterator



很多算法对迭代器有要求。`std::sort()` 需要随机访问迭代器，所以能排 `vector`，不能排 `list`。`list` 有自己的成员函数 `list.sort()`。



### **遍历时删除元素**



错误写法：



```C++
// for (auto it = v.begin(); it != v.end(); ++it) {
//     if (*it == 2) v.erase(it);  // erase 之后 it 失效，再 ++it 就出问题
// }
```



正确写法之一：



```C++
for (auto it = v.begin(); it != v.end(); ) {
    if (*it == 2) {
        it = v.erase(it);  // erase 返回被删元素后面的有效位置
    } else {
        ++it;
    }
}
```



要批量删除满足条件的元素，更常用的是后面讲的 `remove_if` \+ `erase`。



## **17\. STL 算法**



### **排序\`sort\(\)\`**



```C++
#include <algorithm>
#include <functional>
#include <vector>

std::vector<int> v{4, 1, 7, 3};
std::sort(v.begin(), v.end());                       // 升序
std::sort(v.begin(), v.end(), std::greater<int>{});  // 降序
```



`std::sort` 需要随机访问迭代器，复杂度 O\(n log n\)。





### **\*查找、统计、求和**



```C++
#include <algorithm>
#include <numeric>
#include <vector>

std::vector<int> v{1, 2, 2, 3, 4};

auto it = std::find(v.begin(), v.end(), 3);        // 指向 3
int twos = std::count(v.begin(), v.end(), 2);      // 2
int sum = std::accumulate(v.begin(), v.end(), 0);  // 12
```



数据可能很大时，初始值写 `0LL`。否则累加过程用的是 `int`，即使最后赋给 `long long`，中间也可能已经溢出：



```C++
long long sum = std::accumulate(v.begin(), v.end(), 0LL);
```



### **\*二分查找\`lower\_bound\(\)\`**



区间必须已经有序。



```C++
#include <algorithm>
#include <vector>

std::vector<int> v{1, 2, 2, 4, 7};

auto it = std::lower_bound(v.begin(), v.end(), 2);
// 指向第一个 >= 2 的位置，下标 1

auto jt = std::upper_bound(v.begin(), v.end(), 2);
// 指向第一个 > 2 的位置，下标 3
```



没有满足条件的位置时返回 `end()`。



对 `vector` 这类随机访问区间，二分是 O\(log n\)。`map`、`set` 要用自己的成员函数 `.lower_bound()`。通用的 `std::lower_bound` 在树的迭代器上比较次数虽然少，但移动迭代器需要线性步数，反而慢。



### **\*去重和删除：\`unique\`、\`remove\`**



```C++
#include <algorithm>
#include <vector>

std::vector<int> v{3, 1, 2, 2, 3};
//std::sort(v.begin(), v.end());
v.erase(std::unique(v.begin(), v.end()), v.end());
// v = {1, 2, 3}
```



`unique()` 只是把相邻的重复元素挪到后面，返回新的逻辑末尾，容器长度没变。所以要先排序，再用 `erase()` 真正删掉多余的部分。



删除所有偶数同理：



```C++
v.erase(std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }), v.end());
```



这种写法叫 erase\-remove 惯用法。



参考文献：[c\_and\_cpp](https://ex4z7odckj5.feishu.cn/wiki/HEwiwIsvPihbhzkLd1tcTAC3nwc)

