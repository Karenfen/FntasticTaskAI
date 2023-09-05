// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/MyCharacterI.h"

#include "MyEnemy.generated.h"

UCLASS()
class MYPROJECT_API AMyEnemy : public AMyCharacterI
{
	GENERATED_BODY()

	DECLARE_EVENT(AMyEnemy, FOnDie)
	DECLARE_EVENT_OneParam(AMyEnemy, FOnAttacked, FDamageData)

public:
	FOnDie OnDie;
	FOnAttacked OnAttacked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Range")
	float attackRange = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Damage")
	int attackDamage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Speed")
	float attackTimeout = 3.0f;

	FTimerHandle AttackTimerHandle;

public:
	// Sets default values for this character's properties
	AMyEnemy();
	UFUNCTION()
	void Attack();

protected:
	virtual void DamageTaked(FDamageData damageData) override;
	virtual void Die(FDamageData killer) override;

};
