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

    // Calls when hero shots
    UFUNCTION(BlueprintCallable, Category = "Hero|Combat")
    void RegisterShot();

    // Calls when music beats
    UFUNCTION(BlueprintCallable, Category = "Hero|Combat")
    void Count_Combo();

    // Call with timer delay
    void ProcessCountCombo();

    // Current damage (including combo)
    UPROPERTY(BlueprintReadOnly, Category = "Hero|Combat")
    double Damage;

    // Flag for powered mode
    UPROPERTY(BlueprintReadOnly, Category = "Hero|Combat")
    bool IsReinforced;

    UPROPERTY(BlueprintReadWrite, Category = "Hero|Combat")
    bool IsFailed;

    UPROPERTY(BlueprintReadWrite, Category = "Hero|Combat")
    bool IsCanReinforced;

    // setting for hit frae and power combo
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
    // Services fields:
    double Timer;                  // Timer from las beat
    // Absolute time of shots
    TArray<double> ShotTimes;

    // Beat's moment we need to process
    TArray<double> PendingBeats;
};