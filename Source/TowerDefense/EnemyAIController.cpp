#include "EnemyAIController.h"
#include "EnemyCharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core_GameState.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	senseSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sense Sight"));
	senseSightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	senseSightConfig->SightRadius = 1000;
	senseSightConfig->LoseSightRadius = 1200.f;
	senseSightConfig->SetMaxAge(1.f);
	senseSightConfig->PeripheralVisionAngleDegrees = 90.f;
	senseSightConfig->PointOfViewBackwardOffset = 300.f;
	senseSightConfig->NearClippingRadius = 50.f;  

	AIPerceptionComponent->ConfigureSense(*senseSightConfig);
	AIPerceptionComponent->SetDominantSense(senseSightConfig->GetSenseImplementation());

}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	bHasPlayerLost = false;

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BeginPlay: AI Perception Component NOT SET UP CORRECTLY WITHIN - %s"), *this->GetName());
	}

	AGameStateBase* gameState = UGameplayStatics::GetGameState(GetWorld());
	ACore_GameState* coreGameState = Cast<ACore_GameState>(gameState);
	if (!coreGameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay: CORE GAME STATE NOT CASTED CORRECTLY WITHIN - %s"), *this->GetName());
		return;
	}
	coreGameState->OnPlayerLostEvent.AddUObject(this, &AEnemyAIController::UpdatePlayerLost);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	//Asign the blackboard to the behaviour tree
	if (AEnemyCharacterBase* const Enemy = Cast<AEnemyCharacterBase>(InPawn))
	{
		cachedBehaviorTree = Enemy->GetBehaviourTree();
		//if (UBehaviorTree* const Tree = Enemy->GetBehaviourTree())
		if (cachedBehaviorTree)
		{
			bool bUseBBSuccess = UseBlackboard(cachedBehaviorTree->BlackboardAsset, cachedAIBlackboard);
			if (bUseBBSuccess && cachedAIBlackboard)
			{
				Blackboard = cachedAIBlackboard;
				RunBehaviorTree(cachedBehaviorTree);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnPossess: BLACKBOARD NULL IN ENEMY AI CONTROLLER - %s"), *this->GetName());
		}

		Enemy->OnDisableAIControllerEvent.BindUObject(this, &AEnemyAIController::DisableAIController);
	}

	SetIfPawnCanPerceive(true);
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* actor, FAIStimulus stimulus)
{

	if (!actor->Tags.Contains(FName("Player")))
	{
		//UE_LOG(LogTemp, Error, TEXT("OnTargetPerceptionUpdated: %s is not Player Actor"), *actor->GetName());
		return;
	}

	if (!cachedAIBlackboard || !Blackboard)
	{
		/*UE_LOG(LogTemp, Error, TEXT("OnTargetPerceptionUpdated: cachedAIBlackboard = %s"), cachedAIBlackboard ? TEXT("Valid") : TEXT("nullptr"));
		UE_LOG(LogTemp, Error, TEXT("OnTargetPerceptionUpdated: Blackboard = %s"), Blackboard ? TEXT("Valid") : TEXT("nullptr"));
		UE_LOG(LogTemp, Error, TEXT("OnTargetPerceptionUpdated: actor = %s"), *actor->GetName());*/

		return;
	}
	else
	{
	/*	UE_LOG(LogTemp, Display, TEXT("OnTargetPerceptionUpdated: cachedAIBlackboard = %s"), cachedAIBlackboard ? TEXT("Valid") : TEXT("nullptr"));
		UE_LOG(LogTemp, Display, TEXT("OnTargetPerceptionUpdated: Blackboard = %s"), Blackboard ? TEXT("Valid") : TEXT("nullptr"));
		UE_LOG(LogTemp, Display, TEXT("OnTargetPerceptionUpdated: actor = %s"), *actor->GetName());*/
	}

	if (stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Sensed = %s"), *actor->GetName());

		//Set the player actor var in the blackboard to the player
		if (!cachedAIBlackboard->GetValueAsObject("PlayerActor"))
		{
			cachedAIBlackboard->SetValueAsObject("PlayerActor", actor);
		}

		//Update blackboard to register that the enemy can see the player
		cachedAIBlackboard->SetValueAsBool("bCanSeePlayer", true);
	}
	else
	{
		//Update blackboard to register that the enemy can no longer see the player
		UE_LOG(LogTemp, Warning, TEXT("No longer Sense - %s"), *actor->GetName());
		cachedAIBlackboard->SetValueAsBool("bCanSeePlayer", false);
	}
}

void AEnemyAIController::DisableAIController()
{
	UBrainComponent* BrainComp = this->GetBrainComponent();
		if (BrainComp)
		{
			UBehaviorTreeComponent* BehaviorComp = Cast<UBehaviorTreeComponent>(BrainComp);
			if (BehaviorComp)
			{
				BehaviorComp->StopLogic("Enemy Is Disabled");
				BehaviorComp->Cleanup();
				cachedAIBlackboard->SetValueAsBool("bHasReachedNode", true);
			}
		}
		StopMovement();
		SetIfPawnCanPerceive(false);
}

void AEnemyAIController::SetIfPawnCanPerceive(bool bCanPercieve)
{
	if (bCanPercieve && !bHasPlayerLost)
	{
		AIPerceptionComponent->Activate();
		AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
		//UE_LOG(LogTemp, Warning, TEXT("SetIfPawnCanPerceive: AIPerceptionComponent is Enabled!"));
	}
	else
	{
		AIPerceptionComponent->Deactivate();
		AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
		if (cachedAIBlackboard)
		{
			cachedAIBlackboard->SetValueAsBool("bCanSeePlayer", false);
		}
		//UE_LOG(LogTemp, Warning, TEXT("SetIfPawnCanPerceive: AIPerceptionComponent is Disabled!"));
	}
}

void AEnemyAIController::UpdatePlayerLost()
{
	bHasPlayerLost = true;
	SetIfPawnCanPerceive(false);
}
