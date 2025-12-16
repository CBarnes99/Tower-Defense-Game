#include "EnemyPathSpline.h"

AEnemyPathSpline::AEnemyPathSpline()
{
	PrimaryActorTick.bCanEverTick = false;

	splinePath = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Path"));
	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

	RootComponent = splinePath;
	arrow->SetupAttachment(RootComponent.Get());
}

void AEnemyPathSpline::BeginPlay()
{
	Super::BeginPlay();	
	GetSplinePointLocations();
}

TArray<FVector> AEnemyPathSpline::GetSplinePointLocations()
{
	TArray<FVector> splinePointLocations;
	int amountOfSplinePoints = -1;

	amountOfSplinePoints = splinePath->GetNumberOfSplinePoints();
	if (amountOfSplinePoints < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("NO SPLINE POINTS DETECTED WITHIN - %s... NO FVECTOR HAS BEEN ASIGNED"), *this->GetName());
		return TArray<FVector>();
	}
	
	for (int i = 0; i < amountOfSplinePoints; i++)
	{
		splinePointLocations.Add(splinePath->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));

		/*
		UE_LOG(LogTemp, Warning, TEXT("Spline point - %d"), i);
		UE_LOG(LogTemp, Warning, TEXT("Location = %s"), *splinePath->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World).ToString());
		*/

	}
	return splinePointLocations;
}