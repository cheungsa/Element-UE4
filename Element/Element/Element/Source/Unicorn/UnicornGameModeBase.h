// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnicornGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UNICORN_API AUnicornGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	virtual void StartPlay() override;

private:
	/** The AI manager spawned at the start of play */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	class AUnicornAIManager* AIManager;
public:
	AUnicornGameModeBase(const FObjectInitializer& ObjectInitializer);
	
	/** Getter for the AI manager*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	AUnicornAIManager* GetAIManager() const;
};
