################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ui/BilateralFilterPanel.cpp \
../ui/CannyPanel.cpp \
../ui/ConvolutionFilterPanel.cpp \
../ui/DilationPanel.cpp \
../ui/ErosionPanel.cpp \
../ui/GaussianBlurPanel.cpp \
../ui/LaplacePanel.cpp \
../ui/MedianBlurPanel.cpp \
../ui/MorphoExtPanel.cpp \
../ui/NormalBlurPanel.cpp \
../ui/ScharrPanel.cpp \
../ui/SobelPanel.cpp \
../ui/SurfPanel.cpp \
../ui/ThresholdingPanel.cpp 

OBJS += \
./ui/BilateralFilterPanel.o \
./ui/CannyPanel.o \
./ui/ConvolutionFilterPanel.o \
./ui/DilationPanel.o \
./ui/ErosionPanel.o \
./ui/GaussianBlurPanel.o \
./ui/LaplacePanel.o \
./ui/MedianBlurPanel.o \
./ui/MorphoExtPanel.o \
./ui/NormalBlurPanel.o \
./ui/ScharrPanel.o \
./ui/SobelPanel.o \
./ui/SurfPanel.o \
./ui/ThresholdingPanel.o 

CPP_DEPS += \
./ui/BilateralFilterPanel.d \
./ui/CannyPanel.d \
./ui/ConvolutionFilterPanel.d \
./ui/DilationPanel.d \
./ui/ErosionPanel.d \
./ui/GaussianBlurPanel.d \
./ui/LaplacePanel.d \
./ui/MedianBlurPanel.d \
./ui/MorphoExtPanel.d \
./ui/NormalBlurPanel.d \
./ui/ScharrPanel.d \
./ui/SobelPanel.d \
./ui/SurfPanel.d \
./ui/ThresholdingPanel.d 


# Each subdirectory must supply rules for building sources it contributes
ui/%.o: ../ui/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/usr/include/qt4 -I/usr/include/qt4/QtCore -I/usr/include/qt4/Qt -I/usr/include/qt4/QtGui -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


