#pragma once

// һ���򵥵ĵ�Ԫ���Կ�ܣ������������� TestCase �� UnitTest���Լ�һϵ�����ڲ��Եĺ�

#include <ctime>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

namespace mystl
{
	namespace test
	{
#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#endif

#define LOG_GREEN(str) std::cout<<"\033[32m\033[1m"<<str<<"\033[0m"
#define LOG_RED(str) std::cout<<"\033[31m\033[1m"<<str<<"\033[0m"
		// TestCase ��
		// ��װ�������԰���
		class TestCase
		{
		public:
			TestCase(const char* case_name):testcase_name(case_name){ }

			virtual void Run() = 0;
			
			const char* testcase_name;
			int TestResult;
			double nFailed;
			double nPassed;
		};

		// UnitTest ��
		// ��Ԫ���ԣ������в��԰������뵽 vector �У�����ִ�в��԰���
		class UnitTest
		{
		public:
			static UnitTest* GetInstance();

			TestCase* RegisterTestCase(TestCase* testcase);

			void Run();

			TestCase* CurTestCase;
			double nFailed, nPassed;

		protected:
			std::vector<TestCase*> testcases;
		};

		UnitTest* UnitTest::GetInstance()
		{
			static UnitTest instance;
			return &instance;
		}

		TestCase* UnitTest::RegisterTestCase(TestCase* testcase)
		{
			testcases.emplace_back(testcase);
			return testcase;
		}

