//===- pat.cpp --------------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pat/pat.h>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <ios>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <algorithm>
#include <cstdio>
#include <time.h>

#include <pat/Config/Config.h>

using namespace pat;

//===----------------------------------------------------------------------===//
// Config
//===----------------------------------------------------------------------===//
/* Define if this is Unixish platform */
#define PAT_ON_UNIX 1

/* Define if this is Win32ish platform */
/* #undef PAT_ON_WIN32 */

/* Define if this is SkyDragon platform */
/* #undef PAT_ON_DRAGON */

/* Define the numebr of iteration of performance loop */
#define PAT_PERFORM_LOOP_TIMES 1

//===----------------------------------------------------------------------===//
// Support
//===----------------------------------------------------------------------===//
namespace pat {

template <class Container, typename Functor>
void ForEach(const Container& c, Functor functor) {
  std::for_each(c.begin(), c.end(), functor);
}

template<class C>
void* object_creator() {
  return new C();
}

template<typename T> struct object_deleter {
  static void call(void * Ptr) { delete (T*)Ptr; }
};

/// ManagedStaticBase - Common base class for ManagedStatic instances.
class ManagedStaticBase
{
public:
  bool isConstructed() const { return (NULL != m_Ptr); }

  void destroy() const;

protected:
  typedef void (*DeleterFuncType)(void*);

protected:
  // This should only be used as a static variable, which guarantees that this
  // will be zero initialized.
  mutable void* m_Ptr;
  mutable DeleterFuncType m_pDeleter;
  mutable const ManagedStaticBase* m_pNext;

  void RegisterManagedStatic(void *(*creator)(), void (*deleter)(void*)) const;

};

/** \class ManagedStatic
 *  \brief ManagedStatic changes the behavior of global static variables to
 *  be lazily constructed on demand and explicitly destructed by Shutdown()
 *  function call.
 */
template<class C>
class ManagedStatic : public ManagedStaticBase
{
public:

  // Accessors.
  C &operator*() {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);
    return *static_cast<C*>(m_Ptr);
  }

  const C &operator*() const {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);
    return *static_cast<C*>(m_Ptr);
  }

  C *operator->() {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);

    return static_cast<C*>(m_Ptr);
  }

  const C *operator->() const {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);

    return static_cast<C*>(m_Ptr);
  }
};

/// shutdown - Deallocate and destroy all ManagedStatic variables.
void shutdown();

/** \class Shutdown
 *  \brief Shutdown is a helper class that call shutdown() when it's destroyed.
 */
class Shutdown : private testing::internal::Uncopyable
{
public:
  Shutdown() { }

  ~Shutdown() { shutdown(); }
};

//===----------------------------------------------------------------------===//
// ManagedStatic Details
//===----------------------------------------------------------------------===//

static const ManagedStaticBase *StaticList = NULL;

//===----------------------------------------------------------------------===//
// ManagedStaticBase
//===----------------------------------------------------------------------===//
void ManagedStaticBase::RegisterManagedStatic(void *(*pCreator)(),
                                              void (*pDeleter)(void*)) const
{
  assert(NULL == m_Ptr && NULL == m_pDeleter && m_pNext == 0 &&
         "Partially initialized ManagedStatic!?");
  m_Ptr = pCreator ? pCreator() : NULL;
  m_pDeleter = pDeleter;

  // Add to list of managed statics.
  m_pNext = StaticList;
  StaticList = this;
}

