#pragma once

#include <EnhancedInputLibrary.h>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TOWERDEFENSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:


	//Player Input Mapping Variables which are editable in BP to select the button inputs for each action
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefualtMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* RunAction;


	//Editable Vars for the players Movement Speed and Jump Height
	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	int MaxSpeed;

	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	int JumpHeight;



	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;


	void MovementAction(const FInputActionValue& Value);
	void MouseLookAction(const FInputActionValue& Value);
	void RunningAction();
	void RunningActionStop();
	

};
