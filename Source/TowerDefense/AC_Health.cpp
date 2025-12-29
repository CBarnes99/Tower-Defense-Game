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

float UAC_Health::GetHealth()
{
	return health;
}

void UAC_Health::SetHealth(float healthAmount)
{
	health = healthAmount;
	maxHealth = healthAmount;
}

void UAC_Health::ResetHealth()
{
	health = maxHealth;
}

void UAC_Health::RecieveDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	FString eventInstigatorString = EventInstigator ? EventInstigator->GetName() : "No Instigator";
	health = FMath::Clamp(health - DamageAmount, 0, maxHealth);
	UE_LOG(LogTemp, Display, TEXT("%s health = %f, Damaged By = %s"), *GetOwner()->GetName(), health, *eventInstigatorString);
}

void UAC_Health::RecieveHealing(float healAmount)
{
	health = FMath::Clamp(health + healAmount, 0, maxHealth);
	UE_LOG(LogTemp, Display, TEXT("%s health = %f"), *GetOwner()->GetName(), health);
}

