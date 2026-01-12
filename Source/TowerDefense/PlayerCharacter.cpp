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

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<AActor> weaponBP(TEXT("/Game/Weapons/BP_WeaponBase"));
	if (weaponBP.Class)
	{
		weaponClass = weaponBP.Class;
		UE_LOG(LogTemp, Display, TEXT("Weapon BP found in player character"));

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon BP not found in player character"));
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
	//lineTraceComponent = CreateDefaultSubobject<UAC_LineTrace>(TEXT("Line Trace Component"));

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

	healthComponent->SetHealth(DA_playerInfo->health);
	manaComponent->SetMana(DA_playerInfo->mana);

	GetCharacterMovement()->MaxWalkSpeed = DA_playerInfo->movementSpeed;
	GetCharacterMovement()->JumpZVelocity = DA_playerInfo->jumpHeight;
	UE_LOG(LogTemp, Warning, TEXT("Name: %s, Health: %f, Mana: %f, Movement Speed: %f, Run Speed: %f, Jump Height: %f"),
		*DA_playerInfo->name, healthComponent->GetHealth(), manaComponent->GetMana(), DA_playerInfo->movementSpeed, DA_playerInfo->runSpeed, DA_playerInfo->jumpHeight);
	
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
		UE_LOG(LogTemp, Error, TEXT("Weapon Class is not set"));
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
		UE_LOG(LogTemp, Display, TEXT("Weapon Equipped Successfully!!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon Equipped FAILED!!"));
	}
}

FRotator APlayerCharacter::GetCameraRotation() const 
{
	return camera->GetComponentRotation();
}

FVector APlayerCharacter::GetCameraForwardVector() const
{
	//UE_LOG(LogTemp, Warning, TEXT("Camera Location from PlayerCharacter = %s"), *camera->GetComponentLocation().ToString());

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

	if (healthComponent->GetHealth() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Health is less than 0! Player Dead!"))
		//Do Death function here
	}
	return DamageAmount;
}

void APlayerCharacter::AttackAction()
{
	equippedWeapon->FireProjectile(GetCameraLocation(), GetCameraForwardVector());
}