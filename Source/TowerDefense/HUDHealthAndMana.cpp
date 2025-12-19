#include "HUDHealthAndMana.h"
#include "Components/ProgressBar.h"

void UHUDHealthAndMana::UpdateHealthBar(float currentHealth, float maxHealth)
{
	HealthBar->SetPercent(maxHealth / currentHealth);
}

void UHUDHealthAndMana::UpdateManaBar(float currentMana, float maxMana)
{
	ManaBar->SetPercent(maxMana / currentMana);
}