		void UnitTest::Run()
		{
			for (auto it : testcases)
			{
				it->TestResult = 1;
				it->nFailed = 0;
				it->nPassed = 0;
				LOG_GREEN("============================================\n");
				LOG_GREEN(" Run TestCase:" << it->testcase_name << "\n");
				it->Run();
				if (!it->nFailed)
				{
					LOG_GREEN(" " << it->nPassed << " / " << it->nFailed + it->nPassed
						<< " Cases passed. ( " << (float)it->nPassed /
						(it->nFailed + it->nPassed) * 100 << "% )\n");
				}
				else
				{
					LOG_RED(" " << it->nPassed << " / " << it->nFailed + it->nPassed
						<< " Cases passed. ( " << (float)it->nPassed /
						(it->nFailed + it->nPassed) * 100 << "% )\n");
				}
				LOG_GREEN(" End TestCase:" << it->testcase_name << "\n");
				if (it->TestResult)
					++nPassed;
				else
					++nFailed;
			}
			LOG_GREEN("============================================\n");
			LOG_GREEN(" Total TestCase : " << nPassed + nFailed << "\n");
			LOG_GREEN(" Total Passed : " << nPassed << "\n");
			LOG_RED(" Total Failed : " << nFailed << "\n");
			LOG_GREEN(" " << nPassed << " / " << nFailed + nPassed
				<< " TestCases passed. ( " << nPassed / (nFailed + nPassed) * 100 << "% )\n");
		}


#define TESTCASE_NAME(testcase_name) testcase_name##_TEST

#define MYSTL_TEST(testcase_name)																		\
class TESTCASE_NAME(testcase_name):public TestCase														\
{																										\
private:																								\
	static TestCase* const testcase;																	\
public:																									\
	TESTCASE_NAME(testcase_name)(const char* case_name): TestCase(case_name) {};						\
    virtual void Run();																					\
}																										\
TestCase* const TESTCASE_NAME(testcase_name)::testcase=													\
UnitTest::GetInstance()->RegisterTestCase(new TESTCASE_NAME(testcase_name)(#testcase_name));			\
void TESTCASE_NAME(testcase_name)::Run()

		/*
		Run()���û��дʵ�֣���Ϊ���ú궨�彫�����������뵽 Run ��ʵ������磺
		TEST(AddTestDemo)
		{
		EXPECT_EQ(3, Add(1, 2));
		EXPECT_EQ(2, Add(1, 1));
		}
		�������뽫 { EXPECT_EQ(3, Add(1, 2)); EXPECT_EQ(2, Add(1, 1)); } �ӵ� Run() �ĺ���
		*/


		/*****************************************************************************************/

		// �򵥲��Եĺ궨��
		// ���� : �궨����ʽΪ EXPECT_* ��������֤�����ģ���������ͨ��������ʧ��
		// ʹ��һϵ�еĺ�����װ��֤��������Ϊ���¼����� :

		/*
		��ٶ���
		EXPECT_TRUE  ��֤����: Condition Ϊ true
		EXPECT_FALSE ��֤����: Condition Ϊ false

		Example:
		bool isPrime(int n);         һ���ж������ĺ���
		EXPECT_TRUE(isPrime(2));     ͨ��
		EXPECT_FALSE(isPrime(4));    ͨ��
		EXPECT_TRUE(isPrime(6));     ʧ��
		EXPECT_FALSE(isPrime(3));    ʧ��
		*/

#define EXPECT_TRUE(condition) if(condition)			\
{														\
	UnitTest::GetInstance()->CurTestCase->nPassed++;	\
	LOG_GREEN(" EXPECT_TRUE succeeded!\n");	}else {		\
	UnitTest::GetInstance()->TestResult = 0;			\
	UnitTest::GetInstance()->nFailed++;					\
	LOG_RED(" EXPECT_TRUE failed!\n"); 					\
}														

#define EXPECT_FALSE(condition) if(condition)			\
{														\
	UnitTest::GetInstance()->CurTestCase->nFailed++;	\
	UnitTest::GetInstance()->TestResult = 0;			\
	LOG_RED(" EXPECT_FALSE failed!\n");					\
}else {													\
	UnitTest::GetInstance()->CurTestCase->nPassed++;	\
	LOG_GREEN(" EXPECT_FALSE succeeded!\n");			\
}
		/*
		�Ƚ϶���
		EXPECT_EQ(v1, v2) ��֤����: v1 == v2
		EXPECT_NE(v1, v2) ��֤����: v1 != v2
		EXPECT_LT(v1, v2) ��֤����: v1 <  v2
		EXPECT_LE(v1, v2) ��֤����: v1 <= v2
		EXPECT_GT(v1, v2) ��֤����: v1 >  v2
		EXPECT_GE(v1, v2) ��֤����: v1 >= v2

		Note:
		1. ����Ӧ���� EXPECT_*(Expect, Actual)�ĸ�ʽ�����������ֵ���ұ���ʵ��ֵ
		2. �ڶ���ʧ��ʱ���Ὣ����ֵ��ʵ��ֵ��ӡ����
		3. ����ֵ�����ǿ�ͨ�����ԵıȽϲ��������бȽϵģ�����ֵ������֧�� << ��������
		��ֵ���뵽 ostream ��
		4. ��Щ���Կ��������û��Զ����ͱ𣬵�����������Ӧ�ıȽϲ��������� == ��< �ȣ�
		5. EXPECT_EQ ��ָ����е��ǵ�ַ�Ƚϡ����Ƚϵ��������Ƿ�ָ����ͬ���ڴ��ַ��
		����������ָ��������Ƿ���ȡ������Ƚ����� C �ַ���(const char*)��ֵ��
		��ʹ�� EXPECT_STREQ ���ر�һ����ǣ�Ҫ��֤һ�� C �ַ����Ƿ�Ϊ��(NULL)��
		��ʹ�� EXPECT_STREQ(NULL, c_str)������Ҫ�Ƚ����� string ����ʱ��
		Ӧ��ʹ�� EXPECT_EQ

		Example:
		EXPECT_EQ(3, foo());
		EXPECT_NE(NULL, pointer);
		EXPECT_LT(len, v.size());
		*/

#define EXPECT_EQ(v1,v2) if(v1==v2)											 \
		{																	 \
			UnitTest::GetInstance()->CurTestCase->nPassed++;				 \
			LOG_GREEN(" EXPECT_EQ succeeded!\n");							 \
		}else {																 \
			UnitTest::GetInstance()->CurTestCase->TestResult = 0;   		 \
			UnitTest::GetInstance()->CurTestCase->nFailed++;				 \
			LOG_RED(" EXPECT_EQ failed!\n");								 \
			LOG_RED(" Expect:" << v1 << "\n");								 \
			LOG_RED(" Actual:" << v2 << "\n");								 \
		}

#define EXPECT_NE(v1,v2) if(v1!=v2)											 \
		{																	 \
			UnitTest::GetInstance()->CurTestCase->nPassed++;				 \
			LOG_GREEN(" EXPECT_NE succeeded!\n");							 \
		}else {																 \
			UnitTest::GetInstance()->CurTestCase->TestResult = 0;	 		 \
			UnitTest::GetInstance()->CurTestCase->nFailed++;				 \
			LOG_RED(" EXPECT_NE failed!\n");								 \
			LOG_RED(" Expect:" << v1 << "\n");								 \
			LOG_RED(" Actual:" << v2 << "\n");								 \
		}

#define EXPECT_LT(v1,v2) if(v1<v2)											 \
		{																	 \
			UnitTest::GetInstance()->CurTestCase->nPassed++;				 \
			LOG_GREEN(" EXPECT_LT succeeded!\n");							 \
		}else {																 \
			UnitTest::GetInstance()->CurTestCase->TestResult = 0;	 		 \
			UnitTest::GetInstance()->CurTestCase->nFailed++;				 \
			LOG_RED(" EXPECT_LT failed!\n");								 \
			LOG_RED(" Expect:" << v1 << "\n");								 \
			LOG_RED(" Actual:" << v2 << "\n");								 \
		}

#define EXPECT_LE(v1,v2) if(v1<=v2)											 \
		{																	 \
			UnitTest::GetInstance()->CurTestCase->nPassed++;				 \
			LOG_GREEN(" EXPECT_LE succeeded!\n");							 \
		}else {																 \
			UnitTest::GetInstance()->CurTestCase->TestResult = 0;	 		 \
			UnitTest::GetInstance()->CurTestCase->nFailed++;				 \
			LOG_RED(" EXPECT_LE failed!\n");								 \
			LOG_RED(" Expect:" << v1 << "\n");								 \
			LOG_RED(" Actual:" << v2 << "\n");								 \
		}

#define EXPECT_GT(v1,v2) if(v1>v2)											 \
		{																	 \
			UnitTest::GetInstance()->CurTestCase->nPassed++;				 \
			LOG_GREEN(" EXPECT_GT succeeded!\n");							 \
		}else {																 \
			UnitTest::GetInstance()->CurTestCase->TestResult = 0;	 		 \
			UnitTest::GetInstance()->CurTestCase->nFailed++;				 \
			LOG_RED(" EXPECT_GT failed!\n");								 \
			LOG_RED(" Expect:" << v1 << "\n");								 \
			LOG_RED(" Actual:" << v2 << "\n");								 \
		}

#define EXPECT_GE(v1,v2) if(v1>=v2)											 \
		{																	 \
			UnitTest::GetInstance()->CurTestCase->nPassed++;				 \
			LOG_GREEN(" EXPECT_GE succeeded!\n");							 \
		}else {																 \
			UnitTest::GetInstance()->CurTestCase->TestResult = 0;	 		 \
			UnitTest::GetInstance()->CurTestCase->nFailed++;				 \
			LOG_RED(" EXPECT_GE failed!\n");								 \
			LOG_RED(" Expect:" << v1 << "\n");								 \
			LOG_RED(" Actual:" << v2 << "\n");								 \
		}

		/*
		�ַ����Ƚ�
		EXPECT_STREQ(s1, s2) ��֤����: ���� C �ַ�������ͬ��ֵ
		EXPECT_STRNE(s1, s2) ��֤����: ���� C �ַ����в�ͬ��ֵ

		Note:
		1. ����Ӧ���� EXPECT_STR*(Expect, Actual)�ĸ�ʽ�����������ֵ���ұ���ʵ��ֵ
		2. ����������ڱȽ����� C �ַ������������Ҫ�Ƚ����� string ������Ӧ��ʹ��
		EXPECT_EQ��EXPECT_NE �ȶ���
		3. EXPECT_STREQ �� EXPECT_STRNE �����ܿ��ַ�����wchar_t*��
		4. һ�� NULL ָ���һ�����ַ����᲻��һ����

		Example:
		char* s1 = "", char* s2 = "abc", char* s3 = NULL;
		EXPECT_STREQ("abc", s2);  ͨ��
		EXPECT_STREQ(s1, s3);     ʧ��
		EXPECT_STREQ(NULL, s3);   ͨ��
		EXPECT_STRNE(" ", s1);    ͨ��
		*/

#define EXPECT_STREQ(s1,s2)   if (s1 == NULL || s2 == NULL) {		\
		if (s1 == NULL && s2 == NULL) {								\
			UnitTest::GetInstance()->CurTestCase->nPassed++;		\
			LOG_GREEN(" EXPECT_STREQ succeeded!\n");				\
		}															\
		else {														\
			UnitTest::GetInstance()->CurTestCase->TestResult = 0;	\
			UnitTest::GetInstance()->CurTestCase->nFailed++;		\
			LOG_RED(" EXPECT_STREQ failed!\n");						\
			if(s1 == NULL) LOG_RED( " Expect: NULL\n");				\
			else LOG_RED( " Expect:\"" << s1 << "\"\n");			\
			if(s2 == NULL) LOG_RED( " Actual: NULL\n");				\
			else LOG_RED( " Actual:\"" << s2 << "\"\n");			\
		}															\
	}																\
	else if (strcmp(s1, s2) == 0) {									\
		UnitTest::GetInstance()->CurTestCase->nPassed++;			\
		LOG_GREEN( " EXPECT_STREQ succeeded!\n");					\
	}																\
	else {															\
		UnitTest::GetInstance()->CurTestCase->TestResult = 0;		\
		UnitTest::GetInstance()->CurTestCase->nFailed++;			\
		LOG_RED(" EXPECT_STREQ failed!\n");							\
		LOG_RED(" Expect:\"" << s1 << "\"\n");						\
		LOG_RED(" Actual:\"" << s2 << "\"\n");						\
	}

#define EXPECT_STRNE(s1,s2)   if (s1 == NULL || s2 == NULL) {		\
		if (s1 != NULL || s2 != NULL) {								\
			UnitTest::GetInstance()->CurTestCase->nPassed++;		\
			LOG_GREEN(" EXPECT_STRNE succeeded!\n");				\
		}															\
		else {														\
			UnitTest::GetInstance()->CurTestCase->TestResult = 0;	\
			UnitTest::GetInstance()->CurTestCase->nFailed++;		\
			LOG_RED(" EXPECT_STRNE failed!\n");						\
			if(s1 == NULL) LOG_RED( " Expect: NULL\n");				\
			else LOG_RED( " Expect:\"" << s1 << "\"\n");			\
			if(s2 == NULL) LOG_RED( " Actual: NULL\n");				\
			else LOG_RED( " Actual:\"" << s2 << "\"\n");			\
		}															\
	}																\
	else if (strcmp(s1, s2) != 0) {									\
		UnitTest::GetInstance()->CurTestCase->nPassed++;			\
		LOG_GREEN( " EXPECT_STRNE succeeded!\n");					\
	}																\
	else {															\
		UnitTest::GetInstance()->CurTestCase->TestResult = 0;		\
		UnitTest::GetInstance()->CurTestCase->nFailed++;			\
		LOG_RED(" EXPECT_STRNE failed!\n");							\
		LOG_RED(" Expect:\"" << s1 << "\"\n");						\
		LOG_RED(" Actual:\"" << s2 << "\"\n");						\
	}


