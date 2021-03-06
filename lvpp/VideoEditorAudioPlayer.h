/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef VE_AUDIO_PLAYER_H_

#define VE_AUDIO_PLAYER_H_

#include <media/MediaPlayerInterface.h>
#include <media/stagefright/MediaBuffer.h>
#include <media/stagefright/TimeSource.h>
#include <utils/threads.h>
#include "M4xVSS_API.h"
#include "VideoEditorMain.h"
#include "M4OSA_FileReader.h"
#include "VideoEditorBGAudioProcessing.h"
#include "AudioPlayerBase.h"
#include "PreviewPlayerBase.h"

namespace android {

class MediaSource;
class AudioTrack;
class PreviewPlayer;


class VideoEditorAudioPlayer : public AudioPlayerBase {
public:
    enum {
        REACHED_EOS,
        SEEK_COMPLETE
    };

    VideoEditorAudioPlayer(const sp<MediaPlayerBase::AudioSink> &audioSink,
        PreviewPlayerBase *audioObserver = NULL);

    virtual ~VideoEditorAudioPlayer();

    status_t start(bool sourceAlreadyStarted = false);
    void resume();

    void setAudioMixSettings(M4xVSS_AudioMixingSettings* pAudioMixSettings);
    void setAudioMixPCMFileHandle(M4OSA_Context pBGAudioPCMFileHandle);
    void setAudioMixStoryBoardSkimTimeStamp(
        M4OSA_UInt32 pBGAudioStoryBoardSkimTimeStamp,
        M4OSA_UInt32 pBGAudioCurrentMediaBeginCutTS,
        M4OSA_UInt32 pBGAudioCurrentMediaVolumeVal);

    void setObserver(PreviewPlayerBase *observer);
    void setSource(const sp<MediaSource> &source);
    sp<MediaSource> getSource();

    bool isStarted();
private:

    M4xVSS_AudioMixingSettings *mAudioMixSettings;
    VideoEditorBGAudioProcessing *mAudioProcess;

    M4OSA_Context mBGAudioPCMFileHandle;
    int64_t mBGAudioPCMFileLength;
    int64_t mBGAudioPCMFileTrimmedLength;
    int64_t mBGAudioPCMFileDuration;
    int64_t mBGAudioPCMFileSeekPoint;
    int64_t mBGAudioPCMFileOriginalSeekPoint;
    int64_t mBGAudioStoryBoardSkimTimeStamp;
    int64_t mBGAudioStoryBoardCurrentMediaBeginCutTS;
    int64_t mBGAudioStoryBoardCurrentMediaVolumeVal;

    size_t fillBuffer(void *data, size_t size);

    void reset();
    void setPrimaryTrackVolume(M4OSA_Int16 *data, M4OSA_UInt32 size, M4OSA_Float volLevel);

    static size_t AudioSinkCallback(
            MediaPlayerBase::AudioSink *audioSink,
            void *data, size_t size, void *me);

    VideoEditorAudioPlayer(const VideoEditorAudioPlayer &);
    VideoEditorAudioPlayer &operator=(const VideoEditorAudioPlayer &);
};

}  // namespace android

#endif  // VE_AUDIO_PLAYER_H_
