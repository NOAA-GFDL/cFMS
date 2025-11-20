# Directories
PREFIX := ${CONDA_PREFIX}
INCLUDE_DIRS := ${PREFIX}/include test_cfms/c_fms test_cfms/c_fms_utils
LIB_DIRS := ${PREFIX}/lib

# Compiler and flags
CC := mpicc
FC := mpifort
CFLAGS := -g
LDFLAGS := $(addprefix -L, $(LIB_DIRS)) -lcFMS
INCLUDES := $(addprefix -I, ${INCLUDE_DIRS})

R8_KIND_MACROS := -DCFMS_GATHER_PELIST_2D_=cFMS_gather_pelist_2d_cdouble -DCFMS_TEST_KIND_=double \
                  -DCFMS_GATHER_1D_=cFMS_gather_1d_cdouble \
                  -DCFMS_GATHER_PELIST_2D_=cFMS_gather_pelist_2d_cdouble \
                  -DCFMS_GATHER_V_1D_=cFMS_gather_v_1d_cdouble

R4_KIND_MACROS := -DCFMS_GATHER_PELIST_2D_=cFMS_gather_pelist_2d_cfloat -DCFMS_TEST_KIND_=float

# find src
ALL_SRCS := $(wildcard test_cfms/*/test*.c)
EXCLUDE_SRCS := test_cfms/c_fms_utils/%.c test_cfms/c_fms_utils/%.F90
SRCS := $(filter-out $(EXCLUDE_SRCS), $(ALL_SRCS))
OBJS := $(SRCS:.c=.o)
EXES := $(OBJS:.o=)

all: c_mpp_domains_helper.o $(OBJS) $(EXES)

# helper routines need to be compiled first
c_mpp_domains_helper.o : test_cfms/c_fms/c_mpp_domains_helper.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $(INCLUDES) $< -o $@

# pattern rules for tests
%.o: %.c
	$(CC) -c $(CFLAGS) ${R8_KIND_MACROS} $(INCLUDES) -o $@ $<

%: %.o
	$(CC) $(CFLAGS) ${R8_KIND_MACROS} $(INCLUDES) $(LDFLAGS) c_mpp_domains_helper.o -o $@ $^

clean:
	rm -f $(OBJS) $(EXES) c_mpp_domains_helper.o #test_utils_c.o test_utils.o
