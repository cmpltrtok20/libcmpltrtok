DEBUG=1
DIR_OUTPUT_H=/usr/local/include
DIR_OUTPUT_SO=/usr/local/lib
DIR_FFCALL_H=/usr/include
DIR_FFCALL_SO=/usr/lib/x86_64-linux-gnu

###############################################################################
# CAUTION! Modify below only if you are famaliar with Make

current_dir := $(shell pwd)
SHELL := /bin/bash 

ifeq ($(DEBUG), 1) 
CFLAGS=-O0 -g
config_label='Debug'
else
CFLAGS=-Ofast
config_label='Release'
endif

common=-D_GNU_SOURCE
common+=
LDFLAGS=
LDLIBS=-lstdc++
ldflags_this=-L$(current_dir)
ldlibs_this=-lcmpltrtok
ldlibs_this_trace=-lcmpltrtok_trace
libs_spec=$(ldflags_this) $(ldlibs_this) $(LDFLAGS) $(LDLIBS)
libs_spec_trace=$(ldflags_this) $(ldlibs_this_trace) $(LDFLAGS) $(LDLIBS)

objects=log.o math.o other.o params_dict.o path.o path_cpp.o pointer.o string.o string_cpp.o time.o
traceable_objects=log.o math.trace.o other.o params_dict.o path.o path_cpp.o pointer.o string.o string_cpp.trace.o time.o
common_prerequisites=Makefile
output_h=cmpltrtok.h
output_so=libcmpltrtok.so
traceable_output_so=libcmpltrtok_trace.so

all: $(output_so)

test: all \
test_path_funcs.out \
test_path_funcs_cxx.out \
test_rand_perm.out \
test_rand_perm.trace.out \
test_string_funcs.out \
test_time.out \
test_string_cpp.out \
test_string_cpp.trace.out \
test_path_cpp.out

$(output_so):$(objects) $(output_h) $(common_prerequisites)
	gcc $(common) $(CFLAGS) -shared $(objects) -o $(output_so)

$(traceable_output_so):$(traceable_objects) $(output_h) $(common_prerequisites)
	gcc $(common) $(CFLAGS) -shared $(traceable_objects) -o $(traceable_output_so)

test_path_funcs.out: test_path_funcs.c $(output_h) $(output_so) $(common_prerequisites)
	gcc $(common) $(CFLAGS) test_path_funcs.c -o test_path_funcs.out $(libs_spec)
	
test_path_funcs_cxx.out: test_path_funcs.cpp $(output_h) $(output_so) $(common_prerequisites)
	g++ $(common) $(CFLAGS) -Wno-write-strings test_path_funcs.cpp -o test_path_funcs_cxx.out $(libs_spec)

test_rand_perm.out: test_rand_perm.c $(output_h) $(output_so) $(common_prerequisites)
	gcc $(common) $(CFLAGS) test_rand_perm.c -o test_rand_perm.out $(libs_spec)

test_rand_perm.trace.out: test_rand_perm.c $(output_h) $(traceable_output_so) $(common_prerequisites)
	gcc $(common) $(CFLAGS) test_rand_perm.c -o test_rand_perm.trace.out $(libs_spec_trace)

test_string_funcs.out: test_string_funcs.c $(output_h) $(output_so) $(common_prerequisites)
	gcc $(common) $(CFLAGS) test_string_funcs.c -o test_string_funcs.out $(libs_spec)

test_time.out: test_time.c $(output_h) $(output_so) $(common_prerequisites)
	gcc $(common) $(CFLAGS) test_time.c -o test_time.out $(libs_spec)

test_path_join_many.out: test_path_join_many.c $(output_h) $(output_so) $(common_prerequisites)
	gcc $(common) -I$(DIR_FFCALL_H) $(CFLAGS) test_path_join_many.c -o test_path_join_many.out $(libs_spec) -L$(DIR_FFCALL_SO) -lffcall

test_string_cpp.out: test_string_cpp.c $(output_h) $(output_so) $(common_prerequisites)
	gcc $(common) -I$(DIR_FFCALL_H) $(CFLAGS) test_string_cpp.c -o test_string_cpp.out $(libs_spec)

test_string_cpp.trace.out: test_string_cpp.c $(output_h) $(traceable_output_so) $(common_prerequisites)
	gcc $(common) $(CFLAGS) test_string_cpp.c -o test_string_cpp.trace.out $(libs_spec_trace)

test_path_cpp.out: test_path_cpp.c $(output_h) $(output_so) $(common_prerequisites)
	gcc $(common) -I$(DIR_FFCALL_H) $(CFLAGS) test_path_cpp.c -o test_path_cpp.out $(libs_spec)

log.o: log.c $(output_h) $(common_prerequisites)
	gcc $(common) $(CFLAGS) -fpic -c log.c -o log.o

math.o: math.c $(output_h) $(common_prerequisites)
	gcc $(common) $(CFLAGS) -fpic -c math.c -o math.o

math.trace.o: math.c $(output_h) $(common_prerequisites)
	gcc $(common) $(CFLAGS) -D TRACE -fpic -c math.c -o math.trace.o

other.o: other.c $(output_h) $(common_prerequisites)
	gcc $(common) $(CFLAGS) -fpic -c other.c -o other.o

params_dict.o: params_dict.c $(output_h) $(common_prerequisites)
	gcc $(common) $(CFLAGS) -fpic -c params_dict.c -o params_dict.o

path.o: path.c $(output_h) $(common_prerequisites)
	gcc $(common) $(CFLAGS) -Wno-format-truncation -fpic -c path.c -o path.o

path_cpp.o: path.cpp $(output_h) $(common_prerequisites)
	g++ $(common) $(CFLAGS) -fpic -c path.cpp -o path_cpp.o

pointer.o: pointer.c $(output_h) $(common_prerequisites)
	gcc $(common) $(CFLAGS) -fpic -c pointer.c -o pointer.o
	
string.o: string.c $(output_h) $(common_prerequisites)
	gcc $(common) $(CFLAGS) -fpic -c string.c -o string.o

string_cpp.o: string.cpp $(output_h) $(common_prerequisites)
	g++ $(common) $(CFLAGS) -fpic -c string.cpp -o string_cpp.o

string_cpp.trace.o: string.cpp $(output_h) $(common_prerequisites)
	g++ $(common) $(CFLAGS) -D TRACE -fpic -c string.cpp -o string_cpp.trace.o

time.o: time.c $(output_h) $(common_prerequisites)
	gcc $(common) $(CFLAGS) -fpic -c time.c -o time.o

.PHONY : check
check : 
	@echo "Is for Debug or Release:" $(config_label)
	@echo "Path to install the header:" $(DIR_OUTPUT_H)/$(output_h)
	@echo "Path to install the so:" $(DIR_OUTPUT_SO)/$(output_so)

.PHONY : install
install : $(output_so) $(output_h)
	cp -iv $(output_h) $(DIR_OUTPUT_H)
	chmod -v 0444 $(DIR_OUTPUT_H)/$(output_h)
	cp -iv $(output_so) $(DIR_OUTPUT_SO)
	chown -v root:root $(DIR_OUTPUT_SO)/$(output_so)
	chmod -v 0755 $(DIR_OUTPUT_SO)/$(output_so)

.PHONY : uninstall
uninstall :
	rm -v $(DIR_OUTPUT_H)/$(output_h)
	rm -v $(DIR_OUTPUT_SO)/$(output_so)

.PHONY : clean
clean : 
	-rm -v *.o
	-rm -v *.out
	-rm -v *.so
