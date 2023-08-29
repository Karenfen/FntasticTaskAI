#include "MyEnemiesAIController.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "MyProjectGameMode.h"
#include "MyAITrigger.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"






AMyEnemiesAIController::AMyEnemiesAIController()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));

	if(IsValid(AIPerceptionComponent)) {
		if (IsValid(SightConfig)) {
			AIPerceptionComponent->ConfigureSense(*SightConfig);
			AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
		}
		if (IsValid(HearingConfig)) {
			AIPerceptionComponent->ConfigureSense(*HearingConfig);
		}
		SetPerceptionComponent(*AIPerceptionComponent);
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMyEnemiesAIController::OnTargetPerceptionUpdated);
	}
}

void AMyEnemiesAIController::BeginPlay() {
	Super::BeginPlay();

	_character = GetCharacter();

	if(IsValid(_character))	{
		_patrolPoint = _character->GetActorLocation();
		_targetPoint = _patrolPoint;
		_characterMovement = _character->GetCharacterMovement();
	}
	UWorld* world = GetWorld();
	GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AMyEnemiesAIController::OnMovementCompleted);

	if (IsValid(world)) {
		_navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);

		AMyProjectGameMode* gameMode = Cast<AMyProjectGameMode>(world->GetAuthGameMode());
		
		if (IsValid(gameMode)) {
			AMyAITrigger* triggerAI = Cast<AMyAITrigger>(gameMode->GetAITrigger());
			if (IsValid(triggerAI)) {
				triggerAI->OnActivated.AddUObject(this, &AMyEnemiesAIController::OnAIActivated);
				_isActive = false;
				return;
			}
		}
	}
	
	Patrol();
}

void AMyEnemiesAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_state == EAIControllerState::Chasing) {
		MoveToActor(_intruder);
	}
	else if (_state == EAIControllerState::Researching) {
		MoveToLocation(_targetPoint);
	}
}

void AMyEnemiesAIController::OnMovementCompleted( FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess()) {
		Patrol();
	}
}

void AMyEnemiesAIController::OnAIActivated(AActor* actor)
{
	_isActive = !_isActive;
	PrimaryActorTick.bCanEverTick = _isActive;
	_character->SetActorTickEnabled(_isActive);
	_character->SetActorHiddenInGame(!_isActive);
	
	if (_isActive){
		Patrol();
		_intruder = actor;
	} else {
		StopMovement();
		_character->SetActorLocation(_patrolPoint);
		_intruder = nullptr;
	}
}

void AMyEnemiesAIController::OnActorDetected(AActor* actor)
{
	if (!IsValid(_intruder) || _intruder != actor) {
		return;
	}

	if (_state == EAIControllerState::Chasing) {
		SetState(EAIControllerState::Researching);
		_targetPoint = AIPerceptionComponent->GetActorInfo(*actor)->GetLastStimulusLocation();
	}
	else {
		SetState(EAIControllerState::Chasing);
	}
}

void AMyEnemiesAIController::SetState(EAIControllerState newState)
{
	switch (newState)
	{
	case EAIControllerState::Chasing:
		_state = EAIControllerState::Chasing;
		if (IsValid(_characterMovement)) {
			_characterMovement->MaxWalkSpeed = chaseSpeed;
		}
		break;
	case EAIControllerState::Researching:
		_state = EAIControllerState::Researching;
		if (IsValid(_characterMovement)) {
			_characterMovement->MaxWalkSpeed = researchSpeed;
		}
		break;
	case EAIControllerState::Patrol:
		_state = EAIControllerState::Patrol;
		if (IsValid(_characterMovement)) {
			_characterMovement->MaxWalkSpeed = patrolSpeed;
		}
		break;
	default:
		break;
	}
}

void AMyEnemiesAIController::OnTargetPerceptionUpdated(AActor* actor, FAIStimulus simulus)
{
	if (simulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass())) {
		OnActorDetected(actor);
	}
	else if (simulus.Type == UAISense::GetSenseID(UAISense_Hearing::StaticClass())) {
		OnNoiseHeard(actor);
	}
}

void AMyEnemiesAIController::OnNoiseHeard(AActor* NoiseInstigator)
{
	if (!IsValid(_intruder) || _intruder != NoiseInstigator) {
		return;
	}

	if (_state == EAIControllerState::Patrol) {
		SetState(EAIControllerState::Researching);
		_targetPoint = GetRandomPointInRadius(_intruder->GetActorLocation(), researchRadius);
	}
}

void AMyEnemiesAIController::Patrol()
{
	SetState(EAIControllerState::Patrol);
	MoveToLocation(GetRandomPointInRadius(_patrolPoint, patrolRadius));
}

FVector AMyEnemiesAIController::GetRandomPointInRadius(const FVector& Taregt, float Radius)
{
	if (IsValid(_navSystem)) {
		FNavPathPoint newLocation;
		if (_navSystem->GetRandomReachablePointInRadius(Taregt, Radius, newLocation)) {
			return newLocation.Location;
		}
	}

	return _patrolPoint;
}