void ManagedStaticBase::destroy() const
{
  assert(m_pDeleter && "ManagedStatic not initialized correctly!");
  assert(StaticList == this &&
         "Not destroyed in reverse order of construction?");
  // Unlink from list.
  StaticList = m_pNext;
  m_pNext = NULL;

  // Destroy memory.
  m_pDeleter(m_Ptr);

  // Cleanup.
  m_Ptr = NULL;
  m_pDeleter = NULL;
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
void shutdown()
{
  while (StaticList)
    StaticList->destroy();
}

//===----------------------------------------------------------------------===//
// Color
//===----------------------------------------------------------------------===//
/** \class Color
 *  \brief Color is a std::ostream manipulator for changing the color.
 */
class Color
{
public:
  // color order matches ANSI escape sequence, don't change
  enum kColor {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,

    RESET
  };

  enum kType {
    NORMAL,
    BOLD,
    BACKGROUND
  };

public:
  Color(kColor pColor = BLACK, kType pType = NORMAL)
    : m_Color(pColor), m_Type(pType)
  {}

  static Color Bold(kColor pColor) {
    return Color(pColor, BOLD);
  }
  static Color BackGround(kColor pColor) {
    return Color(pColor, BACKGROUND);
  }

  template<typename CharT, typename Traits>
  void setColor(::std::basic_ostream<CharT, Traits>& pOS) {
    pOS << "\033[";
    if (m_Color != RESET) {
      if (m_Type == BOLD) {
        pOS << "1;";
      }
      // TODO: Turn off bold mode;
      int base_color = m_Type == BACKGROUND ? 40 : 30;
      pOS << (base_color + m_Color);
    }
    pOS << 'm';
  }

private:
  kColor m_Color : 24;
  kType m_Type : 8;
};

template<typename CharT, typename Traits>
static inline
::std::basic_ostream<CharT, Traits>&
operator<<(::std::basic_ostream<CharT, Traits>& pOS, Color pColor) {
  pColor.setColor(pOS);
  return pOS;
}

template<typename CharT, typename Traits>
static inline
::std::basic_ostream<CharT, Traits>&
operator<<(::std::basic_ostream<CharT, Traits>& pOS, Color::kColor pColor) {
  Color(pColor).setColor(pOS);
  return pOS;
}

//===----------------------------------------------------------------------===//
// Stream Forwarding
//===----------------------------------------------------------------------===//
template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicOStream;

template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicFileBuf;

template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicOFStream;

template<typename CharT, typename Traits = std::char_traits<CharT>,
         typename Alloc = std::allocator<CharT> >
  class BasicOStringBuf;

template<typename CharT, typename Traits = std::char_traits<CharT>,
         typename Alloc = std::allocator<CharT> >
  class BasicOStrStream;

/// @defgroup I/O Forward Declaration
/// @{
typedef BasicOStream<char>    OStream;
typedef BasicFileBuf<char>    FileBuf;
typedef BasicOFStream<char>   OFStream;
typedef BasicOStringBuf<char> OStringBuf;
typedef BasicOStrStream<char> OStrStream;
/// @}

//===----------------------------------------------------------------------===//
// Support::OStrStream
//===----------------------------------------------------------------------===//
/** \class BasicOStringBuf
 *  \brief The actual work of input and output (for std::string)
 *
 *  Unlike std::basic_stringbuf, BasicOStringBuf doesn't own a string for the
 *  internal buffer. Otherwise, BasicOStringBuf uses the given string as the
 *  internal buffer. Every change to the BasicOStringBuf also changes the
 *  content of given string directly.
 */
template<typename CharT, typename Traits, typename Alloc>
class BasicOStringBuf : public std::basic_streambuf<CharT, Traits>
{
public:
  typedef CharT  char_type;
  typedef Traits traits_type;
  typedef Alloc  allocator_type;
  typedef typename traits_type::int_type    int_type;
  typedef typename traits_type::pos_type    pos_type;
  typedef typename traits_type::off_type    off_type;

  typedef std::basic_streambuf<char_type, traits_type> streambuf_type;
  typedef std::basic_string<char_type, Traits, Alloc>  string_type;
  typedef typename string_type::size_type              size_type;

public:
  explicit BasicOStringBuf(string_type& pStr,
                           std::ios_base::openmode pMode = std::ios_base::out);

  ~BasicOStringBuf() { }

  /// str - return a copy
  string_type str() const;

protected:
  void BasicOStringBufInit(std::ios_base::openmode pMode);

  void doSync(char_type* pBase, size_type pO);

  int_type overflow(int_type pC = traits_type::eof());

  std::streamsize xsputn(const char_type* pS, std::streamsize pN);

protected:
  std::ios_base::openmode m_Mode;
  string_type&            m_String;
};

/** \class BasicOStrStream
 *  \brief BasicOStrStream is an OStream that writes to an std::string.
 */
template<typename CharT, typename Traits, typename Alloc>
class BasicOStrStream : public std::basic_ostream<CharT, Traits>
{
public:
  typedef CharT  char_type;
  typedef Traits traits_type;
  typedef Alloc  allocator_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  // Non-standard types:
  typedef std::basic_string<CharT, Traits, Alloc> string_type;
  typedef BasicOStringBuf<CharT, Traits, Alloc> stringbuf_type;
  typedef std::basic_ostream<CharT, Traits> ostream_type;

public:
  explicit BasicOStrStream(string_type& pStr,
                           std::ios_base::openmode pMode = std::ios_base::out);

  /// Destructor - does nothing
  ~BasicOStrStream() {}

  stringbuf_type* rdbuf() const {
    return const_cast<stringbuf_type*>(&m_StringBuf);
  }

  /// str - get a copy of output stream
  string_type str() const { return m_StringBuf.str(); }

private:
  stringbuf_type m_StringBuf;
};

//===----------------------------------------------------------------------===//
// Support::OStrStream::Details
//===----------------------------------------------------------------------===//
// BasicOStringBuf
//===----------------------------------------------------------------------===//
template<typename CharT, typename Traits, typename Alloc>
BasicOStringBuf<CharT, Traits, Alloc>::BasicOStringBuf(string_type& pStr,
                                                  std::ios_base::openmode pMode)
  : streambuf_type(), m_Mode(), m_String(pStr) {
  BasicOStringBufInit(pMode);
}

template<typename CharT, typename Traits, typename Alloc> void
BasicOStringBuf<CharT, Traits, Alloc>::BasicOStringBufInit(
                                                  std::ios_base::openmode pMode)
{
  m_Mode = pMode;
  size_type len = 0;
  if (m_Mode & (std::ios_base::ate | std::ios_base::app))
    len = m_String.size();

  doSync(const_cast<char_type*>(m_String.data()), len);
}

/// doSync - Internal function for correctly updating the internal buffer
/// for particular m_String.
template<typename CharT, typename Traits, typename Alloc> void
BasicOStringBuf<CharT, Traits, Alloc>::doSync(char_type* pBase, size_type pO)
{
  char_type* endp = pBase + m_String.capacity();
  this->setp(pBase, endp);
  this->pbump(pO);
}

template<typename CharT, typename Traits, typename Alloc>
typename BasicOStringBuf<CharT, Traits, Alloc>::string_type
BasicOStringBuf<CharT, Traits, Alloc>::str() const
{
  return m_String;
}

template<typename CharT, typename Traits, typename Alloc>
typename BasicOStringBuf<CharT, Traits, Alloc>::int_type
BasicOStringBuf<CharT, Traits, Alloc>::overflow(int_type pC)
{
  const bool testout = this->m_Mode & std::ios_base::out;
  if (!testout)
    return traits_type::eof();

  const bool testeof = traits_type::eq_int_type(pC, traits_type::eof());
  if (testeof)
    return traits_type::not_eof(pC);

  const size_type capacity = m_String.capacity();
  const size_type max_size = m_String.max_size();
  const bool testput = this->pptr() < this->epptr();
  if (!testput && capacity == max_size)
    return traits_type::eof();

  // try to append pC into output sequence
  const char_type conv = traits_type::to_char_type(pC);
  if (!testput) {
    string_type tmp;
    // magic number. 512 looks like the best configuration on Mac OSX, 64 bits.
    const size_type opt_len = std::max(size_type(2 * capacity), size_type(512));
    tmp.reserve(std::min(opt_len, max_size));
    if (this->pbase())
      tmp.assign(this->pbase(), this->epptr() - this->pbase());
    tmp.push_back(conv);
    m_String.swap(tmp);
    doSync(const_cast<char_type*>(m_String.data()), this->pptr()-this->pbase());
  }
  else {
    // Although pptr >= epptr, we still have capacity.
    *this->pptr() = conv;
  }
  this->pbump(1);
  return pC;
}

template<typename CharT, typename Traits, typename Alloc> std::streamsize
BasicOStringBuf<CharT, Traits, Alloc>::xsputn(const char_type* pS,
                                              std::streamsize pN)
{
  std::streamsize result = 0;
  while (result < pN) {
    const std::streamsize buf_len = this->epptr() - this->pptr();
    if (buf_len) {
      const std::streamsize remaining = pN - result;
      const std::streamsize len = std::min(buf_len, remaining);
      // In order to correct m_String's bookkeeping, use append instead of
      // char_traits::copy.
      m_String.append(pS, len);
      result += len;
      pS += len;
      this->pbump(len);
    }

    if (result < pN) {
      int_type c = this->overflow(traits_type::to_int_type(*pS));
      if (!traits_type::eq_int_type(c, traits_type::eof())) {
        ++result;
        ++pS;
      }
      else
        break;
    }
  }
  return result;
}

//===----------------------------------------------------------------------===//
// BasicOStrStream
//===----------------------------------------------------------------------===//
template<typename CharT, typename Traits, typename Alloc>
BasicOStrStream<CharT, Traits, Alloc>::BasicOStrStream(string_type& pStr,
                                                  std::ios_base::openmode pMode)
  : ostream_type(), m_StringBuf(pStr, pMode | std::ios_base::out) {
  this->init(&m_StringBuf);
}

namespace testing {
namespace internal {

//===----------------------------------------------------------------------===//
// Timer
//===----------------------------------------------------------------------===//
class Timer
{
public:
  Timer();
  ~Timer();

  bool isActive() const { return m_bIsActive; }
  testing::Interval interval() const { return m_Interval; }

  void start();
  void stop();

  static std::string unit();

private:
  testing::Interval m_Interval;
  bool m_bIsActive;
};

//===----------------------------------------------------------------------===//
// Timer Implementation
//===----------------------------------------------------------------------===//
#if defined(PAT_ON_WIN32)
#include "Windows/Timer.inc"
#endif
#if defined(PAT_ON_UNIX)

#include <time.h>
#include <unistd.h>
#include <sys/times.h>

#if defined(HAVE_SYS_TIME_H) && defined(ENABLE_GETTIMEOFDAY)
#include <sys/time.h>
#endif

class TimerImpl
{
public:
  TimerImpl() {
    if (-1 == g_ClkTick) {
      g_ClkTick = sysconf(_SC_CLK_TCK);
      assert((0 < g_ClkTick) && "sysconf error");
    }
  }

   testing::Interval ns() {
#if defined(HAVE_CLOCK_GETTIME) && defined(ENABLE_CLOCK_GETTIME)
     struct timespec ts;
     int r = clock_gettime(CLOCK_MONOTONIC, &ts);
     return r == -1 ? -1 : ts.tv_sec * 1000000000LL + ts.tv_nsec;
#elif defined(HAVE_GETTIMEOFDAY) && defined(ENABLE_GETTIMEOFDAY)
     struct timeval tv;
     int r = gettimeofday(&tv, NULL);
     return r == -1 ? -1 : tv.tv_sec * 1000000000LL + (tv.tv_usec * 1000LL);
#else
     struct tms tm;
     clock_t r = times(&tm);
     return r == -1 ? -1 : r * 1000000000LL / g_ClkTick;
#endif
  }

  void start() {
    assert(-1 != (m_Start = ns()) && "fail to get starting time");
  }

  void stop() {
    assert(-1 != (m_End = ns()) && "fail to get elapsed time");
  }

  testing::Interval clock() const {
    return (m_End - m_Start);
  }

private:
  testing::Interval m_Start;
  testing::Interval m_End;

  static long g_ClkTick;
};

long TimerImpl::g_ClkTick = -1;

static ManagedStatic<TimerImpl> g_Timer;

Timer::Timer()
  : m_Interval(0), m_bIsActive(false) {
}

Timer::~Timer()
{
}

void Timer::start()
{
  m_bIsActive = true;
  g_Timer->start();
}

void Timer::stop()
{
  g_Timer->stop();
  m_bIsActive = false;
  m_Interval = g_Timer->clock();
}

std::string Timer::unit()
{
  return "ns";
}

#endif
#if defined(PAT_ON_DRAGON)
#include "Dragon/Timer.inc"
#endif

} // namespace of internal
} // namespace of testing
} // namespace of pat


