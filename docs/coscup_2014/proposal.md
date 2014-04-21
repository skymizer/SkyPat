長版：
SkyPat是一個開源效能分析工具。透過整合Linux perf_event與單元測試，用戶僅需寫測試案例，就可以得到程式片段執行的cycle數，並排除了OS scheduler的影響。
如Compiler等對程式運行時間有低誤差容忍的程式，能夠排除程式閒置時間而取得實質的執行時間，藉此找出程式運行瓶頸。

SkyPat已經以New BSD License開源釋出，並已獲bold專案採納做為其效能檢測工具使用。

短版：（100字內）
SkyPat是一個開源效能分析工具，藉由perf_event與Unit-test，用戶僅需寫test-case，即可透過SkyPat得到程式片段在CPU上運行的指令數，排除OS scheduler的影響。使對運行時間有低誤差容忍的程式，能夠取得實質的執行時間，藉此找出程式運行瓶頸。
