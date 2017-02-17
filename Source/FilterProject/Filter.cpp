// Fill out your copyright notice in the Description page of Project Settings.

#include "FilterProject.h"
#include "Filter.h"

UFilter* UFilter::Create(EFilterTypes FilterType, float Rate, float CutFrequency, bool Adaptive)
{
  UFilter *Filter = NewObject<UFilter>();
  Filter->Initialize(FilterType, Rate, CutFrequency, Adaptive);
  return Filter;
}

void UFilter::Initialize(EFilterTypes FilterType, float Rate, float CutFrequency, bool Adaptive)
{
  MFilterType = FilterType;
  MRate = Rate;
  MCutFrequency = CutFrequency;
  IsAdaptive = Adaptive;

  double dt = 1.0f / MRate;
  double rc = 1.0f / MCutFrequency;

  switch (MFilterType) {
  case EFilterTypes::FilterHighPass:
    MFilterConstant = rc / (dt + rc);
    break;
  case EFilterTypes::FilterLowPass:
  default:
    MFilterConstant = dt / (dt + rc);
  }
}

void UFilter::CalculateVector(FVector Vector, FVector &Calculated)
{
  double Alpha = MFilterConstant;

  switch (MFilterType) {
  case EFilterTypes::FilterHighPass:
  {
    if (IsAdaptive)
    {
      float d = Clamp(fabs(Normal(MLastCalculated.X, MLastCalculated.Y, MLastCalculated.Z) - Normal(Vector.X, Vector.Y, Vector.Z)) / VectorMinStep - 1.0f, 0.0f, 1.0f);
      Alpha = (1.0f - d) * MFilterConstant / VectorNoiseAttenuation + d * MFilterConstant;
    }

    MLastCalculated.X = Vector.X * Alpha + MLastCalculated.X * (1.0f - Alpha);
    MLastCalculated.Y = Vector.Y * Alpha + MLastCalculated.Y * (1.0f - Alpha);
    MLastCalculated.Z = Vector.Z * Alpha + MLastCalculated.Z * (1.0f - Alpha);
  }
  break;
  case EFilterTypes::FilterLowPass:
  default:
  {
    if (IsAdaptive)
    {
      float d = Clamp(fabs(Normal(MLastCalculated.X, MLastCalculated.Y, MLastCalculated.Z) - Normal(Vector.X, Vector.Y, Vector.Z)) / VectorMinStep - 1.0f, 0.0f, 1.0f);
      Alpha = d * MFilterConstant / VectorNoiseAttenuation + (1.0f - d) * MFilterConstant;
    }

    MLastCalculated.X = Alpha * (MLastCalculated.X + Vector.X - MLastInput.X);
    MLastCalculated.Y = Alpha * (MLastCalculated.Y + Vector.Y - MLastInput.Y);
    MLastCalculated.Z = Alpha * (MLastCalculated.Z + Vector.Z - MLastInput.Z);

    MLastInput.X = Vector.X;
    MLastInput.Y = Vector.Y;
    MLastInput.Z = Vector.Z;
  }
  }

  Calculated = MLastCalculated;
}
