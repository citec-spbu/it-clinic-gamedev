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
    Timer += DeltaTime; // Increase timer every frame
}

void AHero_Base::RegisterShot()
{
    double Now = UGameplayStatics::GetTimeSeconds(GetWorld());
        ShotTimes.Add(Now);
}

void AHero_Base::Count_Combo()
{
    if (!GetWorld()) {
        return;
    }
    if (!IsCanReinforced) {
        return;
    }

    // capture beat moment in current time (BeatTime)
    double BeatTimeNow = UGameplayStatics::GetTimeSeconds(GetWorld());
    PendingBeats.Add(BeatTimeNow);

    // set timer to call ProcessCountCombo after BeatWindow delay
    FTimerHandle ComboTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        ComboTimerHandle,
        this,
        &AHero_Base::ProcessCountCombo,
        BeatWindow,   // delay
        false
    );
}

void AHero_Base::ProcessCountCombo()
{
    if (!GetWorld()) return;

    double Now = UGameplayStatics::GetTimeSeconds(GetWorld());
    UE_LOG(LogTemp, Log, TEXT("[Hero] ProcessCountCombo called Now=%f Pending=%d Shots=%d"),
        Now, PendingBeats.Num(), ShotTimes.Num());
    // take all PendingBeats, which are not processed.
    TArray<double> BeatsToProcess = PendingBeats;
    PendingBeats.Empty(); // point it as "in process" to prevent repeated processing

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

        // delete from ShotTimes anythyng that was processed(<= right board)
        ShotTimes.RemoveAll([WindowEnd](double T) { return T <= WindowEnd; });
    }
}