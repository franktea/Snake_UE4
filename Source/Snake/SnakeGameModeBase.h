// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Head.h"
#include "SnakeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE_API ASnakeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
    ASnakeGameModeBase();
    
    virtual void BeginPlay() override;
};
