// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "CombatPlayerController.h"
#include <Kismet/GameplayStatics.h>


ACombatGameMode::ACombatGameMode()
{
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/BP_PlayerCharacter"));
	if (PlayerPawnClassFinder.Class)
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find player character Blueprint at /Game/Player/BP_PlayerCharacter"));
	}

	static ConstructorHelpers::FClassFinder<ACombatPlayerController> CombatPlayerControllerClass(TEXT("/Script/Engine.Blueprint/Game/BP_CombatPlayerController"));
	if (CombatPlayerControllerClass.Class)
	{
		PlayerControllerClass = CombatPlayerControllerClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find player controller Blueprint at /Game/BP_CombatPlayerController"));
	}*/
}


void ACombatGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatGameMode::startEnemyWave()
{
	if (!spawnerManager)
	{
		AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnerManager::StaticClass());
		spawnerManager = Cast<ASpawnerManager>(actor);
	}

	spawnerManager->startSpawningEnemies();
}
