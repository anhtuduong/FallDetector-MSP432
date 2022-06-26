################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Lib/spi_cc3100/%.obj: ../Lib/spi_cc3100/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/opt/ti/ccs1100/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/opt/ti/ccs1100/ccs/ccs_base/arm/include" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30/Lib/board" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30/Lib/cli_uart" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30/Lib/simplelink/include" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30/Lib/simplelink/source" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30/Lib/spi_cc3100" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30/Lib/uart_cc3100" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30/Lib/driverlib/MSP432P4xx" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30/Lib/GrLib/fonts" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30/Lib/GrLib/grlib" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30/Lib/LcdDriver" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30/Lib" --include_path="/opt/ti/ccs1100/ccs/ccs_base/arm/include/CMSIS" --include_path="/home/toto/workspace_v11/AnhTu_FallDetector_Dec20_15_30" --include_path="/opt/ti/ccs1100/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Lib/spi_cc3100/$(basename $(<F)).d_raw" --obj_directory="Lib/spi_cc3100" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


