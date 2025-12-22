#include "Core_GameState.h"

void ACore_GameState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("%s has initilized!"), *this->GetName());


	if (playerCurrencyAmount == NULL)
	{
		playerCurrencyAmount = 100;
		UE_LOG(LogTemp, Error, TEXT("Player Currency was not set! Set to 100 by Default!"))
	}

	if (maxListSizeInWeaponTurretHud == NULL)
	{
		maxListSizeInWeaponTurretHud = 5;
		UE_LOG(LogTemp, Error, TEXT("amountInWeaponTurretHud was not set! Set to 5 by Default!"))
	}
	
	currentListSizeInWeaponTurretHud = 0;
}

float ACore_GameState::GetPlayerCurrencyAmount()
{
	return playerCurrencyAmount;
}

void ACore_GameState::SetPlayerCurrencyAmount(bool addCurrency, float amount)
{
	if (addCurrency)
	{
		playerCurrencyAmount = playerCurrencyAmount + amount;
	}
	else
	{
		playerCurrencyAmount = playerCurrencyAmount - amount;
	}
}

int ACore_GameState::GetMaxListSizeInWeaponTurretHud()
{
	return maxListSizeInWeaponTurretHud;
}

void ACore_GameState::SetMaxListSizeInWeaponTurretHud(int newMax)
{
	maxListSizeInWeaponTurretHud = newMax;
}

int ACore_GameState::GetCurrentListSizeInWeaponTurretHud()
{
	return currentListSizeInWeaponTurretHud;
}

void ACore_GameState::IncreaseCurrentListSizeInWeaponTurretHud(bool addSize)
{
	if (addSize)
	{
		currentListSizeInWeaponTurretHud++;

		if (currentListSizeInWeaponTurretHud > maxListSizeInWeaponTurretHud)
		{
			UE_LOG(LogTemp, Error, TEXT("Current List Size In Weapon Turret Hud Has Exceeded Max Size!!!"))
		}
	}
	else
	{
		currentListSizeInWeaponTurretHud--;

		if (currentListSizeInWeaponTurretHud < 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Current List Size In Weapon Turret Hud Is Below 0!!!"))
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Current List Size In Weapon Turret Hud = %d"), currentListSizeInWeaponTurretHud);
}

bool ACore_GameState::IsCurrentListSizeLessThanMax()
{
	return currentListSizeInWeaponTurretHud < maxListSizeInWeaponTurretHud;
}

void ACore_GameState::SetCurrentTurretClass(TSubclassOf<ATurretStatic> turretClass)
{
	currentTurretClass = turretClass;
}

TSubclassOf<ATurretStatic> ACore_GameState::GetCurrentTurretClass()
{
	return currentTurretClass;
}
