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
#include "MyEnemy.h"
#include "components/GameStruct.h"






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

	_character = Cast<AMyEnemy>(GetCharacter());

	if(IsValid(_character))	{
		_patrolPoint = _character->GetActorLocation();
		_characterMovement = _character->GetCharacterMovement();
		_character->OnDie.AddUObject(this, &AMyEnemiesAIController::CharacterIsDead);
		_character->OnAttacked.AddUObject(this, &AMyEnemiesAIController::CharacterIsAttaked);
		_attackDistance = _character->attackRange * 0.7;
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
	if (!_isActive) {
		return;
	}

	Super::Tick(DeltaTime);

	if (_state == EAIControllerState::Chasing) {
		MoveToActor(_intruder, _attackDistance);
	}
}

void AMyEnemiesAIController::OnMovementCompleted( FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess()) {
		if(_state != EAIControllerState::Chasing) {
			Patrol();
		}
		else {
			Attack();
		}
	}
}

void AMyEnemiesAIController::OnAIActivated(AActor* actor)
{
	_isActive = !_isActive;
	PrimaryActorTick.bCanEverTick = _isActive;
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
		MoveToLocation(_intruder->GetActorLocation());
	}
	else {
		SetState(EAIControllerState::Chasing);
	}
}

void AMyEnemiesAIController::Attack()
{
	if (IsValid(_character)) {
		_character->Attack();
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

void AMyEnemiesAIController::CharacterIsDead()
{
	_isActive = false;
	PrimaryActorTick.bCanEverTick = false;
	StopMovement();
}

void AMyEnemiesAIController::CharacterIsAttaked(FDamageData damageData)
{
	if (_state == EAIControllerState::Chasing) {
		return;
	}
	if (IsValid(_characterMovement) && IsValid(damageData.Gunner)) {
		_characterMovement->MaxWalkSpeed = chaseSpeed;
		MoveToLocation(damageData.Gunner->GetActorLocation());
	}
}

void AMyEnemiesAIController::OnTargetPerceptionUpdated(AActor* actor, FAIStimulus simulus)
{
	if (!_isActive) {
		return;
	}

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
		MoveToLocation(GetRandomPointInRadius(_intruder->GetActorLocation(), researchRadius));
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