//===----------------------------------------------------------------------===//
// Details of UnitTest
//===----------------------------------------------------------------------===//
testing::UnitTest::UnitTest()
  : m_pCurrentInfo(NULL), m_NumOfTests(0), m_NumOfFails(0) {
}

testing::UnitTest::~UnitTest()
{
  CaseMap::iterator iCase, iEnd = m_CaseMap.end();
  for (iCase = m_CaseMap.begin(); iCase != iEnd; ++iCase) {
    delete iCase->second;
  }
}

void testing::UnitTest::addTestInfo(const std::string& pCaseName,
                                    const std::string& pTestName,
                                    testing::TestFactoryBase& pFactory)
{
  CaseMap::iterator iCase = m_CaseMap.find(pCaseName);
  TestCase* test_case;
  if (iCase != m_CaseMap.end())
    test_case = iCase->second;
  else {
    test_case = new TestCase(pCaseName);
    m_CaseMap.insert(make_pair(pCaseName, test_case));
  }
  test_case->addTestInfo(pTestName, pFactory);
  ++m_NumOfTests;
}

void testing::UnitTest::addTestPartResult(const TestPartResult& pPartResult)
{
  m_pCurrentInfo->addTestPartResult(pPartResult);
  m_Repeater.OnTestPartResult(pPartResult);
  if (testing::TestPartResult::kSuccess != pPartResult.type())
    ++m_NumOfFails;
}

