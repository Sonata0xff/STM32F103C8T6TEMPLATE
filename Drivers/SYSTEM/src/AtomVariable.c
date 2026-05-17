#include "AtomVariable.h"

#ifdef ATOM_VAR_API_EN

void Atom_Write(AtomVarType* var, AtomVarType value)
{
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	*var = value;
	__DSB();
	__set_PRIMASK(primask);
}


AtomVarType Atom_Read(AtomVarType* var)
{
	AtomVarType value = AtomVarDefaultValue;
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	value = *var;
	__DSB();
	__set_PRIMASK(primask);
	return value;
}

AtomVarType Atom_Change(AtomVarType* var, AtomVarType value)
{
	AtomVarType tmpValue = AtomVarDefaultValue;
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	tmpValue = *var;
	*var = value;
	__DSB();
	__set_PRIMASK(primask);
	return tmpValue;
}

void Atom_Add(AtomVarType* var, AtomVarType value)
{
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	(*var) += value;
	__DSB();
	__set_PRIMASK(primask);
}

void Atom_Sub(AtomVarType* var, AtomVarType value)
{
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	(*var) -= value;
	__DSB();
	__set_PRIMASK(primask);
}

uint32_t Atom_Block_Start()
{
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	__DSB();
	return primask;
}

void Atom_Block_Stop(uint32_t primask)
{
	__set_PRIMASK(primask);
}
#endif