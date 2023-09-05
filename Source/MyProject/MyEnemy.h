// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageTaker.h"
#include "MyEnemy.generated.h"

UCLASS()
class MYPROJECT_API AMyEnemy : public ACharacter, public IDamageTaker
{
	GENERATED_BODY()

	DECLARE_EVENT(AMyEnemy, FOnDie)
	DECLARE_EVENT_OneParam(AMyEnemy, FOnAttacked, FDamageData)

public:
	FOnDie OnDie;
	FOnAttacked OnAttacked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Range")
	float attackRange = 100.0f;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Damage")
	int attackDamage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Speed")
	float attackTimeout = 3.0f;

	FTimerHandle AttackTimerHandle;

public:
	// Sets default values for this character's properties
	AMyEnemy();
	UFUNCTION()
	void TakeDamage_(FDamageData DamageData) override;
	void Attack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void DamageTaked(FDamageData damageData);
	UFUNCTION()
	virtual void Die(FDamageData killer);

};