testing::PerfPartResult*
testing::UnitTest::addPerfPartResult(const char* pFile, int pLine)
{
  /* XXX: */
  m_Repeater.OnPerfPartResult(testing::PerfPartResult(pFile, pLine));
  return m_pCurrentInfo->addPerfPartResult(pFile, pLine);
}

void testing::UnitTest::RunAll()
{
  m_Repeater.OnTestProgramStart(*this);

  CaseMap::iterator iCase, iEnd = m_CaseMap.end();
  for (iCase = m_CaseMap.begin(); iCase != iEnd; ++iCase) {
    TestCase* test_case = iCase->second;
    m_Repeater.OnTestCaseStart(*test_case);

    TestCase::iterator it   = test_case->begin();
    TestCase::iterator iEnd = test_case->end();
    while (it != iEnd) {
      (*it)->run();
      ++it;
    }

    m_Repeater.OnTestCaseEnd(*test_case);
  }

  m_Repeater.OnTestProgramEnd(*this);
}

//===----------------------------------------------------------------------===//
// Non-member function
//===----------------------------------------------------------------------===//
testing::TestInfo*
testing::MakeAndRegisterTestInfo(const char* pCaseName, const char* pTestName,
                                 testing::TestFactoryBase* pFactory)
{
  testing::UnitTest::self()->addTestInfo(pCaseName, pTestName, *pFactory);
  return testing::UnitTest::self()->getCurrentInfo();
}

