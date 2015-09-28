################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../impls/algorithms/MyImpl.cpp \
../impls/algorithms/ant.cpp \
../impls/algorithms/position.cpp 

OBJS += \
./impls/algorithms/MyImpl.o \
./impls/algorithms/ant.o \
./impls/algorithms/position.o 

CPP_DEPS += \
./impls/algorithms/MyImpl.d \
./impls/algorithms/ant.d \
./impls/algorithms/position.d 


# Each subdirectory must supply rules for building sources it contributes
impls/algorithms/%.o: ../impls/algorithms/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/usr/include/qt4 -I/usr/include/qt4/QtCore -I/usr/include/qt4/Qt -I/usr/include/qt4/QtGui -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


