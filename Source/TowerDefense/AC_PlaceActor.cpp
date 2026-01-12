#include "AC_PlaceActor.h"

UAC_PlaceActor::UAC_PlaceActor()
{
	PrimaryComponentTick.bCanEverTick = false;

	traceDistance = 1000.f;
	rotationDegrees = 45.f;
}

void UAC_PlaceActor::BeginPlay()
{
	Super::BeginPlay();

	bIsPlacing = false;
}

void UAC_PlaceActor::StartPlacement(TSubclassOf<AActor> actorClass)
{

	if (currentPreviewActor != nullptr) 
	{
		DisablePreviewActor(currentPreviewActor);
		currentPreviewActor = nullptr;
	}

	actorClassToSpawn = actorClass;

	currentPreviewActor = GetPreviewActorFromPool(actorClass);
	EnablePreviewActor(currentPreviewActor);
	bIsPlacing = true;

}

void UAC_PlaceActor::UpdatePlacementLocation(FVector traceStartLocation, FVector forwardVector)
{
	if (!HasImpactPoint(traceStartLocation, forwardVector))
	{
		return;
	}

	FVector traceHitLocation = GetTraceTargetLocation(traceStartLocation, forwardVector);

	currentPreviewActor->SetActorLocation(traceHitLocation);

}

void UAC_PlaceActor::ConfirmPlacement()
{
	if (actorClassToSpawn == NULL || currentPreviewActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Within Confirm Placement in AC_PlaceActor, either ActorClassToSpawn is NULL or PreviewActor is nullptr!!!"));
		return;
	}

	FActorSpawnParameters spawnParams;
	GetWorld()->SpawnActor<AActor>(actorClassToSpawn, currentPreviewActor->GetActorTransform(), spawnParams);
	
}

void UAC_PlaceActor::CancelPlacement()
{
	if (currentPreviewActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Within CancelPlacement in AC_PlaceActor, either PreviewActor is nullptr!"));
		return;
	}

	DisablePreviewActor(currentPreviewActor);
	currentPreviewActor = nullptr;
	bIsPlacing = false;
	actorClassToSpawn = NULL;
}

void UAC_PlaceActor::RotatePlacement(float direction)
{
	if (currentPreviewActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Within RotatePlacement in AC_PlaceActor, either PreviewActor is nullptr!"));
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("test %f"), direction);

	float actorCurrentYawRotation = currentPreviewActor->GetActorRotation().Yaw;
	
	float newYawRotation = ((direction * rotationDegrees) + actorCurrentYawRotation);

	FRotator newRotation = FRotator(currentPreviewActor->GetActorRotation().Pitch, newYawRotation, currentPreviewActor->GetActorRotation().Roll);

	currentPreviewActor->SetActorRotation(newRotation);
}

bool UAC_PlaceActor::IsPlacing()
{
	return bIsPlacing;
}

void UAC_PlaceActor::UpdateIgnoreActors(AActor* actor, bool addToArray)
{
	if (addToArray)
	{
		actorsToIgnore.Add(actor);
	}
	else
	{
		actorsToIgnore.Remove(actor);
	}
}

AActor* UAC_PlaceActor::GetPreviewActorFromPool(TSubclassOf<AActor> actorClass)
{
	for (AActor* actor : previewActorPool)
	{
		if (actor->IsA(actorClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Current Actor To actor within GetPreviewActorFromPool"));
			return actor;
		}
	}
	FActorSpawnParameters spawnParams;
	AActor* newActor = GetWorld()->SpawnActor<AActor>(actorClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
	previewActorPool.AddUnique(newActor);
	DisablePreviewActor(newActor);
	UpdateIgnoreActors(newActor, true);
	UE_LOG(LogTemp, Warning, TEXT("Created new preview actor, number of actors within pool = %d"), previewActorPool.Num());

	return newActor;
}

void UAC_PlaceActor::DisablePreviewActor(AActor* actor)
{
	actor->SetActorHiddenInGame(true);
	actor->SetActorEnableCollision(false);
	actor->SetActorTickEnabled(false);
	actor->SetActorRotation(FRotator::ZeroRotator);
}

void UAC_PlaceActor::EnablePreviewActor(AActor* actor)
{
	actor->SetActorHiddenInGame(false);
}

FVector UAC_PlaceActor::GetTraceTargetLocation(FVector traceStartLocation, FVector actorForwardVector)
{
	FVector targetPos = FVector::ZeroVector;
	TArray<FHitResult> hits;
	FVector traceEnd = traceStartLocation + actorForwardVector * traceDistance;

	params.AddIgnoredActor(GetOwner());
	params.AddIgnoredActors(actorsToIgnore);

	GetWorld()->LineTraceMultiByChannel(hits, traceStartLocation, traceEnd, ECC_Visibility, params);

	DrawDebugLine(GetWorld(), traceStartLocation, traceEnd, FColor::Red, false, 1.f, 1.f);

	for (const FHitResult& Hit : hits)
	{
		if (Hit.GetActor() && !Hit.GetActor()->IsA(ignoredActorClass))
		{
			return Hit.ImpactPoint;
		}
	}
	return traceEnd;
};

void UAC_PlaceActor::SetIgnoredActors(TArray<AActor*> ignoredActors)
{
	params.AddIgnoredActors(ignoredActors);
}
void UAC_PlaceActor::SetIgnoredActor(AActor* ignoredActor)
{
	ignoredActorClass = ignoredActor->GetClass();
}

bool UAC_PlaceActor::HasImpactPoint(FVector traceStartLocation, FVector actorForwardVector)
{
	FVector targetPos = FVector::ZeroVector;
	TArray<FHitResult> hits;
	FVector traceEnd = traceStartLocation + actorForwardVector * traceDistance;

	params.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceMultiByChannel(hits, traceStartLocation, traceEnd, ECC_Visibility, params);

	DrawDebugLine(GetWorld(), traceStartLocation, traceEnd, FColor::Red, false, 1.f, 1.f);

	for (const FHitResult& Hit : hits)
	{
		if (Hit.GetActor() && !Hit.GetActor()->IsA(ignoredActorClass))
		{
			return true;
		}
	}

	return false;
}