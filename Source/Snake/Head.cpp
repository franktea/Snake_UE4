// Fill out your copyright notice in the Description page of Project Settings.


#include "Head.h"
#include "Food.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AHead::AHead()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>("Head Box");
    Box->SetBoxExtent(FVector(24.0, 24.0, 24.0));
    RootComponent = Box;
    Box->bDynamicObstacle = true;
    //Box->bGenerateOverlapEvents = true;
    FScriptDelegate OverlapDelegate;
    OverlapDelegate.BindUFunction(this, "OnOverlapBegin");
    Box->OnComponentBeginOverlap.Add(OverlapDelegate);
    
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Head Mesh");
    Mesh->SetWorldRotation(FRotator(0, -90, 90.0));
    Mesh->SetupAttachment(Box);
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> HeadAsset(TEXT("StaticMesh'/Game/Meshes/head.head'"));
    if(HeadAsset.Succeeded())
    {
        Mesh->SetStaticMesh(HeadAsset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("can not load asset from StaticMesh'/Game/Meshes/head.head'"));
    }
}

// Called when the game starts or when spawned
void AHead::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("Height: %f"), GetActorLocation().Z);
	
    //Draw debug gird
    /*for(int i = -8; i <= 8; ++i)
    {
        FVector V1(50.0*i, -400, 100);
        FVector V2(50.0*i, 400, 100);
        DrawDebugLine(GetWorld(), V1, V2, FColor::Red, true, -1, 0, 2);
    }
    for(int i = -8; i <= 8; ++i)
    {
        FVector V1(-400, 50.0*i, 100);
        FVector V2(400, 50.0*i, 100);
        DrawDebugLine(GetWorld(), V1, V2, FColor::Green, true, -1, 0, 2);
    }*/
    
}

// Called every frame
void AHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if(bGameOver)
    {
        return;
    }

    TimeElapsed += DeltaTime;
    if(TimeElapsed >= CoolDown)
    {
        MoveForward();
        TimeElapsed = 0.0f;
    }
    
    FoodTimeElapsed += DeltaTime;
    if(FoodTimeElapsed >= FoodCoolDown)
    {
        MakeFood();
        FoodTimeElapsed = 0;
    }
}

// Called to bind functionality to input
void AHead::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("ToLeft", IE_Pressed, this, &AHead::TurnLeft);
    PlayerInputComponent->BindAction("ToRight", IE_Pressed, this, &AHead::TurnRight);
    PlayerInputComponent->BindAction("ToUp", IE_Pressed, this, &AHead::TurnUp);
    PlayerInputComponent->BindAction("ToDown", IE_Pressed, this, &AHead::TurnDown);
}

void AHead::TurnLeft()
{
    //UE_LOG(LogTemp, Warning, TEXT("Left"));
    FRotator R = GetActorRotation();
    if(R.Equals(FRotator(0, 0, 0)) || R.Equals(FRotator(0, 180, 0), 0.01))
    {
        SetActorRotation(FRotator(0, -90, 0));
    }
}

void AHead::TurnRight()
{
    //UE_LOG(LogTemp, Warning, TEXT("Right"));
    FRotator R = GetActorRotation();
    //UE_LOG(LogTemp, Warning, TEXT("now rotation: %s"), *R.ToString());
    if(R.Equals(FRotator(0, 0, 0), 0.01f) || R.Equals(FRotator(0, 180, 0), 0.01f))
    {
        SetActorRotation(FRotator(0, 90, 0));
    }}

void AHead::TurnDown()
{
    //UE_LOG(LogTemp, Warning, TEXT("Down"));
    FRotator R = GetActorRotation();
    if(R.Equals(FRotator(0, 90, 0), 0.01f) || R.Equals(FRotator(0, -90, 0), 0.01))
    {
        SetActorRotation(FRotator(0, 180, 0));
    }
}

void AHead::TurnUp()
{
    //UE_LOG(LogTemp, Warning, TEXT("Up"));
    FRotator R = GetActorRotation();
    if(R.Equals(FRotator(0, 90, 0), 0.01f) || R.Equals(FRotator(0, -90, 0), 0.01f))
    {
        SetActorRotation(FRotator(0, 0, 0));
    }
}

void AHead::MoveForward()
{
    OldLocation = GetActorLocation();
    
    FVector V = GetActorForwardVector() * 50;
    SetActorLocation(GetActorLocation() + V);
    
    if(Next)
    {
        Next->Move(OldLocation);
    }
}

void AHead::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("overlapped"));
    if(OtherActor && OtherActor != this && OtherComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("ok ok ok, name: %s"), *(OtherComp->GetFName().ToString()));
        AFood* food = Cast<AFood>(OtherComp);
        if(food)
        {
            UE_LOG(LogTemp, Warning, TEXT("overlap on food"));
            food->Destroy();
            NewTail();
            --FoodCount;
            return;
        }
        
        ATail* tail = Cast<ATail>(OtherComp);
        if(tail && !bGameOver)
        {
            UE_LOG(LogTemp, Warning, TEXT("collision on tail...."));
            bGameOver = true;
            FTimerHandle UnusedTimer;
            GetWorldTimerManager().SetTimer(
                UnusedTimer, this, &AHead::RestartGame, 3.0f, false);
        }
    }
}

void AHead::NewTail()
{
    if(!Next)
    {
        Next = Cast<ATail>(GetWorld()->SpawnActor(ATail::StaticClass(), &OldLocation));
    }
    else
    {
        Next->NewTail();
    }
}

void AHead::RestartGame()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AHead::MakeFood()
{
    if(FoodCount >= MaxFoodCount)
    {
        return;
    }
    
    int x = FMath::RandRange(-8, 8);
    int y = FMath::RandRange(-8, 8);
    
    FVector Location = FVector(x * 50.0, y * 50.0, 100.0);
    FRotator Rotation(0.0f);
    
    //GetWorld()->SpawnActor(AFood::StaticClass(), &Location, &Rotator);
    FActorSpawnParameters Parameters;
    Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
    AActor* food = GetWorld()->SpawnActor(AFood::StaticClass(),
        &Location, &Rotation, Parameters);
    if(!food)
    {
        UE_LOG(LogTemp, Warning, TEXT("failed to spawn food"));
    }
    else
    {
        ++FoodCount;
    }
}
