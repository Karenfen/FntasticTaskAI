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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol|Range")
	float patrolRange = 2000.0f;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Research|Radius")
	float researchRadius = 1000.0f;

	// perceptions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	class UAISenseConfig_Hearing* HearingConfig;

public:
	AMyEnemiesAIController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void OnMovementCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	UFUNCTION()
	virtual void OnAIActivated(AActor* actor);
	UFUNCTION()
	virtual void ActorsPerceptionUpdated(const TArray< AActor* >& UpdatedActors) override;
	UFUNCTION()
	void OnNoiseHeard(AActor* NoiseInstigator, const FVector& Location, float Volume);

private:
	void Patrol();
	void MoveToRandomPointInRadius(const FVector& Taregt, float Radius);

private:
	class ACharacter* _character{ nullptr };
	class UCharacterMovementComponent* _characterMovement{ nullptr };
	class UNavigationSystemV1* _navSystem{ nullptr };
	FVector _patrolPoint;
	FVector _targetPoint;
	bool _isActive;
	AActor* _intruder{ nullptr };
	bool _isChasing;
};
