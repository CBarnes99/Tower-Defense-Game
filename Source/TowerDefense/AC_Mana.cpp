#include "AC_Mana.h"

UAC_Mana::UAC_Mana()
{
	PrimaryComponentTick.bCanEverTick = false;

	rechargeFrequency = 3;
}

void UAC_Mana::SetMana(float manaAmount)
{
	if (manaAmount <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("SetMana: MANA AMOUNT IS 0 WITHIN - %s"), *this->GetName());
	}

	currentMana = manaAmount;
	maxMana = manaAmount;
}

void UAC_Mana::SetRechargeRate(float rechargeRate)
{
	if (rechargeRate <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("SetRechargeRate: RECHARGE RATE IS 0 WITHIN - %s"), *this->GetName());
	}
	manaRechargeRate = rechargeRate;
}

float UAC_Mana::GetMana()
{
	return currentMana;
}

float UAC_Mana::GetMaxMana()
{
	return maxMana;
}

void UAC_Mana::GainMana(float gainAmount)
{
	currentMana = FMath::Clamp(currentMana + gainAmount, 0, maxMana);
	UE_LOG(LogTemp, Display, TEXT("GainMana: Mana = %f"), currentMana);
	ManaUpdatedEvent.ExecuteIfBound(currentMana, maxMana);

	float realTimeSeconds = GetWorld()->RealTimeSeconds;
	UE_LOG(LogTemp, Display, TEXT("GainMana: Time = %f"), realTimeSeconds);

	
	if (currentMana == maxMana)
	{
		RechargeManaTimer(false);
	}
}

bool UAC_Mana::SpendMana(float manaCostAmount)
{
	if (HasEnoughMana(manaCostAmount))
	{
		currentMana -= manaCostAmount;
		UE_LOG(LogTemp, Display, TEXT("SpendMana: Mana = %f"), currentMana);
		ManaUpdatedEvent.ExecuteIfBound(currentMana, maxMana);
		RechargeManaTimer(true);
		return true;
	}
	else 
	{
		return false;
	}
}

bool UAC_Mana::HasEnoughMana(float manaCostAmount)
{
	return currentMana - manaCostAmount >= 0 ? true : false;
}

void UAC_Mana::RechargeManaTimer(bool bStartTimer)
{
	if (bStartTimer && !GetWorld()->GetTimerManager().IsTimerActive(RechargeTimerHandle) && manaRechargeRate > 0)
	{
		FTimerDelegate RechargeTimerDelegate;
		RechargeTimerDelegate.BindUObject(this, &UAC_Mana::GainMana, (60 / manaRechargeRate) / rechargeFrequency);
		GetWorld()->GetTimerManager().SetTimer(RechargeTimerHandle, RechargeTimerDelegate, (1 / rechargeFrequency), true);
		UE_LOG(LogTemp, Display, TEXT("RechargeManaTimer: Start Timer Called"));

	}
	else if (!bStartTimer && GetWorld()->GetTimerManager().IsTimerActive(RechargeTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(RechargeTimerHandle);
		UE_LOG(LogTemp, Display, TEXT("SpendMana: Clear Timer Called"));

	}
	else if (manaRechargeRate < 0 || manaRechargeRate == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("SetRechargeRate: RECHARGE RATE IS 0 WITHIN - %s"), *this->GetName());
	}
}

