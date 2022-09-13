/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2021 Audiokinetic Inc.
*******************************************************************************/

#include "MyAudiFX.h"
#include "../MyAudiConfig.h"

#include <AK/AkWwiseSDKVersion.h>

AK::IAkPlugin* CreateMyAudiFX(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, MyAudiFX());
}

AK::IAkPluginParam* CreateMyAudiFXParams(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, MyAudiFXParams());
}

AK_IMPLEMENT_PLUGIN_FACTORY(MyAudiFX, AkPluginTypeEffect, MyAudiConfig::CompanyID, MyAudiConfig::PluginID)

MyAudiFX::MyAudiFX()
    : m_pParams(nullptr)
    , m_pAllocator(nullptr)
    , m_pContext(nullptr)
{
}

MyAudiFX::~MyAudiFX()
{
}

AKRESULT MyAudiFX::Init(
    AK::IAkPluginMemAlloc* in_pAllocator, 
    AK::IAkEffectPluginContext* in_pContext, 
    AK::IAkPluginParam* in_pParams, 
    AkAudioFormat& in_rFormat
)
{
    m_pParams = (MyAudiFXParams*)in_pParams;
    m_pAllocator = in_pAllocator;
    m_pContext = in_pContext;

    m_FXState.Setup(m_pParams, in_rFormat.uSampleRate);
    AKRESULT eResult = m_FXState.InitDelay(in_pAllocator, m_pParams, in_rFormat.channelConfig);
    m_FXState.ComputeTailLength(m_pParams->RTPC.bFeedbackEnabled, m_pParams->RTPC.fFeedback);
    m_pParams->NonRTPC.bHasChanged = false;
    m_pParams->RTPC.bHasChanged = false;

    AK_PERF_RECORDING_RESET();

    return AK_Success;
}

AKRESULT MyAudiFX::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    m_FXState.TermDelay(in_pAllocator);
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT MyAudiFX::Reset()
{
    m_FXState.ResetDelay();
    return AK_Success;
}

AKRESULT MyAudiFX::GetPluginInfo(AkPluginInfo& out_rPluginInfo)
{
    out_rPluginInfo.eType = AkPluginTypeEffect;
    out_rPluginInfo.bIsInPlace = true;
	out_rPluginInfo.bCanProcessObjects = false;
    out_rPluginInfo.uBuildVersion = AK_WWISESDK_VERSION_COMBINED;
    return AK_Success;

}

/*
void MyAudiFX::Execute(AkAudioBuffer* io_pBuffer)
{
    const AkUInt32 uNumChannels = io_pBuffer->NumChannels();

    AkUInt16 uFramesProcessed;
    for (AkUInt32 i = 0; i < uNumChannels; ++i)
    {
        AkReal32* AK_RESTRICT pBuf = (AkReal32 * AK_RESTRICT)io_pBuffer->GetChannel(i);

        uFramesProcessed = 0;
        while (uFramesProcessed < io_pBuffer->uValidFrames)
        {
            // Execute DSP in-place here
            pBuf[uFramesProcessed] = pBuf[uFramesProcessed] * AK_DBTOLIN(m_pParams->RTPC.fPlaceholder);
            ++uFramesProcessed;
        }
    }
}
*/

/// Effect plug-in DSP processing
void MyAudiFX::Execute(AkAudioBuffer* io_pBuffer)
{
    if (AK_EXPECT_FALSE(m_pParams->NonRTPC.bHasChanged))
    {
        AKRESULT eResult = m_FXState.InitDelay(m_pAllocator, m_pParams, io_pBuffer->GetChannelConfig());
        if (eResult != AK_Success)
            return; // passthrough
        m_FXState.ResetDelay();
        m_pParams->NonRTPC.bHasChanged = false;
    }

    if (AK_EXPECT_FALSE(m_pParams->RTPC.bHasChanged))
    {
        m_FXState.ComputeTailLength(m_pParams->RTPC.bFeedbackEnabled, m_pParams->RTPC.fFeedback);
        m_pParams->RTPC.bHasChanged = false;
    }

    AK_PERF_RECORDING_START("Delay", 25, 30);
    // Execute DSP processing synchronously here
    m_FXState.Process(io_pBuffer, m_pParams);
    AK_PERF_RECORDING_STOP("Delay", 25, 30);
}




AKRESULT MyAudiFX::TimeSkip(AkUInt32 in_uFrames)
{
    return AK_DataReady;
}
