#include "AC_Mana.h"

UAC_Mana::UAC_Mana()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_Mana::SetMana(float manaAmount)
{
	mana = manaAmount;
	maxMana = manaAmount;
}

float UAC_Mana::GetMana()
{
	return mana;
}

void UAC_Mana::GainMana(float gainAmount)
{
	mana = FMath::Clamp(mana + gainAmount, 0, maxMana);
	UE_LOG(LogTemp, Display, TEXT("Mana = %f"), mana);
}

bool UAC_Mana::SpendMana(float manaCostAmount)
{
	if (HasEnoughMana(manaCostAmount))
	{
		mana -= manaCostAmount;
		UE_LOG(LogTemp, Display, TEXT("Mana = %f"), mana);
		return true;
	}
	else 
	{
		return false;
	}
}

bool UAC_Mana::HasEnoughMana(float manaCostAmount)
{
	return mana - manaCostAmount >= 0 ? true : false;
}

