#include "TurretManager.h"
#include "F_TurretObjectPath.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "AC_PlaceActor.h"
#include "Core_GameState.h"

ATurretManager::ATurretManager()
{
	PrimaryActorTick.bCanEverTick = false;

	placeActorComponent = CreateDefaultSubobject<UAC_PlaceActor>(TEXT("Place Actor Component"));
}

void ATurretManager::BeginPlay()
{
	Super::BeginPlay();

	if (turretDataTable) 
	{
		dataTableRowNames = turretDataTable->GetRowNames();

		int amount = 0;
		for (auto element : dataTableRowNames)
		{
			amount++;
		}
		UE_LOG(LogTemp, Warning, TEXT("Elemets in DataTable = %d"), amount);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Data Table Within - %s"), *this->GetName());
	}

	//placeActorComponent->IsAbleToPlaceActorEvent.BindUObject(this, &ATurretManager::IsAbleToPlaceTurret);

	PoolAllPreivewTurrets();
	
	currentPreviewTurretIndex = -1;
	//previousPreviewTurretSelection = -1;
	isPreviewTurretActive = false;

	/*AActor* playerClass = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	myPlayerCharacter = Cast<APlayerCharacter>(playerClass);*/
}

void ATurretManager::PoolAllPreivewTurrets()
{
	for (FName indexName : dataTableRowNames)
	{
		F_TurretObjectPath* turretPtr = turretDataTable->FindRow<F_TurretObjectPath>(indexName, TEXT("Pooling the Preview Turrets"));
		UClass* turretClass = turretPtr->turretClass.LoadSynchronous();
		FActorSpawnParameters spawnParams;
		ATurretStatic* spawnedTurret = GetWorld()->SpawnActor<ATurretStatic>(turretClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
		spawnedTurret->SetPreviewMaterial(true);
		FString turretNameString = FString::Printf(TEXT("Pooled Preview Turret - %s"), *indexName.ToString());
		//spawnedTurret->SetActorLabel(turretNameString);
		pooledPreviewTurrets.Add(spawnedTurret);
		DisablePreviewTurret(spawnedTurret);
	}
}

void ATurretManager::UpdatePreviewTurretLocation(FVector placeTurretLoc, int turretIndex)
{
	//if (!lineTraceComponent->HasImpactPoint(myPlayerCharacter->GetCameraLocation(), myPlayerCharacter->GetCameraForwardVector(), 2000.f)) return;

	if (currentPreviewTurretIndex != turretIndex)
	{
		currentPreviewTurretIndex = turretIndex;
		for (auto turret : pooledPreviewTurrets)
		{
			if (!turret->IsHidden())
			{
				DisablePreviewTurret(turret);
			}
		}
		EnablePreviewTurret(pooledPreviewTurrets[currentPreviewTurretIndex]);
		isPreviewTurretActive = true;
	}
	
	//if (previousPreviewTurretSelection != currentPreviewTurretIndex)
	//{
	//	if (previousPreviewTurretSelection >= 0)
	//	{
	//		DisablePreviewTurret(pooledPreviewTurrets[previousPreviewTurretSelection]);
	//	}
	//	//previousPreviewTurretSelection = currentPreviewTurretIndex;
	//}

	//if (currentPreviewTurretIndex != turretIndex)
	//{
	//	currentPreviewTurretIndex = turretIndex;
	//	//if (!isPreviewTurretActive) previousPreviewTurretSelection = currentPreviewTurretIndex;
	//}
	

	//Getting the position of where you want the turret to be placed
	//FVector turretPlacePos = lineTraceComponent->GetTraceTargetLocation(myPlayerCharacter->GetCameraLocation(), myPlayerCharacter->GetCameraForwardVector(), 2000.f);

	//The Movement of the pooled turret
	//pooledPreviewTurrets[currentPreviewTurretIndex]->SetActorLocation(turretPlacePos);
	/*if (pooledPreviewTurrets[currentPreviewTurretIndex]->IsHidden()) 
		EnablePreviewTurret(pooledPreviewTurrets[currentPreviewTurretIndex]);*/

	pooledPreviewTurrets[currentPreviewTurretIndex]->SetActorLocation(placeTurretLoc);
	


	//if (!previewTurretActor && turretClass)
	//{
	//	previewTurretActor = GetWorld()->SpawnActor<ATurretStatic>(turretClass, placeTurretPos, FRotator::ZeroRotator);
	//	previewTurretActor->SetMaterial(true);
	//	//Set preview material here
	//}
	//else if (previewTurretActor)
	//{
	//	lineTraceComponent->SetIgnoredActor(previewTurretActor);
	//	previewTurretActor->SetActorLocation(placeTurretPos);
	//}
}


void ATurretManager::DisablePreviewTurret(ATurretStatic* turret)
{
	turret->SetActorHiddenInGame(true);
	turret->SetActorEnableCollision(false);
	turret->SetActorTickEnabled(false);
	turret->SetActorRotation(FRotator::ZeroRotator);
}

void ATurretManager::EnablePreviewTurret(ATurretStatic* turret)
{
	turret->SetActorHiddenInGame(false);
	//turret->SetActorEnableCollision(true);
	//turret->SetActorTickEnabled(true);
}

void ATurretManager::NoLongerPlacingTurrets()
{
	DisablePreviewTurret(pooledPreviewTurrets[currentPreviewTurretIndex]);
	currentPreviewTurretIndex = -1;
	//previousPreviewTurretSelection = -1;
	isPreviewTurretActive = false;
}

bool ATurretManager::GetisPreviewTurretActive()
{
	return isPreviewTurretActive;
}

void ATurretManager::RotatePreviewTurret(float dir)
{
	if (!isPreviewTurretActive) return;
	FRotator rotation = pooledPreviewTurrets[currentPreviewTurretIndex]->GetActorRotation();
	//UE_LOG(LogTemp, Warning, TEXT("Current Rotation is = %s"), *rotation.ToString());

	rotation.Yaw += (45.f * dir);
	UE_LOG(LogTemp, Warning, TEXT("New Rotation is is = %s"), *rotation.ToString());

	pooledPreviewTurrets[currentPreviewTurretIndex]->SetActorRotation(rotation);
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
		//create notifcation of some kind to let the player know they cant place the turret
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







ATurretStatic* ATurretManager::SpawnTurretByRow(int rowIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Row Index from SpawnTurretByRow Function within %s = %d"), *this->GetName(), rowIndex);
	int index = rowIndex;
	if (!dataTableRowNames.IsValidIndex(index))
	{
		UE_LOG(LogTemp, Warning, TEXT("Index %d out of range (0 to %d)"), rowIndex, dataTableRowNames.Num() - 1);
		return nullptr;
	}
	FName rowName = dataTableRowNames[index];

	F_TurretObjectPath* rowData = turretDataTable->FindRow<F_TurretObjectPath>(rowName, TEXT("Getting Turret from Data Table"));

	UClass* turretClass = rowData->turretClass.LoadSynchronous();

	FActorSpawnParameters spawnParams;

	ATurretStatic* spawnedTurret = GetWorld()->SpawnActor<ATurretStatic>(turretClass, pooledPreviewTurrets[currentPreviewTurretIndex]->GetActorLocation(), pooledPreviewTurrets[currentPreviewTurretIndex]->GetActorRotation(), spawnParams);
	spawnedTurret->SetPreviewMaterial(false);

	NoLongerPlacingTurrets();
	return spawnedTurret;
}
