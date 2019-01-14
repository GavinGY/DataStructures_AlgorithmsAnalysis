
# *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*
#  > Author  ： Gavin | Zhang GuiYang
#  > Mail    ： gavin.gy.zhang@gmail.com
#  > Date    ： Dec/25/2018
#  > Company ： Foxconn·CNSBG·CPEGBBD·RD
#  > Funciton:  Makefile
#  > Version :  v1.0 
#  > HowToUse:  make 
#               make clean
# *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*


# 编译目标文件名
Target = DataStructures_AlgorithmsAnalysis.elf

# 使用编译器
CC = gcc
CFLAGS = # -Wall -O
CMODES = c99

# 项目工程文件夹定义
BIN = bin
OBJ = objs
SRC = src
INC = include
EXE = employ

# 根据条件自动执行
# inc_h = $(shell find ./ -name "*.h")
src_c = $(shell find ./ -name "*.c")
obj_o = $(patsubst  %.c, $(OBJ)/%.o, $(notdir $(src_c)))
Target := $(addprefix $(BIN)/,$(Target))
 
# 编译目标
$(Target): $(obj_o)
	$(CC) -o $(Target) $(obj_o)
	cp -a $(Target) $(EXE)
	
$(OBJ)/%.o: $(SRC)/%.c 
# $(inc_h)	
	$(CC) -o $@ $(CFLAGS) -std=$(CMODES) -c $< -l $(INC)

clean:
	rm -rf $(obj_o) $(Target) $(EXE)/$(notdir $(Target)) $(EXE)/*.bin $(EXE)/*.big $(EXE)/*.oob