		/*
		ָ��Ƚ�
		EXPECT_PTR_EQ(p1, p2)            ��֤����: *p1 == *p2
		EXPECT_PTR_NE(p1, p2)            ��֤����: *p1 != *p2
		EXPECT_PTR_RANGE_EQ(p1, p2, len) ��֤����: ���� i (*p1 + i) == (*p2 + i)  i��[0,len)
		EXPECT_PTR_RANGE_NE(p1, p2, len) ��֤����: ���� i (*p1 + i) != (*p2 + i)  i��[0,len)

		Note:
		1. ����Ӧ���� EXPECT_PTR_*(Expect, Actual)��
		EXPECT_PTR_RANGE_*(Expect, Actual, len)�ĸ�ʽ��
		��������������ֵ��ʵ��ֵ���
		2. EXPECT_PTR_EQ �Ƚϵ���ָ����ָԪ�ص�ֵ�����Ҫ�Ƚ�
		ָ��ָ��ĵ�ַ�Ƿ���ȣ����� EXPECT_EQ
		3. EXPECT_PTR_RANGE_* �Ƚϵ��Ǵ� p1��p2 ��ʼ��
		����Ϊ len �����䣬��ȷ�����䳤����Ч

		Example:
		int a[] = {1,2,3,4,5};
		int b[] = {1,2,3,4,6};
		int *p1 = a, *p2 = b;
		EXPECT_PTR_EQ(p1, p2);                      ͨ��
		p1 = a + 4, p2 = b + 4;
		EXPECT_PTR_EQ(p1, p2);                      ʧ��
		EXPECT_PTR_EQ(p1, std::find(a, a + 5, 5));  ͨ��
		EXPECT_PTR_RANGE_EQ(a, b, 5);               ʧ��
		EXPECT_PTR_RANGE_EQ(a, b, 4);               ͨ��
		*/

#define EXPECT_PTR_EQ(p1,p2) if(*p1==*p2)						  \
{																  \
UnitTest::GetInstance()->CurTestCase->nPassed++;				  \
LOG_GREEN(" EXPECT_PTR_EQ succeeded!\n");						  \
}else {															  \
	UnitTest::GetInstance()->CurTestCase->TestResult = 0;		  \
	UnitTest::GetInstance()->CurTestCase->nFailed++;			  \
	LOG_RED(" EXPECT_PTR_EQ failed!\n");						  \
	LOG_RED(" Expect:" << *p1 << "\n");							  \
	LOG_RED(" Actual:" << *p2 << "\n");							  \
}

#define EXPECT_PTR_NE(p1,p2) if(*p1!=*p2)						  \
{																  \
UnitTest::GetInstance()->CurTestCase->nPassed++;				  \
LOG_GREEN(" EXPECT_PTR_NE succeeded!\n");						  \
}else {															  \
	UnitTest::GetInstance()->CurTestCase->TestResult = 0;		  \
	UnitTest::GetInstance()->CurTestCase->nFailed++;			  \
	LOG_RED(" EXPECT_PTR_NE failed!\n");						  \
	LOG_RED(" Expect:" << *p1 << "\n");							  \
	LOG_RED(" Actual:" << *p2 << "\n");							  \
}

#define EXPECT_PTR_RANGE_EQ(p1,p2,len) if(std::equal(p1,p1+len,p2))				\
{																				\
	UnitTest::GetInstance()->CurTestCase->nPassed++;							\
	LOG_GREEN(" EXPECT_PTR_RANGE_EQ succeeded!\n");								\
}else {																			\
	UnitTest::GetInstance()->CurTestCase->TestResult = 0;						\
	UnitTest::GetInstance()->CurTestCase->nFailed++;							\
	LOG_RED(" EXPECT_PTR_RANGE_EQ failed!\n");									\
}

#define EXPECT_PTR_RANGE_NE(p1,p2,len) if(!std::equal(p1,p1+len,p2))			\
{																				\
	UnitTest::GetInstance()->CurTestCase->nPassed++;							\
	LOG_GREEN(" EXPECT_PTR_RANGE_NE succeeded!\n");								\
}else {																			\
	UnitTest::GetInstance()->CurTestCase->TestResult = 0;						\
	UnitTest::GetInstance()->CurTestCase->nFailed++;							\
	LOG_RED(" EXPECT_PTR_RANGE_NE failed!\n");									\
}

