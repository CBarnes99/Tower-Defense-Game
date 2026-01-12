#include "TurretStatic.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "DA_TurretInfo.h"

ATurretStatic::ATurretStatic()
{
	PrimaryActorTick.bCanEverTick = false;

	invisableRootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Invisable Root Mesh"));
	RootComponent = invisableRootMesh;
	invisableRootMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	turretMesh->SetupAttachment(RootComponent);

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision Area"));
	collisionBox->SetCollisionProfileName(TEXT("Trigger"));
	collisionBox->SetGenerateOverlapEvents(true);
	collisionBoxSize = FVector(150, 50, 50);
	collisionBox->SetBoxExtent(FVector(collisionBoxSize));
	collisionBox->SetRelativeLocation(FVector(collisionBoxSize.X, 0, 0));
	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATurretStatic::OnOverlap);

	collisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision Texture"));
	collisionMesh->SetCollisionProfileName("NoCollision", false);

    collisionBox->SetupAttachment(RootComponent);
	collisionMesh->SetupAttachment(collisionBox);
}

void ATurretStatic::BeginPlay()
{
	Super::BeginPlay();

	turretActive = true;
	turretRecharging = false;

}
void ATurretStatic::OnConstruction(const FTransform& Transform)
{
	collisionMesh->SetRelativeScale3D(collisionBoxSize / 125);	
}

void ATurretStatic::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA(AEnemyCharacterBase::StaticClass()) && !damagedActors.Contains(OtherActor))
    {
		if (turretActive)
		{
			turretActive = false;
			GetWorld()->GetTimerManager().SetTimer(activeAndRechargeTimerHandle, this, &ATurretStatic::disableTurret, turretStats->activeTime, false);
		}

		if (GetLocalRole() == ROLE_Authority)
		{
			UE_LOG(LogTemp, Display, TEXT("%s Overlaps %s"), *OtherActor->GetName(), *this->GetName());

			UGameplayStatics::ApplyDamage(OtherActor, turretStats->damageAmount, GetInstigatorController(), this, UDamageType::StaticClass());
			damagedActors.Add(OtherActor);
		}
    }
}

void ATurretStatic::disableTurret()
{
	UE_LOG(LogTemp, Display, TEXT("%s is disabled"), *this->GetName());
	collisionBox->SetGenerateOverlapEvents(false);
	turretRecharging = true;
	damagedActors.Empty();
	GetWorld()->GetTimerManager().SetTimer(activeAndRechargeTimerHandle, this, &ATurretStatic::enableTurret, turretStats->rechargeTime, false);
}

void ATurretStatic::enableTurret()
{
	UE_LOG(LogTemp, Display, TEXT("%s is enabled"), *this->GetName());
	collisionBox->SetGenerateOverlapEvents(true);
	turretRecharging = false;
	turretActive = true;
}
