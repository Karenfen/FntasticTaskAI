// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyAITrigger.generated.h"

class AMyProjectCharacter;
class UBoxComponent;

UCLASS()
class MYPROJECT_API AMyAITrigger : public AActor
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(AMyAITrigger, FOnActivated, AActor*)

public:
	FOnActivated OnActivated;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* TriggerCollider;
	void BeginPlay() override;
	
public:	
	// Sets default values for this actor's properties
	AMyAITrigger();

protected:
	UFUNCTION()
	void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const	FHitResult& SweepResult);

};
