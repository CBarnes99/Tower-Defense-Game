#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Components/StaticMeshComponent.h"

//#include "AC_LineTrace.h"
//#include "TurretManager.h"
#include "PlayerCharacter.generated.h"

class UDA_PlayerCharacterStats;
class AWeaponBase;
class UAC_Health;
class UAC_Mana;
class UCameraComponent;
class USpringArmComponent;
class AWeaponBase;

UCLASS()
class TOWERDEFENSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Player information from a data asset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TObjectPtr<UDA_PlayerCharacterStats> DA_playerInfo;

	UFUNCTION(BlueprintCallable)
	float& GetMovementSpeed();

	UFUNCTION(BlueprintCallable)
	float& GetRunSpeed();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	AWeaponBase* equippedWeapon;

	UFUNCTION(BlueprintCallable)
	FRotator GetCameraRotation() const;

	UFUNCTION(BlueprintCallable)
	FVector GetCameraForwardVector() const;

	UFUNCTION(BlueprintCallable)
	FVector GetCameraLocation();

	/*UPROPERTY(VisibleDefaultsOnly, Category = "Turret")
	int hotbarSelectionIndex;*/

	/*UFUNCTION(BlueprintCallable)
	void UpdateTurretPlacement();

	UFUNCTION(BlueprintCallable)
	void DestroyTurretPlacement();

	UFUNCTION(BlueprintCallable)
	void PlaceTurret();

	UFUNCTION(BlueprintCallable)
	void RotateTurret(float dir);

	UFUNCTION(BlueprintCallable)
	void SetTurretManager();*/

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	class UDataTable* turretDataTableClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class ATurretManager* turretManager;*/

	/** An Actor Component that controlls the health of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	UAC_Health* healthComponent;

	/** An Actor Component that controlls the mana of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	UAC_Mana* manaComponent;

	/** When the attack action is called by the Core_PlayerController, it fires a projectile from the weapon */
	UFUNCTION(BlueprintCallable)
	void AttackAction();

protected:
	virtual void BeginPlay() override;

	/** Camera Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* camera;

	/** Spring Arm Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* springArm;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAC_LineTrace* lineTraceComponent;*/

	/** This is the socket name on the skeleton, this is where the weapon will attach to on the skeleton */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName weaponSocket;

	////These properties are assigned in the begin play by referencing the DA_playerInfo Data Asset
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	//FString name;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	//float jumpHeight;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	//float movementSpeed;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	//float runSpeed;

	/** The weapons class the character holds and fires */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeaponBase> weaponClass;

	/** On begin play, this function spawns the weapon above and equips it to the socket of the skeleton that has the name of weaponSocket */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon();

	/** The defualt takeDamage function */
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
