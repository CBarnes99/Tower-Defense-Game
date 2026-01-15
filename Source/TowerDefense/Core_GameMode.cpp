#include "Core_GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerCharacter.h"
#include "Core_PlayerController.h"
#include "SpawnerManager.h"
#include "Kismet/GameplayStatics.h"

ACore_GameMode::ACore_GameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Characters/Player/BP_PlayerCharacter"));
	if (PlayerPawnClassFinder.Class)
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;	
		UE_LOG(LogTemp, Warning, TEXT("ACore_GameMode: Player Pawn/Character found in Game Mode!"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACore_GameMode: Player Pawn/Character NOT found in Game Mode!"))
	}

	static ConstructorHelpers::FClassFinder<APlayerController> CorePlayerControllerClass(TEXT("/Game/Core/BP_Core_PlayerController"));
	if (CorePlayerControllerClass.Class)
	{
		PlayerControllerClass = CorePlayerControllerClass.Class;
		
		UE_LOG(LogTemp, Warning, TEXT("ACore_GameMode: Player Controller class found in Game Mode!"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACore_GameMode: Player Controller class NOT found in Game Mode!"))
	}

	static ConstructorHelpers::FClassFinder<AHUD> CoreHUDClass(TEXT("/Game/Core/BP_Core_HUD"));
	if (CoreHUDClass.Class)
	{
		HUDClass = CoreHUDClass.Class;

		UE_LOG(LogTemp, Warning, TEXT("ACore_GameMode: HUD class found in Game Mode!"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACore_GameMode: HUD class NOT found in Game Mode!"))
	}

	static ConstructorHelpers::FClassFinder<AGameStateBase> CoreGameStateClass(TEXT("/Game/Core/BP_Core_GameState"));
	if (CoreGameStateClass.Class)
	{
		GameStateClass = CoreGameStateClass.Class;

		UE_LOG(LogTemp, Warning, TEXT("ACore_GameMode: GameState class found in Game Mode!"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACore_GameMode: GameState class NOT found in Game Mode!"))
	}
}

void ACore_GameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("BeginPlay: %s has initilized!"), *this->GetName());

	currentWave = 0;
	corePlayerController = Cast<ACore_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (!corePlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Begin Play: CAST TO PLAYER CONTROLLER FAILED IN CORE_GAMEMODE!"));
		return;
	}
	corePlayerController->StartWaveEvent.AddDynamic(this, &ACore_GameMode::StartEnemyWave);

	APlayerCharacter* player = Cast<APlayerCharacter>(corePlayerController->GetPawn());
	if (!player)
	{
		UE_LOG(LogTemp, Error, TEXT("Begin Play: PLAYER FAILED IN CORE_GAMEMODE!"));
		return;
	}
	player->OnPlayerDeathStateEvent.AddUObject(this, &ACore_GameMode::OnPlayerDeathStateChange);
}

void ACore_GameMode::StartEnemyWave()
{
	//Check to see if there is a valid spawner manager
	if (!spawnerManager)
	{
		AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnerManager::StaticClass());
		spawnerManager = Cast<ASpawnerManager>(actor);
	
		spawnerManager->WaveEndedEvent.AddDynamic(this, &ACore_GameMode::PrepareNewWave);
	}

	//Check to see if the spawner manager has all the spawners refereneced
	if (!spawnerManager->enemySpawners.IsValidIndex(0))
	{
		spawnerManager->SetAllSpawners();
	}
	
	//cCeck to see if the last wave has been initilized
	if (!lastWave)
	{
		lastWave = spawnerManager->CalculateLastWave();
		UE_LOG(LogTemp, Warning, TEXT("StartEnemyWave: The last wave of this level is: %i"), lastWave);		
	}

	//Check to see if a wave is already active, if not, start spawning enemies
	if (!spawnerManager->IsWaveActive())
	{
		currentWave++;

		UE_LOG(LogTemp, Warning, TEXT("StartEnemyWave: Wave %d is now active!"), currentWave);

		spawnerManager->StartSpawningEnemies(currentWave);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StartEnemyWave: Wave %d is already active!"), currentWave);
	}
}

void ACore_GameMode::PrepareNewWave()
{
	if (currentWave == lastWave)
	{
		UE_LOG(LogTemp, Display, TEXT("PrepareNewWave: Last Wave has been Defeated"));
		
		LevelCompleteEvent.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("PrepareNewWave: New Wave has been prepared"));
		//Do setup phase thing here
	}
}

void ACore_GameMode::OnPlayerDeathStateChange(bool bPlayerDefeatedCheck)
{
	if (bPlayerDefeatedCheck)
	{
		corePlayerController->DisableInput(corePlayerController);
	}
	else
	{
		corePlayerController->EnableInput(corePlayerController);
		RestartPlayer(corePlayerController);
	}

	spawnerManager->ShouldEnemiesPerceptPlayer(bPlayerDefeatedCheck);
}
