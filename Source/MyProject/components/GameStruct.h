#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameStruct.generated.h"

class AActor;


UCLASS()
class MYPROJECT_API UDamageStruct : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT()
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY()
	int DamageValue;

	UPROPERTY()
	AActor* Gun;

	UPROPERTY()
	AActor* Gunner;
};