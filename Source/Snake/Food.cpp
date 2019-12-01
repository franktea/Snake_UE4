// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BoxComponent.h"
#include "Food.h"

// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

//    Box = CreateDefaultSubobject<UBoxComponent>("Food Box");
//    RootComponent = Box;
//    Box->SetBoxExtent(FVector(24.9, 24.9, 24.9));
    
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Food Mesh");
    RootComponent = Mesh;
    //Mesh->SetupAttachment(Box);
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FoodAsset(TEXT("StaticMesh'/Game/Meshes/food.food'"));
    if(FoodAsset.Succeeded())
    {
        Mesh->SetStaticMesh(FoodAsset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("can not load food asset from file"));
    }
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

