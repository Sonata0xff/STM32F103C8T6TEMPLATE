#include "bsp_config.h"

#ifdef ATOM_VAR_API_EN
#ifndef ATOM_VAR_H
#define ATOM_VAR_H

#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#define AtomVarType uint32_t
#define AtomVarDefaultValue 0
#define ATOM_VALUE_SET 1
#define ATOM_VALUE_RESET 0

void Atom_Write(AtomVarType* var, AtomVarType value);

AtomVarType Atom_Read(AtomVarType* var);

AtomVarType Atom_Change(AtomVarType* var, AtomVarType value);

void Atom_Add(AtomVarType* var, AtomVarType value);

void Atom_Sub(AtomVarType* var, AtomVarType value);

//This two func must be used as a pair to set the block sector.
uint32_t Atom_Block_Start();
void Atom_Block_Stop(uint32_t primask);
#endif
#endif