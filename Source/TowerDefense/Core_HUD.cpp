#include "Core_HUD.h"
#include "Kismet/GameplayStatics.h"

#include "HUDTurretSelectionMenu.h"
#include "HUDPlayerHud.h"
#include "HUDHealthAndMana.h"
#include "HUDWeaponTurretSelector.h"
#include "HUDVictoryScreen.h"
#include "HUDPlayerDefeated.h"

#include "DA_TurretInfo.h"
#include "Core_PlayerController.h"
#include "PlayerCharacter.h"
#include "Core_GameMode.h"

void ACore_HUD::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("BeginPlay: %s has initilized!"), *this->GetName());

	localCorePlayerController = Cast<ACore_PlayerController>(GetOwningPlayerController());

	if (!localCorePlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("BeginPlay: PLAYER CONTROLLER WITHIN %s IS NOT SET CORRECTLY!!"), *this->GetName());
	}

	ACore_GameMode* gameMode = Cast<ACore_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->LevelCompleteEvent.AddUObject(this, &ACore_HUD::ToggleVictoryScreenWidget);

	SetUpInGameWidgetList();
	
	SetUpGameMenusWidgetList();
	SetUpMenusWidgetList();
	SetUpModalWidgetList();

	BindDelegates();

	SetFocusToGame();
}

bool ACore_HUD::CheckVaildWidgetPointer(TSubclassOf<UUserWidget> widgetClass)
{
	if (widgetClass)
	{
		UE_LOG(LogTemp, Display, TEXT("CheckVaildWidgetPointer: Widget Class is valid - %s"), *widgetClass->GetName());
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CheckVaildWidgetPointer: A WIDGET CLASS IS NOT SET WITHIN CORE_HUD!!"));
		return false;
	}
}

