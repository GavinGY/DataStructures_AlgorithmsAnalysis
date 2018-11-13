cc = gcc
target = DataStructures_AlgorithmsAnalysis.e

inc = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.c")
obj = $(src:%.c=%.o)
 
$(target): $(obj)
	$(cc) -o $(target) $(obj)
	
%.o: %.c $(inc)
	$(cc) -o $@ -c -std=c99  $<

clean:
	rm -rf $(obj) $(target)
