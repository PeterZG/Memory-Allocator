
# 内存分配器：简单的子分配器

该项目是一个自定义的内存分配器，展示了基本的内存管理概念。该分配器支持内存分配、释放和调试，允许用户定义堆大小以进行测试和实验。

## 特性

1. **堆初始化**：
   - 使用`init_heap`初始化一个用户定义大小的堆。
   - 在固定大小的环境中模拟动态内存管理。

2. **内存管理**：
   - `my_malloc(size)`：从堆中分配内存块。
   - `my_free(ptr)`：释放已分配的内存，并合并相邻的空闲块。
   - 自动更新并维护一个空闲列表，以提高分配效率。

3. **调试工具**：
   - `dump_heap`：打印堆的当前状态（例如，已分配和空闲的块）。
   - `heap_offset`：计算堆中指针的偏移量，便于调试。

4. **命令模拟**：
   - 通过`test3`的交互命令进行动态内存操作（分配、释放），并反馈结果。

## 文件

### 核心文件
- **`allocator.c`**：内存分配器函数的实现。
- **`allocator.h`**：分配器的接口头文件。

### 测试文件
- **`test1.c`**：堆初始化和内存分配的基础测试。
- **`test2.c`**：包括多次分配和释放的高级测试。
- **`test3.c`**：通过命令动态分配和释放内存的交互式测试程序。
- **`test3_example_data.txt`**：`test3`的示例命令文件，模拟分配和释放操作。

### 构建文件
- **`Makefile`**：自动化编译所有测试程序和内存分配器。

## 使用方法

### 编译
使用提供的`Makefile`来编译分配器和测试程序：
```bash
make
```

### 运行测试
1. **基础测试  (`test1`)**:
   ```bash
   ./test1
   ```
   - 验证堆初始化和简单的内存分配。

2. **高级测试 (`test2`)**:
   ```bash
   ./test2
   ```
   - 测试包含多次分配和释放的复杂场景。

3. **交互式测试 (`test3`)**:
   ```bash
   ./test3 <heap_size> < test3_example_data.txt
   ```
   - 使用`test3_example_data.txt`中的命令模拟动态内存操作。

### 示例输出
运行 `test3` 并使用提供的示例数据文件：
```bash
./test3 4096 < test3_example_data.txt
```

生成类似如下的输出：
```plaintext
a = allocate(100);
b = allocate(200);
c = allocate(300);
free b;
d = allocate(200);
free c;
free a;
```

每次操作后，将使用 `dump_heap` 显示堆的状态。

## 注意事项

- **堆对齐**: 分配内存时对齐到4字节，以确保与系统要求兼容。
- **错误处理**: 包括对无效操作的检查（例如，双重释放、无效指针）。
- **自定义**: 修改test3_example_data.txt文件，模拟不同的内存操作场景。

## 鸣谢

该项目作为UNSW COMP1521课程的作业一部分进行开发，旨在提供自定义内存分配器的实现经验。感谢课程提供的知识框架和实践机会，帮助我们更深入地理解内存管理的基础概念。

