################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Lib/simplelink/source/%.obj: ../Lib/simplelink/source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/opt/ti/ccs1100/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="/opt/ti/ccs1100/ccs/ccs_base/arm/include" --include_path="/opt/ti/ccs1100/ccs/ccs_base/arm/include/CMSIS" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30" --include_path="/opt/ti/ccs1100/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Lib/simplelink/source/$(basename $(<F)).d_raw" --obj_directory="Lib/simplelink/source" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


