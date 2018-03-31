// Copyright 2018 Team Unicorn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnicornFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNICORN_API UUnicornFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Gets the aim location on target actor */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI|Vision")
	static FVector GetAimLocationOnActor(AActor* TargetActor, float VRAimLocationHeightMultiplier = 0.9f);
	
	
};
