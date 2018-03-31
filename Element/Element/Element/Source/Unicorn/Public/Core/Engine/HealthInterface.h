// Copyright 2018 Team Unicorn All Rights Reserved

#pragma once

#include "Object.h"
#include "Interface.h"
#include "HealthInterface.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType)
class UNICORN_API UHealthInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

//Create a new class ILookable and use GENERATED_IINTERFACE_BODY() Makro in class body
class UNICORN_API IHealthInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool LoseHealth(float HealthLost, bool& bDied);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool GainHealth(float HealthGained);
};