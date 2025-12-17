#pragma once

#include <string>
#include <unordered_map>
const std::unordered_map<std::string, std::string> logical_commands{
    {"add", "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=M\n"
            "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=D+M\n"
            "@SP\n"
            "A=M\n"
            "M=D\n"
            "@SP\n"
            "M=M+1\n"},
    {"sub", "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=M\n"
            "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=D-M\n"
            "@SP\n"
            "A=M\n"
            "M=D\n"
            "@SP\n"
            "M=M+1\n"},
    {"neg", "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=M\n"
            "@SP\n"
            "A=M\n"
            "M=-D\n"
            "@SP\n"
            "M=M+1\n"},
    {"eq", "@0\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@is_zero\n"
           "D;JEQ\n"
           "@no_zero\n"
           "0;JMP\n"
           "(is_zero)\n"
           "@SP\n"
           "A=M\n"
           "M=-1\n"
           "@SP\n"
           "M=M+1\n"
           "@END\n"
           "0;JMP\n"
           "(no_zero)\n"
           "@SP\n"
           "A=M\n"
           "M=0\n"
           "@SP\n"
           "M=M+1\n"
           "@END\n"
           "0;JMP\n"
           "(END)\n"},
    {"gt", "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M-D\n"
           "@is_greater\n"
           "D;JGT\n"
           "@no_zero\n"
           "0;JMP\n"
           "(is_greater)\n"
           "@SP\n"
           "A=M\n"
           "M=-1\n"
           "@SP\n"
           "M=M+1\n"
           "@END\n"
           "0;JMP\n"
           "(no_zero)\n"
           "@SP\n"
           "A=M\n"
           "M=0\n"
           "@SP\n"
           "M=M+1\n"
           "@END\n"
           "0;JMP\n"
           "(END)\n"},
    {"lt", "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M-D\n"
           "@is_less\n"
           "D;JLT\n"
           "@no_zero\n"
           "0;JMP\n"
           "(is_less)\n"
           "@SP\n"
           "A=M\n"
           "M=-1\n"
           "@SP\n"
           "M=M+1\n"
           "@END\n"
           "0;JMP\n"
           "(no_zero)\n"
           "@SP\n"
           "A=M\n"
           "M=0\n"
           "@SP\n"
           "M=M+1\n"
           "@END\n"
           "0;JMP\n"
           "(END)\n"},
    {"and", "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=M\n"
            "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=D&M\n"
            "@SP\n"
            "A=M\n"
            "M=D\n"
            "@SP\n"
            "M=M+1\n"},
    {"or", "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=D|M\n"
           "@SP\n"
           "A=M\n"
           "M=D\n"
           "@SP\n"
           "M=M+1\n"},
{
    "not",
    "@SP\n"
    "M=M-1\n"
    "A=M\n"
    "D=!M\n"
    "@SP\n"
    "A=M\n"
    "M=D\n"
    "@SP\n"
    "M=M+1\n"
}

};

const std::unordered_map<std::string, std::string> memory_commands{
    // push / pop helpers
    {"push", "@SP\n"
             "M=M+1\n"},
    {"pop", "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=M\n"},
    {"addr", "@R13\n"
             "A=M\n"
             "M=D\n"},
    {"index", "D=A\n"},
    {"stack", "@SP\n"
              "A=M\n"
              "M=D\n"},

    // locals / segments
    {"local", "@LCL\n"
              "A=D+M\n"
              "D=M\n"},
    {"argument", "@ARG\n"
                 "A=D+M\n"
                 "D=M\n"},
    {"this", "@THIS\n"
             "A=D+M\n"
             "D=M\n"},
    {"that", "@THAT\n"
             "A=D+M\n"
             "D=M\n"},

    // static
    {"static", "M=D\n"},

    // constant
    {"constant", "M=D\n"},

    // temp
    {"temp", "@R5\n"
             "A=M\n"
             "M=D+M\n"},

    // pointer
    {"0_push", "@THIS\n"
               "A=M\n"
               "D=M\n"},
    {"1_push", "@THAT\n"
               "A=M\n"
               "D=M\n"},
    {"0_pop", "@THIS\n"
              "A=D\n"},
    {"1_pop", "@THAT\n"
              "A=D\n"}};
