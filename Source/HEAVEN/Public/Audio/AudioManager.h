#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AudioManager.generated.h"

// 音频元数据结构（蓝图可访问）
USTRUCT(BlueprintType)
struct FAudioMetaData
{
	GENERATED_BODY()

	// 基础节拍数（单位：拍/分钟）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BPM = 120.0f;

	// 音频同步偏移量（单位：秒，用于校准时间）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AudioOffset = 0.0f;

	// 实时频谱数据数组（用于可视化分析）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> SpectrumData;
};

// 音频管理组件（负责音频分析与实时处理）
UCLASS(meta=(BlueprintSpawnableComponent))
class HEAVEN_API UAudioManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// 音频文件分析入口（支持外部文件路径）
	UFUNCTION(BlueprintCallable, Category="Audio Analysis")
	void AnalyzeAudio(const FString& FilePath);

	// 实时音高偏移处理（ShiftAmount：半音程偏移量，正值为升调，负值为降调）
	UFUNCTION(BlueprintCallable, Category="Audio Processing")
	void RealTimePitchShift(float ShiftAmount);

	// 当前音频元数据（蓝图可访问和编辑）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio Settings")
	FAudioMetaData CurrentAudioMeta;

private:
	// 节拍检测核心算法实现
	void ProcessBeatDetection();
	
	// 频谱计算核心逻辑
	void CalculateSpectrum();
};