#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core_GameMode.generated.h"

class ASpawnerManager;
class ACore_PlayerController;

UCLASS()
class TOWERDEFENSE_API ACore_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	ACore_GameMode();

protected:

	virtual void BeginPlay() override;

	/** The spawner manager in the level */
	UPROPERTY(VisibleAnywhere)
	ASpawnerManager* spawnerManager;

	/** The current wave the level is on, starts at 0 on begin play */
	UPROPERTY(VisibleAnywhere)
	int currentWave;

	/** This is called when the player calls to start the next */
	UFUNCTION(BlueprintCallable)
	void StartEnemyWave();

	/** The final wave in the level, is set in StartEnemyWave() */
	UPROPERTY(VisibleAnywhere)
	int lastWave;

	/** The player controller used during combat */
	UPROPERTY(VisibleAnywhere)
	ACore_PlayerController* corePlayerController;

	/** Checks if theres a new wave or if you've finished the last wave */
	UFUNCTION(BlueprintCallable)
	void PrepareNewWave();
};
