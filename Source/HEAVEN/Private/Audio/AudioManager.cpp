#include "AudioManager.h"
#include "AudioDevice.h"
#include "Kismet/KismetMathLibrary.h"

void UAudioManager::AnalyzeAudio(const FString& FilePath)
{
    // 初始化音频分析模块
    FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice();
    
    // 加载音频文件并创建音效实例
    USoundWave* SoundWave = LoadObject<USoundWave>(nullptr, *FilePath);
    if(SoundWave)
    {
        // 创建音频组件并开始播放
        UAudioComponent* AudioComponent = AudioDevice->CreateComponent(SoundWave);
        AudioComponent->Play();

        // 初始化节拍检测参数
        CurrentAudioMeta.BPM = DetectAudioBPM(SoundWave);
        CurrentAudioMeta.AudioOffset = 0.0f;

        // 启动频谱分析定时器
        GetWorld()->GetTimerManager().SetTimer(SpectrumTimerHandle, this, &UAudioManager::CalculateSpectrum, 0.1f, true);
    }
}

void UAudioManager::RealTimePitchShift(float ShiftAmount)
{
    // 实现实时音高变换逻辑
    FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice();
    if(AudioDevice)
    {
        AudioDevice->SetGlobalPitchModulation(ShiftAmount);
    }
}

void UAudioManager::CalculateSpectrum()
{
    // 获取当前音频频谱数据
    FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice();
    if(AudioDevice)
    {
        TArray<float> SpectrumValues;
        AudioDevice->GetCurrentAudioSpectrum(SpectrumValues);

        // 更新元数据
        CurrentAudioMeta.SpectrumData = SpectrumValues;

        // 触发立方体表面光效更新事件
        OnSpectrumUpdated.Broadcast(SpectrumValues);
    }
}

float UAudioManager::DetectAudioBPM(USoundWave* SoundWave)
{
    // 获取音频原始数据
    const float* PCMData = SoundWave->RawData.GetData();
    int32 NumSamples = SoundWave->RawData.Num() / sizeof(float);

    // 初始化节拍检测参数
    float EnergyThreshold = 0.5f;
    float LastPeakTime = 0.0f;
    TArray<float> BeatIntervals;

    // 进行时域分析
    for(int32 i=0; i<NumSamples; i+=1024)
    {
        float WindowEnergy = 0.0f;
        for(int32 j=0; j<1024; ++j)
        {
            WindowEnergy += FMath::Abs(PCMData[i+j]);
        }
        WindowEnergy /= 1024.0f;

        // 检测能量峰值
        if(WindowEnergy > EnergyThreshold)
        {
            float CurrentTime = static_cast<float>(i) / SoundWave->SampleRate;
            if(LastPeakTime > 0.0f)
            {
                BeatIntervals.Add(CurrentTime - LastPeakTime);
            }
            LastPeakTime = CurrentTime;
        }
    }

    // 计算平均BPM
    if(BeatIntervals.Num() > 10)
    {
        float AverageInterval = 0.0f;
        for(float Interval : BeatIntervals)
        {
            AverageInterval += Interval;
        }
        AverageInterval /= BeatIntervals.Num();
        return 60.0f / AverageInterval;
    }
    
    return 120.0f;
}