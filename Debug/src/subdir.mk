################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CCon3270.cpp \
../src/CMenu.cpp \
../src/CTxtCrptDecript.cpp \
../src/syslogdDownload.cpp 

OBJS += \
./src/CCon3270.o \
./src/CMenu.o \
./src/CTxtCrptDecript.o \
./src/syslogdDownload.o 

CPP_DEPS += \
./src/CCon3270.d \
./src/CMenu.d \
./src/CTxtCrptDecript.d \
./src/syslogdDownload.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


