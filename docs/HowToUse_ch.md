這份文件用於說明怎麼用Skypat來測試你的程式與評估它的效能。 

### 介紹

SkyPat 用於 C++ 效能分析與提供一個在Android與Linux平台上效能評估與測試框架。
這個專案主要源自 Google Test 的觀念，並新增效能測試的框架

有了 SkyPat ，想要分析程式的程式設計者只要透過 SkyPat API 就可以寫自己的測試，並且同時分析程式表現的正確性及效能。

接下來為 SkyPat 使用上的範例。

### 基礎觀念

基本的 SkyPat 操作說明

1. 使用 _ASSERT_, _EXPECT_, and _PERFORM_ 巨集來撰寫測試
2. 使用 SkyPat 的動態函式庫以編譯你的測試程式
3. 最後執行便可獲得其正確性與效能的報告

與其他的測試函式庫（像 Google Test）一樣，Skypat 提供不少巨集以整合你的測試。下個部份將會介紹各個巨集以及解釋該如何使用這些巨集 

### 巨集

SkyPat 總共有三類巨集，分別是 _ASSERT_，_EXPECT_，跟 _PERFORM_。
_ASSERT_，_EXPECT_ 提供在條件判斷上的 assertion(中譯為「斷言」)，而 _PERFORM_ 可以對一個程式段落做效能分析。_ASSERT_ 與 _EXPECT_ 的差別僅在於程式遇到錯誤是否會中斷執行。

_ASSERT_ 是針對測試中嚴重的錯誤處理，如果 _ASSERT_ 條件判斷失敗，整個測試案例將會中斷並馬上離開。

_EXPECT_ 則是負責那些不是那麼嚴重，尚可容忍的錯誤。當 _EXPECT_ 的條件失敗時，程式並不會因此中斷，Skypat 會把錯誤訊息記錄下來並繼續執行，並且此時整個測試案例還不算是失敗

_PERFORM_ 負責評估程式碼的效能，測試一段程式碼內的性能表現

下個部份將提供你怎麼使用這些巨集的範例程式，並解釋程式輸出的意義

### 範例

以下所有範例程式碼也存在於 SkyPat 的程式中, _${PAT_SRC}/examples_

#### 引入 SkyPat 函式庫

讓我們從這個做常用的巨集 _ASSERT_ 開始。

所有的巨集以及定義都位於 <pat/pat.h>

舉個例子，在 `assertion/main.cpp`，它包含

      #include "pat/pat.h"

#### 透過 _PAT\_F_ 宣告測試

_PAT\_F_ 巨集用以宣告一個測試
該測試需要兩個參數，「測試案例的名字」與「測試函式名字」

在 SkyPat 架構邏輯中，整個測試流程會被分成不同的測試案例，而每個測試案例又由測試函式組成。

以下是一個宣告測試函式的基本範例

    PAT_F(AircraftCase, take_off_test)
    {
       // Your Test Code
    }
    PAT_F(AircraftCase, landing_test)
    {
       // Your Test Code
    }

這個範例定義了一個名叫 _AircraftCase_ 的測試案例，其中由兩個測試函式，_take\_off\_test_ 與 _landing\_test_ 組成

這就是 SkyPat 測試邏輯的架構
建議您可以將邏輯上相關連的測試函式放入同一個測試案例中

#### 撰寫一個測試函式

藉由撰寫測試函式，使用者可以評估該程式的效能與可信度
可信度的評估可透過在一段程式中使用 _ASSERT_ 或 _EXPECT_
效能評估則是使用 _PERFORM_ 巨集
下個部份將會解釋：
  * 該如何插入一個致命的條件判斷(fatal condition)，用以偵測嚴重的錯誤
  * 該如何插入多個致命的條件判斷
  * 該如何插入不致命的條件判斷，用以處理不嚴重錯誤
  * 評估效能 

##### 插入一個致命的條件判斷
下列案例展示你該如何使用 _ASSERT\_TRUE_ 與 _ASSERT\_FALSE_ 以撰寫測試函式

這是一個 _ASSERT\_TRUE_ :

    PAT_F(SeriesCase, fibonacci_test)
    {
      int result = 0;
    
      ASSERT_TRUE(0 != fibonacci(10));
    }

