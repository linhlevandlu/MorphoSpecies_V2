################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../impls_2015/Lines.cpp \
../impls_2015/YellowGrid.cpp 

OBJS += \
./impls_2015/Lines.o \
./impls_2015/YellowGrid.o 

CPP_DEPS += \
./impls_2015/Lines.d \
./impls_2015/YellowGrid.d 


# Each subdirectory must supply rules for building sources it contributes
impls_2015/%.o: ../impls_2015/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/usr/include/qt4 -I/usr/include/qt4/QtCore -I/usr/include/qt4/Qt -I/usr/include/qt4/QtGui -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


