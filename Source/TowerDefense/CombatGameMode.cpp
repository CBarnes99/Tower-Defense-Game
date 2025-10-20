// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"
#include "CombatPlayerController.h"

ACombatGameMode::ACombatGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/BP_PlayerCharacter.BP_PlayerCharacter"));
	if (PlayerPawnClassFinder.Class)
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to find player character in game mode"));
	}

	static ConstructorHelpers::FClassFinder<APlayerController> CombatPlayerControllerClass(TEXT("/Game/Player/BP_CombatPlayerController.BP_CombatPlayerController"));
	if (CombatPlayerControllerClass.Class)
	{
		PlayerControllerClass = CombatPlayerControllerClass.Class;
	}
}


void ACombatGameMode::BeginPlay()
{
	Super::BeginPlay();
}