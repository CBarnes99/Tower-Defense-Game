#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Core_HUD.generated.h"

class UHUDTurretSelectionMenu;
class UHUDPlayerHud;
class ACore_PlayerController;
class UDA_TurretInfo;
class UHUDVictoryScreen;
class UHUDPlayerDefeated;
class UHUDPlayerLost;

UCLASS()
class TOWERDEFENSE_API ACore_HUD : public AHUD
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void ToggleTurretSelectionWidget();

	UFUNCTION(BlueprintCallable)
	void ToggleVictoryScreenWidget();

	UFUNCTION(BlueprintCallable)
	bool GetIsTurretSelectionMenuVisable();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
	ACore_PlayerController* localCorePlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "DataAssets")
	TArray<UDA_TurretInfo*> turretInfo;

	UFUNCTION(BlueprintCallable)
	bool CheckVaildWidgetPointer(TSubclassOf<UUserWidget> widgetClass);

	UFUNCTION(BlueprintCallable)
	void SetFocusToGame();

	UFUNCTION(BlueprintCallable)
	void SetFocusToWidget();

	UFUNCTION(BlueprintCallable)
	void BindDelegates();

	UFUNCTION(BlueprintCallable)
	void PlayerDefeatedState(bool bIsPlayerDefeated, float respwanTime);

	UFUNCTION(BlueprintCallable)
	void PlayerLost();

	UPROPERTY(VisibleAnywhere)
	bool bPlayerHasLost;

	//In Game Widgets------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, Category = "Widgets - In Game")
	TArray<UUserWidget*> inGameWidgetList;

	UFUNCTION(BlueprintCallable, Category = "Widgets - In Game")
	void SetUpInGameWidgetList();

	UFUNCTION(BlueprintCallable)
	void ToggleInGameWidgets();

	UPROPERTY(VisibleAnywhere)
	bool bInGameWidgetsEnabled;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets - In Game")
	TSubclassOf<UHUDPlayerHud> playerHudClass;

	UPROPERTY(VisibleAnywhere, Category = "Widgets - In Game")
	UHUDPlayerHud* playerHud;


	//Game Menu Widgets-------------------------------------------------------
	UPROPERTY(VisibleAnywhere, Category = "Widgets - Game Menu")
	TArray<UUserWidget*> gameMenusWidgetList;

	UFUNCTION(BlueprintCallable, Category = "Widgets - Game Menu")
	void SetUpGameMenusWidgetList();

	UPROPERTY(VisibleAnywhere)
	UUserWidget* gameMenuWidgetToFocus;

	UFUNCTION(BlueprintCallable)
	void ToggleGameMenuWidgets(UUserWidget* widget);

	UPROPERTY(EditDefaultsOnly, Category = "Widgets - Game Menu")
	TSubclassOf<UHUDTurretSelectionMenu> turretSelectionMenuClass;

	UPROPERTY(VisibleAnywhere, Category = "Widgets - Game Menu")
	UHUDTurretSelectionMenu* turretSelectionMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets - Game Menu")
	TSubclassOf<UHUDVictoryScreen> victoryScreenClass;

	UPROPERTY(VisibleAnywhere, Category = "Widgets - Game Menu")
	UHUDVictoryScreen* victoryScreenMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets - Game Menu")
	TSubclassOf<UHUDPlayerDefeated> playerDefeatedClass;

	UPROPERTY(VisibleAnywhere, Category = "Widgets - Game Menu")
	UHUDPlayerDefeated* playerDefeatedMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets - Game Menu")
	TSubclassOf<UHUDPlayerLost> playerLostClass;

	UPROPERTY(VisibleAnywhere, Category = "Widgets - Game Menu")
	UHUDPlayerLost* playerLostMenu;
	
	

	//Menu Widgets-----------------------------------------------------------
	UPROPERTY(VisibleAnywhere, Category = "Widgets - Menus")
	TArray<UUserWidget*> menusWidgetList;

	UFUNCTION(BlueprintCallable, Category = "Widgets - Menus")
	void SetUpMenusWidgetList();
	
	//Modal Widgets----------------------------------------------------------
	UPROPERTY(VisibleAnywhere, Category = "Widgets - Modal")
	TArray<UUserWidget*> modalWidgetList;

	UFUNCTION(BlueprintCallable, Category = "Widgets - Modal")
	void SetUpModalWidgetList();


};