std::string testing::GetBoolAssertionFailureMessage(
    const pat::testing::AssertionResult& pAssertionResult,
    const char* pExpressionText,
    const char* pActualPredicateValue,
    const char* pExpectedPredicateValue)
{
  std::string result;
  OStrStream OS(result);
  OS << "Value of: " << pExpressionText
     << "\n  Actual:   " << pActualPredicateValue;
  if (pAssertionResult.hasMessage())
    OS << "(" << pAssertionResult.message() << ")";
  OS << "\n  Expected: " << pExpectedPredicateValue;
  return result;
}

//===----------------------------------------------------------------------===//
// PerfIterator
//===----------------------------------------------------------------------===//
testing::PerfIterator::PerfIterator(const char* pFile, int pLine)
  : m_Counter(-1) // -1 for the for-loop initial test.
{
  m_pPerfResult = testing::UnitTest::self()->addPerfPartResult(pFile, pLine);
  m_pTimer = new internal::Timer();
  m_pTimer->start();
}

testing::PerfIterator::~PerfIterator()
{
  m_pTimer->stop();
  m_pPerfResult->setPerformance(m_pTimer->interval());
  delete m_pTimer;
}

bool testing::PerfIterator::next()
{
  ++m_Counter;
  if (m_Counter >= PAT_PERFORM_LOOP_TIMES) {
    return false;
  }
  return true;
}

//===----------------------------------------------------------------------===//
// PartResult
//===----------------------------------------------------------------------===//
testing::PartResult::PartResult(const std::string& pFileName, int pLoC)
  : m_FileName(pFileName), m_LoC(pLoC), m_Message() {
}

testing::PartResult::PartResult(const std::string& pFileName,
                                int pLoC,
                                const std::string& pMessage)
  : m_FileName(pFileName), m_LoC(pLoC), m_Message(pMessage) {
}

//===----------------------------------------------------------------------===//
// TestPartResult
//===----------------------------------------------------------------------===//
testing::TestPartResult::TestPartResult(Type pType,
                                        const std::string& pFileName,
                                        int pLoC,
                                        const std::string& pMessage)
  : PartResult(pFileName, pLoC, pMessage), m_Type(pType) {
}

//===----------------------------------------------------------------------===//
// PerfPartResult
//===----------------------------------------------------------------------===//
testing::PerfPartResult::PerfPartResult(const std::string& pFileName,
                                        int pLoC)
  : PartResult(pFileName, pLoC) {
}

testing::Interval testing::PerfPartResult::getPerformance() const
{
  return m_PerfNum;
}

void testing::PerfPartResult::setPerformance(testing::Interval pNum)
{
  m_PerfNum = pNum;
  OStrStream os(m_Message);
  os << pNum << " ns";
}

//===----------------------------------------------------------------------===//
// TestResult
//===----------------------------------------------------------------------===//
testing::TestResult::TestResult(const TestInfo& pInfo)
  : m_Info(pInfo), m_Conclusion(kNotTested) {
}

testing::TestResult::~TestResult()
{
}

bool testing::TestResult::isPassed() const
{
  return (kPassed == m_Conclusion);
}

bool testing::TestResult::isFailed() const
{
  return (kFailed == m_Conclusion);
}

const testing::TestResult::Reliability& testing::TestResult::reliability() const
{
  return m_Info.getTestResults();
}

const testing::TestResult::Performance& testing::TestResult::performance() const
{
  return m_Info.getPerfResults();
}

