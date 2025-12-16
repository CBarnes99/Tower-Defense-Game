#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
#include "EnemyPathSpline.generated.h"

UCLASS()
class TOWERDEFENSE_API AEnemyPathSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyPathSpline();

	/** Gets all the locations of the points within the spline */
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSplinePointLocations();

protected:
	virtual void BeginPlay() override;

	/** The Spline Path*/
	UPROPERTY(EditDefaultsOnly)
	USplineComponent* splinePath;

	/** An arrow to show the direction of the spline */
	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* arrow;

};
