// Copyright 2018 Team Unicorn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UnicornMaterialParameters.generated.h"

/**
 * 
 */
UCLASS()
class UNICORN_API UUnicornMaterialParameters : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Material Parameters")
	static const FName Opacity();

	UFUNCTION(BlueprintPure, Category = "Material Parameters")
	static const FName Emissive();

	UFUNCTION(BlueprintPure, Category = "Material Parameters")
	static const FName FringeSize();
	
	UFUNCTION(BlueprintPure, Category = "Material Parameters")
	static const FName Color();
};
