// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"

// Sets default values
ATree::ATree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticMesh"));

}

// Called when the game starts or when spawned
void ATree::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATree::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UE_LOG(LogTemp, Display, TEXT("%s has changed in construction"), *this->GetName());

	switch (treeType)
	{
	case ETreeType::Tree1:
	{
		mesh->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Fab/Stylized_guns___Lowpoly_Free/SM_Gun.SM_Gun"))));
		break;
	}
	case ETreeType::Tree2:
	{
		mesh->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/Fab/Stylized_guns___Lowpoly_Free/SM__Cannon.SM__Cannon'"))));
		break;
	}
	case ETreeType::Tree3:
	{
		mesh->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/Fab/Stylized_guns___Lowpoly_Free/gun_lowpoly_02.gun_lowpoly_02'"))));
		break;
	}
	}
}

// Called every frame
void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

