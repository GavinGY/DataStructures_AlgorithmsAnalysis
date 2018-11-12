cc = gcc
target = DataStructures_AlgorithmsAnalysis.ef

inc = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.c")
obj = $(src:%.c=%.o) 

$(target): $(obj)
	$(cc) -o $(target) $(obj)

%.o: $(src) $(inc)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(target)
  