		/*
		�����Ƚ�
		EXPECT_CON_EQ(c1, c2) ��֤����: c1 == c2
		EXPECT_CON_NE(c1, c2) ��֤����: c1 != c2

		Note:
		1. ���������� STL �������Զ�����������������飬����������ָ��
		2. ��������������Ҫ�ܹ����бȽϣ�����һ�»���Է�����ʽת��
		3. EXPECT_CON_EQ ����ʧ��ʱ�����ӡ�״β���ȵ�����ֵ

		Example:
		int arr[] = {1,2,3};
		std::vector<int> v1{1, 2, 3};
		std::vector<int> v2{2, 3, 4};
		mystl::vector<long> v3(arr, arr + 3);
		EXPECT_CON_NE(v1, v2)   ok
		EXPECT_CON_EQ(arr, v1)  ok
		EXPECT_CON_EQ(v1, v3)   ok
		*/

#define EXPECT_CON_EQ(c1,c2) 												 \
{auto first1 = std::begin(c1), last1 = std::end(c1);							 \
auto first2 = std::begin(c2), last2 = std::end(c2);							 \
while (first1 != last1 && first2 != last2 && *first1 == *first2)			 \
{																			 \
	++first1;																 \
	++first2;																 \
}																			 \
if (first1 == last1 && first2 == last2)										 \
{																			 \
	UnitTest::GetInstance()->CurTestCase->nPassed++;						 \
	LOG_GREEN(" EXPECT_CON_EQ succeeded!\n");								 \
}																			 \
else																		 \
{																			 \
	UnitTest::GetInstance()->CurTestCase->TestResult = 0;					 \
	UnitTest::GetInstance()->CurTestCase->nFailed++;						 \
	LOG_RED(" EXPECT_CON_EQ failed!\n");									 \
	LOG_RED(" Expect:" << *first1 << "\n");									 \
	LOG_RED(" Actual:" << *first2 << "\n");									 \
}}

#define EXPECT_CON_NE(c1,c2) 												 \
{auto first1 = std::begin(c1), last1 = std::end(c1);							 \
auto first2 = std::begin(c2), last2 = std::end(c2);							 \
while (first1 != last1 && first2 != last2 && *first1 == *first2)			 \
{																			 \
	++first1;																 \
	++first2;																 \
}																			 \
if (first1 != last1 || first2 != last2)										 \
{																			 \
	UnitTest::GetInstance()->CurTestCase->nPassed++;						 \
	LOG_GREEN(" EXPECT_CON_NE succeeded!\n");								 \
}																			 \
else																		 \
{																			 \
	UnitTest::GetInstance()->CurTestCase->TestResult = 0;					 \
	UnitTest::GetInstance()->CurTestCase->nFailed++;						 \
	LOG_RED(" EXPECT_CON_NE failed!\n");									 \
}}


