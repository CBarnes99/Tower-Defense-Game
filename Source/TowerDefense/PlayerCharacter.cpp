#pragma once

#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 

#include "Kismet/GameplayStatics.h"

#include "UObject/ConstructorHelpers.h"

APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	//Change this to a data table, possibly remove it entirely
	/*static ConstructorHelpers::FClassFinder<AActor> turretBP(TEXT("/Game/Turrets/MyTurretStatic"));
	if (turretBP.Class)
	{
		turretClass = turretBP.Class;
		UE_LOG(LogTemp, Display, TEXT("Turret BP found in player character"));

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Turret BP not found in player character"));
	}*/

	static ConstructorHelpers::FClassFinder<UDataTable> turretDataTable(TEXT("/Game/Data/DataTables/DT_TurretObjectPath"));
	if (turretDataTable.Class)
	{
		turretDataTableClass = turretDataTable.Class;
		UE_LOG(LogTemp, Display, TEXT("turretDataTable BP found in player character"));

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("turretDataTable BP not found in player character"));
	}



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
	lineTraceComponent = CreateDefaultSubobject<UAC_LineTrace>(TEXT("Line Trace Component"));

	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	name = DA_playerInfo->name;
	healthComponent->SetHealth(DA_playerInfo->health);
	manaComponent->SetMana(DA_playerInfo->mana);
	movementSpeed = DA_playerInfo->movementSpeed;
	runSpeed = DA_playerInfo->runSpeed;
	jumpHeight = DA_playerInfo->jumpHeight;

	hotbarSelectionIndex = 1;

	GetCharacterMovement()->MaxWalkSpeed = movementSpeed;
	GetCharacterMovement()->JumpZVelocity = jumpHeight;
	UE_LOG(LogTemp, Warning, TEXT("Name: %s, Health: %f, Mana: %f, Movement Speed: %f, Run Speed: %f, Jump Height: %f"),
		*name, healthComponent->GetHealth(), manaComponent->GetMana(), movementSpeed, runSpeed, jumpHeight);
	
	EquipWeapon();

	//Temporary until I have a database of turrets to pull from
	previewTurretActor = nullptr;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), turretClass, turretsToIgnore);

	FString TurretListString;
	for (auto* Turret : turretsToIgnore)
	{
		if (Turret)
		{
			TurretListString += Turret->GetName() + TEXT(", ");
		}
	}
	if (TurretListString.Len() > 2)
	{
		TurretListString.LeftChopInline(2); // Remove trailing ", "
	}

	UE_LOG(LogTemp, Warning, TEXT("turrets to ignore = %s"), *TurretListString);



	//lineTraceComponent->SetIgnoredActor(previewTurretActor);
	//Temporary until I have a database of turrets to pull from

}


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

void APlayerCharacter::UpdateTurretPlacement()
{

	if (!lineTraceComponent->HasImpactPoint(GetCameraLocation(), GetCameraForwardVector(), 2000.f)) return;


	FVector placeTurretPos = lineTraceComponent->GetTraceTargetLocation(GetCameraLocation(), GetCameraForwardVector(), 2000.f);


	if (!previewTurretActor && turretClass)
	{
		previewTurretActor = GetWorld()->SpawnActor<ATurretStatic>(turretClass, placeTurretPos, FRotator::ZeroRotator);
		previewTurretActor->SetMaterial(true);
		//Set preview material here
	}
	else if (previewTurretActor)
	{
		lineTraceComponent->SetIgnoredActor(previewTurretActor);
		previewTurretActor->SetActorLocation(placeTurretPos);
	}
}

void APlayerCharacter::DestroyTurretPlacement()
{
	hotbarSelectionIndex = 1;
	previewTurretActor->Destroy();
	previewTurretActor = nullptr;
}

void APlayerCharacter::PlaceTurret()
{
	FVector spawnLoc = previewTurretActor->GetActorLocation();
	FRotator spawnRot = previewTurretActor->GetActorRotation();
	DestroyTurretPlacement();
	
	ATurretStatic* spawnedTurret = GetWorld()->SpawnActor<ATurretStatic>(turretClass, spawnLoc, spawnRot);
	spawnedTurret->SetMaterial(false);


}
