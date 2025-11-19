#include "Hero_Base.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

AHero_Base::AHero_Base()
{
    PrimaryActorTick.bCanEverTick = true;

    Damage = 10.0;
    Timer = 0.0;
    IsReinforced = false;
    IsCanReinforced = true;
    IsFailed = false;
}

void AHero_Base::BeginPlay()
{
    Super::BeginPlay();
    Timer = 0.0;
    ShotTimes.Empty();
}

void AHero_Base::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    Timer += DeltaTime; // Увеличиваем таймер каждый кадр
}

void AHero_Base::RegisterShot()
{
    double Now = UGameplayStatics::GetTimeSeconds(GetWorld());
        ShotTimes.Add(Now);
}

void AHero_Base::Count_Combo()
{
    if (!GetWorld()) return;

    // захватываем момент бита именно сейчас (BeatTime)
    double BeatTimeNow = UGameplayStatics::GetTimeSeconds(GetWorld());
    PendingBeats.Add(BeatTimeNow);

    // ставим таймер, который вызовет ProcessCountCombo через задержку BeatWindow
    FTimerHandle ComboTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        ComboTimerHandle,
        this,
        &AHero_Base::ProcessCountCombo,
        BeatWindow,  // задержка
        false
    );
}

void AHero_Base::ProcessCountCombo()
{
    if (!GetWorld()) return;

    double Now = UGameplayStatics::GetTimeSeconds(GetWorld());
    UE_LOG(LogTemp, Log, TEXT("[Hero] ProcessCountCombo called Now=%f Pending=%d Shots=%d"),
        Now, PendingBeats.Num(), ShotTimes.Num());
    // Соберём все PendingBeats, которые ещё не обработаны.
    // (Поскольку мы ставим таймер на каждый scheduled beat, обычно они все уже готовы.)
    TArray<double> BeatsToProcess = PendingBeats;
    PendingBeats.Empty(); // помечаем их как "в обработке/удаляем" — предотвратит повторную обработку

    for (double BeatTime : BeatsToProcess)
    {
        double WindowStart = BeatTime - BeatWindow;
        double WindowEnd = BeatTime + BeatWindow;

        bool ShotInWindow = false;
        for (double ShotTime : ShotTimes)
        {
            if (ShotTime >= WindowStart && ShotTime <= WindowEnd)
            {
                ShotInWindow = true;
            }
            else {
                ShotInWindow = false;
            }
            
        }

        if (!ShotInWindow)
        {
            if (ShotTimes.Num() == 0)
            {
                Damage -= 2*DamageStep;
                if (Damage < MinDamage) Damage = MinDamage;
            }
            else
            {
                Damage = MinDamage;
                IsFailed = true;
            }
            IsReinforced = false;
        }
        else
        {
            Damage += DamageStep;
            if (Damage > MaxDamage) Damage = MaxDamage;
            IsReinforced = true;
            IsFailed = false;
        }

        // удаляем из ShotTimes всё, что уже обработано (<= правой границы)
        ShotTimes.RemoveAll([WindowEnd](double T) { return T <= WindowEnd; });
    }
}