		/*****************************************************************************************/
		// ���õĺ궨��

		// ��ͬ����Ĳ���������
#if defined(DEBUG)||defined(_DEBUG)
#define LEN1 10000
#define LEN2 100000
#define LEN3 1000000
#else
#define LEN1 100000
#define LEN2 1000000
#define LEN3 10000000
#endif 

#define SCALE_LLL(N) (N * 20)
#define SCALE_LL(N)  (N * 10)
#define SCALE_L(N)   (N * 5)
#define SCALE_M(N)   (N)
#define SCALE_S(N)   (N / 5)
#define SCALE_SS(N)  (N / 10)
#define SCALE_SSS(N) (N / 20)

#define WIDE    14

//���ͨ����ʾ
#define PASSED LOG_GREEN("[PASSED]\n")

//�����������
#define COUT(container) 						  \
std::string cout_name = #container;				  \
std::cout << cout_name << ":";					  \
for (auto it : container)						  \
{												  \
	std::cout << " " << it;						  \
}												  \
std::cout << std::endl;								  

#define STR_COUT(str)								   \
std::string str_name = #str;					   \
std::cout << str_name << ":" << str << std::endl;

// ����������ú�����Ľ��
#define FUN_AFTER(container,func)					 \
{std::string func_name = #func;						 \
std::cout << " After " << func_name << ":";			 \
func;												 \
COUT(container);}

#define STR_FUN_AFTER(str,func)						 \
{std::string func_name = #func;						 \
std::cout << " After " << func_name << ":";			 \
func;												 \
STR_COUT(str);}

// ����������ú�����ֵ
#define FUN_VALUE(func)								  \
{std::string func_name = #func;						  \
std::cout << func_name << ":" << func << std::endl;}

