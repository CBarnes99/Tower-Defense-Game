#pragma once

#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 
#include "UObject/ConstructorHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AActor> weaponBP(TEXT("/Game/BP_WeaponBase"));
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
	manaComponent = CreateDefaultSubobject<UAC_Mana >(TEXT("Mana Component"));

	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	name = DA_playerInfo->name;
	healthComponent->SetHealth(DA_playerInfo->health);
	manaComponent->SetMana(DA_playerInfo->mana);
	movementSpeed = DA_playerInfo->movementSpeed;
	runSpeed = DA_playerInfo->runSpeed;
	jumpHeight = DA_playerInfo->jumpHeight;

	GetCharacterMovement()->MaxWalkSpeed = movementSpeed;
	GetCharacterMovement()->JumpZVelocity = jumpHeight;
	UE_LOG(LogTemp, Warning, TEXT("Name: %s, Health: %f, Mana: %f, Movement Speed: %f, Run Speed: %f, Jump Height: %f"),
		*name, healthComponent->GetHealth(), manaComponent->GetMana(), movementSpeed, runSpeed, jumpHeight);
	
	EquipWeapon();
}

// Called every frames
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	equippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, weaponSocket);
	equippedWeapon->SetOwner(this);

	if (equippedWeapon)
	{
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
	return movementSpeed;
}

float& APlayerCharacter::GetRunSpeed()
{
	return runSpeed;
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