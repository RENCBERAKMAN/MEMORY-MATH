#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardActor.h"               // Delegate ve sýnýf tanýmý
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CardGameManager.generated.h"

UCLASS()
class MEMORYMATH_API ACardGameManager : public AActor
{
    GENERATED_BODY()

public:
    ACardGameManager();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere)
    TSubclassOf<ACardActor> CardBlueprint;

    UPROPERTY(EditAnywhere)
    TArray<FVector> CardSpawnPoints;

    TArray<int32> CardIDs;
    TArray<ACardActor*> SpawnedCards;
    TArray<ACardActor*> SelectedCards;

    int32 CurrentPlayer;
    int32 PlayerAScore;
    int32 PlayerBScore;

    FTimerHandle MatchDelayHandle;

    // Widget baðlantýsý
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UUserWidget> ScoreWidgetClass;

    UUserWidget* ScoreWidget;

    void SpawnCards();
    void OnCardSelected(ACardActor* SelectedCard);
    void CheckMatch();
    void SwitchTurn();
    void UpdateScoreUI();
};