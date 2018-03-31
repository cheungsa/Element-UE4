// Copyright 2018 Team Unicorn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UnicornBBKeys.generated.h"

/**
 * 
 */
UCLASS()
class UNICORN_API UUnicornBBKeys : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "AI|Blackboard Keys")
	static const FName IsPassive();

	UFUNCTION(BlueprintPure, Category = "AI|Blackboard Keys")
	static const FName TargetActor();
	
	UFUNCTION(BlueprintPure, Category = "AI|Blackboard Keys")
	static const FName GoalLocation();

	UFUNCTION(BlueprintPure, Category = "AI|Blackboard Keys")
	static const FName TargetRecentlyTeleported();

	UFUNCTION(BlueprintPure, Category = "AI|Blackboard Keys")
	static const FName Stunned();
};
