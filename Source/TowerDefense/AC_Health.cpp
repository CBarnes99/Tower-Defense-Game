#include "AC_Health.h"

UAC_Health::UAC_Health()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_Health::BeginPlay()
{
	Super::BeginPlay();
}

float UAC_Health::GetCurrentHealth()
{
	return currentHealth;
}

float UAC_Health::GetMaxHealth()
{
	return maxHealth;
}

void UAC_Health::SetHealth(float healthAmount)
{
	if (healthAmount <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("SetHealth: HEALTH AMOUNT IS 0 WITHIN - %s"), *this->GetName());
	}
	currentHealth = healthAmount;
	maxHealth = healthAmount;
}

void UAC_Health::ResetHealth()
{
	currentHealth = maxHealth;
	HealthUpdatedEvent.ExecuteIfBound(currentHealth, maxHealth);
}

void UAC_Health::RecieveDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	FString eventInstigatorString = EventInstigator ? EventInstigator->GetName() : "No Instigator";
	currentHealth = FMath::Clamp(currentHealth - DamageAmount, 0, maxHealth);
	HealthUpdatedEvent.ExecuteIfBound(currentHealth, maxHealth);
	//UE_LOG(LogTemp, Display, TEXT("RecieveDamage: DamageAmount - %f, New Health amount = %f, Damaged By = %s"), DamageAmount, currentHealth, *eventInstigatorString);
}

void UAC_Health::RecieveHealing(float healAmount)
{
	currentHealth = FMath::Clamp(currentHealth + healAmount, 0, maxHealth);
	HealthUpdatedEvent.ExecuteIfBound(currentHealth, maxHealth);
	//UE_LOG(LogTemp, Display, TEXT("RecieveHealing: Healthing Amount - %f, New health amount - %f"), healAmount, currentHealth);
}