//===----------------------------------------------------------------------===//
// TestCase
//===----------------------------------------------------------------------===//
testing::TestCase::TestCase(const std::string& pCaseName)
  : m_CaseName(pCaseName)
{
}

testing::TestCase::~TestCase()
{
  InfoList::iterator info, iEnd = m_InfoList.end();
  for (info = m_InfoList.begin(); info != iEnd; ++info) {
    delete (*info);
  }
}

void
testing::TestCase::addTestInfo(const std::string& pTestName,
                               testing::TestFactoryBase& pFactory)
{
  m_InfoList.push_back(new testing::TestInfo(this, pTestName, pFactory));
}

//===----------------------------------------------------------------------===//
// TestInfo
//===----------------------------------------------------------------------===//
testing::TestInfo::TestInfo(TestCase* pTestCase,
                            const std::string& pTestName,
                            testing::TestFactoryBase& pFactory)
  : m_pTestCase(pTestCase),
    m_TestName(pTestName),
    m_Result(*this),
    m_pFactory(&pFactory) {
}

testing::TestInfo::~TestInfo()
{
  delete m_pFactory;
  TestPartResultList::iterator tt, tEnd = m_TestResultList.end();
  for (tt = m_TestResultList.begin(); tt != tEnd; ++tt) {
    delete (*tt);
  }
  PerfPartResultList::iterator pt, pEnd = m_PerfResultList.end();
  for (pt = m_PerfResultList.begin(); pt != pEnd; ++pt) {
    delete (*pt);
  }
}

void testing::TestInfo::run()
{
  UnitTest& unittest = *UnitTest::self();
  Repeater& repeater = unittest.repeater();
  unittest.setCurrentInfo(*this);
  pat::Test* test = m_pFactory->CreateTest();
  if (NULL != test) {
    repeater.OnSetUpStart(unittest);
    test->SetUp();
    repeater.OnSetUpEnd(unittest);

    repeater.OnTestStart(*this);
    test->run();
    repeater.OnTestEnd(*this);

    repeater.OnTearDownStart(unittest);
    test->TearDown();
    repeater.OnTearDownEnd(unittest);
  }
  delete test;
}

void testing::TestInfo::addTestPartResult(const TestPartResult& pResult)
{
  if (m_TestResultList.empty()) {
    m_Result.setConclusion(testing::TestResult::kPassed);
  }

  if (testing::TestPartResult::kSuccess != pResult.type()) {
    m_Result.setConclusion(testing::TestResult::kFailed);
    m_TestResultList.push_back(new TestPartResult(pResult));
  }
}

testing::PerfPartResult*
testing::TestInfo::addPerfPartResult(const char* pFile, int pLine)
{
  PerfPartResult* perf_pr = new PerfPartResult(pFile, pLine);
  m_PerfResultList.push_back(perf_pr);
  return perf_pr;
}

//===----------------------------------------------------------------------===//
// AssertionResult
//===----------------------------------------------------------------------===//
pat::testing::AssertionResult::AssertionResult(const AssertionResult& pOther)
  : m_bSuccess(pOther.m_bSuccess), m_Message(pOther.m_Message) {
}

pat::testing::AssertionResult::AssertionResult(bool pSuccess)
  : m_bSuccess(pSuccess) {
}

pat::testing::AssertionResult
pat::testing::AssertionResult::operator!() const
{
  AssertionResult negative(!m_bSuccess);
  negative << m_Message;
  return negative;
}

template <typename T> pat::testing::AssertionResult&
pat::testing::AssertionResult::operator<<(const T& pValue)
{
  pat::OStrStream OS(m_Message);
  OS << pValue;
  return *this;
}

pat::testing::AssertionResult&
pat::testing::AssertionResult::operator<<(
                  ::std::ostream& (*basic_manipulator)(::std::ostream& stream))
{
  pat::OStrStream OS(m_Message);
  OS << basic_manipulator;
  return *this;
}

//===----------------------------------------------------------------------===//
// AssertHelper
//===----------------------------------------------------------------------===//
pat::testing::AssertHelper::AssertHelper(TestPartResult::Type pType,
                                              const std::string& pFile,
                                              int pLineOfCode,
                                              const std::string& pMessage)
  : m_Result(pType, pFile, pLineOfCode, pMessage) {
}

void pat::testing::AssertHelper::operator=(bool pValue) const
{
  UnitTest::self()->addTestPartResult(m_Result);
}

