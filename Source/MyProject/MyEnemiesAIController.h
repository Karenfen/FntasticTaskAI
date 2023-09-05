// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MyEnemiesAIController.generated.h"

struct FDamageData;


UENUM(BlueprintType)
enum class EAIControllerState : uint8
{
	Chasing,
	Researching,
	Patrol
};

UCLASS()
class MYPROJECT_API AMyEnemiesAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol|Radius")
	float patrolRadius = 2000.0f;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Research|Radius")
	float researchRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Speed|Research")
	float researchSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Speed|Chase")
	float chaseSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Speed|Patrol")
	float patrolSpeed = 300.0f;


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
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* actor, FAIStimulus simulus);

private:
	void Patrol();
	FVector GetRandomPointInRadius(const FVector& Taregt, float Radius);
	void OnMovementCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	virtual void OnAIActivated(AActor* actor);
	void OnNoiseHeard(AActor* NoiseInstigator);
	void OnActorDetected(AActor* actor);
	void Attack();
	void SetState(EAIControllerState newState);
	void CharacterIsDead();
	void CharacterIsAttaked(FDamageData damageData);

private:
	class AMyEnemy* _character { nullptr };
	class UCharacterMovementComponent* _characterMovement { nullptr };
	class UNavigationSystemV1* _navSystem { nullptr };
	FVector _patrolPoint;
	AActor* _intruder { nullptr };
	bool _isActive{ true };
	EAIControllerState _state{ EAIControllerState::Patrol };
	float _attackDistance;
};
