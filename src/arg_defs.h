#pragma once
#ifdef __cplusplus
extern "C" {
#endif
enum REG
{
	REG_F,REG_A, 
	REG_C,REG_B, 
	REG_E,REG_D, 
	REG_L,REG_H, 
	NUM_REGISTERS
    /*
	REG16_AF = 0,
	REG16_BC,
	REG16_DE,
	REG16_HL,
	REG16_SP,
	NUM_REGISTERS16
    */
};

enum REG16
{
	REG16_AF,
	REG16_BC,
	REG16_DE,
	REG16_HL,
	REG16_SP,
	NUM_REGISTERS16
};

enum ARG_TYPE
{
	ARG_TYPE_REG8,
	ARG_TYPE_REG8_INDIRECT,
	ARG_TYPE_REG16,
	ARG_TYPE_REG16_INDIRECT,
	ARG_TYPE_DATA8,
	ARG_TYPE_DATA8_UNSIGNED,
	ARG_TYPE_DATA8_UNSIGNED_INDIRECT,
	ARG_TYPE_DATA16,
	ARG_TYPE_DATA16_UNSIGNED,
	ARG_TYPE_DATA16_UNSIGNED_INDIRECT,
	ARG_TYPE_REL8,
	ARG_TYPE_REL8_ADD_SP,
	ARG_TYPE_HL_INDIRECT_DEC,
	ARG_TYPE_HL_INDIRECT_INC,
	ARG_TYPE_00H,
	ARG_TYPE_08H,
	ARG_TYPE_10H,
	ARG_TYPE_18H,
	ARG_TYPE_20H,
	ARG_TYPE_28H,
	ARG_TYPE_30H,
	ARG_TYPE_38H,
	ARG_TYPE_0,
	ARG_TYPE_1,
	ARG_TYPE_2,
	ARG_TYPE_3,
	ARG_TYPE_4,
	ARG_TYPE_5,
	ARG_TYPE_6,
	ARG_TYPE_7,
	ARG_TYPE_Z,
	ARG_TYPE_NC,
	ARG_TYPE_NZ,
	ARG_TYPE_NONE
};

extern const char *arg_type_strings[];

typedef union REG_INPUT
{
	enum REG   r8;
	enum REG16 r16;
}REG_INPUT;

#ifndef __cplusplus
static const REG_INPUT A_REG  = {.r8 = REG_A};
#endif

#ifdef __cplusplus
} // extern "C"
#endif