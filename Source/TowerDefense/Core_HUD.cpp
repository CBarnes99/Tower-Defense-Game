#include "Core_HUD.h"
#include "HUDTurretSelectionMenu.h"
#include "HUDPlayerHud.h"
#include "HUDHealthAndMana.h"
#include "DA_TurretInfo.h"
#include "Core_PlayerController.h"
#include "HUDWeaponTurretSelector.h"

void ACore_HUD::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("%s has initilized!"), *this->GetName());

	localCorePlayerController = Cast<ACore_PlayerController>(GetOwningPlayerController());

	if (!localCorePlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PLAYER CONTROLLER WITHIN %s IS NOT SET CORRECTLY!!"), *this->GetName());
	}

	SetUpInGameWidgetList();
	
	SetUpGameMenusWidgetList();
	SetUpMenusWidgetList();
	SetUpModalWidgetList();

	BindDelegates();

	SetFocusToGame();
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

	FVector2D viewportSize;
	GEngine->GameViewport->GetViewportSize(viewportSize);
	localCorePlayerController->SetMouseLocation((viewportSize.X / 2), (viewportSize.Y / 2));
}

void ACore_HUD::SetUpInGameWidgetList()
{
	bInGameWidgetsEnabled = false;

	playerHud = CreateWidget<UHUDPlayerHud>(localCorePlayerController, playerHudClass);
	inGameWidgetList.Add(playerHud);

	/*healthAndMana = CreateWidget<UHUDHealthAndMana>(localCorePlayerController, healthAndManaClass);
	inGameWidgetList.Add(healthAndMana);*/

	for (UUserWidget* widget : inGameWidgetList)
	{
		if (widget == nullptr)
		{
			UE_LOG(LogTemp, Fatal, TEXT("WIDGET IS RETURNING NULL WITHIN CORE HUD SET UP IN GAME WIDGET LIST FUNCTION"));
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

	turretSelectionMenu = CreateWidget<UHUDTurretSelectionMenu>(localCorePlayerController, turretSelectionMenuClass);
	turretSelectionMenu->SetUpWidget(turretInfo);

	gameMenusWidgetList.Add(turretSelectionMenu);
	
	//UUserWidget* test = CreateWidget<UHUDTurretSelectionMenu>()
	/*UUserWidget* createdWidget = CreateWidget<UUserWidget>(this, turretSelectionMenuClass);
	turretSelectionMenu = Cast<UHUDTurretSelectionMenu>(createdWidget);
	gameMenusWidgetList.Add(createdWidget);*/


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
		UE_LOG(LogTemp, Error, TEXT("WIDGET IS RETURNING NULL WITHIN CORE HUD TOGGLE WIDGET FUNCTION"));
		return;
	}
	gameMenuWidgetToFocus = widget;

	for (UUserWidget* widgetInList : gameMenusWidgetList)
	{
		if (widgetInList == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("widgetInList IS RETURNING NULL WITHIN CORE HUD gameMenusWidgetList"));
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
	UE_LOG(LogTemp, Warning, TEXT("Toggle Turret Selection Widget Function"));
	ToggleGameMenuWidgets(turretSelectionMenu);
}

void ACore_HUD::BindDelegates()
{
	turretSelectionMenu->OnMenuSelectionSigniture.AddDynamic(playerHud->WeaponAndTurretSelector, &UHUDWeaponTurretSelector::GetInfoFromTurretMenu);
}
