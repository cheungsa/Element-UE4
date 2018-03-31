// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthInterface.h"

UHealthInterface::UHealthInterface(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

bool IHealthInterface::LoseHealth_Implementation(float HealthLost, bool& bDied)
{
	return false;
}

bool IHealthInterface::GainHealth_Implementation(float HealthGained)
{
	return false;
}
