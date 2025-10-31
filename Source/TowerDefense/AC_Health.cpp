// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Health.h"

// Sets default values for this component's properties
UAC_Health::UAC_Health()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_Health::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAC_Health::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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

void UAC_Health::RecieveDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	health = FMath::Clamp(health - DamageAmount, 0, maxHealth);
	UE_LOG(LogTemp, Display, TEXT("%s health = %f"), *GetOwner()->GetName(), health);
}

void UAC_Health::RecieveHealing(float healAmount)
{
	health = FMath::Clamp(health + healAmount, 0, maxHealth);
	UE_LOG(LogTemp, Display, TEXT("%s health = %f"), *GetOwner()->GetName(), health);
}

