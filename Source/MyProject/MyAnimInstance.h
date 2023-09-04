// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(UMyAnimInstance, FOnPlaySound, FName)

public:
	FOnPlaySound OnPlaySound;

protected:
	virtual bool HandleNotify(const FAnimNotifyEvent& AnimNotifyEvent) override;
};