		// �������������
		void test_len(size_t len1, size_t len2, size_t len3, size_t wide)
		{
			std::string str1, str2, str3;
			std::stringstream ss;
			ss << len1 << " " << len2 << " " << len3;
			ss >> str1 >> str2 >> str3;
			str1 += "   |";
			std::cout << std::setw(wide) << str1;
			str2 += "   |";
			std::cout << std::setw(wide) << str2;
			str3 += "   |";
			std::cout << std::setw(wide) << str3 << "\n";
		}

#define TEST_LEN(len1, len2, len3, wide) test_len(len1, len2, len3, wide)

		// ���ò������ܵĺ�
#define FUN_TEST_FORMAT1(mode,fun,arg,count)													  \
{std::srand((int)time(0));																		  \
clock_t start, end;																				  \
mode c;																							  \
char buf[10];																					  \
start = clock();																				  \
for (size_t i = 0; i < count; ++i)																  \
{																								  \
	c.fun(arg);																					  \
}																								  \
end = clock();																					  \
int n =static_cast<int>(static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000);				  \
std::snprintf(buf, sizeof(buf), "%d", n);														  \
std::string t = buf;																			  \
t += "ms    |";																					  \
std::cout << std::setw(WIDE) << t;}

#define FUN_TEST_FORMAT2(mode, fun, arg1, arg2, count) 											 \
{srand((int)time(0));																			 \
clock_t start, end;																				 \
mode c;																							 \
char buf[10];																					 \
start = clock();																				 \
for (size_t i = 0; i < count; ++i)																 \
	c.fun(c.arg1(), arg2);																		 \
end = clock();																					 \
int n = static_cast<int>(static_cast<double>(end - start)										 \
	/ CLOCKS_PER_SEC * 1000);																	 \
std::snprintf(buf, sizeof(buf), "%d", n);														 \
std::string t = buf;																			 \
t += "ms    |";																					 \
std::cout << std::setw(WIDE) << t;}

#define LIST_SORT_DO_TEST(mode,count)							 \
{srand((int)time(0));											 \
clock_t start, end;												 \
mode::list<int> l;												 \
char buf[10];													 \
for (int i = 0; i < count; ++i)									 \
{																 \
	l.insert(l.end(), rand());									 \
}																 \
start = clock();												 \
l.sort();														 \
end = clock();													 \
int n = static_cast<int>(static_cast<double>(end - start)		 \
	/ CLOCKS_PER_SEC * 1000);									 \
std::snprintf(buf, sizeof(buf), "%d", n);						 \
std::string t = buf;											 \
t += "ms    |";													 \
std::cout << std::setw(WIDE) << t;}

#define MAP_EMPLACE_DO_TEST(mode, con, count)	        		 \
{srand((int)time(0));											 \
clock_t start, end;												 \
mode::con<int, int> c;											 \
char buf[10];													 \
start = clock();												 \
for (size_t i = 0; i < count; ++i)								 \
	c.emplace(mode::make_pair(rand(), rand()));					 \
end = clock();													 \
int n = static_cast<int>(static_cast<double>(end - start)		 \
	/ CLOCKS_PER_SEC * 1000);									 \
std::snprintf(buf, sizeof(buf), "%d", n);						 \
std::string t = buf;											 \
t += "ms    |";													 \
std::cout << std::setw(WIDE) << t;}

