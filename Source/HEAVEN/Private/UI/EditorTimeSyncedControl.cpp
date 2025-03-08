#include "EditorTimeSyncedControl.h"

void UEditorTimeSyncedControl::SyncWithAudioTimeline(float CurrentPlaybackTime)
{
	// 时间轴精度计算
	float QuantizedTime = FMath::GridSnap(CurrentPlaybackTime, TimelineResolution);
	
	// 触发轨迹可视化更新
	Update3DTrajectoryVisualization(CalculateTrajectoryPoints(QuantizedTime));
}

TArray<FVector> UEditorTimeSyncedControl::CalculateTrajectoryPoints(float TimeStamp)
{
	TArray<FVector> Points;
	float BPM = GetAudioManager()->CurrentAudioMeta.BPM;
	float BeatDuration = 60.0f / BPM;

	// 生成螺旋轨迹点
	for(int i=0; i<10; i++)
	{
		float Theta = TimeStamp * 2 * PI + i * PI/5;
		float Radius = i * 50.0f;
		Points.Add(FVector(
			Radius * FMath::Cos(Theta),
			Radius * FMath::Sin(Theta),
			FMath::Sin(TimeStamp * 2 * PI) * 100.0f
		));
	}

	// 应用频谱数据
	TArray<float> Spectrum = GetAudioManager()->CurrentAudioMeta.SpectrumData;
	if(Spectrum.Num() > 0)
	{
		for(int i=0; i<Points.Num(); i++)
		{
			Points[i].Z += Spectrum[FMath::Min(i, Spectrum.Num()-1)] * 300.0f;
		}
	}

	return Points;
}