//===----------------------------------------------------------------------===//
// Log
//===----------------------------------------------------------------------===//
testing::Log::Log(Severity pSeverity,
                                const std::string& pFileName,
                                int pLoC)
  : m_Severity(pSeverity) {
  const char* const mesg =
    kInfo    == pSeverity ? "[  INFO ]" :
    kWarning == pSeverity ? "[WARNING]" :
    kError   == pSeverity ? "[ ERROR ]" : "[ FATAL ]";

  getOStream() << std::endl << mesg << FormatFileLocation(pFileName, pLoC)
               << ": ";
}

testing::Log::~Log()
{
  getOStream() << std::endl;
  if (kFatal == m_Severity) {
    shutdown();
    fflush(stderr);
    exit(1);
  }
}

::std::ostream& testing::Log::getOStream()
{
  return ::std::cerr;
}

std::string
testing::Log::FormatFileLocation(const std::string& pFileName, int pLoC)
{
  std::string result;
  OStrStream OS(result);
  if (pFileName.empty())
    OS << "(unknown file)";
  else
    OS << pFileName;

  if (pLoC < 0) {
    OS << ":";
    return result;
  }
#ifdef _MSC_VER
  OS << "(" << pLoC << "):";
#else
  OS << ":" << pLoC << ":";
#endif  // _MSC_VER
  return result;
}

//===----------------------------------------------------------------------===//
// Repeater
//===----------------------------------------------------------------------===//
testing::Repeater::Repeater()
  : m_bForward(true) {
}

testing::Repeater::~Repeater()
{
  // Do nothing. Since we arn't the owner of Listeners.
}

void testing::Repeater::add(Listener* pListener)
{
  if (NULL != pListener) {
    m_Listeners.push_back(pListener);
  }
}

void testing::Repeater::release(Listener& pListener)
{
  ListenerList::iterator listener, lEnd = m_Listeners.end();
  for (listener = m_Listeners.begin(); listener != lEnd; ++listener) {
    if (*listener == &pListener) {
      m_Listeners.erase(listener);
      return;
    }
  }
}

#define PAT_REPEATER_METHOD(Name, ParamType) \
void testing::Repeater::Name(const ParamType& pParam) { \
  if (m_bForward) { \
    ListenerList::iterator listener, lEnd = m_Listeners.end(); \
    for (listener = m_Listeners.begin(); listener != lEnd; ++listener) { \
      (*listener)->Name(pParam); \
    } \
  } \
}

PAT_REPEATER_METHOD(OnTestProgramStart, testing::UnitTest)
PAT_REPEATER_METHOD(OnTestCaseStart, TestCase)
PAT_REPEATER_METHOD(OnSetUpStart, UnitTest)
PAT_REPEATER_METHOD(OnSetUpEnd, UnitTest)
PAT_REPEATER_METHOD(OnTestStart, TestInfo)
PAT_REPEATER_METHOD(OnTestPartResult, TestPartResult)
PAT_REPEATER_METHOD(OnPerfPartResult, PerfPartResult)
PAT_REPEATER_METHOD(OnTestEnd, TestInfo)
PAT_REPEATER_METHOD(OnTearDownStart, UnitTest)
PAT_REPEATER_METHOD(OnTearDownEnd, UnitTest)
PAT_REPEATER_METHOD(OnTestCaseEnd, TestCase)
PAT_REPEATER_METHOD(OnTestProgramEnd, UnitTest)

//===----------------------------------------------------------------------===//
// PrettyResultPrinter
//===----------------------------------------------------------------------===//
class PrettyResultPrinter : public pat::testing::Listener
{
public:
  static void PrintCaseName(const std::string& pCase, const std::string& pTest)
  {
    testing::Log::getOStream() << pCase << "." << pTest;
  }

  void OnTestProgramStart(const testing::UnitTest& pUnitTest)
  {
    testing::Log::getOStream() << Color::CYAN << "[    pat   ] "
      << "Running " << pUnitTest.getNumOfTests()
      << " tests from " << pUnitTest.getNumOfCases() << " cases." << std::endl;
  }

  void OnTestCaseStart(const testing::TestCase& pTestCase)
  {
    testing::Log::getOStream() << Color::GREEN << "[----------] " << Color::YELLOW;
    int num_tests = pTestCase.getNumOfTests();
    if (1 == num_tests)
      testing::Log::getOStream() << "1 test from ";
    else if (1 < num_tests)
      testing::Log::getOStream() << num_tests << " tests from ";

    testing::Log::getOStream() << pTestCase.getCaseName() << "."
                                      << Color::RESET << std::endl;
  }

  void OnTestStart(const testing::TestInfo& pTestInfo)
  {
    testing::Log::getOStream() << Color::GREEN
        << "[ RUN      ] " << Color::WHITE;
    PrintCaseName(pTestInfo.getCaseName(), pTestInfo.getTestName());
    testing::Log::getOStream() << Color::RESET << std::endl;
  }

