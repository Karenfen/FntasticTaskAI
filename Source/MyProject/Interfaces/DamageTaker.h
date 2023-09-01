// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageTaker.generated.h"


struct  FDamageData;


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageTaker : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT_API IDamageTaker
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void TakeDamage_(FDamageData DamageData) = 0;
};
