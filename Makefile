def = PRINT 
# 宏定义条件编译 PRINT NULL
target = DataStructures_AlgorithmsAnalysis.ef

cc = gcc
inc = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.c")
obj = $(src:%.c=%.o)
 
$(target): $(obj)
	$(cc) -o $(target) $(obj)
	
%.o: %.c $(inc)
	$(cc) -o $@ -c -std=c99 -D $(def) $<

clean:
	rm -rf $(obj) $(target)
