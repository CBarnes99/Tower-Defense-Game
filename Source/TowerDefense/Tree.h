// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tree.generated.h"

UENUM(BlueprintType)
enum class ETreeType : uint8
{
	Tree1 UMETA(DisplayName = "Tree1"),
	Tree2 UMETA(DisplayName = "Tree2"),
	Tree3 UMETA(DisplayName = "Tree3")
};

UCLASS()
class TOWERDEFENSE_API ATree : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATree();

	// Called every frame
	virtual void Tick(float DeltaTime) override;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	class UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
	ETreeType treeType = ETreeType::Tree1;

};
