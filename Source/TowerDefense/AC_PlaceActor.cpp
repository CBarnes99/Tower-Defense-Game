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

	if (previewActor != nullptr) 
	{
		previewActor->Destroy();
		previewActor = nullptr;
	}

	actorClassToSpawn = actorClass;
	FActorSpawnParameters spawnParams;

	previewActor = GetWorld()->SpawnActor<AActor>(actorClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
	previewActor->SetActorEnableCollision(false);
	UpdateIgnoreActors(previewActor, true);
	bIsPlacing = true;

}

void UAC_PlaceActor::UpdatePlacementLocation(FVector traceStartLocation, FVector forwardVector)
{
	if (!HasImpactPoint(traceStartLocation, forwardVector))
	{
		return;
	}

	FVector traceHitLocation = GetTraceTargetLocation(traceStartLocation, forwardVector);

	previewActor->SetActorLocation(traceHitLocation);

}

void UAC_PlaceActor::ConfirmPlacement()
{
	if (actorClassToSpawn == NULL || previewActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Within Confirm Placement in AC_PlaceActor, either ActorClassToSpawn is NULL or PreviewActor is nullptr!!!"));
		return;
	}
	FActorSpawnParameters spawnParams;
	GetWorld()->SpawnActor<AActor>(actorClassToSpawn, previewActor->GetActorTransform(), spawnParams);

}

void UAC_PlaceActor::CancelPlacement()
{
	if (previewActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Within CancelPlacement in AC_PlaceActor, either PreviewActor is nullptr!"));
		return;
	}

	UpdateIgnoreActors(previewActor, false);

	previewActor->Destroy();
	bIsPlacing = false;
	actorClassToSpawn = NULL;


}

void UAC_PlaceActor::RotatePlacement(float directon)
{
	if (previewActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Within RotatePlacement in AC_PlaceActor, either PreviewActor is nullptr!"));
		return;
	}

	float actorCurrentYawRotation = previewActor->GetActorRotation().Yaw;
	
	float newYawRotation = ((directon * rotationDegrees) + actorCurrentYawRotation);

	FRotator newRotation = FRotator(previewActor->GetActorRotation().Pitch, newYawRotation, previewActor->GetActorRotation().Roll);

	previewActor->SetActorRotation(newRotation);

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


FVector UAC_PlaceActor::GetTraceTargetLocation(FVector traceStartLocation, FVector actorForwardVector)
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