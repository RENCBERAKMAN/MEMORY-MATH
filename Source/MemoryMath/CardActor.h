#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardActor.generated.h"

// 💡 Delegate tanımı UCLASS sonrasına konur — bu doğru kabul edilen yöntemdir
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardClickedSignature, ACardActor*, ClickedCard);

UCLASS()
class MEMORYMATH_API ACardActor : public AActor
{
    GENERATED_BODY()

public:
    ACardActor();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    UPROPERTY(BlueprintAssignable)
    FCardClickedSignature OnCardClickedDelegate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* CardMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CardID;

    bool bIsMatched;
    bool bIsFlipped;
    FRotator TargetRotation;
    bool bIsRotating;
    float RotationSpeed;

    void FlipCard();
    void UnflipCard();
    void HideCard();

    UFUNCTION()
    void OnCardClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
};