#include "MyEnemiesAIController.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemiesAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"




void AMyEnemiesAIController::BeginPlay() {
	Super::BeginPlay();

	_character = GetCharacter();

	if(IsValid(_character))	{
		_patrolPoint = _character->GetActorLocation();
		_targetPoint = _patrolPoint;
		_characterMovement = _character->GetCharacterMovement();
	}

	_navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AMyEnemiesAIController::OnMovementCompleted);
	Patrol();
}

void AMyEnemiesAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyEnemiesAIController::OnMovementCompleted( FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Patrol();
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
