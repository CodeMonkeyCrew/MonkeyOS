################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
src/Drivers/GPIO-Driver.obj: ../src/Drivers/GPIO-Driver.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7A8 --code_state=32 -me --include_path="C:/Users/carom/MonkeyOS" --include_path="C:/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --define=dm3730 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/Drivers/GPIO-Driver.d" --obj_directory="src/Drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


