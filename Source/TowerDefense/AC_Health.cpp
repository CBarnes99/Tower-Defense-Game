#include "AC_Health.h"

UAC_Health::UAC_Health()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
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
	currentHealth = healthAmount;
	maxHealth = healthAmount;
}

void UAC_Health::ResetHealth()
{
	currentHealth = maxHealth;
}

void UAC_Health::RecieveDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	FString eventInstigatorString = EventInstigator ? EventInstigator->GetName() : "No Instigator";
	currentHealth = FMath::Clamp(currentHealth - DamageAmount, 0, maxHealth);
	UE_LOG(LogTemp, Display, TEXT("%s currentHealth = %f, Damaged By = %s"), *GetOwner()->GetName(), currentHealth, *eventInstigatorString);
}

void UAC_Health::RecieveHealing(float healAmount)
{
	currentHealth = FMath::Clamp(currentHealth + healAmount, 0, maxHealth);
	UE_LOG(LogTemp, Display, TEXT("%s currentHealth = %f"), *GetOwner()->GetName(), currentHealth);
}