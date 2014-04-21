長版：
本演講將介紹由我們團隊開發的開源效能分析工具：SkyPat。
透過整合Linux perf_event與單元測試，用戶僅需寫測試案例，即可透過SkyPat得到程式片段在CPU上運行的指令數，而排除了OS scheduler的影響。使Compiler等對程式運行時間有低誤差容忍的程式，能夠排除程序閒置期的時間而取得實質的執行時間，藉此找出程式運行瓶頸。

SkyPat已經以New BSD License開源釋出，並已獲bold專案採納做為其效能檢測工具使用。

短版：（100字內）
本演講將介紹一個開源效能分析工具：SkyPat。
藉由perf_event與Unit-test，用戶僅需寫test-case，即可透過SkyPat得到程式片段在CPU上運行的指令數，排除OS scheduler的影響。使對運行時間有低誤差容忍的程式，能夠取得實質的執行時間，藉此找出程式運行瓶頸。
