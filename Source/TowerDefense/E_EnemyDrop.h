#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEnemyDrop : uint8
{
	NONE UMETA(DisplayName = "None"),
	HEALTH UMETA(DisplayName = "Health Potion"),
	MANA UMETA(DisplayName = "Mana Potion"),
	SMALL_CURRENCY UMETA(DisplayName = "Small Currency"),
	LARGE_CURRENCY UMETA(DisplayName = "Large Currency")
};