_ASSERT_ 是一個類似函式的巨集，透過使用假設(assertion)來測試一個類別(class)或函式(function) 的行為
如上所述，_ASSERT_總共有兩種， _ASSERT\_TRUE_ 與 _ASSERT\_FALSE_

_ASSERT\_TRUE_ 只會在條件判斷是 TRUE 的情況下通過，而在這個例子裡，條件判斷為 "0 != fibonacci(10)" 很明顯為 TRUE，因此成功通過此假設
反之，若 _ASSERT\_TRUE_ 條件判斷失敗，此測試案例立即中斷並且離開

這裡是一個 FALSE 的條件判斷

Here is an example to show a false condition:

    PAT_F(SeriesCase, factorial_test_fail)
    {
      ASSERT_TRUE(360 == factorial(5));
    
      cout<<"end of the test"<<endl;
    }

因為 `360 == factorial(5)` 這個條件判斷也很明顯是 FALSE，這也代表 `ASSERT_TRUE(360 == factorial(5))` 也失敗了。因此這個測試失敗，你可以在終端看到這樣的訊息：

    [ RUN      ] MyCase.factorial_test_fail
    [  FAILED  ] 
    main.cpp:62: fatal: failed to assert
    Value of: 360 == factorial(5)
      Actual:   false
      Expected: true

因為 _ASSERT\_TRUE_ 失敗，此次測試中止並立即離開，因此並沒有執行的程式`cout<<"end of the test"<<endl`

_ASSERT\_FALSE_ 類似 _ASSERT\_TRUE_，不過該測試會在測試條件不成立的時候才通過

    PAT_F(SeriesCase, factorial_test)
    {
      ASSERT_FALSE(360 == factorial(5));
    }

在這個例子裡，`360 == factorial(5)`明顯不符合條件，因此測試通過

在你的測試案例中，你可以任意改變判斷條件
SkyPat 提供了許多的測試巨集，包含 「條件類假設」 - _ASSERT\_TRUE/FALSE 與 「預測類假設」 - _ASSERT\_EQ/ NE/ LE/ GT/ GE

##### 插入了多個致命的條件判斷

如果你有多於一個致命條件，你也可以在一個測試案例中使用複數個 _ASSERT_ 巨集
不過只要一個致命條件發生錯誤，整個測試案例將會中止並且立即離開，剩餘的程式將不會被執行

這是一個複數 _ASSERT_ 的範例

    PAT_F(MyCase, fibonacci_test)
    {
      ASSERT_TRUE(0 != fibonacci(10));
      ASSERT_TRUE(2 == fibonacci(10));
      ASSERT_TRUE(0 != fibonacci(10));
    }

因為`2 == fibonacci(10)`明顯不成立， `ASSERT_TRUE(2 == fibonacci(10))`自然也就隨之失敗，你也可以在終端機看到下列輸出

    [ RUN      ] MyCase.fibonacci_test
    [  FAILED  ] 
    main.cpp:53: fatal: failed to assert
    Value of: 2 == fibonacci(10)
      Actual:   false
      Expected: true

因為 `ASSERT_TRUE(2 == fibonacci(10))`失敗導致程式中止，後續程式碼將不會被執行

##### 插入非致命的條件

_EXPECT_ 是一種非致命的假設，當_EXPECT_失敗，失敗的原因顯示在終端機上並繼續執行

SkyPat 總共有兩類 _EXPECT_ 巨集，_EXPECT\_TRUE_ 與 _EXPECT\_FALSE_. _EXPECT\_TRUE_ 只有在成立時才能成功，反之亦然
與 _ASSERT_ 類似，您也可以在一個測試案例中插入複數個 _EXPECT_

這是一個 _EXPECT_ 使用範例：

    PAT_F(MyCase, fibonacci_test)
    {
      EXPECT_TRUE(0 != fibonacci(10));
      EXPECT_TRUE(2 == fibonacci(10));
      EXPECT_TRUE(10 == fibonacci(10));
      EXPECT_FALSE(10 != fibonacci(10));
    }

