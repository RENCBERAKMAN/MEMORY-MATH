#include "CardGameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "CardActor.h"

ACardGameManager::ACardGameManager()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentPlayer = 0;
    PlayerAScore = 0;
    PlayerBScore = 0;
}

void ACardGameManager::BeginPlay()
{
    Super::BeginPlay();

    // 🃏 Sahnedeki kartları bul
    TArray<AActor*> FoundCards;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACardActor::StaticClass(), FoundCards);

    // 🔀 Konumları rastgele karıştır
    FRandomStream RandStream(FDateTime::Now().GetMillisecond());
    RandStream.GenerateNewSeed();
    for (int32 i = 0; i < FoundCards.Num(); ++i)
    {
        int32 SwapIndex = RandStream.RandRange(0, FoundCards.Num() - 1);
        FVector Temp = FoundCards[i]->GetActorLocation();
        FoundCards[i]->SetActorLocation(FoundCards[SwapIndex]->GetActorLocation());
        FoundCards[SwapIndex]->SetActorLocation(Temp);
    }

    // 🔗 Delegate bağlantısı kur
    for (AActor* Actor : FoundCards)
    {
        ACardActor* Card = Cast<ACardActor>(Actor);
        if (Card)
        {
            Card->OnCardClickedDelegate.AddDynamic(this, &ACardGameManager::OnCardSelected);
            SpawnedCards.Add(Card);
        }
    }

    // 🖼️ Skor panelini oluştur ve göster
    if (ScoreWidgetClass)
    {
        ScoreWidget = CreateWidget<UUserWidget>(GetWorld(), ScoreWidgetClass);
        if (ScoreWidget)
        {
            ScoreWidget->AddToViewport();
            UpdateScoreUI();
        }
    }
}

void ACardGameManager::OnCardSelected(ACardActor* SelectedCard)
{
    if (SelectedCards.Contains(SelectedCard) || SelectedCard->bIsMatched || SelectedCards.Num() >= 2)
        return;

    SelectedCard->FlipCard();
    SelectedCards.Add(SelectedCard);

    if (SelectedCards.Num() == 2)
    {
        GetWorld()->GetTimerManager().SetTimer(MatchDelayHandle, this, &ACardGameManager::CheckMatch, 1.0f, false);
    }
}

void ACardGameManager::CheckMatch()
{
    if (SelectedCards.Num() < 2) return;

    ACardActor* CardA = SelectedCards[0];
    ACardActor* CardB = SelectedCards[1];

    if (CardA && CardB && CardA->CardID + CardB->CardID == 0)
    {
        CardA->HideCard();
        CardB->HideCard();

        if (CurrentPlayer == 0) PlayerAScore++;
        else PlayerBScore++;

        UpdateScoreUI();
    }
    else
    {
        CardA->UnflipCard();
        CardB->UnflipCard();
        SwitchTurn();
    }

    SelectedCards.Empty();
}

void ACardGameManager::SwitchTurn()
{
    CurrentPlayer = (CurrentPlayer == 0) ? 1 : 0;
}

void ACardGameManager::UpdateScoreUI()
{
    if (!ScoreWidget) return;

    UTextBlock* Player1Text = Cast<UTextBlock>(ScoreWidget->GetWidgetFromName("TextBlock_1"));
    UTextBlock* Player2Text = Cast<UTextBlock>(ScoreWidget->GetWidgetFromName("TextBlock_2"));

    if (Player1Text)
        Player1Text->SetText(FText::FromString(FString::Printf(TEXT("Player A: %d"), PlayerAScore)));

    if (Player2Text)
        Player2Text->SetText(FText::FromString(FString::Printf(TEXT("Player B: %d"), PlayerBScore)));
}