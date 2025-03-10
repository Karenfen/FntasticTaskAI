// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProjectGameMode.generated.h"

UCLASS(minimalapi)
class AMyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyProjectGameMode();
	void SetAITrigger(AActor* trigger) { AItrigger = trigger; };
	AActor* GetAITrigger() { return AItrigger; };

private:
	AActor* AItrigger{ nullptr };
};



