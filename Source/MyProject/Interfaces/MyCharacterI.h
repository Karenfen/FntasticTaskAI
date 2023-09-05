// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageTaker.h"
#include "../components/GameStruct.h"

#include "MyCharacterI.generated.h"

UCLASS()
class MYPROJECT_API AMyCharacterI : public ACharacter, public IDamageTaker
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

public:
	AMyCharacterI();
	UFUNCTION()
	void TakeDamage_(FDamageData DamageData) override;

protected:
	UFUNCTION()
	virtual void DamageTaked(FDamageData damageData);
	UFUNCTION()
	virtual void Die(FDamageData data);
};
