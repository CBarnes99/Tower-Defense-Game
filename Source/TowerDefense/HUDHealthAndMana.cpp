#include "HUDHealthAndMana.h"
#include "Components/ProgressBar.h"

void UHUDHealthAndMana::NativeConstruct()
{
	HealthBar->SetPercent(1);
	ManaBar->SetPercent(1);
}

void UHUDHealthAndMana::UpdateHealthBar(float currentHealth, float maxHealth)
{
	HealthBar->SetPercent(currentHealth / maxHealth);
	//UE_LOG(LogTemp, Display, TEXT("UpdateHealthBar: Current Health - %f, Max Health - %f, Percent - %f"), currentHealth, maxHealth, currentHealth / maxHealth);
}

void UHUDHealthAndMana::UpdateManaBar(float currentMana, float maxMana)
{
	ManaBar->SetPercent(currentMana / maxMana);
	//UE_LOG(LogTemp, Display, TEXT("UpdateManaBar: Current Mana - %f, Max Mana - %f, Percent - %f"), currentMana, maxMana, currentMana / maxMana);
}