/*
** LHades
** Copyright (C) 2017 tacigar
*/

#ifndef LHADES_CONFIG_HPP
#define LHADES_CONFIG_HPP

#define LHADES_SIGNATURE "\x1bLua"
#define LHADES_DATA      "\x19\x93\r\n\x1a\n"
#define LHADES_INT       0x5678
#define LHADES_NUM       370.5

#if LUAI_BITSINT >= 32
    typedef unsigned int Instruction;
#else
    typedef unsigned long Instruction;
#endif

#endif // LHADES_CONFIG_HPP
