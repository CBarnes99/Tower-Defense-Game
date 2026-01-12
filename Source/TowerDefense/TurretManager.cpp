#include "TurretManager.h"
//#include "F_TurretObjectPath.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "AC_PlaceActor.h"
#include "Core_GameState.h"
#include "DA_TurretInfo.h"

ATurretManager::ATurretManager()
{
	PrimaryActorTick.bCanEverTick = false;

	placeActorComponent = CreateDefaultSubobject<UAC_PlaceActor>(TEXT("Place Actor Component"));
}

void ATurretManager::BeginPlay()
{
	Super::BeginPlay();
}

void ATurretManager::StartTurretPlacement(UDA_TurretInfo* turretInfo)
{
	if (!turretInfo)
	{
		UE_LOG(LogTemp, Error, TEXT("Turret Info Within Turret Manager - Start Turret Placement Is NULL!!!"));
		return;
	}
	currentTurretInfo = turretInfo;

	placeActorComponent->StartPlacement(currentTurretInfo->turretClass);
}

void ATurretManager::ConfirmTurretPlacement()
{

	if (IsAbleToPlaceTurret())
	{
		coreGameState->UpdatePlayerCurrencyAmount(false, currentTurretInfo->cost);
		UE_LOG(LogTemp, Warning, TEXT("Spawned Turret! - New Player Currency = %f, Turret Cost = %d"), coreGameState->GetPlayerCurrencyAmount(), currentTurretInfo->cost);
		placeActorComponent->ConfirmPlacement();
	}
	else
	{
		//create notifcation to let the player know they cant place the turret
		UE_LOG(LogTemp, Warning, TEXT("Cant Spawn Turret! - Player Currency = %f, Turret Cost = %d"), coreGameState->GetPlayerCurrencyAmount(), currentTurretInfo->cost);
	}
}

bool ATurretManager::IsAbleToPlaceTurret()
{
	if (!coreGameState)
	{
		AGameStateBase* gameState = UGameplayStatics::GetGameState(GetWorld());
		coreGameState = Cast<ACore_GameState>(gameState);
		if (!coreGameState)
		{
			UE_LOG(LogTemp, Error, TEXT("CORE GAME STATE NOT SET CORRECTLY IN - %s"), *this->GetName());
			return false;
		}
	}

	if (currentTurretInfo->cost <= coreGameState->GetPlayerCurrencyAmount())
		return true;

	return false;
}

void ATurretManager::CancelTurretPlacement()
{
	placeActorComponent->CancelPlacement();
}

void ATurretManager::RotateTurretPlacement(float directon)
{
	placeActorComponent->RotatePlacement(directon);
}

bool ATurretManager::IsPlacingTurret()
{
	return placeActorComponent->IsPlacing();
}

void ATurretManager::UpdateIgnoreActors(AActor* actor, bool addToArray)
{
	placeActorComponent->UpdateIgnoreActors(actor, addToArray);
}

void ATurretManager::UpdateTurretPlacementLocation(FVector traceStartLocation, FVector forwardVector)
{
	placeActorComponent->UpdatePlacementLocation(traceStartLocation, forwardVector);
}