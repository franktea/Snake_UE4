// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tail.generated.h"

UCLASS()
class SNAKE_API ATail : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    
    UPROPERTY(EditAnywhere)
    class UBoxComponent* Box;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Mesh;
    
    ATail* Next = nullptr;
    
public:
    FVector OldLocation;
    
    void Move(FVector NewLocation);
    void NewTail();
};
