#include "MyEnemiesAIController.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "MyProjectGameMode.h"
#include "MyAITrigger.h"
#include "Perception/AIPerceptionTypes.h"
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
	}
}

void AMyEnemiesAIController::BeginPlay() {
	Super::BeginPlay();

	_isActive = true;
	_isChasing = false;
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

	if (_isChasing && IsValid(_intruder)) {
		MoveToActor(_intruder);
	}
}

void AMyEnemiesAIController::OnMovementCompleted( FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!_isActive) {
		return;
	}
	if (_isChasing) {
		return;
	}

	Patrol();
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

void AMyEnemiesAIController::ActorsPerceptionUpdated(const TArray< AActor* >& UpdatedActors)
{
	if (IsValid(_intruder) && UpdatedActors.Contains(_intruder)) {
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(_character);
		GetWorld()->LineTraceSingleByChannel(HitResult, _character->GetActorLocation(),
			_intruder->GetActorLocation(), ECC_Visibility, Params);

		if (HitResult.GetActor() == _intruder) {
			_isChasing = true;
		}
		else {
			_isChasing = false;
			MoveToLocation(AIPerceptionComponent->GetActorInfo(*_intruder)->GetLastStimulusLocation());
		}

	}
	else {
		_isChasing = false;
		Patrol();
	}
}

void AMyEnemiesAIController::OnNoiseHeard(AActor* NoiseInstigator, const FVector& Location, float Volume)
{
	if (IsValid(_intruder) && _intruder == NoiseInstigator) {
		MoveToRandomPointInRadius(Location, researchRadius);
	}
}

void AMyEnemiesAIController::Patrol()
{
	MoveToRandomPointInRadius(_patrolPoint, patrolRange);
}

void AMyEnemiesAIController::MoveToRandomPointInRadius(const FVector& Taregt, float Radius)
{
	if (IsValid(_navSystem)) {
		FNavPathPoint newLocation;
		if (_navSystem->GetRandomReachablePointInRadius(_patrolPoint, patrolRange, newLocation)) {
			_targetPoint = newLocation.Location;
		}
	}

	MoveToLocation(_targetPoint);
}
