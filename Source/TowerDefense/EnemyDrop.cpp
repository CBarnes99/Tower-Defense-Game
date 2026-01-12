#include "EnemyDrop.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "Core_GameState.h"

AEnemyDrop::AEnemyDrop()
{
	PrimaryActorTick.bCanEverTick = false;

	dropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drop Mesh"));
	dropMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

	collisionComponent->InitSphereRadius(20.f);
	collisionComponent->SetSimulatePhysics(false);
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	collisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	collisionComponent->SetEnableGravity(false);


	RootComponent = collisionComponent;
	dropMesh->SetupAttachment(RootComponent);


	collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyDrop::OnPickUp);

	healthPotionHealAmount = 20;
	manaPotionGainAmount = 20;
	smallCurrencyGainAmount = 30;
	largeCurrencyGainAmount = 100;

}

void AEnemyDrop::BeginPlay()
{
	Super::BeginPlay();

	rotationSpeed = 20.f;
	bobbingAmplitude = 3.f;
	bobbingFrequency = 0.3f;
	timerInterval = 0.02f;
	runningTime = 0.f;
	initialLocation = GetActorLocation();

	GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &AEnemyDrop::UpdateMotion, timerInterval, true);
}

void AEnemyDrop::SetDrop(EEnemyDrop type)
{
	dropType = type;

	switch (dropType) 
	{
	case EEnemyDrop::HEALTH:
		UE_LOG(LogTemp, Display, TEXT("Set Drop To Health"));

		dropMesh->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Assets/EnemyDrops/Potions/StaticMeshes/SM_Health_Potion.SM_Health_Potion"))));
		break;

	case EEnemyDrop::MANA:
		UE_LOG(LogTemp, Display, TEXT("Set Drop To Mana"));
		dropMesh->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Assets/EnemyDrops/Potions/StaticMeshes/SM_Mana_Potion.SM_Mana_Potion"))));
		break;

	case EEnemyDrop::SMALL_CURRENCY:
		UE_LOG(LogTemp, Display, TEXT("Set Drop To Small Currency"));
		dropMesh->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Assets/EnemyDrops/coin/StaticMeshes/coin.coin"))));
		dropMesh->SetRelativeScale3D(FVector(0.5));
		break;

	case EEnemyDrop::LARGE_CURRENCY:
		UE_LOG(LogTemp, Display, TEXT("Set Drop To Large Currency"));
		dropMesh->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Assets/EnemyDrops/coin/StaticMeshes/coin.coin"))));
		break;

	default:
		UE_LOG(LogTemp, Error, TEXT("Defualt Switch Statement, create new case in set drop function within EnemyDrop.cpp!!"));
		break;
	}
}

void AEnemyDrop::OnPickUp(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//UE_LOG(LogTemp, Warning, TEXT("%s - Collision Dectected with %s"), *this->GetName(), *OtherActor->GetName());

	if (!OtherActor->IsA(APlayerCharacter::StaticClass())) return;

	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	ACore_GameState* gameState = Cast<ACore_GameState>(GetWorld()->GetGameState());

	switch (dropType)
	{
	case EEnemyDrop::HEALTH:
		//UE_LOG(LogTemp, Display, TEXT("Gain Health!"));
		player->ReceiveHealing(healthPotionHealAmount);
		break;

	case EEnemyDrop::MANA:
		//UE_LOG(LogTemp, Display, TEXT("Gain Mana!"));
		player->ReceiveMana(manaPotionGainAmount);
		break;

	case EEnemyDrop::SMALL_CURRENCY:
		//UE_LOG(LogTemp, Display, TEXT("Gain Small Currency!"));
		gameState->UpdatePlayerCurrencyAmount(true, smallCurrencyGainAmount);
		break;

	case EEnemyDrop::LARGE_CURRENCY:
		//UE_LOG(LogTemp, Display, TEXT("Gain Large Currency!"));
		gameState->UpdatePlayerCurrencyAmount(true, largeCurrencyGainAmount);
		break;

	default:
		UE_LOG(LogTemp, Error, TEXT("Defualt Switch Statement, create new case in set drop function within EnemyDrop.cpp!!"));
		break;
	}

	GetWorldTimerManager().ClearTimer(MovementTimerHandle);
	Destroy();
}

void AEnemyDrop::UpdateMotion()
{
	runningTime += timerInterval;

	float deltaRotation = rotationSpeed * timerInterval;
	FRotator newRotation = GetActorRotation();
	newRotation.Yaw += deltaRotation;

	float bobbingOffset = FMath::Sin(runningTime * 2.f * PI * bobbingFrequency) * bobbingAmplitude;
	FVector newLocation = initialLocation;
	newLocation.Z += bobbingOffset;

	SetActorLocationAndRotation(newLocation, newRotation);
}
;