		// �ع��ظ�����
#define CON_TEST_P1(con, fun, arg, len1, len2, len3)			\
	TEST_LEN(len1, len2, len3, WIDE);                          \
	std::cout << "|         std         |";                    \
	FUN_TEST_FORMAT1(std::con, fun, arg, len1);                \
	FUN_TEST_FORMAT1(std::con, fun, arg, len2);                \
	FUN_TEST_FORMAT1(std::con, fun, arg, len3);                \
	std::cout << "\n|        mystl        |";                  \
	FUN_TEST_FORMAT1(mystl::con, fun, arg, len1);              \
	FUN_TEST_FORMAT1(mystl::con, fun, arg, len2);              \
	FUN_TEST_FORMAT1(mystl::con, fun, arg, len3);    

#define CON_TEST_P2(con, fun, arg1, arg2, len1, len2, len3)  \
	TEST_LEN(len1, len2, len3, WIDE);                          \
	std::cout << "|         std         |";                    \
	FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len1);         \
	FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len2);         \
	FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len3);         \
	std::cout << "\n|        mystl        |";                  \
	FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len1);       \
	FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len2);       \
	FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len3);    

#define MAP_EMPLACE_TEST(con, len1, len2, len3)              \
	TEST_LEN(len1, len2, len3, WIDE);                          \
	std::cout << "|         std         |";                    \
	MAP_EMPLACE_DO_TEST(std, con, len1);                       \
	MAP_EMPLACE_DO_TEST(std, con, len2);                       \
	MAP_EMPLACE_DO_TEST(std, con, len3);                       \
	std::cout << "\n|        mystl        |";                  \
	MAP_EMPLACE_DO_TEST(mystl, con, len1);                     \
	MAP_EMPLACE_DO_TEST(mystl, con, len2);                     \
	MAP_EMPLACE_DO_TEST(mystl, con, len3);

#define LIST_SORT_TEST(len1, len2, len3)                     \
	TEST_LEN(len1, len2, len3, WIDE);                          \
	std::cout << "|         std         |";                    \
	LIST_SORT_DO_TEST(std, len1);                              \
	LIST_SORT_DO_TEST(std, len2);                              \
	LIST_SORT_DO_TEST(std, len3);                              \
	std::cout << "\n|        mystl        |";                  \
	LIST_SORT_DO_TEST(mystl, len1);                            \
	LIST_SORT_DO_TEST(mystl, len2);                            \
	LIST_SORT_DO_TEST(mystl, len3);

#define TEST(testcase_name) MYSTL_TEST(testcase_name)

#define RUN_ALL_TEST() mystl::test::UnitTest::GetInstance()->Run()

// �Ƿ������ܲ���
#ifndef PERFORMANCE_TEST_ON
#define PERFORMANCE_TEST_ON 1
#endif // !PERFORMANCE_TEST_ON

// �Ƿ���������������
#ifndef LARGER_TEST_DATA_ON
#define LARGER_TEST_DATA_ON 0
#endif // !LARGER_TEST_DATA_ON
	}
}