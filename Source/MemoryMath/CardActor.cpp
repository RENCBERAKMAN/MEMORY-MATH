#include "CardActor.h"
#include "Components/StaticMeshComponent.h"

ACardActor::ACardActor()
{
    PrimaryActorTick.bCanEverTick = true;

    CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CardMesh"));
    RootComponent = CardMesh;

    CardMesh->SetSimulatePhysics(false);
    CardMesh->SetEnableGravity(false);
    CardMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CardMesh->SetCollisionProfileName(TEXT("BlockAll"));

    // 👆 Tıklama Event'i tanımlanıyor
    CardMesh->OnClicked.AddDynamic(this, &ACardActor::OnCardClicked);

    bIsMatched = false;
    bIsFlipped = false;
    bIsRotating = false;
    RotationSpeed = 180.0f;
}

void ACardActor::BeginPlay()
{
    Super::BeginPlay();
}

void ACardActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsRotating)
    {
        FRotator CurrentRotation = GetActorRotation();
        FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

        SetActorRotation(NewRotation);

        if (NewRotation.Equals(TargetRotation, 1.0f))
        {
            bIsRotating = false;
        }
    }
}

void ACardActor::FlipCard()
{
    if (bIsMatched || bIsFlipped) return;

    TargetRotation = GetActorRotation() + FRotator(0, 180, 0);
    bIsRotating = true;
    bIsFlipped = true;
}

void ACardActor::UnflipCard()
{
    if (!bIsFlipped || bIsMatched) return;

    TargetRotation = GetActorRotation() - FRotator(0, 180, 0);
    bIsRotating = true;
    bIsFlipped = false;
}

void ACardActor::HideCard()
{
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    bIsMatched = true;
}

void ACardActor::OnCardClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
    if (!bIsMatched && !bIsRotating)
    {
        FlipCard();

        // 🔔 GameManager’a bu kartın seçildiğini bildiriyoruz
        OnCardClickedDelegate.Broadcast(this);
    }
}