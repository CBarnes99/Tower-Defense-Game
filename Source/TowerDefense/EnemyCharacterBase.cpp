// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterBase.h"

// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	name = enemyInfo->name;
	health = enemyInfo->health;
	movementSpeed = enemyInfo->movementSpeed;
	damageDelt = enemyInfo->damageDelt;

	GetCharacterMovement()->MaxWalkSpeed = movementSpeed;

}

// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UBehaviorTree* AEnemyCharacterBase::GetBehaviourTree() const
{
	return enemyBehaviorTree;
}
// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	/*
	UE_LOG(LogTemp, Warning, TEXT("Damage Amount = %f"), DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("EventInstigator = %s"), *EventInstigator->GetName())
	UE_LOG(LogTemp, Warning, TEXT("DamageCauser = %s"), *DamageCauser->GetName());
	*/

	health -= DamageAmount;	
	if (health <= 0)
	{
		OnDeath();
	}
	return DamageAmount;
}

void AEnemyCharacterBase::OnDeath()
{
	/*
	play death animation (ragdoll?)
	tell spawner manager that it has died
	spawner manager - 1 to enemy count
	if 0 end round and can start the next	
	*/


	Destroy();
}
