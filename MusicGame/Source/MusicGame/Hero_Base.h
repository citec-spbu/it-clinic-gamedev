#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Hero_Base.generated.h"

UCLASS()
class MUSICGAME_API AHero_Base : public APaperZDCharacter
{
    GENERATED_BODY()

public:
    AHero_Base();

    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    // Вызывать при выстреле (например, из блюпринта или кода)
    UFUNCTION(BlueprintCallable, Category = "Hero|Combat")
    void RegisterShot();

    // Вызывать на каждый бит музыки
    UFUNCTION(BlueprintCallable, Category = "Hero|Combat")
    void Count_Combo();

    // Вызывать через таймер — считает попадания в окно
    void ProcessCountCombo();

    // Текущий урон (с учётом комбо)
    UPROPERTY(BlueprintReadOnly, Category = "Hero|Combat")
    double Damage;

    // Флаг усиленного режима по текущему урону
    UPROPERTY(BlueprintReadOnly, Category = "Hero|Combat")
    bool IsReinforced;

    UPROPERTY(BlueprintReadWrite, Category = "Hero|Combat")
    bool IsFailed;

    UPROPERTY(BlueprintReadWrite, Category = "Hero|Combat")
    bool IsCanReinforced;

    // Настройки окна попадания и сил комбо
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Settings")
    double BeatWindow = 0.19;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Settings")
    double DamageStep = 15.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Settings")
    double MinDamage = 10.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Settings")
    double MaxDamage = 70.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Settings")
    double ReinforcedThreshold = 15.0;

private:
    // Сервисные поля:
    double Timer;                  // Таймер с предыдущего бита
    // Абсолютные времена выстрелов (GetTimeSeconds)
    TArray<double> ShotTimes;

    // Моменты битов, которые нужно обработать (хранятся в момент ScheduleCountCombo)
    TArray<double> PendingBeats;
};