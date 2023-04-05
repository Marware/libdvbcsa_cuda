TARGET = dvbcsa_cuda
TEMPLATE = lib
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt
# Compute capability of my GPU
QMAKE_CFLAGS_RELEASE += -fPIC
QMAKE_LFLAGS_RELEASE += -O3 -fPIC #-ggdb3
QMAKE_EXTRA_COMPILERS += cuda

CUDA_SOURCES += \
        src/dvbcsa_kernel.cu

SOURCES += \
    src/dvbcsa_cuda.c

DFLA =  -O3 #-g -G  --generate-line-info
CUDA_DIR = /usr/local/cuda
INCLUDEPATH  += $$CUDA_DIR/include
QMAKE_LIBDIR += $$CUDA_DIR/lib64
LIBS += -lcudart -lcuda

CUDA_ARCH = -gencode arch=compute_60,code=sm_60 -gencode arch=compute_61,code=sm_61 -gencode arch=compute_70,code=sm_70 -gencode arch=compute_75,code=sm_75 \
-gencode arch=compute_80,code=sm_80 -gencode arch=compute_86,code=sm_86 -gencode arch=compute_86,code=compute_86

NVCCFLAGS = --compiler-options '-fPIC' -use_fast_math -O3

CUDA_INC = $$join(INCLUDEPATH,' -I','-I',' ')
cuda.commands = $$CUDA_DIR/bin/nvcc  $$DFLA -maxrregcount=128 $$CUDA_ARCH -c $$NVCCFLAGS \
            $$CUDA_INC $$LIBS  ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT} \
            2>&1 | sed -r \"s/\\(([0-9]+)\\)/:\\1/g\" 1>&2
cuda.dependency_type = TYPE_C
cuda.depend_command = $$CUDA_DIR/bin/nvcc $$DFLA -maxrregcount=128 --generate-line-info -M $$CUDA_INC $$NVCCFLAGS ${QMAKE_FILE_NAME}| sed \"s/^.*: //\"
cuda.input = CUDA_SOURCES
cuda.output = ${OBJECTS_DIR}${QMAKE_FILE_BASE}.o

unix:!macx: LIBS += -L$$CUDA_DIR/targets/x86_64-linux/lib/ -lcuda -lcudart -lcurand -fPIC

INCLUDEPATH += $$CUDA_DIR/targets/x86_64-linux/include
DEPENDPATH  += $$CUDA_DIR/targets/x86_64-linux/include

HEADERS += \
    src/dvbcsa_pv.h

DISTFILES += \
    src/dvbcsa_kernel.cu
