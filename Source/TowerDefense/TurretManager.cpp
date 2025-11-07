#include "TurretManager.h"
#include "F_TurretObjectPath.h"

ATurretManager::ATurretManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
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

	PoolAllPreivewTurrets();
}

// Called every frame
void ATurretManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ATurretStatic* ATurretManager::SpawnTurretByRow(int rowIndex, FVector spawnLoc, FRotator spawnRot)
{
	UE_LOG(LogTemp, Warning, TEXT("Row Index from SpawnTurretByRow Function within %s = %d"), *this->GetName(), rowIndex);
	int index = rowIndex - 2;
	if (!dataTableRowNames.IsValidIndex(index))
	{
		UE_LOG(LogTemp, Warning, TEXT("Index %d out of range (0 to %d)"), rowIndex, dataTableRowNames.Num() - 1);
		return nullptr;
	}
	FName rowName = dataTableRowNames[index];

	F_TurretObjectPath* rowData = turretDataTable->FindRow<F_TurretObjectPath>(rowName, TEXT("Getting Data Table Row"));

	UClass* turretClass = rowData->turretClass.LoadSynchronous();

	FActorSpawnParameters spawnParams;

	ATurretStatic* spawnedTurret = GetWorld()->SpawnActor<ATurretStatic>(turretClass, spawnLoc, spawnRot, spawnParams);

	return spawnedTurret;
}

void ATurretManager::PoolAllPreivewTurrets()
{
	for (FName indexName : dataTableRowNames)
	{
		F_TurretObjectPath* turretPtr = turretDataTable->FindRow<F_TurretObjectPath>(indexName, TEXT("Pooling the Preview Turrets"));
		UClass* turretClass = turretPtr->turretClass.LoadSynchronous();
		FActorSpawnParameters spawnParams;
		ATurretStatic* spawnedTurret = GetWorld()->SpawnActor<ATurretStatic>(turretClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
		pooledPreviewTurrets.Add(spawnedTurret);
		DisablePreviewTurret(spawnedTurret);
	}
}

//void ATurretManager::UpdatePreviewTurretLocation(FVector previewTurretPos, int index)
//{
//	if (!previewTurretActor && turretClass)
//	{
//		previewTurretActor = GetWorld()->SpawnActor<ATurretStatic>(turretClass, placeTurretPos, FRotator::ZeroRotator);
//		previewTurretActor->SetMaterial(true);
//		//Set preview material here
//	}
//	else if (previewTurretActor)
//	{
//		lineTraceComponent->SetIgnoredActor(previewTurretActor);
//		previewTurretActor->SetActorLocation(placeTurretPos);
//	}
//}

void ATurretManager::DisablePreviewTurret(ATurretStatic* turret)
{
	turret->SetActorHiddenInGame(true);
	turret->SetActorEnableCollision(false);
	turret->SetActorTickEnabled(false);
}

void ATurretManager::EnablePreviewTurret(ATurretStatic* turret)
{
	turret->SetActorHiddenInGame(false);
	turret->SetActorEnableCollision(true);
	turret->SetActorTickEnabled(true);
}

//void ATurretManager::PlaceTurret()
//{
//
//}

//////////////////////////////////////////////////////////////////////

//void APlayerCharacter::UpdateTurretPlacement()
//{
//
//	if (!lineTraceComponent->HasImpactPoint(GetCameraLocation(), GetCameraForwardVector(), 2000.f)) return;
//
//	FVector placeTurretPos = lineTraceComponent->GetTraceTargetLocation(GetCameraLocation(), GetCameraForwardVector(), 2000.f);
//
//
//	if (!previewTurretActor && turretClass)
//	{
//		previewTurretActor = GetWorld()->SpawnActor<ATurretStatic>(turretClass, placeTurretPos, FRotator::ZeroRotator);
//		previewTurretActor->SetMaterial(true);
//		//Set preview material here
//	}
//	else if (previewTurretActor)
//	{
//		lineTraceComponent->SetIgnoredActor(previewTurretActor);
//		previewTurretActor->SetActorLocation(placeTurretPos);
//	}
//}
//
//void APlayerCharacter::DestroyTurretPlacement()
//{
//	hotbarSelectionIndex = 1;
//	previewTurretActor->Destroy();
//	previewTurretActor = nullptr;
//}
//
//void APlayerCharacter::PlaceTurret()
//{
//	FVector spawnLoc = previewTurretActor->GetActorLocation();
//	FRotator spawnRot = previewTurretActor->GetActorRotation();
//	DestroyTurretPlacement();
//
//	ATurretStatic* spawnedTurret = GetWorld()->SpawnActor<ATurretStatic>(turretClass, spawnLoc, spawnRot);
//	spawnedTurret->SetMaterial(false);
//}