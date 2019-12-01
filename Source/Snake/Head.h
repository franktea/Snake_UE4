// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tail.h"
#include "Head.generated.h"

UCLASS()
class SNAKE_API AHead : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    void RestartGame();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void TurnLeft();
    void TurnRight();
    void TurnDown();
    void TurnUp();
    
    UPROPERTY(EditAnywhere)
    class UBoxComponent* Box;
    
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Mesh;
    
    UFUNCTION()
    void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
private:
    bool bGameOver = false;
    float CoolDown = 0.5f;
    float TimeElapsed = 0.0f;
    
    const int MaxFoodCount = 5;
    float FoodCoolDown = 3.0f;
    float FoodTimeElapsed = 0.0f;
    int FoodCount = 0;
    
    ATail* Next = nullptr;
    FVector OldLocation;
    
    void MoveForward();
    void NewTail();
    
    void MakeFood();
};
