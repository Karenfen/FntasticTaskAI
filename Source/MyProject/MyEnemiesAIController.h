// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyEnemiesAIController.generated.h"


UCLASS()
class MYPROJECT_API AMyEnemiesAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol|Speed")
	float patrolSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol|Range")
	float patrolRange = 2000.0f;	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void OnMovementCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	UFUNCTION()
	virtual void OnAIActivated(AActor* actor);

private:
	void Patrol();

private:
	class ACharacter* _character{ nullptr };
	class UCharacterMovementComponent* _characterMovement{ nullptr };
	class UNavigationSystemV1* _navSystem{ nullptr };
	FVector _patrolPoint;
	FVector _targetPoint;
	bool _isActive;
};