在這個範例中種共有三個非致命假設，`EXPECT_TRUE(2 == fibonacci(10))`, `EXPECT_TRUE(10 == fibonacci(10))`, 與 `EXPECT_FALSE(10 != fibonacci(10))`失敗，但是程式會繼續執行並在螢幕上展現出這三個錯誤訊息

    [ RUN      ] MyCase.fibonacci_test
    [  FAILED  ] 
    main.cpp:53: error: failed to expect
    Value of: 2 == fibonacci(10)
      Actual:   false
      Expected: true
    main.cpp:54: error: failed to expect
    Value of: 10 == fibonacci(10)
      Actual:   false
      Expected: true
    main.cpp:55: error: failed to expect
    Value of: 10 != fibonacci(10)
      Actual:   true
      Expected: false

##### 效能評估

使用 _PERFORM_ 巨集可以測量一段程式的效能表現
下面為 _PERFROM_ 的使用範例:

    PERFORM(pat::CPU_CLOCK) {
      fibonacci(10);
    }

在這個範例中， _PERFORM_ 巨集測量執行`fibonacci(10)`與上下文交換的執行時間

_SkyPat_ 顯示出時間資訊如下

    [ RUN      ] MyCase.fibonacci_test
    [ TIME (ns)]         1063
    [EVENT TYPE] [CPU  CLOCK]
    [RESULT NUM]         1086

以下是另外一個 _PERFORM_ 範例

    PAT_F(MyCase, fibonacci_test)
    {
      PERFORM(pat::CONTEXT_SWITCHES) {
        fibonacci(10);
      } 
      PERFORM(pat::CPU_CLOCK) {
        fibonacci(10);
      } 
      PERFORM(pat::TASK_CLOCK) {
        fibonacci(10);
      } 
      PERFORM(pat::CPU_CYCLES) {
        fibonacci(10);
      } 
      PERFORM(pat::INSTRUCTIONS) {
        fibonacci(10);
      } 
    }

以上範例表示一個測試案例可以有複數個 _PERFORM_ 巨集監測不同的事件
這代表你可以在一個測試案例裡觀察執行時間，與監測事件的數值

以下分別是顯示在螢幕上的資訊

    [ RUN      ] MyCase.fibonacci_test
    [ TIME (ns)]         4380         1950         1577         3279         2548
    [EVENT TYPE] [CTX SWITCH] [CPU  CLOCK] [TASK CLOCK] [CPU CYCLES] [INST   NUM]
    [RESULT NUM]            0         1718         1505         3269         4338

你同時也可以透過 _PERFORM_ 巨集參數關鍵字去監測不同事件
以下是全部 SkyPat _PERFORM_ 關鍵字：

|  SkyPat PERFORM | 關鍵字 | 
|:-----------------:|:------------------:|
|  CPU_CYCLES  | CPU指令週期數 | 
|  INSTRUCTIONS | 關鍵字 | 
|  CACHE_REFERENCES | 快取命中 | 
|  CACHE_MISSES | 快取未命中 | 
|  BRANCH_INSTRUCTIONS | 分支指令 | 
|  BRANCH_MISSES | 分支預測失敗 | 
|  BUS_CYCLES | 匯流排用時 |
|  STALLED_CYCLES_FRONTEND | Pipeline前端停止CPU週期數 |
|  STALLED_CYCLES_BACKEND | Pipeline後端停止CPU週期數 |
|  REF_CPU_CYCLES | 總CPU週期數 |
|  CPU_CLOCK | CPU時脈 |
|  TASK_CLOCK | 程式CPU執行時間 |
|  PAGE_FAULTS | 分頁缺失 |
|  CONTEXT_SWITCHES | 上下文交換 |
|  CPU_MIGRATIONS | CPU遷移 |
|  PAGE_FAULTS_MIN | 最小分頁缺失 |
|  PAGE_FAULTS_MAJ | 最大分頁缺失 |
|  ALIGNMENT_FAULTS | 記憶體非對齊 |
|  EMULATION_FAULTS | 模擬錯誤 |


#### 透過`RunAll()`測試所有函式

呼叫 `Initialize(&argc, argv)` 與 `RunAll()`. 

    int main(int argc, char* argv[])
    {
      pat::Test::Initialize(&argc, argv);
      pat::Test::RunAll();
    }

SkyPat 使用 `Initialize(&argc, argv)` 初始化輸出界面
也就是說，如果您不呼叫 `Initialize`，你變無法看到 SkyPat 輸出

`RunAll()` 可以神奇的抓到所有的測試案例
`RunAll()` 測試所有的案例, 並且輸出結果
並只有在成功執行時，程式才會傳回0給 c-runtime，反之則回傳 1