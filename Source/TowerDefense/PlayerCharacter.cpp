#pragma once

#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "DA_PlayerCharacterStats.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AC_Health.h"
#include "AC_Mana.h"
#include "WeaponBase.h"
#include "Core_GameState.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<AActor> weaponBP(TEXT("/Game/Weapons/BP_WeaponBase"));
	if (weaponBP.Class)
	{
		weaponClass = weaponBP.Class;
		UE_LOG(LogTemp, Display, TEXT("APlayerCharacter: Weapon BP found in player character"));

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter: Weapon BP not found in player character"));
	}
	weaponSocket = "weapon_righthand";

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f ));
	

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	springArm->SetupAttachment(RootComponent.Get());
	springArm->TargetArmLength = 250.0f;
	springArm->bUsePawnControlRotation = true;
	springArm->SetRelativeLocation(FVector(0.0f, 90.0f, 90.0f));

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	camera->SetupAttachment(this->springArm, USpringArmComponent::SocketName);
	camera->bUsePawnControlRotation = false;

	healthComponent = CreateDefaultSubobject<UAC_Health>(TEXT("Health Component"));
	manaComponent = CreateDefaultSubobject<UAC_Mana>(TEXT("Mana Component"));

	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Tag for the enemy ai perception to differentiate whos the player and who isn't
	Tags.Add(FName("Player"));

	bIsDefeated = false;
	bHasPlayerLost = false;

	AGameStateBase* gameState = UGameplayStatics::GetGameState(GetWorld());
	ACore_GameState* coreGameState = Cast<ACore_GameState>(gameState);
	if (!coreGameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("BindDelegates: CORE GAME STATE NOT CASTED CORRECTLY WITHIN - %s"), *this->GetName());
		return;
	}
	coreGameState->OnPlayerLostEvent.AddUObject(this, &APlayerCharacter::SetPlayerLost);


	respawnPoint = GetActorLocation();
	respawnRotation = GetActorRotation();

	//Setting defualts within the components
	healthComponent->SetHealth(DA_playerInfo->health);
	manaComponent->SetMana(DA_playerInfo->resource);
	manaComponent->SetRechargeRate(DA_playerInfo->resourceChargeTime);

	//Binding delegates for health and mana updates
	healthComponent->HealthUpdatedEvent.BindUObject(this, &APlayerCharacter::ReceiveHealingDelegate);
	manaComponent->ManaUpdatedEvent.BindUObject(this, &APlayerCharacter::ReceiveManaDelegate);

	GetCharacterMovement()->MaxWalkSpeed = DA_playerInfo->movementSpeed;
	GetCharacterMovement()->JumpZVelocity = DA_playerInfo->jumpHeight;
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay: Name: %s, Health: %f, Mana: %f, Movement Speed: %f, Run Speed: %f, Jump Height: %f"),
		*DA_playerInfo->name, healthComponent->GetCurrentHealth(), manaComponent->GetMana(), DA_playerInfo->movementSpeed, DA_playerInfo->runSpeed, DA_playerInfo->jumpHeight);

	EquipWeapon();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//Equips the weapon that is in the weaponClass var to the player
void APlayerCharacter::EquipWeapon()
{
	if (!weaponClass)
	{
		UE_LOG(LogTemp, Error, TEXT("EquipWeapon: WEAPON CLASS NOT SET"));
		return;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = this;

	equippedWeapon = GetWorld()->SpawnActor<AWeaponBase>(weaponClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);

	if (equippedWeapon)
	{
		equippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, weaponSocket);
		equippedWeapon->SetOwner(this);
		UE_LOG(LogTemp, Display, TEXT("EquipWeapon: Weapon Equipped Successfully!!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EquipWeapon: Weapon Equipped FAILED!!"));
	}
}

void APlayerCharacter::AttackAction()
{
	
	if (!equippedWeapon->CanWeaponFire())
		return;

	float resourceCost = manaComponent->GetMaxMana() / equippedWeapon->GetResourceCost();

	if (!manaComponent->HasEnoughMana(resourceCost))
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackAction: Dont have enough resource. Current resource - %f, Resource Cost - %f"), manaComponent->GetMana(), resourceCost);
		return;
	}
	
	manaComponent->SpendMana(resourceCost);
	OnManaUpdatedEvent.Broadcast(manaComponent->GetMana(), manaComponent->GetMaxMana());
	equippedWeapon->FireProjectile(GetCameraLocation(), GetCameraForwardVector());
}

FRotator APlayerCharacter::GetCameraRotation() const 
{
	return camera->GetComponentRotation();
}

FVector APlayerCharacter::GetCameraForwardVector() const
{
	//UE_LOG(LogTemp, Warning, TEXT("GetCameraForwardVector: Camera Location from PlayerCharacter = %s"), *camera->GetComponentLocation().ToString());

	return camera->GetForwardVector();
}

FVector APlayerCharacter::GetCameraLocation()
{
	return camera->GetComponentLocation();
}

float& APlayerCharacter::GetMovementSpeed()
{
	return DA_playerInfo->movementSpeed;
}

float& APlayerCharacter::GetRunSpeed()
{
	return DA_playerInfo->runSpeed;
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	healthComponent->RecieveDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnHealthUpdatedEvent.Broadcast(healthComponent->GetCurrentHealth(), healthComponent->GetMaxHealth());

	if (healthComponent->GetCurrentHealth() <= 0)
	{
		OnPlayerDefeated();
	}
	return DamageAmount;
}

void APlayerCharacter::OnPlayerDefeated()
{
	if (bHasPlayerLost)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnPlayerDefeated: Player has lost, break function!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("OnPlayerDefeated: Player Health is less than 0! Player Dead!"));
	bIsDefeated = true;
	OnPlayerDeathStateBlueprintEvent.Broadcast(bIsDefeated);
	OnPlayerDeathStateEvent.Broadcast(bIsDefeated, DA_playerInfo->respawnTime);

	SetActorEnableCollision(false);

	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &APlayerCharacter::RespawnPlayer, DA_playerInfo->respawnTime, false);
}

void APlayerCharacter::RespawnPlayer()
{
	if (bHasPlayerLost)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnPlayerDefeated: Player has lost, break function!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("RespawnPlayer: Player Respawned!"));
	bIsDefeated = false;
	SetActorEnableCollision(true);
	SetActorLocation(respawnPoint);
	SetActorRotation(respawnRotation);

	healthComponent->ResetHealth();

	OnPlayerDeathStateBlueprintEvent.Broadcast(bIsDefeated);
	OnPlayerDeathStateEvent.Broadcast(bIsDefeated, DA_playerInfo->respawnTime);

}

void APlayerCharacter::ReceiveHealing(float healAmount)
{
	healthComponent->RecieveHealing(healAmount);
}

void APlayerCharacter::ReceiveHealingDelegate(float currentHealth, float maxHealth)
{
	OnHealthUpdatedEvent.Broadcast(healthComponent->GetCurrentHealth(), healthComponent->GetMaxHealth());
}

void APlayerCharacter::ReceiveMana(float manaAmount)
{
	manaComponent->GainMana(manaAmount);
}

void APlayerCharacter::ReceiveManaDelegate(float currentMana, float maxMana)
{
	OnManaUpdatedEvent.Broadcast(currentMana, maxMana);
}

void APlayerCharacter::SetPlayerLost()
{
	bHasPlayerLost = true;
}
