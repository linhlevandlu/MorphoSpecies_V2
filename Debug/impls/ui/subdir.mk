################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../impls/ui/OpenCVThresholdOpt.cpp \
../impls/ui/antmethodpanel.cpp \
../impls/ui/binaryparapanel.cpp \
../impls/ui/growingpanel.cpp \
../impls/ui/hitmisspanel.cpp \
../impls/ui/otsupanel.cpp 

OBJS += \
./impls/ui/OpenCVThresholdOpt.o \
./impls/ui/antmethodpanel.o \
./impls/ui/binaryparapanel.o \
./impls/ui/growingpanel.o \
./impls/ui/hitmisspanel.o \
./impls/ui/otsupanel.o 

CPP_DEPS += \
./impls/ui/OpenCVThresholdOpt.d \
./impls/ui/antmethodpanel.d \
./impls/ui/binaryparapanel.d \
./impls/ui/growingpanel.d \
./impls/ui/hitmisspanel.d \
./impls/ui/otsupanel.d 


# Each subdirectory must supply rules for building sources it contributes
impls/ui/%.o: ../impls/ui/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/usr/include/qt4 -I/usr/include/qt4/QtCore -I/usr/include/qt4/Qt -I/usr/include/qt4/QtGui -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


