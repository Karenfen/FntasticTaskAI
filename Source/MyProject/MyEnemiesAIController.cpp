#include "MyEnemiesAIController.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemiesAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "MyProjectGameMode.h"
#include "MyAITrigger.h"






void AMyEnemiesAIController::BeginPlay() {
	Super::BeginPlay();

	_isActive = true;
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
}

void AMyEnemiesAIController::OnMovementCompleted( FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!_isActive) {
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
	} else {
		StopMovement();
		_character->SetActorLocation(_patrolPoint);
	}
}

void AMyEnemiesAIController::Patrol()
{
	if (IsValid(_characterMovement)) {
		_characterMovement->MaxWalkSpeed = patrolSpeed;
	}

	if (IsValid(_navSystem)) {
		FNavPathPoint newLocation;
		if (_navSystem->GetRandomReachablePointInRadius(_patrolPoint, patrolRange, newLocation)) {
			_targetPoint = newLocation.Location;
		}
	}

	MoveToLocation(_targetPoint);
}
