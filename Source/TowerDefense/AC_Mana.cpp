// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Mana.h"

// Sets default values for this component's properties
UAC_Mana::UAC_Mana()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
}

bool UAC_Mana::SpendMana(float manaCostAmount)
{
	if (HasEnoughMana(manaCostAmount))
	{
		mana -= manaCostAmount;
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