void ACore_HUD::SetFocusToGame()
{
	//Set focus to the game
	FInputModeGameOnly inputModeGameOnly;
	localCorePlayerController->SetInputMode(inputModeGameOnly);

	//Hide cursor
	localCorePlayerController->SetShowMouseCursor(false);

	if (gameMenuWidgetToFocus != nullptr)
	{
		gameMenuWidgetToFocus->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void ACore_HUD::SetFocusToWidget()
{
	gameMenuWidgetToFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FInputModeGameAndUI inputModeGameAndUI;
	localCorePlayerController->SetInputMode(inputModeGameAndUI);
	localCorePlayerController->SetShowMouseCursor(true);

	//Sets Mouse Location to the Centre of hte screen.
	FVector2D viewportSize;
	GEngine->GameViewport->GetViewportSize(viewportSize);
	localCorePlayerController->SetMouseLocation((viewportSize.X / 2), (viewportSize.Y / 2));
}

void ACore_HUD::SetUpInGameWidgetList()
{
	bInGameWidgetsEnabled = false;

	if(CheckVaildWidgetPointer(playerHudClass))
	{
		playerHud = CreateWidget<UHUDPlayerHud>(localCorePlayerController, playerHudClass);
		inGameWidgetList.Add(playerHud);
	}

	for (UUserWidget* widget : inGameWidgetList)
	{
		if (widget == nullptr)
		{
			UE_LOG(LogTemp, Fatal, TEXT("SetUpInGameWidgetList: WIDGET IS RETURNING NULL WITHIN CORE HUD, SET UP IN GAME WIDGET LIST FUNCTION"));
			return;
		}
		widget->AddToViewport();
		widget->SetVisibility(ESlateVisibility::Collapsed);
	}

	ToggleInGameWidgets();
}


void ACore_HUD::ToggleInGameWidgets()
{

	if (bInGameWidgetsEnabled)
	{
		for (UUserWidget* widget : inGameWidgetList)
		{
			widget->SetVisibility(ESlateVisibility::Collapsed);
		}
		bInGameWidgetsEnabled = false;
	}
	else
	{
		for (UUserWidget* widget : inGameWidgetList)
		{
			widget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		bInGameWidgetsEnabled = true;
	}
}

void ACore_HUD::SetUpGameMenusWidgetList()
{
	if (CheckVaildWidgetPointer(turretSelectionMenuClass))
	{
		turretSelectionMenu = CreateWidget<UHUDTurretSelectionMenu>(localCorePlayerController, turretSelectionMenuClass);
		turretSelectionMenu->SetUpWidget(turretInfo);
		gameMenusWidgetList.Add(turretSelectionMenu);
	}
	
	if (CheckVaildWidgetPointer(victoryScreenClass))
	{
		victoryScreenMenu = CreateWidget<UHUDVictoryScreen>(localCorePlayerController, victoryScreenClass);
		gameMenusWidgetList.Add(victoryScreenMenu);
	}

	if (CheckVaildWidgetPointer(playerDefeatedClass))
	{
		playerDefeatedMenu = CreateWidget<UHUDPlayerDefeated>(localCorePlayerController, playerDefeatedClass);
		gameMenusWidgetList.Add(playerDefeatedMenu);
	}

	for (UUserWidget* widget : gameMenusWidgetList)
	{
		widget->AddToViewport();
		widget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ACore_HUD::SetUpMenusWidgetList()
{
	//menusWidgetList.Add()

	/*for (UUserWidget* widget : menusWidgetList)
	{
		widget->SetVisibility(ESlateVisibility::Collapsed);
	}*/
}

void ACore_HUD::SetUpModalWidgetList()
{
	//modalWidgetList.Add()

	/*for (UUserWidget* widget : modalWidgetList)
	{
		widget->SetVisibility(ESlateVisibility::Collapsed);
	}*/
}

void ACore_HUD::ToggleGameMenuWidgets(UUserWidget* widget)
{
	if (widget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ToggleGameMenuWidgets: WIDGET IS RETURNING NULL WITHIN - %s"), *this->GetName());
		return;
	}
	gameMenuWidgetToFocus = widget;
		
	for (UUserWidget* widgetInList : gameMenusWidgetList)
	{
		if (widgetInList == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("ToggleGameMenuWidgets: widgetInList IS RETURNING NULL WITHIN CORE HUD gameMenusWidgetList array - %s"), *this->GetName());
			return;
		}

		if (widgetInList == gameMenuWidgetToFocus && widgetInList->GetVisibility() == ESlateVisibility::Collapsed)
		{
			SetFocusToWidget();
			widgetInList->SetVisibility(ESlateVisibility::Visible);
		}
		else if (widgetInList->GetVisibility() == ESlateVisibility::Visible)
		{
			SetFocusToGame();
			widgetInList->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

}

void ACore_HUD::ToggleTurretSelectionWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("ToggleTurretSelectionWidget: Toggle Turret Selection"));
	ToggleGameMenuWidgets(turretSelectionMenu);
}

void ACore_HUD::ToggleVictoryScreenWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("ToggleVictoryScreenWidget: Toggle Victory Screen Widget"));
	ToggleGameMenuWidgets(victoryScreenMenu);
	victoryScreenMenu->PlayVictoryAnimation();
	localCorePlayerController->DisableInput(localCorePlayerController);
}

void ACore_HUD::BindDelegates()
{
	turretSelectionMenu->OnMenuSelectionEvent.AddDynamic(playerHud->WeaponAndTurretSelector, &UHUDWeaponTurretSelector::GetInfoFromTurretMenu);
	
	APlayerCharacter* player = Cast<APlayerCharacter>(localCorePlayerController->GetPawn());
	if (!player)
	{
		UE_LOG(LogTemp, Warning, TEXT("BindDelegates: PLAYER NOT CASTED CORRECTLY WITHIN - %s"), *this->GetName());
		return;
	}
	player->OnHealthUpdatedEvent.AddUObject(playerHud->HealthAndMana, &UHUDHealthAndMana::UpdateHealthBar);
	player->OnManaUpdatedEvent.AddUObject(playerHud->HealthAndMana, &UHUDHealthAndMana::UpdateManaBar);
	player->OnPlayerDeathStateEvent.AddUObject(this, &ACore_HUD::PlayerDefeatedState);

}

void ACore_HUD::PlayerDefeatedState(bool bIsPlayerDefeated, float respwanTime)
{
	if (bIsPlayerDefeated)
	{
		ToggleGameMenuWidgets(playerDefeatedMenu);
		playerDefeatedMenu->StartCountdown(respwanTime);
	}
	else
	{
		playerDefeatedMenu->EndCountdown();
		ToggleGameMenuWidgets(playerDefeatedMenu);
	}
}

bool ACore_HUD::GetIsTurretSelectionMenuVisable()
{
	return turretSelectionMenu->IsVisible();
}