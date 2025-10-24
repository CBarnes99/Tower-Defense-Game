#pragma once

#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f ));

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	springArm->SetupAttachment(RootComponent.Get());
	springArm->TargetArmLength = 300.0f;
	springArm->bUsePawnControlRotation = true;
	springArm->SetRelativeLocation(FVector(0.0f, 70.0f, 50.0f));

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	camera->SetupAttachment(this->springArm, USpringArmComponent::SocketName);
	camera->bUsePawnControlRotation = false;

	weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	weapon->SetupAttachment(RootComponent.Get());

	weapon->SetRelativeLocation(FVector::ZeroVector);
	weapon->SetRelativeRotation(FRotator::ZeroRotator);

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
	health = DA_playerInfo->health;
	mana = DA_playerInfo->mana;
	damageDelt = DA_playerInfo->damageDelt;
	movementSpeed = DA_playerInfo->movementSpeed;
	runSpeed = DA_playerInfo->runSpeed;
	jumpHeight = DA_playerInfo->jumpHeight;

	GetCharacterMovement()->MaxWalkSpeed = movementSpeed;
	GetCharacterMovement()->JumpZVelocity = jumpHeight;
	UE_LOG(LogTemp, Warning, TEXT("Name: %s, Health: %f, Mana: %f, Damage Delt: %f, Movement Speed: %f, Run Speed: %f, Jump Height: %f"), *name, health, mana, damageDelt, movementSpeed, runSpeed, jumpHeight)
	
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