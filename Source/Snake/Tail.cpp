// Fill out your copyright notice in the Description page of Project Settings.


#include "Tail.h"
#include "Components/BoxComponent.h"

// Sets default values
ATail::ATail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>("Box");
    Box->SetBoxExtent(FVector(20.0, 20.0, 20.0));
    RootComponent = Box;
    
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    Mesh->SetupAttachment(Box);
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TailAsset(TEXT("StaticMesh'/Game/Meshes/cube.cube'"));
    if(TailAsset.Succeeded())
    {
        Mesh->SetStaticMesh(TailAsset.Object);
    }
}

// Called when the game starts or when spawned
void ATail::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATail::Move(FVector NewLocation)
{
    OldLocation = GetActorLocation();
    SetActorLocation(NewLocation);
    if(Next)
    {
        Next->Move(OldLocation);
    }
}

void ATail::NewTail()
{
    if(Next)
    {
        Next->NewTail();
    }
    else
    {
        Next = Cast<ATail>(GetWorld()->SpawnActor(ATail::StaticClass(), &OldLocation));
    }
}
