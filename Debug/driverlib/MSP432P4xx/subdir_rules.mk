################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
driverlib/MSP432P4xx/%.obj: ../driverlib/MSP432P4xx/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/opt/ti/ccs1100/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/opt/ti/ccs1100/ccs/ccs_base/arm/include" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_v1.0_Nov20_17_10/Hardware" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_v1.0_Nov20_17_10/Peripherals" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_v1.0_Nov20_17_10/driverlib/MSP432P4xx" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_v1.0_Nov20_17_10/GrLib/fonts" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_v1.0_Nov20_17_10/GrLib/grlib" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_v1.0_Nov20_17_10/LcdDriver" --include_path="/opt/ti/ccs1100/ccs/ccs_base/arm/include/CMSIS" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_v1.0_Nov20_17_10" --include_path="/opt/ti/ccs1100/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="driverlib/MSP432P4xx/$(basename $(<F)).d_raw" --obj_directory="driverlib/MSP432P4xx" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


