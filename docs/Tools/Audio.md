> 一些音频相关知识内容

# 概念




---

# 一些异常情况

## 播放相关

> 当音频率采样率 > 播放器采样率

播放器“以为”数据是低采样率的，播放得 更快，音调也 变高。

例子：24kHz PCM 拿去按 16kHz 播放 → 声音加速、尖锐。

> 当音频流采样率 < 播放器采样率

播放器“以为”数据是高采样率的，播放得 更慢，音调也 变低。

例子：8kHz PCM 拿去按 16kHz 播放 → 声音拖长、低沉。

> 总结: 当音频流的采样率和播放器的采样率不同的时，以音频流采样率的与播放器采样率的差异，更大/更小，对应的播放速度，为更快，更慢。

---

> 音频流尾音截断

在用ALSA-API实现的播放器中，对于停止播放操作，可以优雅退出——即，播放完剩余音频再停止，或是，直接drop丢弃掉剩余音频流，停止播放。如下所示：

```bash
    // play the last audio clip
    if (!immediately_stop) {  // elegant end
    snd_pcm_drain(impl_->pcm_handle_);
    } else {  // end now
    snd_pcm_drop(impl_->pcm_handle_);
    }
```

---

## 音频丢帧

> 如何检测音频文件是否丢帧?

- sox 工具，看Max amplitude / Min amplitude 是否有异常为 0。

```bash
sox input.wav -n stat
```

- 用 Audacity / Sonic Visualiser 打开音频，看波形是否有“断层”。

