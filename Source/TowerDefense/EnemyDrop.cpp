#include "EnemyDrop.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"

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

}

void AEnemyDrop::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyDrop::SetDrop(EEnemyDrop type)
{
	dropType = type;

	switch (dropType) 
	{
	case EEnemyDrop::HEALTH:
		UE_LOG(LogTemp, Display, TEXT("Set Drop To Health"));
		dropMesh->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Fab/Stylized_guns___Lowpoly_Free/SM_Gun.SM_Gun"))));
		break;

	case EEnemyDrop::MANA:
		UE_LOG(LogTemp, Display, TEXT("Set Drop To Mana"));
		dropMesh->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Fab/Stylized_guns___Lowpoly_Free/SM__Cannon.SM__Cannon"))));
		break;

	case EEnemyDrop::SMALL_CURRENCY:
		UE_LOG(LogTemp, Display, TEXT("Set Drop To Small Currency"));
		dropMesh->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Fab/Stylized_guns___Lowpoly_Free/gun_lowpoly_02.gun_lowpoly_02"))));
		break;

	case EEnemyDrop::LARGE_CURRENCY:
		UE_LOG(LogTemp, Display, TEXT("Set Drop To Large Currency"));
		dropMesh->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Fab/Stylized_guns___Lowpoly_Free/gun_lowpoly_02.gun_lowpoly_02"))));
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

	switch (dropType)
	{
	case EEnemyDrop::HEALTH:
		//UE_LOG(LogTemp, Display, TEXT("Gain Health!"));
		player->healthComponent->RecieveHealing(50.f);
		break;

	case EEnemyDrop::MANA:
		//UE_LOG(LogTemp, Display, TEXT("Gain Mana!"));
		player->manaComponent->GainMana(50.f);
		break;

	case EEnemyDrop::SMALL_CURRENCY:
		UE_LOG(LogTemp, Display, TEXT("Gain Small Currency!"));
		UE_LOG(LogTemp, Error, TEXT("NOT IMPLEMENTED YET!"));
		break;

	case EEnemyDrop::LARGE_CURRENCY:
		UE_LOG(LogTemp, Display, TEXT("Gain Large Currency!"));
		UE_LOG(LogTemp, Error, TEXT("NOT IMPLEMENTED YET!"));
		break;

	default:
		UE_LOG(LogTemp, Error, TEXT("Defualt Switch Statement, create new case in set drop function within EnemyDrop.cpp!!"));
		break;
	}
	
	Destroy();
};