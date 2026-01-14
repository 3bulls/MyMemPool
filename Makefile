CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

# 1. 定义最终的目标文件名
LIB_TARGET := libmempool.a    # 静态库
TEST_TARGET := test_runner    # 测试用的可执行程序

# 2. 定义哪些文件属于库，哪些属于测试
# 要排除的文件
TEST_SRC := TestMemPool.cpp

# 获取当前目录下所有的 .cpp
ALL_SRC := $(wildcard *.cpp)

# 关键步骤：从所有文件中，剔除掉 TestMemPool.cpp，剩下的才是库的源码
LIB_SRC := $(filter-out $(TEST_SRC), $(ALL_SRC))
LIB_OBJ := $(LIB_SRC:.cpp=.o)

# 归档工具设置
AR := ar
ARFLAGS := rcs

# 3. 默认目标：同时生成静态库和测试程序
all: $(LIB_TARGET) $(TEST_TARGET)

# --- 规则 A: 生成静态库 ---
$(LIB_TARGET): $(LIB_OBJ)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Static library $@ generated."

# --- 规则 B: 生成测试程序 (依赖于静态库) ---
# 逻辑：编译 TestMemPool.cpp，并链接刚才生成的 libmempool.a
$(TEST_TARGET): $(TEST_SRC) $(LIB_TARGET)
	$(CXX) $(CXXFLAGS) $< -L. -lmempool -o $@
	@echo "Test executable $@ generated."

# --- 通用规则: 编译 .o 文件 ---
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- 清理 ---
clean:
	rm -f *.o $(LIB_TARGET) $(TEST_TARGET)

.PHONY: all clean