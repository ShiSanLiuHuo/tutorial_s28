# Matrix 作业修复记录

原目录 `homework/Matrix/` 保持不动，本目录是修复版，可直接 `cmake -B build && cmake --build build && ./build/app`。
原版备份在 `/home/hamocat/matrix_orig_backup_20261122_233914/`。

## 一、下标约定

全文统一 **0 基**（`get(0, 0)` / `m(0, 0)` 是左上角）。作业模板 main.cpp 的注释是按 1 基写的，
`determinant()` / `inverse()` 里也照抄了 1 基公式，这是原版最致命的 bug 来源。

## 二、修掉的 bug

| # | 位置 | 问题 | 后果 |
|---|------|------|------|
| 1 | 拷贝构造函数 | 只 `new double*[rows_]`，没给每一行申请内存就写 `data_[i][j]` | 写野指针，ASan 实测 SEGV（`0xbebebebebebebebe`） |
| 2 | 析构函数 | 释放了每行，但漏了 `delete [] data_` | 每次析构泄漏行指针数组 |
| 3 | `getCol()` | `double* specificCol;` 未初始化就直接写下标 | 未定义行为；返回的数组也没人释放 |
| 4 | `multiply()` | 结果写到 `newMatrix.data_[q][i]`（行列反了）、临时矩阵下标搞混、依赖坏掉的 `getCol`、每轮 `tempMat` 的全量拷贝没释放 | 结果错 + 越界 + 泄漏，实测在拷贝构造处崩溃 |
| 5 | `transpose()` | 依赖坏掉的 `getCol`，且临时列数组泄漏 | 崩溃 / 泄漏 |
| 6 | `determinant()` | 用 1 基下标（`data_[1][i]`、`get(1,1)`、`get(1,2)`、`get(2,1)`、`get(2,2)`）；按第 2 行展开却用 `pow(-1, i)`；2×2 分支直接取 `get(2,2)` | 堆越界读（ASan 实测 heap-buffer-overflow），结果错 |
| 7 | `inverse()` | `if (determinant())` 判断反了（可逆反而报错）；算的是代数余子式矩阵，既没转置也没除以行列式；`bomb(i,j)` 与内层 1 基下标混用 | 几乎永远报错或结果错 |
| 8 | `bomb()` | `it`/`im` 互相追赶的写法正确但极难读；1×1 或越界时会 `Matrix(-1, -1)` | 传错参数时 `new` 天量内存 |
| 9 | 构造函数 | `rows`/`cols` 为负时直接 `new` | 申请天文数字内存 |
| 10 | `setData()`/移动构造 | 本身逻辑对，但依赖 2 号 bug 的析构 | 泄漏 |

另外：`print()`、`set()`、`get()`、`add()`、`isSquare()`、`setRow()`、`setCol()` 逻辑没问题，只做了小加固。

## 三、运算符重载

| 运算符 | 实现要点 |
|--------|----------|
| `+` | 直接复用 `add()`，形状检查的错误信息只保留一份 |
| `-`、一元 `-` | 逐元素相减 / 取负 |
| `*` | 复用 `multiply()`（行乘列三重循环） |
| `/` | **逐元素相除**，两侧必须同型，遇到 0 元素报错并返回空矩阵 |
| `=`（拷贝） | 先查自赋值；**先申请新内存拷贝完，再释放旧内存**，`new` 抛异常时原矩阵仍完好 |
| `=`（移动） | 直接接管指针，把 `other` 置为空矩阵 |
| `==` / `!=` | 先比尺寸再逐元素精确比较（作业数据都是可精确表示的小整数） |
| `()` | 原模板返回 `int`，会把 `double` 元素截断，这里改成 `double`（const）和 `double&`（非 const，可写）；越界只打印提示不崩溃 |
| `<<`（输入） | 需要记住写到哪了，所以在 Matrix.hpp 里加了一个私有 `cursor_`，按行优先顺序写入，写满后从头覆盖 |
| `<<`（输出） | 友元 `operator<<`，格式与 `print()` 一致 |

**模板注释里的坑**：`matrix << values[0], values[1], values[2], values[3];` 实际只会写入 `values[0]`。
逗号运算符优先级比 `<<` 低，整句被解析成 `(((matrix << values[0]), values[1]), ...)`，每遇到逗号前一段结果就丢弃。
链式输入必须写成 `matrix << values[0] << values[1] << ...`，main.cpp 里已加注释说明。

## 四、验证

- `g++ -std=c++11 -Wall -Wextra -Wpedantic`：**零警告**
- ASan + UBSan（含 LeakSanitizer）：**干净**，无越界、无泄漏、无 UB
- 40+ 条断言全过：2×2 / 3×3 / 4×4 的行列式与逆（`A * A⁻¹ = I`）、1×1、非方阵、奇异矩阵、
  非方阵转置与乘法、深拷贝、自赋值、移动构造/赋值、越界访问、`<<` 行优先写入与输出格式
- 用原版 CMakeLists 构建通过

## 五、还可以继续做的（附加题方向）

1. 异常处理：现在所有错误都是 `std::cout` 打印 + 返回空矩阵/0，改成抛 `std::invalid_argument`
   之类的异常会更像工程代码，也更容易拿附加题的 20 分。
2. 移动赋值理论上应加 `noexcept`（模板里的声明没有，所以没加）。
3. `getCol()` 返回 `new double[]`，所有权在调用者，容易忘 `delete[]`；
   更好的设计是返回 `std::vector<double>`，但那要改接口。
4. `setRows()`/`setCols()`/`setData()` 是危险的公开 setter（会直接改尺寸而不重新分配），
   只建议内部使用。
5. 行列式/逆矩阵用递归实现是 O(n!) 的，作业够用；真要快就得上高斯消元。

## 六、文件对应关系

```
Matrix_fixed/
├── CMakeLists.txt            # 与你原来的完全一致
├── include/Matrix.hpp        # 改：() 返回类型、新增 cursor_
├── src/Matrix.cpp            # 改：修 bug + 全部运算符重载
├── main.cpp                  # 改：setMatrix/printMatrix 改用重载接口，补充测试
└── original-vs-fixed.diff    # 原版到修复版的完整 diff
```
