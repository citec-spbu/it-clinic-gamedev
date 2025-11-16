#include "Hero_Base.h"
#include "GameFramework/Actor.h"

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
    Timer += DeltaTime; // ”величиваем таймер каждый кадр
}

void AHero_Base::RegisterShot()
{
    // ‘иксируем врем€ выстрела дл€ последующего анализа в Count_Combo
    ShotTimes.Add(Timer);
}

void AHero_Base::Count_Combo()
{
    // ќкно попадани€ в бит: [Timer - BeatWindow; Timer]
    double WindowStart = Timer - BeatWindow;
    double WindowEnd = Timer;

    if (IsCanReinforced) {

        bool ShotInWindow = true;
        //bool ShotOutsideWindow = false;

        // ѕроверим все выстрелы с прошлого бита: какие в окне, какие вне окна
        for (double ShotTime : ShotTimes)
        {
            if (!(ShotTime >= WindowStart && ShotTime <= WindowEnd)) {
                ShotInWindow = false;
                break;
            }
        }

        if (ShotTimes.IsEmpty())
        {
            // Ќет выстрелов в окне бита Ч урон падает
            Damage -= DamageStep;
            if (Damage < MinDamage)
                Damage = MinDamage;
            IsReinforced = false;
        }
        else if (!ShotInWindow)
        {
            // —брос комбо Ч выстрел был вне окна
            Damage = MinDamage;
            IsFailed = true;
            IsReinforced = false;
        }
        else if (ShotInWindow)
        {
            // ”спешное попадание по биту Ч увеличиваем урон
            Damage += DamageStep;
            if (Damage > MaxDamage)
                Damage = MaxDamage;
            IsReinforced = true;
        }


        // ќбновл€ем флаг усилени€: если текущий урон превышает порог Ч активен усиленный режим
        //IsReinforced = (Damage >= ReinforcedThreshold);
    }
    else {
        IsReinforced = false;
        Damage = MinDamage;
    }
    // ќчищаем массив выстрелов и сбрасываем таймер дл€ следующего бита
    ShotTimes.Empty();
    Timer = 0.0;
}