  void OnTestEnd(const testing::TestInfo& pTestInfo)
  {
    // unit-test results
    if (pTestInfo.result().isPassed()) {
      testing::Log::getOStream() << Color::GREEN
        << "[       OK ] \n";
    }
    if (pTestInfo.result().isFailed()) {
      testing::Log::getOStream() << Color::RED
        << "[  FAILED  ] ";
      testing::TestResult::Reliability::const_iterator test =
                                      pTestInfo.result().reliability().begin();
      testing::TestResult::Reliability::const_iterator tEnd =
                                      pTestInfo.result().reliability().end();
      while (test != tEnd) {

        testing::Log::getOStream() << "\n" << Color::Bold(Color::YELLOW)
                                          << (*test)->filename()
                                          << ':'
                                          << (*test)->lineNumber()
                                          << ": ";
        switch ((*test)->type()) {
          case testing::TestPartResult::kFatalFailure: {
            testing::Log::getOStream()
                               << Color::RED
                               << "fatal: "
                               << Color::RESET << Color::Bold(Color::YELLOW)
                               << "failed to assert\n";
            break;
          }
          case testing::TestPartResult::kNonFatalFailure: {
            testing::Log::getOStream()
                               << Color::MAGENTA
                               << "error: "
                               << Color::RESET << Color::Bold(Color::YELLOW)
                               << "failed to expect\n";
            break;
          }
          default:
            break;
        }

        testing::Log::getOStream() << Color::RESET
                                          << (*test)->message();
        ++test;
      }
      testing::Log::getOStream() << std::endl;
    }

    // performance test results
    if (!pTestInfo.result().performance().empty()) {
      testing::Log::getOStream() << Color::Bold(Color::BLUE)
          << "[ TIME (ns)]";

      testing::TestResult::Performance::const_iterator perf =
                                      pTestInfo.result().performance().begin();
      testing::TestResult::Performance::const_iterator pEnd =
                                      pTestInfo.result().performance().end();

      while (perf != pEnd) {
        testing::Log::getOStream() << " " << std::setw(7)
                                          << (*perf)->getPerformance();
        ++perf;
      }
      testing::Log::getOStream() << Color::RESET << std::endl;
    }
  }

  void OnTestProgramEnd(const testing::UnitTest& pUnitTest)
  {
    testing::Log::getOStream() << Color::CYAN << "[==========] "
                                      << pUnitTest.getNumOfTests();
    if (1 == pUnitTest.getNumOfTests())
      testing::Log::getOStream() << " test from ";
    if (1 < pUnitTest.getNumOfTests())
      testing::Log::getOStream() << " tests from ";

    testing::Log::getOStream() << pUnitTest.getNumOfCases()
                                      << " cases ran.\n";

    if (0 == pUnitTest.getNumOfFails()) {
      testing::Log::getOStream() << Color::GREEN << "[  PASSED  ] "
                                        << Color::RESET
                                        << pUnitTest.getNumOfTests();

      if (1 == pUnitTest.getNumOfTests()) {
        testing::Log::getOStream() << " test.\n"
                                          << std::flush;
      }

      if (1 < pUnitTest.getNumOfTests()) {
        testing::Log::getOStream() << " tests.\n"
                                          << std::flush;
      }
    }
    else {
      testing::Log::getOStream() << Color::RED << "[  FAILED  ] "
                                        << Color::RESET
                                        << pUnitTest.getNumOfFails();
      if (1 == pUnitTest.getNumOfFails()) {
        testing::Log::getOStream() << " test.\n"
                                          << std::flush;
      }

      if (1 < pUnitTest.getNumOfFails()) {
        testing::Log::getOStream() << " tests.\n"
                                          << std::flush;
      }
    }
  }
};

//===----------------------------------------------------------------------===//
// Test
//===----------------------------------------------------------------------===//
Test::~Test()
{
  // MUST KEEP THIS DESTRUCTOR
}

void Test::run()
{
  this->TestBody();
}

void Test::Initialize(int* pArgc, char* pArgv[])
{
  testing::UnitTest::self()->repeater().add(new PrettyResultPrinter());
}

void Test::RunAll()
{
  testing::UnitTest::self()->RunAll();
}

void Test::Sleep(int pMS)
{
  assert(pMS > 0 && "Cannot sleep zero milliseconds");

  struct timespec ts = { pMS / 1000, (pMS % 1000) * 1000 * 1000 };
  nanosleep(&ts, NULL);
}
