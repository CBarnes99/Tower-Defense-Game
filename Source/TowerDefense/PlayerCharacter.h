#pragma once

#include <EnhancedInputLibrary.h>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "DA_CharacterStats.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TOWERDEFENSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//A Pointer to the enemy spawner so that I can call a function from an input action
	UPROPERTY(EditAnywhere, Category = "References")
	class AEnemySpawner* EnemySpawner;

	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	int MaxSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	int JumpHeight;

	//Player Stats, Edit in Blueprints Data Asset
	UPROPERTY(EditDefaultsOnly, Category = "PlayerStats")
	TObjectPtr<UDA_CharacterStats> PlayerInfo;


	//Components added
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;
};
