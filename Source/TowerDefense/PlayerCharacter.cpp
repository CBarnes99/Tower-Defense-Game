#pragma once

#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	SpringArm->SetupAttachment(RootComponent.Get());
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(this->SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	

	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Name = DA_PlayerInfo->Name;
	Health = DA_PlayerInfo->Health;
	Mana = DA_PlayerInfo->Mana;
	DamageDelt = DA_PlayerInfo->DamageDelt;
	MovementSpeed = DA_PlayerInfo->MovementSpeed;
	RunSpeed = DA_PlayerInfo->RunSpeed;
	JumpHeight = DA_PlayerInfo->JumpHeight;

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	GetCharacterMovement()->JumpZVelocity = JumpHeight;
	UE_LOG(LogTemp, Warning, TEXT("Name: %s, Health: %f, Mana: %f, Damage Delt: %f, Movement Speed: %f, Run Speed: %f, Jump Height: %f"), *Name, Health, Mana, DamageDelt, MovementSpeed, RunSpeed, JumpHeight)
	
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