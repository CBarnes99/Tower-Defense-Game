// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "F_TurretObjectPath.generated.h"

USTRUCT(BlueprintType)
struct TOWERDEFENSE_API F_TurretObjectPath : public FTableRowBase
{
public:
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString nameOfTurret;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString turretObjectFilePath;
};
