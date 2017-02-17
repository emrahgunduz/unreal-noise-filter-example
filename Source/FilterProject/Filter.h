// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Filter.generated.h"

#define VectorMinStep             0.02f
#define VectorNoiseAttenuation    3.0f

UENUM(BlueprintType)
enum class EFilterTypes : uint8
{
  FilterLowPass UMETA(DisplayName = "Low Pass"),
  FilterHighPass UMETA(DisplayName = "High Pass")
};

UCLASS(Blueprintable, BlueprintType)
class FILTERPROJECT_API UFilter : public UObject
{
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintPure, Category = "Filters", Meta = (DisplayName = "Set Up A Filter"))
    static UFilter* Create(EFilterTypes FilterType, float Rate, float CutFrequency, bool Adaptive);

  UFUNCTION(BlueprintCallable, Category = "Filters", Meta = (DisplayName = "Calculate By Vector"))
    void CalculateVector(FVector Vector, FVector &Calculated);

private:
  EFilterTypes MFilterType;

  bool IsAdaptive;
  float MRate;
  float MCutFrequency;
  float MFilterConstant;

  FVector MLastCalculated;
  FVector MLastInput;

  float NLastCalculated;
  float NLastInput;

  void Initialize(EFilterTypes FilterType, float Rate, float CutFrequency, bool Adaptive);

  float Normal(float x, float y, float z)
  {
    return sqrt(x * x + y * y + z * z);
  };

  float Clamp(float v, float min, float max)
  {
    if (v > max) return max;
    else if (v < min) return min;
    else return v;
  };
};
