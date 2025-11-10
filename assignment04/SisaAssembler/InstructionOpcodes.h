#pragma once

//Data Directives
#define DSS "DSS"
#define DWORD "DWORD"

//Assembly mnemonics 
#define MOVI "MOVI"
#define ADD  "ADD"
#define SUB  "SUB"
#define CMP  "CMP"
#define MUL  "MUL"
#define DIV  "DIV"
#define LDR  "LDR"
#define STR  "STR"
#define BE   "BE"
#define BNE  "BNE"
#define BL   "BL"
#define BG   "BG"
#define BLE  "BLE"
#define BGE  "BGE"
#define B    "B"
#define HALT "HALT"

//Machine Opcodes
#define OPCODE_MOVI 0x0
#define OPCODE_ADD  0x1
#define OPCODE_SUB  0x2
#define OPCODE_CMP  0x3
#define OPCODE_MUL  0x4
#define OPCODE_DIV  0x5
#define OPCODE_LDR  0x6
#define OPCODE_STR  0x7
#define OPCODE_BE   0x8
#define OPCODE_BNE  0x9
#define OPCODE_BL   0xA
#define OPCODE_BG   0xB
#define OPCODE_BLE  0xC
#define OPCODE_BGE  0xD
#define OPCODE_B    0xE
#define OPCODE_HALT 0xF
